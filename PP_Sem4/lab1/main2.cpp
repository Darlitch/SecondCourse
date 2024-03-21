#include <mpi.h>

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>

const std::size_t N = 1500;
const double e = 0.000001;
double t = 0.0001;

std::size_t FindLrows(int size, int rank) {
    return (N / size + ((int)(N % size) > rank));
}

std::size_t FindBegin(int size, int rank) {
    std::size_t begin = 0;
    for (int i = 0; i < rank; ++i) {
        begin += ((N / size) + (int)((int)(N % size) > i));
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
    std::size_t lrows = FindLrows(size, rank);
    srand(time(NULL));
    for (std::size_t i = 0; i < lrows; ++i) {
        x[i] = ((double)rand()/RAND_MAX);
        // std::cout << x[i] << " ";
    }
}

int* FindLrowsV(int size) {
    int* array = new int[size];
    for (int j = 0; j < size; ++j) {
        array[j] = (N / size) + (int)((int)(N % size) > j);
    }
    return array;
}

int* FindBeginV(int size) {
    int* array = new int[size];
    for (int j = 0; j < size; j++) {
        array[j] = FindBegin(size, j);
    }
    return array;
}

void AMultX(double** matrix, double* xOld, double* b) {
    int size, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    std::size_t lrows = FindLrows(size, rank);
    double* x = new double[lrows];
    int* lrowsV = FindLrowsV(size);
    int* beginV = FindBeginV(size);

    std::copy_n(xOld, lrows, x);

    for (int p = 0; p < size; ++p) {               // полный подсчёт по всем процессам
        for (std::size_t i = 0; i < lrows; ++i) {  // проходимся по всем строкам матрицы
            for (int j = 0; j < lrowsV[rank]; ++j) {
                b[i] += x[j] * matrix[i][j + beginV[rank]];
            }
        }

        double* x1 = new double[lrowsV[(rank + 1) % size]]();
        MPI_Sendrecv(x, lrowsV[rank], MPI_DOUBLE, (rank + size - 1) % size, 123, x1, lrowsV[(rank + 1) % size], MPI_DOUBLE, (rank + 1) % size, 123, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::swap(x, x1);
        std::rotate(lrowsV, lrowsV + 1, lrowsV + size);
        std::rotate(beginV, beginV + 1, beginV + size);
        delete[] x1;
    }

    delete[] lrowsV;
    delete[] beginV;
}

void Sub(double* x, double* b, std::size_t lrows) {
    for (std::size_t i = 0; i < lrows; ++i) {
        x[i] -= b[i];
    }
}

void MultT(double* x, std::size_t lrows) {
    for (std::size_t i = 0; i < lrows; ++i) {
        x[i] *= t;
    }
}

double Module(double* u, std::size_t& lrows, int& size) {
    double a = 0;
    double sum = 0;
    for (std::size_t i = 0; i < lrows; ++i) {
        a += (u[i] * u[i]);
    }
    if (size == 1) {
        return sqrt(a);
    }
    MPI_Allreduce(&a, &sum, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    return sqrt(sum);
}

void SearchX(double** matrix, double* x, double* b) {
    std::size_t count = 0;
    double u = 0;
    double bNorm = 0;
    double uOld = 0;
    int size, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    std::size_t lrows = FindLrows(size, rank);
    double* x0 = new double[lrows]();
    double* x1 = new double[lrows]();
    bNorm = Module(b, lrows, size);
    AMultX(matrix, x0, x1);
    Sub(x1, b, lrows);
    do {
        if (u > uOld && (count % 5 == 0)) {
            t = (-t);
        }
        uOld = u;
        MultT(x1, lrows);
        Sub(x0, x1, lrows);
        AMultX(matrix, x0, x1);
        Sub(x1, b, lrows);
        u = Module(x1, lrows, size);
        u = u / bNorm;
        count++;
        // std::cout << "u:" << u << std::endl;
    } while (u > e);
    Sub(x0, x, lrows);
    bNorm = Module(x0, lrows, size);
    if (rank == 0) {
        std::cout << "u:" << u << std::endl;
        std::cout << "NormalX: " << bNorm << std::endl;
        std::cout << rank << " Vector found" << std::endl;
    }
}

int main(int argc, char** argv) {
    double startTime, endTime;
    int size, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    std::size_t lrows = FindLrows(size, rank);
    double b[lrows] = {0};
    double* x = new double[lrows]();

    double** matrix = MatrixBuilder(size, rank);
    if (rank == 0) {
        startTime = MPI_Wtime();
    }
    RandomVectorX(x);
    AMultX(matrix, x, b);
    // for (std::size_t i = 0; i < lrows; ++i) {
    //     std::cout << b[i] << ' ';
    // }
    // std::cout << std::endl;
    SearchX(matrix, x, b);
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