#include <iostream>
#include <mpi.h>

const std::size_t N1 = 4;
const std::size_t N2 = 4;
const std::size_t N3 = 8;

double* MatrixBuilder(std::size_t n1, std::size_t n2) {
    double* matrix = new double[n1 * n2];
    std::fill_n(matrix, n1*n2, 1.0);
    return matrix;
}

double* MultMatrix(double* matrixA, double* matrixB, std::size_t Ar, std::size_t Bc) {
    double* partMatrixC = new double[Ar * Bc];
    for (std::size_t i = 0; i < Ar; ++i) {
        for (std::size_t j = 0; j < Bc; ++j) {
            partMatrixC[i * Bc + j] = 0;
            for (std::size_t k = 0; k < N2; ++k) {
                partMatrixC[i * Bc + j] += matrixA[i * N2 + k] * matrixB[k * N3 + j];
            }
        }
    }
    return partMatrixC;
}

int main(int argc, char** argv) {
    double startTime, endTime;
    int rank, size;
    double* matrixA;
    double* matrixB;
    double* matrixC;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size); 
    if (rank == 0) {
        matrixA = MatrixBuilder(N1, N2);
        matrixB = MatrixBuilder(N2, N3);
        matrixC = MultMatrix(matrixA, matrixB, N1, N3);
    }
    for (std::size_t i = 0; i < N1; ++i) {
        for (std::size_t j = 0; j < N3; ++j) {
            std::cout << matrixC[i * N3 + j] << " ";
        }
        std::cout << std::endl;
    }
    // for (std::size_t i = 0; i < N2; ++i) {
    //     for (std::size_t j = 0; j < N3; ++j) {
    //         std::cout << matrixB[i * N3 + j] << " ";
    //     }
    //     std::cout << std::endl;
    // }
    return 0;
}
