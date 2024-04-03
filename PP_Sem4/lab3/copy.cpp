#include <iostream>
#include <mpi.h>
#include <algorithm>
#include <numeric>
#include <array>

    struct TSizes {
        TSizes(int p1, int p2, int N1, int M1, int N2, int M2) {
        sizeA = new int[p1];
        sizeB = new int[p2];
        displsA = new int[p1];
        displsB = new int[p2];

        //size every block
        std::fill(sizeA, sizeA+p1, N1/p1 * M1);
        for(int ost = N1%p1, i = 0; ost != 0; --ost, ++i) {
            sizeA[i] += M1;
        }

        std::fill(sizeB, sizeB+p2, M2/p2 * N2);
        for(int ost = M2%p2, i = 0; ost != 0; --ost, ++i) {
            sizeB[i] += N2;
        }

        //displ every block

        displsA[0] = 0;
        for (int i = 1; i < p1; ++i) {
            displsA[i] = displsA[i-1] + sizeA[i-1];
        }

        displsB[0] = 0;
        for (int i = 1; i < p2; ++i) {
            displsB[i] = displsB[i-1] + sizeB[i-1];
        }
    }
    int *sizeA;
    int *sizeB;
    int *displsA;
    int *displsB;
};

void check(int argc, char** argv, int* ret) {
    int countRank;
    MPI_Comm_size(MPI_COMM_WORLD, &countRank);
    if (argc != 7) throw std::invalid_argument("count argv != 4");
    try {
        ret[0] = std::stoi(argv[1]); //p1
        ret[1] = std::stoi(argv[2]); //p2
        ret[2] = std::stoi(argv[3]); //N1
        ret[3] = std::stoi(argv[4]); //M1
        ret[4] = std::stoi(argv[5]); //N2
        ret[5] = std::stoi(argv[6]); //M2
    } catch (std::invalid_argument ex) {
        throw std::invalid_argument("input parametr is not number");
    } 
    if (ret[0] * ret[1] != countRank) {
        throw std::invalid_argument ("countP1 * count P2 != MPI_COMM_WORLD size");
    }
    if (ret[4] != ret[3] || ret[2] <= 0 ||
    ret[3] <= 0 || ret[4] <= 0 || ret[5] <= 0 ) {
        throw std::invalid_argument("wrong dim matrix");
    }
}

void transposeMatrix(double* matrix, int N, int M) {
    double* transposed = new double[M * N];

    for (int i = 0; i < N; ++ i) {
    for (int j = 0; j < M; ++j) {
    transposed[j * N + i] = matrix[i * M + j];
    }
    }
    std::copy(transposed, transposed + N*M, matrix);
    delete[] transposed;
}

void fillMatrix(double* matrix, int N, int M) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            matrix[i * M + j] = rand() % 100;
            if (rand() % 2) {
                matrix[i * M + j]*= -1;
            }
        }
    }
}

