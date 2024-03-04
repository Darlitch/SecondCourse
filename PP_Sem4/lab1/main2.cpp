#include <mpi.h>

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>

const std::size_t N = 100;
const double e = 0.000001;
double t = 0.001;

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
    for (std::size_t i = 0; i < lrows; ++i) {
        x[i] = rand() % 10;
    }
}

int* FindLrowsV(int size) {
    int* array = new int[size];
    for (int j = 0; j < size; ++j) {
        array[j] = (N / size) + (int)(N % size > j);
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
    std::size_t matrixBegin = FindBegin(size, rank);
    double* x = new double[lrows];
    int* lrowsV = FindLrowsV(size);
    int* beginV = FindBeginV(size);

    std::copy_n(xOld, lrows, x);

    for (std::size_t p = 0; p < size; ++p) {       // полный подсчёт по всем процессам
        for (std::size_t i = 0; i < lrows; ++i) {  // проходимся по всем строкам матрицы
            for (std::size_t j = 0; j < lrowsV[rank]; ++j) {
                // std::cout << "rank: " << rank << " Begin: " << matrixBegin << std::endl;
                // std::cout << "rank: " << rank << " " << b[i + matrixBegin] << " += " << x[j - matrixBegin] << " * " << matrix[i][j] << std::endl;
                b[i] += x[j] * matrix[i][j + beginV[rank]];
            }
            // std::cout << "rank: " << rank << " " << b[i + matrixBegin] << std::endl;
        }
        // delete[] x1;
        double* x1 = new double[lrowsV[(rank + 1) % size]]();
        // std::cout << "1" << std::endl;
        std::swap(x, x1);
        // MPI_Sendrecv(x, lrows, MPI_DOUBLE, rank, 123, x1, lrowsV[(rank + 1) % size], MPI_DOUBLE, (rank + 1) % size, 123, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Sendrecv(x, lrowsV[rank], MPI_DOUBLE, (rank + size - 1) % size, 123, x1, lrowsV[(rank + 1) % size], MPI_DOUBLE, (rank + 1) % size, 123, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::swap(x, x1);
        // delete[] x1;
        // for (std::size_t i = 0; i < lrowsV[(rank + 1) % size]; ++i) {
        //     std::cout << " x1: " << x[i];
        // }
        // std::cout << " p: " << p << std::endl;
        // lrows = lrowsV[(rank + p + 1) % size - 1];
        std::rotate(lrowsV, lrowsV + 1, lrowsV + size);
        std::rotate(beginV, beginV + 1, beginV + size);
        // delete[] x1;
    }
    delete[] lrowsV;
}

void Sub(double* x, double* b, std::size_t lrows) {
    for (std::size_t i = 0; i < lrows; ++i) {
        x[i] -= b[i];
    }
}

void MultT(double* x, std::size_t lrows) {
    for (std::size_t i = 0; i < lrows; ++i) {
        // std::cout << "x1:" << x[i] << " " << std::endl;
        x[i] *= t;
        // std::cout << "xt:" << x[i] << " " << std::endl;
    }
}

double Module(double* u, std::size_t& lrows, int& rank, int& size) {
    double a = 0;
    double sum = 0;  // обязательно ли отдельную переменную?
    for (std::size_t i = 0; i < lrows; ++i) {
        a += (u[i] * u[i]);
        // std::cout << "a:" << a << " " << std::endl;
    }
    if (size == 1) {
        return sqrt(a);
    }
    MPI_Allreduce(&a, &sum, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    return sqrt(sum);
}

void SearchX(double** matrix, double* x, double* b) {
    std::size_t count = 0;
    double xFinal[N] = {0};
    double u = 0;
    double uOld = 0;
    int size, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    std::size_t lrows = FindLrows(size, rank);
    std::size_t matrixBegin = FindBegin(size, rank);
    int* lrowsV = FindLrowsV(size);
    int* beginV = FindBeginV(size);
    double x0[lrows] = {0};
    double x1[lrows] = {0};

    // for (std::size_t i = 0; i < lrows; ++i) {
    //     std::cout << x1[i] << " ";
    // }
    // std::cout << std::endl;
    AMultX(matrix, x0, x1);
    Sub(x1, b, lrows);
    // for (std::size_t i = 0; i < lrows; ++i) {
    //     std::cout << x1[i] << " ";
    // }
    // std::cout << std::endl;
    do {
        // std::cout << "Hello from " << rank << std::endl;
        // for (std::size_t i = 0; i < N; ++i) {
        //     std::cout << x[i] << " ";
        // }
        // std::cout << std::endl;
        // for (std::size_t i = 0; i < N; ++i) {
        //     std::cout << x1[i] << " - ";
        // }
        // std::cout << std::endl;
        if (u > uOld && (count % 5 == 0)) {
            t = (-t);
        }
        uOld = u;
        MultT(x1, lrows);
        Sub(x0, x1, lrows);
        AMultX(matrix, x0, x1);
        Sub(x1, b, lrows);
        u = Module(x1, lrows, rank, size);
        // std::cout << u << std::endl;
        u = u / Module(b, lrows, rank, size);
        // if ((u - uOld) > 10) {
        //     t = -t;
        // }
        // uOld = u;
        // std::cout << "u:" << u << std::endl;
        // getchar();
        count++;
    } while (u > e);
    std::cout << "u:" << u << std::endl;
    MPI_Gatherv(x0, lrows, MPI_DOUBLE, xFinal, lrowsV, beginV, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    // if (rank == 0) {
    //     for (std::size_t i = 0; i < N; ++i) {
    //         std::cout << xFinal[i] << " ";
    //     }
    //     std::cout << std::endl;
    // }
    Sub(x0, x, lrows);
    u = Module(x0, lrows, rank, size);
    std::cout << "NormalX: " << u << std::endl;
    std::cout << rank << " Vector found" << std::endl;
}

int main(int argc, char** argv) {
    double startTime, endTime;
    int rank, size;
    // double* x = new double[N]();
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
    // for (std::size_t i = 0; i < N; ++i) {
    //     std::cout << x[i] << " ";
    // }
    // std::cout << std::endl;
    for (std::size_t i = 0; i < lrows; ++i) {
        std::cout << x[i] << " ";
    }
    std::cout << std::endl;
    AMultX(matrix, x, b);
    std::cout << "b ";
    for (std::size_t i = 0; i < lrows; ++i) {
        std::cout << b[i] << " ";
    }
    std::cout << std::endl;
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
    // delete[] x;
    return 0;
}