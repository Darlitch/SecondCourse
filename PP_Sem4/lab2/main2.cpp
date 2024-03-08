#include <omp.h>

#include <cmath>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <vector>

using matrix_cont = std::vector<std::vector<double>>;

const std::size_t N = 800;
const double e = 0.000001;
double t = 0.001;

matrix_cont MatrixBuilder() {
    matrix_cont matrix(N, std::vector<double>(N));
#pragma omp for
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
#pragma omp for
    for (std::size_t i = 0; i < N; ++i) {
        // x[i] = rand() % 10;
        // #pragma omp atomic  //??
        x[i] = rand() % 10;
    }
}

void AMultX(matrix_cont& matrix, double* x, double* b) {
#pragma omp for
    for (std::size_t i = 0; i < N; ++i) {
        for (std::size_t j = 0; j < N; ++j) {
            double temp = x[j] * matrix[i][j];
            // #pragma omp critical
            //             {
            b[i] += temp;
            // }
        }
    }
}

void Sub(double* x1, double* b) {
#pragma omp for
    for (std::size_t i = 0; i < N; ++i) {
        // x1[i] -= b[i];
        // #pragma omp atomic
        x1[i] -= b[i];
    }
}

void MultT(double* x1) {
#pragma omp for
    for (std::size_t i = 0; i < N; ++i) {
        // x1[i] *= t;
        // #pragma omp atomic
        x1[i] *= t;
    }
}

double Module(double* u, double& sum) {
    // double a = 0;
#pragma omp for reduction(+ : sum)
    for (std::size_t i = 0; i < N; ++i) {
        double temp = u[i] * u[i];
        // a += (u[i] * u[i]);
        // #pragma omp critical
        // {
        sum += temp;
        // }
    }
#pragma omp master
    std::cout << sum << std::endl;
#pragma omp master
    sum = sqrt(sum);
#pragma omp master
    std::cout << sum << std::endl;
    return sum;  // TODO
}

void SearchX(matrix_cont& matrix, double* x, double* b) {
    std::size_t count = 0;
    double x0[N] = {0};
    double x1[N] = {0};
    double u = 0;
    double u2 = 0;
    double uOld = 0;
    double sum = 0;
    double sum2 = 0;
    // #pragma omp parallel
    //     {
    AMultX(matrix, x0, x1);
    Sub(x1, b);
    Module(b, sum);
    u2 = sum;
    sum = 0;
    do {
#pragma omp parallel
        {
            if (u > uOld && (count % 5 == 0)) {
#pragma omp master
                t = (-t);
            }
            uOld = u;
            MultT(x1);
            Sub(x0, x1);
            AMultX(matrix, x0, x1);
            Sub(x1, b);
            Module(x1, sum);
#pragma omp master
            u = sum;
#pragma omp master
            std::cout << "u1:" << u << std::endl;
            // getchar();
// #pragma omp master
//             sum = 0;
//             Module(b, sum);
// #pragma omp master
//             u2 = sum;
// #pragma omp master
//             std::cout << "u2:" << u2 << std::endl;
//             getchar();
#pragma omp master
        u = u / u2;
        }
        sum = 0;
#pragma omp master
        count++;
#pragma omp barrier
    } while (u > e);
    // std::cout << "u:" << u << std::endl;

    Sub(x0, x);
    Module(x0, sum);
    u = sum;
    // }
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