int main(int argc, char** argv) { // argv: p1, p2, N1, M1, N2, M2
    MPI_Init(&argc, &argv);
    srand(21212);
    int myRank, countRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    int *inp = new int[6];

    //check input
    bool error = false;
    if(!myRank) {
    try {
    check(argc, argv, inp);
    } catch (std::invalid_argument ex) {
    std::cout << ex.what() << std::endl;
    error = true;
    }
    }
    MPI_Bcast(&error, 1, MPI_CXX_BOOL, 0, MPI_COMM_WORLD);
    if (error) return 1; 

    // send input args
    MPI_Bcast(inp, 6, MPI_INT, 0, MPI_COMM_WORLD);
    //calc sizes
    TSizes sizes = TSizes(inp[0], inp[1], inp[2], inp[3], inp[4], inp[5]); 
    //create virtual matrix of threads 
    MPI_Comm vMatrix;
    int dimsMatrix[2] = {inp[0], inp[1]};
    int periods[2] = {0, 0};
    MPI_Cart_create(MPI_COMM_WORLD, 2, dimsMatrix, periods, 0, &vMatrix);
    int coords[2];
    MPI_Cart_coords(vMatrix, myRank, 2, coords);

    //std::cout << myRank << " " << coords[0] << " " << coords[1] << std::endl;
    //send

    double *AMatrix;
    if (!myRank) std::cout << "sendA" << std::endl;
    //send A matrix
    { 
    double* AMatrixTMP;
    if (!myRank) {
    AMatrixTMP = new double[inp[2] * inp[3]];
    fillMatrix(AMatrixTMP, inp[2], inp[3]);

    /*std::cout << "Matrix A: " << std::endl;
    for (int i = 0; i < inp[2]; ++i) {
    std::for_each(AMatrixTMP + i*inp[3], AMatrixTMP + i*inp[3] + inp[3], [](double &n){
    std::cout << n << " ";
    });
    std::cout << std::endl; 
    }*/ 
    } 

    int color = (coords[1] == 0) ? 0 : MPI_UNDEFINED;
    int key = coords[0];
    AMatrix = new double[sizes.sizeA[key]];
    MPI_Comm lineComm = MPI_COMM_NULL;
    MPI_Comm_split(MPI_COMM_WORLD, color, key, &lineComm);
    if (lineComm != MPI_COMM_NULL) {
    if (coords[0] == 0) {
    std::copy(AMatrixTMP, AMatrixTMP + sizes.sizeA[key], AMatrix);
    }
    MPI_Scatterv(AMatrixTMP, sizes.sizeA, sizes.displsA,
    MPI_DOUBLE, AMatrix,
    sizes.sizeA[key], MPI_DOUBLE,
    0, lineComm);
    MPI_Comm_free(&lineComm);
    }

    if (!myRank) delete[] AMatrixTMP; 
    }

    double *BMatrix = new double[sizes.sizeB[0]];
    //send B matrix
    if (!myRank) std::cout << "sendB" << std::endl;
    {
    double* BMatrixTMP = nullptr;
    if (!myRank) {
    BMatrixTMP = new double[inp[4] * inp[5]];
    std::cout << std::endl;
    fillMatrix(BMatrixTMP, inp[4], inp[5]);
    /* std::cout << "Matrix B: " << std::endl;
    for (int i = 0; i < inp[4]; ++i) {
    std::for_each(BMatrixTMP + i*inp[5], BMatrixTMP + i*inp[5] + inp[5], [](double &n){
    std::cout << n << " ";
    });
    std::cout << std::endl; 
    } */
    transposeMatrix(BMatrixTMP, inp[4], inp[5]);
    }
    int color = (coords[0] == 0) ? 0 : MPI_UNDEFINED;
    int key = coords[1];
    MPI_Comm lineComm = MPI_COMM_NULL;
    MPI_Comm_split(MPI_COMM_WORLD, color, key, &lineComm);
    if (lineComm != MPI_COMM_NULL) {
    if (coords[1] == 0) {
    std::copy(BMatrixTMP, BMatrixTMP + sizes.sizeB[key], BMatrix);
    }
    MPI_Scatterv(BMatrixTMP, sizes.sizeB, sizes.displsB,
    MPI_DOUBLE, BMatrix,
    sizes.sizeB[key], MPI_DOUBLE,
    0, lineComm);
    MPI_Comm_free(&lineComm);
    }
    if (!myRank) delete[] BMatrixTMP;
    }

    if (!myRank) std::cout << "sendSubA" << std::endl;
    {
    int color = coords[0];
    int key = coords[1];
    MPI_Comm lineComm = MPI_COMM_NULL;
    MPI_Comm_split(MPI_COMM_WORLD, color, key, &lineComm);
    MPI_Bcast(AMatrix, sizes.sizeA[color], MPI_DOUBLE, 0, lineComm);
    MPI_Comm_free(&lineComm);
    }

    //bcast submatrix B
    if (!myRank) std::cout << "sendSubB" << std::endl;
    {
    int color = coords[1];
    int key = coords[0];
    MPI_Comm lineComm = MPI_COMM_NULL;
    MPI_Comm_split(MPI_COMM_WORLD, color, key, &lineComm);
    MPI_Bcast(BMatrix, sizes.sizeB[color], MPI_DOUBLE, 0, lineComm);
    MPI_Comm_free(&lineComm);
    }

    //mult submatrix
    int localN = sizes.sizeA[coords[0]] / inp[3],
    localM = sizes.sizeB[coords[1]] / inp[4];
    double *localResult = new double[localN * localM];

    for (int i = 0; i < localN; ++i) {
    for (int j = 0; j < localM; ++j) {
    int posA = i * inp[3],
    posB = j * inp[3];
    localResult[i*localM + j] = std::inner_product(AMatrix + posA, AMatrix + posA + inp[3], BMatrix + posB, 0);
    }
    }

    //send/recv 
    if (myRank) {
    MPI_Send(localResult, localN*localM, MPI_DOUBLE, 0, myRank, MPI_COMM_WORLD);
    }
    double* result = nullptr;
    if (!myRank) {

    result = new double[inp[2] * inp[5]];
    for (int i = 0; i < inp[0]*inp[1]; ++i) {

    int tmpCoords[2];
    MPI_Cart_coords(vMatrix, i, 2, tmpCoords);
    int recvN = sizes.sizeA[tmpCoords[0]] / inp[3],
    recvM = sizes.sizeB[tmpCoords[1]] / inp[4];
    double* tmpBuf = new double[recvN * recvM];
    if (i) {
    MPI_Recv(tmpBuf, recvN * recvM, MPI_DOUBLE, i, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } else {
    std::copy(localResult, localResult + recvM * recvN, tmpBuf);
    }

    int displN = sizes.displsA[tmpCoords[0]] / inp[3],
    displM = sizes.displsB[tmpCoords[1]] / inp[3];
    for(int j = 0; j < recvN; ++j) {
    std::copy(tmpBuf + j * recvM, tmpBuf + j * recvM + recvM, result + displM + (displN + j) * inp[5]);
    }
    delete[] tmpBuf;
    }
    }

    delete[] AMatrix;
    delete[] BMatrix;
    delete[] localResult;
    MPI_Comm_free(&vMatrix);

    // out
    if (!myRank) {
    /*std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    for (int i = 0; i < inp[2]; ++i) {
    std::for_each(result + i * inp[5], result + i * inp[5] + inp[5], 
    [](double &n) { std::cout << n << " "; });
    std::cout << std::endl;
    } */
    delete[] result;
    }

    MPI_Finalize();
    return 0;
}
