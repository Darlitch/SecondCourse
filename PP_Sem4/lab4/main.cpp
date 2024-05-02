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
    Nx = 500,
    Ny = 350,
    Nz = 350,
    size2D = Nx * Ny;

constexpr double
    Dx = x2 - x1,
    Dy = y2 - y1,
    Dz = z2 - z1;

constexpr double
    hx = Dx / (Nx - 1),
    hy = Dy / (Ny - 1),
    hz = Dz / (Nz - 1);

double Fi(double x, double y, double z) {
    x = x1 + x * hx;
    y = y1 + y * hy;
    z = z1 + z * hz;
    return x * x + y * y + z * z;
}

double ro(double x, double y, double z) {
    // auto fi = Fi(x, y, z);
    // x = x1 + x * hx;
    // y = y1 + y * hy;
    // z = z1 + z * hz;
    return 6 - a * Fi(x, y, z);
}

bool isLimit(int x, int y, int z, int height, int width, int depth) {
    return x == 0 || x == height - 1 || y == 0 || y == width - 1 || z == 0 || z == depth - 1;
}

void fillMatrix(double* matrix) {
    for (int z = 0; z < Nz; ++z) {
        for (int y = 0; y < Ny; ++y) {
            for (int x = 0; x < Nx; ++x) {
                if (isLimit(x, y, z, Nx, Ny, Nz)) {
                    matrix[x + Nx * y + size2D * z] = Fi(x, y, z);
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
    // просто для красоты
    if (rank == 0) {
        prev = -1;
        flags = new bool[size];
    }
    if (rank == size - 1) {
        next = -1;
    }

    while (flag) {
        MPI_Request trash = MPI_REQUEST_NULL;
        MPI_Request rNext, rPrev;

        // Отправляем и получаем граничные значения между процессами
        if (rank != 0) {
            MPI_Isend(submatrix + size2D, size2D, MPI_DOUBLE, prev, 0,
                      MPI_COMM_WORLD, &trash);
            MPI_Irecv(submatrix, size2D, MPI_DOUBLE, prev, 1, MPI_COMM_WORLD, &rPrev);
        }
        if (rank != size - 1) {
            MPI_Isend(submatrix + sizes[rank], size2D, MPI_DOUBLE, next, 1,
                      MPI_COMM_WORLD, &trash);
            MPI_Irecv(submatrix + sizes[rank] + size2D, size2D, MPI_DOUBLE, next,
                      0, MPI_COMM_WORLD, &rNext);
        }

        // Если мы в процессе с рангом 0, то нет предыдущего процесса
        if (!rank) {
            rPrev = MPI_REQUEST_NULL;
        }
        // Если мы в последнем процессе, то нет следующего процесса
        if (rank == size - 1) {
            rNext = MPI_REQUEST_NULL;
        }
        double alpha = 1 / (2 / (hx * hx) + 2 / (hy * hy) + 2 / (hz + hz) + a);
        // Смещение для текущей подматрицы
        int displ = 0;
        for (int i = 0; i < rank; ++i) {
            displ += sizes[i];
        }

        // Вычисляем центральные значения
        for (int i = size2D; i < sizes[rank] + size2D; ++i) {
            int realI = i + displ - size2D;
            int realZ = realI / (size2D),
                realY = (realI - realZ * size2D) / Nx,
                realX = realI - realZ * size2D - realY * Nx;

            int curZ = i / size2D - 1,
                curY = realY,
                curX = realX;
            if (!isLimit(curX, curY, curZ, Nx, Ny, sizes[rank] / size2D) &&
                !isLimit(realX, realY, realZ, Nx, Ny, Nz)) {
                auto tmpX = (submatrix[i - 1] + submatrix[i + 1]) / hx * hx,
                     tmpY = (submatrix[i - Nx] + submatrix[i + Nx]) / hy * hy,
                     tmpZ = (submatrix[i - size2D] + submatrix[i + size2D]) / hz * hz;
                auto tmpRo = ro(realX, realY, realZ);

                tmpMatrix[i] = alpha * (tmpX + tmpY + tmpZ - tmpRo);
            }
        }

        // Ожидаем завершения всех неблокирующих операций
        MPI_Request requestArray[2];
        requestArray[0] = rNext;
        requestArray[1] = rPrev;
        if (MPI_Waitall(2, requestArray, MPI_STATUS_IGNORE) != MPI_SUCCESS) {
            std::cout << "Rank: " << rank << " error on waitall" << std::endl;
        }

        // Вычисляем граничные значения
        for (int i = size2D; i < sizes[rank] + size2D; ++i) {
            int realI = i + displ - size2D;
            int realZ = realI / (size2D);
            int realY = (realI - realZ * size2D) / Nx;
            int realX = realI - realZ * size2D - realY * Nx;

            if (isLimit(realX, realY, realZ, Nx, Ny, Nz)) {
                // Если элемент является граничным, оставляем его значение без изменений
                tmpMatrix[i] = submatrix[i];
                continue;
            }
            // Вычисление координаты z для элемента в подматрице
            int curZ = i / size2D - 1;

            if (curZ == 0 || curZ == (sizes[rank] / size2D) - 1) {
                // Вычисление значений направлений x, y, z для элемента
                auto tmpX = (submatrix[i - 1] + submatrix[i + 1]) / hx * hx;
                auto tmpY = (submatrix[i - Nx] + submatrix[i + Nx]) / hy * hy;
                auto tmpZ = (submatrix[i - size2D] + submatrix[i + size2D]) / hz * hz;
                auto tmpRo = ro(realX, realY, realZ);

                // Вычисление нового значения для элемента в временной подматрице
                tmpMatrix[i] = alpha * (tmpX + tmpY + tmpZ - tmpRo);
            }
        }

        // Проверяем условие завершения
        flag = false;
        for (int i = size2D; i < sizes[rank] + size2D; ++i) {
            if (std::abs(submatrix[i] - tmpMatrix[i]) > epsilon) {
                flag = true;
                break;
            };
        }

        // Собираем флаги из всех процессов и проверяем, есть ли еще необходимость в итерации
        MPI_Gather(&flag, 1, MPI_CXX_BOOL, flags, 1, MPI_CXX_BOOL, 0, MPI_COMM_WORLD);
        if (!rank) {
            flag = false;
            for (int i = 0; i < size; ++i) {
                if (flags[i] == true) {
                    flag = true;
                    break;
                }
            }
        }
        // Рассылаем флаг всем процессам
        MPI_Bcast(&flag, 1, MPI_CXX_BOOL, 0, MPI_COMM_WORLD);

        // Копируем временную подматрицу в основную подматрицу
        std::copy(tmpMatrix, tmpMatrix + sizes[rank] + 2 * size2D, submatrix);
    }

    MPI_Gatherv(submatrix + size2D, sizes[rank], MPI_DOUBLE, fullMatrix,
                sizes, displs, MPI_DOUBLE, 0, MPI_COMM_WORLD);

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