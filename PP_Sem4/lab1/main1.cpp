#include <mpi.h>

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

using matrix_cont = std::vector<std::vector<double>>;

const std::size_t N = 17;
const double e = 0.00001;
const double t = 0.01;

std::size_t FindLraws(int size, int rank) {
    return (N / size + (N % size > rank));
}

std::size_t FindBegin(int size, int rank) {
    std::size_t begin = 0;
    for (std::size_t i; i < rank; ++i) {
        begin += (N / size) + (N % size > i);
    }
    return begin;
}

matrix_cont MatrixBuilder(int size, int rank) {
    std::size_t matrixBegin = FindBegin(size, rank);
    std::size_t lraws = FindLraws(size, rank);
    matrix_cont matrix(lraws, std::vector<double>(N));
    for (std::size_t i = 0; i < lraws; ++i) {
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
    for (std::size_t i = 0; i < N; ++i) {
        x[i] = rand() % 10;
    }
}

void AMultX(matrix_cont& matrix, double* x, double* b) {
    int size, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    std::size_t lraws = FindLraws(size, rank);
    std::size_t matrixBegin = FindBegin(size, rank);
    for (std::size_t i = 0; i < lraws; ++i) {
        for (std::size_t j = 0; j < N; ++j) {
            b[i + matrixBegin] = x[i + matrixBegin] * matrix[i][j];
            MPI_Bcast(&b[i + matrixBegin], 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        }
    }
}

void SubB(double* x1, double* b) {
    for (std::size_t i = 0; i < N; ++i) {
        x1[i] -= b[i];
    }
}

void MultT(double* x1) {
    for (std::size_t i = 0; i < N; ++i) {
        x1[i] *= t;
    }
}

void SubXX(double* x0, double* x1) {
    for (std::size_t i = 0; i < N; ++i) {
        x0[i] -= x1[i];
    }
}

double Module(double* u) {
    double a = 0;
    for (std::size_t i = 0; i < N; ++i) {
        a += u[i];
    }
    return a;
}

void SearchX(matrix_cont& matrix, double* x, double* b) {
    double x0[N];
    double x1[N];
    double u;
    int size, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    std::size_t lraws = FindLraws(size, rank);
    std::size_t matrixBegin = FindBegin(size, rank);
    AMultX(matrix, x0, x1);
    SubB(x1, b);
    do {
        MultT(x1);
        SubXX(x0, x1);
        AMultX(matrix, x0, x1);
        SubB(x1, b);
        u = Module(x1);
    } while ((u / Module(b)) < e);
    std::cout << "Vector found" << std::endl;
}

int main(int argc, char** argv) {
    int rank, size;
    // std::vector<int> x(N);
    double x[N];
    double b[N];
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    matrix_cont matrix = MatrixBuilder(size, rank);
    if (rank == 0) {
        RandomVectorX(x);
    }
    MPI_Bcast(x, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    AMultX(matrix, x, b);
    SearchX(matrix, x, b);
    MPI_Finalize();
    return 0;
}
