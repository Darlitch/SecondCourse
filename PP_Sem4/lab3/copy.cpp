#include <malloc.h>
#include <math.h>
#include <mpi.h>
#include <stdio.h>

#include <ctime>
#include <iostream>

int N1 = 4;
int N2 = 4;
int N3 = 4;

int* Recvcounts_for_matrixA(int N, int size) {
    int* array = new int[size];
    for (int j = 0; j < size; j++) {
        array[j] = ((N / size) + (int)(N % size > j)) * N2;
    }
    return array;
}

int OffSet(int size, int rank, int N) {
    int count = 0;
    for (int i = 0; i < rank; i++) {
        count += (N / size) + (int)(N % size > i);
    }
    return count;
}

int* Displs_for_matrixA(int N, int size) {
    int* array = new int[size];
    for (int j = 0; j < size; j++) {
        array[j] = OffSet(size, j, N) * N2;
    }
    return array;
}

double* Build_matrix_C(double* part_C, int ABlockSize, int BBlockSize, int rank, int* processesOnYandX) {
    double* C = nullptr;
    MPI_Datatype block, blocktype;
    MPI_Type_vector(ABlockSize, BBlockSize, N3, MPI_DOUBLE, &block);
    MPI_Type_commit(&block);

    MPI_Type_create_resized(block, 0, BBlockSize * sizeof(double), &blocktype);
    MPI_Type_commit(&blocktype);

    int* displ = new int[processesOnYandX[0] * processesOnYandX[1]]{0};
    int* rcount = new int[processesOnYandX[0] * processesOnYandX[1]]{0};
    int BlockCount = 0;
    int BlockSize = ABlockSize * BBlockSize;

    for (int i = 0; i < processesOnYandX[0]; i++) {
        for (int j = 0; j < processesOnYandX[1]; j++) {
            rcount[i * processesOnYandX[1] + j] = 1;
            displ[i * processesOnYandX[1] + j] = BlockCount;
            BlockCount += 1;
        }
        BlockCount += ((N1 / processesOnYandX[0]) + (int)(N1 % processesOnYandX[0] > i) - 1) * processesOnYandX[1];
    }

    if (rank == 0) {
        C = new double[N1 * N3];
    }

    MPI_Gatherv(part_C, BlockSize, MPI_DOUBLE, C,
                rcount, displ, blocktype, 0, MPI_COMM_WORLD);
    delete[] displ;
    delete[] rcount;
    return C;
}

void New_MPI_Comm(int* processesOnXandY, int rank, int size, MPI_Comm* comm_cart, MPI_Comm* comm_rows, MPI_Comm* comm_colm) {
    const int period[2] = {false, false};
    MPI_Cart_create(MPI_COMM_WORLD, 2, processesOnXandY, period, true, comm_cart);
    int* remain_dims = new int[2];
    remain_dims[1] = true;
    remain_dims[0] = false;
    MPI_Cart_sub(*comm_cart, remain_dims, comm_colm);
    remain_dims[1] = false;
    remain_dims[0] = true;
    MPI_Cart_sub(*comm_cart, remain_dims, comm_rows);
}

void Multiplication(double* part_A, int lenPart_A, double* part_B, int lenPart_B, double* part_C) {
    for (int i = 0; i < lenPart_A; i++) {
        for (int k = 0; k < N2; k++) {
            for (int j = 0; j < lenPart_B; j++) {
                part_C[i * lenPart_B + j] += part_A[i * N2 + k] * part_B[k * lenPart_B + j];
            }
        }
    }
}

void Check(int rank, double* A, double* B, double* C) {
    if (rank == 0) {
        double* proof_C = new double[N3 * N1]{0};
        for (int i = 0; i < N1; i++) {
            for (int j = 0; j < N3; j++) {
                for (int k = 0; k < N2; k++) {
                    proof_C[i * N3 + j] += A[i * N2 + k] * B[k * N3 + j];
                }
            }
        }
        bool corect = true;
        for (int i = 0; i < N3; i++) {
            for (int j = 0; j < N1; j++) {
                if (C[i * N3 + j] - proof_C[i * N3 + j] != 0) {
                    corect = false;
                }
            }
        }
        printf("corect:(%d)\n", (int)corect);
        delete[] proof_C;
    }
}

