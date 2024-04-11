#include <mpi.h>

#include <iostream>

const std::size_t N1 = 16;
const std::size_t N2 = 16;
const std::size_t N3 = 16;

double* MatrixBuilder(const std::size_t n1, const std::size_t n2) {
    double* matrix = new double[n1 * n2];
    std::fill_n(matrix, n1 * n2, 3.0);
    return matrix;
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

int* Sendcounts(const std::size_t n1, const std::size_t n2, const int size) {
    int* sendcounts = new int[size];
    for (std::size_t r = 0; r < size; ++r) {
        sendcounts[r] = ((n1 / size) + (int)(n1 % size > r)) * n2;
    }
    return sendcounts;
}

std::size_t OffSet(const std::size_t n, const int size, const int rank) {
    std::size_t offset = 0;
    for (int i = 0; i < rank; ++i) {
        offset += (n / size) + (int)(n % size > i);
    }
    return offset;
}

int* Displs(const std::size_t n1, const std::size_t n2, const int size) {
    int* displs = new int[size];
    for (std::size_t r = 0; r < size; ++r) {
        displs[r] = OffSet(n1, size, r) * n2;
    }
    return displs;
}

double* ScatterRows(MPI_Comm* distr_comm, double* m, int* dims, const int len) {
    double* partM = new double[len * N2];
    int* sendcounts = Sendcounts(N1, N2, dims[0]);
    int* displs = Displs(N1, N2, dims[0]);
    MPI_Scatterv(m, sendcounts, displs, MPI_DOUBLE, partM, len * N2, MPI_DOUBLE, 0, *distr_comm);
    delete[] sendcounts;
    delete[] displs;
    return partM;
}

double* ScatterCols(MPI_Comm* distr_comm, double* m, int* dims, const int len) {
    double* partM = new double[len * N2];
    MPI_Datatype cols, colsFinal;
    MPI_Type_vector(N2, len, N3, MPI_DOUBLE, &cols);
    MPI_Type_commit(&cols);
    MPI_Type_create_resized(cols, 0, len * sizeof(double), &colsFinal);
    MPI_Type_commit(&colsFinal);
    MPI_Scatter(m, 1, colsFinal, partM, len * N2, MPI_DOUBLE, 0, *distr_comm);
    MPI_Type_free(&colsFinal);
    MPI_Type_free(&cols);
    return partM;
}

double* MultMatrix(double* partA, double* partB, const std::size_t rowsA, const std::size_t colsB) {
    double* partMatrixC = new double[rowsA * colsB];
    for (std::size_t i = 0; i < rowsA; ++i) {
        for (std::size_t j = 0; j < colsB; ++j) {
            partMatrixC[i * colsB + j] = 0;
            for (std::size_t k = 0; k < N2; ++k) {
                partMatrixC[i * colsB + j] += partA[i * N2 + k] * partB[k * colsB + j];
            }
        }
    }
    return partMatrixC;
}

int* DisplsC(int* dims) {
    int countPart = 0;
    int* displs = new int[dims[0] * dims[1]]{0};
    for (int i = 0; i < dims[0]; ++i) {
        for (int j = 0; j < dims[1]; ++j) {
            displs[i * dims[1] + j] = countPart;
            countPart += 1;
        }
        countPart += ((N1 / dims[0]) + (int)(N1 % dims[0] > i) - 1) * dims[1];
    }
    return displs;
}

int* SendcountsC(int* dims) {
    int* sendcounts = new int[dims[0] * dims[1]]{0};
    for (int i = 0; i < dims[0]; ++i) {
        for (int j = 0; j < dims[1]; ++j) {
            sendcounts[i * dims[1] + j] = 1;
        }
    }
    return sendcounts;
}

double* GatherC(int* dims, const int sizeA, const int sizeB, double* partC) {
    int rank;
    double* mC;
    MPI_Datatype part, partFinale;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Type_vector(sizeA, sizeB, N3, MPI_DOUBLE, &part);
    MPI_Type_commit(&part);
    MPI_Type_create_resized(part, 0, sizeB * sizeof(double), &partFinale);
    MPI_Type_commit(&partFinale);

    int* displs = DisplsC(dims);
    int* sendcounts = SendcountsC(dims);
    if (rank == 0) {
        mC = new double[N1 * N3];
    }
    MPI_Gatherv(partC, sizeA * sizeB, MPI_DOUBLE, mC, sendcounts, displs, partFinale, 0, MPI_COMM_WORLD);
    MPI_Type_free(&partFinale);
    MPI_Type_free(&part);
    return mC;
}

void CheckingSolution(double* mA, double* mB, double* mC) {
    double* tempC = MultMatrix(mA, mB, N1, N3);
    bool equal = true;
    for (int i = 0; i < N3; i++) {
        for (int j = 0; j < N1; j++) {
            if (mC[i * N3 + j] - tempC[i * N3 + j] != 0) {
                equal = false;
            }
        }
    }
    printf("Matrix Equally:(%d)\n", (int)equal);
    delete[] tempC;
}

int main(int argc, char** argv) {
    double startTime, endTime;
    int* dims = new int[2]{0};
    int rank, size;
    double* matrixA = nullptr;
    double* matrixB = nullptr;

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
    double* partA = ScatterRows(distr_rows_comm, matrixA, dims, lenPartA);
    MPI_Comm_rank(*distr_cols_comm, &dcols_rank);
    int lenPartB = (N3 / dims[1]) + (int)(N3 % dims[1] > dcols_rank);
    double* partB = ScatterCols(distr_cols_comm, matrixB, dims, lenPartB);

    double* partC = MultMatrix(partA, partB, lenPartA, lenPartB);

    double* matrixC = GatherC(dims, lenPartA, lenPartB, partC);

    // matrixC = MultMatrix(matrixA, matrixB, N1, N3);
    // if (rank == 0) {
        CheckingSolution(matrixA, matrixB, matrixC);
        endTime = MPI_Wtime();
        std::cout << "Time: " << endTime - startTime << std::endl;
        delete[] matrixA;
        delete[] matrixB;
        delete[] matrixC;
    // }
    delete distr_rows_comm;
    delete distr_cols_comm;
    delete[] partA;
    delete[] partB;
    delete[] partC;
    delete[] dims;
    MPI_Finalize();
    return 0;
}
