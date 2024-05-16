#include <mpi.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define TASKS_SIZE 20
#define COUNT_THREAD 2

pthread_mutex_t mutexTask;
int* tasks = NULL;
int* localTasks = NULL;
int counter = 0;
int localTaskCounter = 0;
int refusalCounter = 0;

void InitTasks(int* tasks) {
    // srand(time(NULL));
    for (int i = 0; i < TASKS_SIZE; ++i) {
        // tasks[i] = rand() % 10 + 1;
        tasks[i] = i + 1;
    }
}

int GetCount(int rank, int size) {
    int start = TASKS_SIZE * rank / size;
    int end = TASKS_SIZE * (rank + 1) / size;
    return end - start;
}

void SharedWork(int rank, int size) {
    int* counts = (int*)malloc(size * sizeof(int));
    for (int i = 0; i < size; ++i) {
        counts[i] = GetCount(i, size);
    }
    int* displs = (int*)malloc(size * sizeof(int));
    displs[0] = 0;
    for (int i = 1; i < size; ++i) {
        displs[i] = displs[i - 1] + counts[i - 1];
    }
    MPI_Scatterv(tasks, counts, displs, MPI_INT, localTasks, counts[rank], MPI_INT, 0, MPI_COMM_WORLD);
    free(counts);
    free(displs);
}

void* Loader() {
    int size;
    int refusalCounter = 0; 
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    while(refusalCounter < size - 1) {
        int request = -1;
        MPI_Status status;  
        MPI_Recv(&request, 1, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);
        int sendRank = status.MPI_SOURCE;
        if (counter < localTaskCounter - 1) {
            pthread_mutex_lock(&mutexTask);
            int numOfTasks = localTaskCounter - 1 - counter;
            int sharedSize = numOfTasks / 2 + 1;
            MPI_Send(&sharedSize, 1, MPI_INT, sendRank, 0, MPI_COMM_WORLD);
            int* share = localTasks + localTaskCounter - sharedSize;
            MPI_Send(share, sharedSize, MPI_INT, sendRank, 0, MPI_COMM_WORLD);
            localTaskCounter -= sharedSize;
            pthread_mutex_unlock(&mutexTask);
        } else {
            request = -1;
            MPI_Send(&request, 1, MPI_INT, sendRank, 0, MPI_COMM_WORLD);
            refusalCounter++;
        }
    }
}

void GetWork(int rank, int size, int* visited) {
    for (int i = 0; i < size; ++i) {
        if (visited[i] != 1) {
            MPI_Send(&rank, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
            int newCount;
            MPI_Recv(&newCount, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if (newCount == -1) {
                // printf("NOT WORK for %d  form %d \n", rank, i);
                visited[i] = 1;
                continue;
            }
            // printf("WORK form %d to %d count %d\n", i, rank, countNewWork );
            MPI_Recv(localTasks, newCount, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            localTaskCounter = newCount;
            break;
        }
    }
}

void* Worker() {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int* visited = (int*)malloc(size * sizeof(int));
    visited[rank] = 1;
    while (localTaskCounter > 0) {
        while (counter < localTaskCounter) {
            printf("process with rank %d doing: %d  work %d for %d\n", rank, localTasks[counter], counter + 1, localTaskCounter);
            sleep(localTasks[counter]);
            pthread_mutex_lock(&mutexTask);
            counter++;
            pthread_mutex_unlock(&mutexTask);
        }
        localTaskCounter = -1;
        counter = 0;
        // printf("WORKER with rank %d COMLETED all work \n", rank);
        GetWork(rank, size, visited);
    }
    free(visited);
    return NULL;
}

int main(int argc, char** argv) {
    int rank, size;
    int supportLvl;
    double startTime, endTime;
    MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &supportLvl);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (!rank) {
        printf("SupportLvl %d", supportLvl);
        if (supportLvl == MPI_THREAD_MULTIPLE) {
            printf(": thread-compliant! Continue\n");
        } else {
            printf(": not thread-compliant. Continue without garanties\n");
        }
        tasks = (int*)malloc(TASKS_SIZE * sizeof(int));
        InitTasks(tasks);
    }
    localTaskCounter = GetCount(rank, size);
    localTasks = (int*)malloc(localTaskCounter * sizeof(int));
    MPI_Barrier(MPI_COMM_WORLD);
    SharedWork(rank, size);
    startTime = MPI_Wtime();
    pthread_attr_t attrs;
    if (pthread_attr_init(&attrs) != 0) {
        perror("Cannot initialize attributes");
        exit(-1);
    }
    if (pthread_attr_setdetachstate(&attrs, PTHREAD_CREATE_JOINABLE) != 0) {
        perror("Error in setting attributes");
        exit(-1);
    }

    pthread_t* threads = (pthread_t*)malloc(COUNT_THREAD * sizeof(pthread_t));
    if (pthread_create(&threads[0], &attrs, Worker, NULL) != 0) {
        perror("Cannot create a thread");
        abort();
    }
    if (pthread_create(&threads[1], &attrs, Loader, NULL) != 0) {
        perror("Cannot create a thread");
        abort();
    }

    pthread_attr_destroy(&attrs);
    for (int i = 0; i < COUNT_THREAD; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("Cannot join a thread");
            exit(-1);
        }
    }
    free(tasks);
    free(localTasks);
    endTime = MPI_Wtime();
    printf("Total time: %f seconds\n", endTime - startTime);
    MPI_Finalize();
    return 0;
}