void Create_Matrix_Part_A(double* part_A, double* A, int* processesOnYandX, int lenPart_A, MPI_Comm* comm_rows) {
    int* recvcounts_A = Recvcounts_for_matrixA(N1, processesOnYandX[0]);
    int* displs_A = Displs_for_matrixA(N1, processesOnYandX[0]);

    MPI_Scatterv(A, recvcounts_A, displs_A, MPI_DOUBLE, part_A, lenPart_A * N2, MPI_DOUBLE, 0, *comm_rows);

    delete[] recvcounts_A;
    delete[] displs_A;
}

void Create_Matrix_Part_B(double* part_B, double* B, int* processesOnYandX, int lenPart_B, MPI_Comm* comm_colms) {
    MPI_Datatype col, coltype;

    MPI_Type_vector(N2, lenPart_B, N3, MPI_DOUBLE, &col);
    MPI_Type_commit(&col);
    MPI_Type_create_resized(col, 0, (lenPart_B) * sizeof(double), &coltype);
    MPI_Type_commit(&coltype);

    MPI_Scatter(B, 1, coltype, part_B, lenPart_B * N2, MPI_DOUBLE, 0, *comm_colms);
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Comm* comm_cart = new MPI_Comm;
    MPI_Comm* comm_rows = new MPI_Comm;
    MPI_Comm* comm_colms = new MPI_Comm;

    double* A = nullptr;
    double* B = nullptr;

    // if (rank == 0) {
    B = new double[N3 * N2];
    for (size_t i = 0; i < N3; i++) {
        for (size_t j = 0; j < N2; j++) {
            B[i * N2 + j] = rand() % 10;
        }
    }

    A = new double[N1 * N2];
    for (size_t i = 0; i < N1; i++) {
        for (size_t j = 0; j < N2; j++) {
            A[i * N2 + j] = rand() % 10;
        }
    }
    //}

    int* processesOnYandX = new int[2]{0};

    MPI_Dims_create(size, 2, processesOnYandX);

    New_MPI_Comm(processesOnYandX, rank, size, comm_cart, comm_rows, comm_colms);

    // ���������� ������� A
    int rank_row;
    MPI_Comm_rank(*comm_rows, &rank_row);
    int lenPart_A = (N1 / processesOnYandX[0]) + (int)(N1 % processesOnYandX[0] > rank_row);
    double* part_A = new double[lenPart_A * N2];
    Create_Matrix_Part_A(part_A, A, processesOnYandX, lenPart_A, comm_rows);

    // ���������� ������� B
    int rank_colm;
    MPI_Comm_rank(*comm_colms, &rank_colm);
    int lenPart_B = (N3 / processesOnYandX[1]) + (int)(N3 % processesOnYandX[1] > rank_colm);
    double* part_B = new double[lenPart_B * N2];
    Create_Matrix_Part_B(part_B, B, processesOnYandX, lenPart_B, comm_colms);

    // �������� ������ ������� C
    double* part_C = new double[lenPart_B * lenPart_A]{0};
    Multiplication(part_A, lenPart_A, part_B, lenPart_B, part_C);

    // �������� ������� C
    double* C = Build_matrix_C(part_C, lenPart_A, lenPart_B, rank, processesOnYandX);

    Check(rank, A, B, C);

    std::cout << "A_Part in process " << rank << "\n";
    for (int i = 0; i < lenPart_A; i++) {
        for (int j = 0; j < N2; j++) {
            std::cout << part_A[i * N2 + j] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";

    std::cout << "B_Part in process " << rank << "\n";
    for (int i = 0; i < N2; i++) {
        for (int j = 0; j < lenPart_B; j++) {
            std::cout << part_B[i * lenPart_B + j] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";

    delete[] A;
    delete[] B;
    delete[] C;
    delete[] part_A;
    delete[] part_B;
    delete[] part_C;
    delete[] processesOnYandX;
    delete comm_cart;
    delete comm_rows;
    delete comm_colms;

    MPI_Finalize();
}
