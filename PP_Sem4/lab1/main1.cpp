#include <mpi.h>

#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

using matrix_cont = std::vector<std::vector<double>>;

const std::size_t N = 6800;
const double e = 0.000001;

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

void AMultX(matrix_cont& matrix, double* x, double* b) {
    int size, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    std::size_t lrows = FindLrows(size, rank);
    std::size_t matrixBegin = FindBegin(size, rank);
    int* lrowsV = FindLrowsV(size);
    int* beginV = FindBeginV(size);
    double b1[lrows];
    // std::cout << "lrows: " << lrows << std::endl;
    for (std::size_t i = 0; i < lrows; ++i) {
        b1[i] = b[i + matrixBegin];
    }
    // for (std::size_t i = 0; i < lrows; ++i) {
    //     std::cout << b1[i] << " ";
    // }
    // std::cout << std::endl;
    for (std::size_t i = 0; i < lrows; ++i) {
        for (std::size_t j = matrixBegin; j < matrixBegin + lrows; ++j) {
            b1[i] += x[i + matrixBegin] * matrix[i][j];
            // MPI_Bcast(&b[j], 1, MPI_DOUBLE, rank, MPI_COMM_WORLD);
            // MPI_Allgatherv(b1, lrows, MPI_DOUBLE, b, lrowsV, beginV, MPI_DOUBLE, MPI_COMM_WORLD);
        }
    }
    MPI_Allgatherv(b1, lrows, MPI_DOUBLE, b, lrowsV, beginV, MPI_DOUBLE, MPI_COMM_WORLD);
}

void SubB(double* x1, double* b) {
    for (std::size_t i = 0; i < N; ++i) {
        x1[i] -= b[i];
    }
}

void MultT(double* x1, double t) {
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
    double uOld;
    double t = 0.01;
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
        MultT(x1, t);
        SubXX(x0, x1);
        AMultX(matrix, x0, x1);
        SubB(x1, b);
        u = Module(x1);
        // std::cout << u << std::endl;
        u = u / Module(b);
        if (u - uOld > 100) {
            t = -t;
        }
        uOld = u;
        // std::cout << u << std::endl;
    } while (u > e);
    if (rank == 0) {
        for (std::size_t i = 0; i < 10; ++i) {
            std::cout << x1[i] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << rank << " Vector found" << std::endl;
}

int main(int argc, char** argv) {
    double startTime, endTime;
    int rank, size;
    double x[N];
    double b[N] = {0};
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    matrix_cont matrix = MatrixBuilder(size, rank);
    if (rank == 0) {
        startTime = MPI_Wtime();
        RandomVectorX(x);
    }
    MPI_Bcast(x, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        for (std::size_t i = 0; i < 10; ++i) {
            std::cout << x[i] << " ";
        }
        std::cout << std::endl;
    }
    AMultX(matrix, x, b);
    // for (std::size_t i = 0; i < N; ++i) {
    //     std::cout << b[i] << " ";
    // }
    // std::cout << std::endl;
    SearchX(matrix, x, b);
    if (rank == 0) {
        endTime = MPI_Wtime();
        std::cout << "Time: " << endTime - startTime << std::endl;
    }
    MPI_Finalize();
    return 0;
}
