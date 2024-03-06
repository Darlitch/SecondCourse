#include <omp.h>

#include <cmath>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <vector>

using matrix_cont = std::vector<std::vector<double>>;

const std::size_t N = 1150;
const double e = 0.000001;
double t = 0.001;

matrix_cont MatrixBuilder() {
    matrix_cont matrix(N, std::vector<double>(N));
#pragma omp parallel for
    for (std::size_t i = 0; i < N; ++i) {
        for (std::size_t j = 0; j < N; ++j) {
            if (j == i) {
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
#pragma omp parallel for
    for (std::size_t i = 0; i < N; ++i) {
        x[i] = rand() % 10;
    }
}

void AMultX(matrix_cont& matrix, double* x, double* b) {
#pragma omp parallel for
    for (std::size_t i = 0; i < N; ++i) {
        for (std::size_t j = 0; j < N; ++j) {
            b[i] += x[j] * matrix[i][j];
        }
    }
}

void Sub(double* x1, double* b) {
#pragma omp parallel for
    for (std::size_t i = 0; i < N; ++i) {
        x1[i] -= b[i];
    }
}

void MultT(double* x1) {
#pragma omp parallel for
    for (std::size_t i = 0; i < N; ++i) {
        x1[i] *= t;
    }
}

double Module(double* u) {
    double a = 0;
    // #pragma omp parallel for
    for (std::size_t i = 0; i < N; ++i) {
        a += (u[i] * u[i]);
    }
    return sqrt(a);
}

void SearchX(matrix_cont& matrix, double* x, double* b) {
    std::size_t count = 0;
    double x0[N] = {0};
    double x1[N] = {0};
    double u = 0;
    double uOld = 0;

    AMultX(matrix, x0, x1);
    Sub(x1, b);
    do {
        if (u > uOld && (count % 5 == 0)) {
            t = (-t);
        }
        uOld = u;
        MultT(x1);
        Sub(x0, x1);
        AMultX(matrix, x0, x1);
        Sub(x1, b);
        u = Module(x1);
        u = u / Module(b);
        count++;
    } while (u > e);
    std::cout << "u:" << u << std::endl;

    Sub(x0, x);
    u = Module(x0);
    std::cout << "NormalX: " << u << std::endl;
}

int main(int argc, char** argv) {
    double startTime, endTime;
    double x[N];
    double b[N] = {0};
    startTime = omp_get_wtime();
    matrix_cont matrix = MatrixBuilder();
    RandomVectorX(x);
    AMultX(matrix, x, b);
    SearchX(matrix, x, b);
    endTime = omp_get_wtime();
    std::cout << "Time: " << endTime - startTime << std::endl;
    return 0;
}
