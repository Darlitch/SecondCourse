#include <mpi.h>

#include <algorithm>
#include <iostream>

constexpr double
    x1 = -2,
    x2 = 1,
    y1 = -3,
    y2 = 1,
    z1 = -5,
    z2 = 1;

constexpr double a = 100000;
constexpr double epsilon = 1e-8;

constexpr int
    Nx = 1000,
    Ny = 700,
    Nz = 700,
    size2D = Nx * Ny;

constexpr double
    Dx = x2 - x1,
    Dy = y2 - y1,
    Dz = z2 - z1;

constexpr double
    hx = Dx / (Nx - 1),
    hy = Dy / (Ny - 1),
    hz = Dz / (Nz - 1);

double refFi(double x, double y, double z) {
    x = x1 + x * hx;
    y = y1 + y * hy;
    z = z1 + z * hz;
    return x * x + y * y + z * z;
}

double ro(double x, double y, double z) {
    auto fi = refFi(x, y, z);
    // x = x1 + x * hx;
    // y = y1 + y * hy;
    // z = z1 + z * hz;
    return 6 - a * fi;
}

bool isLimit(int x, int y, int z, int height, int width, int depth) {
    return x == 0 || x == height - 1 || y == 0 || y == width - 1 || z == 0 || z == depth - 1;
}

void fillMatrix(double* matrix) {
    for (int z = 0; z < Nz; ++z) {
        for (int y = 0; y < Ny; ++y) {
            for (int x = 0; x < Nx; ++x) {
                if (isLimit(x, y, z, Nx, Ny, Nz)) {
                    matrix[x + Nx * y + size2D * z] = refFi(x, y, z);
                } else {
                    matrix[x + Nx * y + size2D * z] = 0;
                }
            }
        }
    }
}

int main(int argc, char** argv) {
    int rank, size;
    double startTime, endTime;
    MPI_Init(&argc, &argv); 
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Вычисление размеров подматриц для каждого процесса
    int* sizes = new int[size];
    std::fill(sizes, sizes + size, Nz / size);
    for (int i = 0, count = Nz % size; count > 0; ++i, --count) {
        sizes[i]++;
    }
    std::for_each(sizes, sizes + size, [](int& n) { n *= size2D; });

    // Вычисление смещений для каждого процесса
    int* displs = new int[size];
    displs[0] = 0;
    for (int i = 1; i < size; ++i) {
        displs[i] = displs[i - 1] + sizes[i - 1];
    }

    // Создание и заполнение полной матрицы (только на нулевом процессе)
    double* fullMatrix = nullptr;
    if (!rank) {
        fullMatrix = new double[Nx * Ny * Nz];
        fillMatrix(fullMatrix);
    }
    MPI_Barrier(MPI_COMM_WORLD);

    if (!rank) {
        startTime = MPI_Wtime();
    }
    // Распределение подматриц между процессами
    double* submatrix = new double[sizes[rank] + 2 * size2D];
    MPI_Scatterv(fullMatrix, sizes, displs, MPI_DOUBLE, submatrix + size2D,
                 sizes[rank], MPI_DOUBLE, 0, MPI_COMM_WORLD);
    if (!rank) {
        std::copy(fullMatrix, fullMatrix + sizes[0], submatrix + size2D);
    }

    int next = rank + 1;
    int prev = rank - 1;
    bool flag = true;
    bool* flags;
    double* tmpMatrix = new double[sizes[rank] + 2 * size2D];
    //просто для красоты
    if (rank == 0) {
        prev = -1;
        flags = new bool[size];
    }
    if (rank == size -1) {
        next = -1;
    }





    if (!rank) {
        endTime = MPI_Wtime();
        std::cout << "Time: " << endTime - startTime << std::endl;
    }
    delete[] displs;
    delete[] sizes;
    delete[] fullMatrix;
    delete[] submatrix;
    delete[] tmpMatrix;
    MPI_Finalize();
}