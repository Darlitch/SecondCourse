#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

using matrix_cont = std::vector<std::vector<int>>;

const std::size_t N = 5;

void RandomVectorX(std::vector<int>& x) {
    srand(time(NULL));
    for (std::size_t i = 0; i < N; ++i) {
        x[i] = rand() % 10;
    }
}

void CountVectorB(matrix_cont& matrix, std::vector<int>& x, std::vector<int>& b) {
    for (std::size_t i = 0; i < N; ++i) {
        for (std::size_t j = 0; j < N; ++j) {
            b[i] += x[i] * matrix[i][j];
        }
    }
}

void SearchX(matrix_cont& matrix, std::vector<int>& x, std::vector<int>& b) {
    std::vector<int> x0(N);
    double e = 0.00001;
}

int main() {
    matrix_cont matrix(N, std::vector<int>(N));
    std::vector<int> x(N);
    std::vector<int> b(N);
    for (std::size_t i = 0; i < N; ++i) {
        for (std::size_t j = 0; j < N; ++j) {
            if (i == j) {
                matrix[i][j] = 2;
            } else {
                matrix[i][j] = 1;
            }
        }
    }
    RandomVectorX(x);
    CountVectorB(matrix, x, b);
    SearchX(matrix, x, b);
    return 0;
}
