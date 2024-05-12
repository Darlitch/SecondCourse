#include <assert.h>
#include <math.h>
#include <mpich/mpi.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

enum worker_stat { have_to_share,
                   no_to_share,
                   finished };

#define REQUEST_TASKS_TAG 1
#define RESPONSE_TASKS_TAG 2

void logs(const char* msg) {
    char dateTime[32];
    time_t rawtime;
    struct tm* info;
    time(&rawtime);
    info = localtime(&rawtime);
    strftime(dateTime, 31, "%T", info);
    fprintf(stdout, "$[%s] - in file[%s] - %s\n", dateTime, __FILE__, msg);
}

const size_t one_iteration_tasks_count = 1000000;
const size_t iteration_count = 1;

pthread_t threads[2];
pthread_mutex_t mutex;

typedef struct task_t {
    size_t weight;
    size_t done;
} task_t;

task_t* current_task;

size_t init_job = 0;
size_t real_job = 0;

int size;
int rank;
long double buf = 0.0;

void execute_task(task_t* t) {
    while (t->done < t->weight) {
        buf += sqrt(3.14);
        t->done++;
        real_job++;
    }
}

enum worker_stat current_worker_status = no_to_share;
enum worker_stat get_worker_status() {
    return current_worker_status;
}

void* run_worker() {
    for (size_t i = 1; i <= iteration_count; i++) {
        pthread_mutex_lock(&mutex);
        current_worker_status = have_to_share;
        pthread_mutex_unlock(&mutex);

        execute_task(current_task);

        pthread_mutex_lock(&mutex);
        current_worker_status = no_to_share;
        pthread_mutex_unlock(&mutex);

        int status = EXIT_FAILURE;
        do {
            int req = 0;
            status = EXIT_FAILURE;
            for (int neighbour = 0; neighbour < size; neighbour++) {
                size_t resp = 0;
                if (neighbour == rank) {
                    continue;
                }
                MPI_Send(&req, 1, MPI_INT, neighbour, REQUEST_TASKS_TAG, MPI_COMM_WORLD);
                MPI_Recv(&resp, 1, MPI_UNSIGNED_LONG, neighbour, RESPONSE_TASKS_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                printf("in rank [%d] responsed %ld\n", rank, resp);
                if (resp) {
                    status = EXIT_SUCCESS;
                    pthread_mutex_lock(&mutex);
                    current_task->weight = resp;
                    execute_task(current_task);
                    pthread_mutex_unlock(&mutex);
                }
            }
        } while (status == EXIT_SUCCESS);
        printf("Before barrier\n");
        MPI_Barrier(MPI_COMM_WORLD);
    }

    pthread_mutex_lock(&mutex);
    current_worker_status = finished;
    pthread_mutex_unlock(&mutex);
    printf("in rank [%d]: init_job: %ld, real_job: %ld\n", rank, init_job, real_job);
    pthread_exit(NULL);
}

void listen_and_handle() {
    logs("listen and handle");
    MPI_Status stat;
    int req;

    // Блокирующий вызов ресив ожидает входящих запросов.
    MPI_Recv(&req, 1, MPI_INT, MPI_ANY_SOURCE, REQUEST_TASKS_TAG, MPI_COMM_WORLD, &stat);
    if (stat.MPI_SOURCE == rank) {
        printf("self to send\n");
    }
    size_t to_send = 0;
    pthread_mutex_lock(&mutex);
    if (current_task->done <= current_task->weight) {
        to_send = (current_task->weight - current_task->done) / 2;
    }

    MPI_Send(&to_send, 1, MPI_UNSIGNED_LONG, stat.MPI_SOURCE, RESPONSE_TASKS_TAG, MPI_COMM_WORLD);
    current_task->done += to_send;
    pthread_mutex_unlock(&mutex);
}

void* run_server(void* data) {
    while (get_worker_status() != finished) {
        if (get_worker_status() == have_to_share) {
            listen_and_handle();
        }
    }
    pthread_exit(NULL);
}

int main(int argc, char** argv) {
    int ThreadSupport;
    MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &ThreadSupport);
    if (ThreadSupport != MPI_THREAD_MULTIPLE) {
        MPI_Finalize();
        return EXIT_FAILURE;
    }

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    pthread_mutex_init(&mutex, NULL);
    pthread_attr_t ThreadAttributes;

    current_task = (task_t*)malloc(sizeof(task_t));
    current_task->weight = rank * one_iteration_tasks_count;
    init_job = current_task->weight;

    current_task->done = 0;

    double start = MPI_Wtime();
    pthread_attr_init(&ThreadAttributes);
    pthread_attr_setdetachstate(&ThreadAttributes, PTHREAD_CREATE_JOINABLE);
    pthread_create(&threads[0], &ThreadAttributes, &run_worker, NULL);
    pthread_create(&threads[1], &ThreadAttributes, &run_server, NULL);
    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);
    pthread_attr_destroy(&ThreadAttributes);
    pthread_mutex_destroy(&mutex);

    // printf("")
    if (rank == 0) {
        printf("time taken: %lf\n", MPI_Wtime() - start);
    }

    MPI_Finalize();
    return EXIT_SUCCESS;
}
