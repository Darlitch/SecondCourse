#include <mpi.h>

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>

const std::size_t N = 5;
const double e = 0.00001;
const double t = 0.01;

std::size_t FindLrows(int size, int rank) {
    return (N / size + (N % size > rank));
}

std::size_t FindBegin(int size, int rank) {
    std::size_t begin = 0;
    for (std::size_t i = 0; i < rank; ++i) {
        begin += ((N / size) + (int)((N % size) > i));
    }
    return begin;
}

double** MatrixBuilder(int size, int rank) {
    std::size_t matrixBegin = FindBegin(size, rank);
    std::size_t lrows = FindLrows(size, rank);
    double** matrix = new double*[lrows];
    for (std::size_t i = 0; i < lrows; ++i) {
        matrix[i] = new double[N];
        for (std::size_t j = 0; j < N; ++j) {
            if (j == i + matrixBegin) {
                matrix[i][j] = 2.0;
            } else {
                matrix[i][j] = 1.0;
            }
        }
    }
    return matrix;
}

void RandomVectorX(double* x) {
    srand(time(NULL));
    int size, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    std::size_t begin = FindBegin(size, rank);
    std::size_t lrows = FindLrows(size, rank);
    srand(time(NULL));
    for (std::size_t i = begin; i < begin + lrows; ++i) {
        x[i] = rand() % 10;
        std::cout << "x: " << x[i] << std::endl;
        // std::cout << "x: " << x[i] << std::endl;
    }
}

int* FindLrowsV(int size) {
    int* array = new int[size];
    for (int j = 0; j < size; ++j) {
        array[j] = (N / size) + (int)(N % size > j);
    }
    return array;
}

void AMultX(double** matrix, double* x, double* b) {
    int size, rank;
    // MPI_Barrier(MPI_COMM_WORLD);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    std::cout << "size: " << size << std::endl;
    std::size_t lrows = FindLrows(size, rank);
    std::size_t matrixBegin = FindBegin(size, rank);
    int* lrowsV = FindLrowsV(size);

    for (std::size_t p = 0; p < size; ++p) {       // полный подсчёт по всем процессам
        for (std::size_t i = 0; i < lrows; ++i) {  // проходимся по всем строкам матрицы
            for (std::size_t j = matrixBegin; j < matrixBegin + lrows; ++j) {
                b[i + matrixBegin] += x[i + matrixBegin] * matrix[i][j];
            }
        }
        double* x1 = new double[lrowsV[(rank + 1) % size]]();
        // std::cout << "1" << std::endl;
        MPI_Sendrecv(x, lrows, MPI_DOUBLE, rank, 123, x1, lrowsV[(rank + 1) % size], MPI_DOUBLE, (rank + 1) % size, 123, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Sendrecv(x, lrows, MPI_DOUBLE, (rank + 1) % size, 123, x1, lrowsV[(rank + 1) % size], MPI_DOUBLE, (rank + size - 1) % size, 123, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::swap(x, x1);
        // lrows = lrowsV[(rank + p + 1) % size - 1];
        std::rotate(lrowsV, lrowsV + 1, lrowsV + size);
        // for (std::size_t i = 0; i < size; ++i) {
        //     std::cout << "lrows: " << lrowsV[i] << std::endl;
        // }
        std::cout << "1" << std::endl;
        delete[] x1;
    }
    delete[] lrowsV;
}

void SubB(double* x1, double* b) {
}

void MultT(double* x1) {
}

void SubXX(double* x0, double* x1) {
}

// double Module(double* u) {
// }

void SearchX(double** matrix, double* x, double* b) {
    double x0[N];
    double x1[N];
    double u;
    int size, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    std::size_t lrows = FindLrows(size, rank);
    std::size_t matrixBegin = FindBegin(size, rank);
}

int main(int argc, char** argv) {
    double startTime, endTime;
    int rank, size;
    double* x = new double[N]();
    double b[N] = {0};
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    std::size_t lrows = FindLrows(size, rank);
    // MPI_Barrier(MPI_COMM_WORLD);
    double** matrix = MatrixBuilder(size, rank);
    if (rank == 0) {
        startTime = MPI_Wtime();
    }

    RandomVectorX(x);
    // for (std::size_t i = 0; i < N; ++i) {
    //     std::cout << x[i] << " ";
    // }
    // std::cout << std::endl;
    // MPI_Barrier(MPI_COMM_WORLD);
    for (std::size_t i = 0; i < N; ++i) {
        std::cout << x[i] << " ";
    }
    std::cout << std::endl;
    AMultX(matrix, x, b);
    for (std::size_t i = 0; i < N; ++i) {
        std::cout << b[i] << " ";
    }
    std::cout << std::endl;
    // MPI_Barrier(MPI_COMM_WORLD);
    SearchX(matrix, x, b);
    std::cout << "Hello from " << rank << std::endl;
    if (rank == 0) {
        endTime = MPI_Wtime();
        std::cout << "Time: " << endTime - startTime << std::endl;
    }
    MPI_Finalize();
    for (std::size_t i = 0; i < lrows; ++i) {
        delete[] matrix[i];
    }
    delete[] matrix;
    delete[] x;
    return 0;
}