#include <mpi.h>
#include <pthread.h>

#include <iostream>

const std::size_t numOfTasks = 2048;
pthread_mutex_t mutexTask;

int* CreateTasks() {
    int* tasks = new int[numOfTasks];
    for (std::size_t i = 0; i < numOfTasks; ++i) {
        tasks[i] = rand() % 100 + 5;
    }
    return tasks;
}

int main(int argc, char** argv) {
    int rank, size;
    int supportLvl;
    int* tasks = nullptr;
    MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &supportLvl);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (!rank) {
        std::cout << "SupportLvl " << supportLvl;
        if (supportLvl == MPI_THREAD_MULTIPLE) {
            std::cout << ": thread-compliant! Continue" << std::endl;
        } else {
            std::cout << ": not thread-compliant. Continue without garanties"
                      << std::endl;
        }
        tasks = CreateTasks();
    }

    int* pthrTasks = new int[numOfTasks / size];
    MPI_Scatter(tasks, numOfTasks / size, MPI_INT, tasks, numOfTasks / size,
                MPI_INT, 0, MPI_COMM_WORLD);

    pthread_mutex_init(&mutexTask, NULL);
}