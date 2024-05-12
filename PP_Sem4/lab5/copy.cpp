#include <mpi.h>
#include <pthread.h>
#include <time.h>

#include <chrono>
#include <iostream>
#include <stack>
#include <thread>

constexpr int countTasks = 2048;
pthread_mutex_t mutexTask;

int* createTasks() {
    int* array = new int[countTasks];
    for (int i = 0; i < countTasks; ++i) {
        array[i] = rand() % 100 + 5;
    }
    return array;
}

void* listenerThread(void* data) {
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int* tasks = (int*)data;
    while (true) {
        int message;
        int request;

        MPI_Recv(&message, 1, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        if (message == -1) {
            break;
        }
        pthread_mutex_lock(&mutexTask);
        if (tasks[0] == 0) {
            request = -1;
        } else {
            for (int i = 0; i < countTasks / size; ++i) {
                if (tasks[i] == 0) {
                    request = tasks[i - 1];
                    tasks[i - 1] = 0;
                }
            }
        }
        pthread_mutex_unlock(&mutexTask);
        MPI_Send(&request, 1, MPI_INT, message, 0, MPI_COMM_WORLD);
    }
    return nullptr;
}

int main(int argc, char** argv) {
    int lvlThread;
    MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &lvlThread);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int* allTasks = nullptr;
    if (!rank) {
        std::cout << "Lvl MPI " << lvlThread;
        if (lvlThread == MPI_THREAD_MULTIPLE) {
            std::cout << ": thread-compliant! Continue";
        } else {
            std::cout << ": not thread-compliant. Continue without garanties";
        }
        std::cout << std::endl;
        allTasks = createTasks();
    }

    int* tasks = new int[countTasks / size];
    MPI_Scatter(allTasks, countTasks / size, MPI_INT, tasks, countTasks / size,
                MPI_INT, 0, MPI_COMM_WORLD);

    pthread_mutex_init(&mutexTask, NULL);

    pthread_attr_t attr;
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_attr_init(&attr);
    pthread_t thread;

    pthread_create(&thread, &attr, &listenerThread, tasks);
    pthread_attr_destroy(&attr);
    clock_t start;
    if (!rank) {
        start = clock();
    }

    int ptr = countTasks / size;
    bool flagTasks = true;
    while (flagTasks) {
        // std::cout << "Rank: " << rank << ". Tasks: " << ptr + 1 << std::endl;
        int task = -1;
        pthread_mutex_lock(&mutexTask);
        for (; tasks[ptr] == 0 && ptr >= 0; --ptr)
            ;
        if (ptr == -1) {
            int newTask = -1;
            pthread_mutex_unlock(&mutexTask);
            for (int i = (rank + 1) % size; i % size != rank; ++i) {
                MPI_Send(&rank, 1, MPI_INT, i % size, 1, MPI_COMM_WORLD);
                MPI_Recv(&newTask, 1, MPI_INT, i % size, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                if (newTask != -1) {
                    break;
                }
            }
            pthread_mutex_lock(&mutexTask);
            if (newTask == -1) {
                flagTasks = false;
            } else {
                tasks[0] = newTask;
            }
        } else {
            task = tasks[ptr];
            tasks[ptr] = 0;
        }
        pthread_mutex_unlock(&mutexTask);
        if (task != -1) {
            std::this_thread::sleep_for(std::chrono::milliseconds(task));
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);
    if (!rank) {
        std::cout << "time: " << (clock() - start) / (double)CLOCKS_PER_SEC << std::endl;
    }
    int end = -1;
    MPI_Send(&end, 1, MPI_INT, rank, 1, MPI_COMM_WORLD);
    pthread_join(thread, nullptr);
    // std::cout << 1 << std::endl;
    pthread_mutex_destroy(&mutexTask);
    MPI_Finalize();
    return 0;
}
