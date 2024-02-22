#include <mpi.h>

#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

using matrix_cont = std::vector<std::vector<double>>;

const std::size_t N = 17;
const double e = 0.00001;
const double t = 0.01;

std::size_t FindLrows(int size, int rank) {
    return (N / size + (N % size > rank));
}

std::size_t FindBegin(int size, int rank) {
    std::size_t begin = 0;
    for (std::size_t i = 0; i < rank; ++i) {
        begin += (N / size) + (N % size > i);
    }
    return begin;
}

matrix_cont MatrixBuilder(int size, int rank) {
    std::size_t matrixBegin = FindBegin(size, rank);
    std::size_t lrows = FindLrows(size, rank);
    matrix_cont matrix(lrows, std::vector<double>(N));
    for (std::size_t i = 0; i < lrows; ++i) {
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
    // MPI_Barrier(MPI_COMM_WORLD);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    std::size_t lrows = FindLrows(size, rank);
    std::size_t matrixBegin = FindBegin(size, rank);
    for (std::size_t i = 0; i < lrows; ++i) {
        for (std::size_t j = matrixBegin; j < matrixBegin + lrows; ++j) {
            b[j] = x[j] * matrix[i][j];
            // MPI_Barrier(MPI_COMM_WORLD);
            MPI_Bcast(&b[j], 1, MPI_DOUBLE, rank, MPI_COMM_WORLD);
            // std::cout << "1" << std::endl;
            // std::cout << "2" << std::endl;
        }
    }
    // MPI_Barrier(MPI_COMM_WORLD);
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
        a += (u[i] * u[i]);
    }
    return sqrt(a);
}

void SearchX(matrix_cont& matrix, double* x, double* b) {
    double x0[N];
    double x1[N];
    double u;
    int size, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    std::size_t lrows = FindLrows(size, rank);
    std::size_t matrixBegin = FindBegin(size, rank);
    AMultX(matrix, x0, x1);
    SubB(x1, b);
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
        MultT(x1);
        SubXX(x0, x1);
        AMultX(matrix, x0, x1);
        // MPI_Barrier(MPI_COMM_WORLD);
        SubB(x1, b);
        u = Module(x1);
        // std::cout << u << std::endl;
        u = u / Module(b);
        // std::cout << u << std::endl;
    } while (u > e);
    std::cout << rank << " Vector found" << std::endl;
}

int main(int argc, char** argv) {
    int rank, size;
    // std::vector<int> x(N);
    double x[N];
    double b[N];
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Barrier(MPI_COMM_WORLD);
    matrix_cont matrix = MatrixBuilder(size, rank);
    if (rank == 0) {
        RandomVectorX(x);
    }
    MPI_Bcast(x, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    for (std::size_t i = 0; i < N; ++i) {
        std::cout << x[i] << " ";
    }
    std::cout << std::endl;
    // MPI_Barrier(MPI_COMM_WORLD);
    AMultX(matrix, x, b);
    // MPI_Barrier(MPI_COMM_WORLD);
    std::cout << "Hello from " << rank << std::endl;
    SearchX(matrix, x, b);
    MPI_Finalize();
    return 0;
}
