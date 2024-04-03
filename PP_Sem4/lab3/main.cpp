#include <mpi.h>

#include <iostream>

const std::size_t N1 = 3;
const std::size_t N2 = 2;
const std::size_t N3 = 6;

double* MatrixBuilder(std::size_t n1, std::size_t n2) {
    double* matrix = new double[n1 * n2];
    std::fill_n(matrix, n1 * n2, 3.0);
    return matrix;
}

double* MultMatrix(double* partA, double* partB, std::size_t rowsA, std::size_t colsB) {
    double* partMatrixC = new double[rowsA * colsB];
    for (std::size_t i = 0; i < rowsA; ++i) {
        for (std::size_t j = 0; j < colsB; ++j) {
            partMatrixC[i * colsB + j] = 0;
            for (std::size_t k = 0; k < N2; ++k) {
                partMatrixC[i * colsB + j] += partA[i * N2 + k] * partB[k * N3 + j];
            }
        }
    }
    return partMatrixC;
}

void CreateComm(int* dims, MPI_Comm* distr_rows_comm, MPI_Comm* distr_cols_comm) {
    int periods[2] = {0};
    MPI_Comm* comm_cart = new MPI_Comm;
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, true, comm_cart);
    int distrDims[2] = {1, 0};
    MPI_Cart_sub(*comm_cart, distrDims, distr_rows_comm);
    distrDims[0] = 0;
    distrDims[1] = 1;
    MPI_Cart_sub(*comm_cart, distrDims, distr_cols_comm);
    delete comm_cart;
}

double* ScatterA(MPI_Comm* distr_comm, double* m, int* dims, int lenPart) {
}

double* ScatterB(MPI_Comm* distr_comm, double* m, int* dims, int lenPart) {
}

int main(int argc, char** argv) {
    double startTime, endTime;
    int* dims = new int[2]{0};
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
    }

    startTime = MPI_Wtime();
    MPI_Dims_create(size, 2, dims);
    MPI_Comm* distr_rows_comm = new MPI_Comm;
    MPI_Comm* distr_cols_comm = new MPI_Comm;
    CreateComm(dims, distr_rows_comm, distr_cols_comm);

    int drows_rank, dcols_rank;
    MPI_Comm_rank(*distr_rows_comm, &drows_rank);
    int lenPartA = (N1 / dims[0]) + (int)(N1 % dims[0] > drows_rank);
    double* partA = ScatterA(distr_rows_comm, matrixA, dims, lenPartA);
    MPI_Comm_rank(*distr_cols_comm, &dcols_rank);
    int lenPartB = (N3 / dims[1]) + (int)(N3 % dims[1] > dcols_rank);
    double* partB = ScatterB(distr_cols_comm, matrixB, dims, lenPartB);

    double* partC = MultMatrix(partA, partB, lenPartA, lenPartB);

    // matrixC = MultMatrix(matrixA, matrixB, N1, N3);
    if (rank == 0) {
        endTime = MPI_Wtime();
        std::cout << "Time: " << endTime - startTime << std::endl;
        delete matrixA;
        delete matrixB;
    }
    // for (std::size_t i = 0; i < N1; ++i) {
    //     for (std::size_t j = 0; j < N3; ++j) {
    //         std::cout << matrixC[i * N3 + j] << " ";
    //     }
    //     std::cout << std::endl;
    // }
    // for (std::size_t i = 0; i < N2; ++i) {
    //     for (std::size_t j = 0; j < N3; ++j) {
    //         std::cout << matrixB[i * N3 + j] << " ";
    //     }
    //     std::cout << std::endl;
    // }
    // delete[] matrixC;
    delete distr_rows_comm;
    delete distr_cols_comm;
    // delete[] partA;
    // delete[] partB;
    // delete[] partC;
    delete[] dims;
    MPI_Finalize();
    return 0;
}
