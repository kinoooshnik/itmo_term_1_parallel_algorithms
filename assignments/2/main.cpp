#include <iostream>
#include <cstdlib>
#include <omp.h>
#include <vector>

using namespace std;

vector<vector<int>> generateMatrix(int N) {
    vector<vector<int> > matrix(N, vector<int>(N));
    for (auto &row: matrix) {
        for (auto &col: row) {
            col = rand() / 10000000;
        }
    }
    return matrix;
}

vector<vector<int>> generateZeroMatrix(int N) {
    vector<vector<int> > matrix(N, vector<int>(N));
    for (auto &row: matrix) {
        for (auto &col: row) {
            col = 0;
        }
    }
    return matrix;
}

void printMatrix(vector<vector<int>> matrix) {
    for (auto &row: matrix) {
        for (auto &col: row) {
            cout << col << " ";
        }
        cout << endl;
    }
}


int main(int argc, char *argv[]) {
    if (argc != 2) {
        cout << "Invalid number of arguments" << endl;
        return 1;
    }

    int N = atoi(argv[1]);
    auto a = generateMatrix(N);
    auto b = generateMatrix(N);
    auto c = generateZeroMatrix(N);


    if (N <= 5) {
        cout << "Matrix A: " << endl;
        printMatrix(a);
        cout << endl << "Matrix B: " << endl;
        printMatrix(b);
    }

    double oneThreadTime;
    cout << "Order - ijk" << endl;
    for (int NUM_THREADS = 1; NUM_THREADS <= 10; NUM_THREADS++) {
        double startTime = omp_get_wtime();
#pragma omp parallel shared(a, b, c) num_threads(NUM_THREADS)
#pragma omp for
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                for (int k = 0; k < N; k++) {
                    c[i][j] += a[i][k] * b[k][j];
                }
            }
        }
        double executionTime = omp_get_wtime() - startTime;
        if (NUM_THREADS == 1) {
            oneThreadTime = executionTime;
        }
        cout << "Num threads = " << NUM_THREADS << "; Time = "
             << executionTime << " sec.;" << " Efficiency = " << (oneThreadTime / executionTime) << endl;
    }

    cout << endl << "Order - ikj" << endl;
    for (int NUM_THREADS = 1; NUM_THREADS <= 10; NUM_THREADS++) {
        double startTime = omp_get_wtime();
#pragma omp parallel shared(a, b, c) num_threads(NUM_THREADS)
#pragma omp for
        for (int i = 0; i < N; i++) {
            for (int k = 0; k < N; k++) {
                for (int j = 0; j < N; j++) {
                    c[i][j] += a[i][k] * b[k][j];
                }
            }
        }
        double executionTime = omp_get_wtime() - startTime;
        if (NUM_THREADS == 1) {
            oneThreadTime = executionTime;
        }
        cout << "Num threads = " << NUM_THREADS << "; Time = "
             << executionTime << " sec.;" << " Efficiency = " << (oneThreadTime / executionTime) << endl;
    }
    cout << endl << "Order - jki" << endl;
    for (int NUM_THREADS = 1; NUM_THREADS <= 10; NUM_THREADS++) {
        double startTime = omp_get_wtime();
#pragma omp parallel shared(a, b, c) num_threads(NUM_THREADS)
#pragma omp for
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                for (int i = 0; i < N; i++) {
                    c[i][j] += a[i][k] * b[k][j];
                }
            }
        }
        double executionTime = omp_get_wtime() - startTime;
        if (NUM_THREADS == 1) {
            oneThreadTime = executionTime;
        }
        cout << "Num threads = " << NUM_THREADS << "; Time = "
             << executionTime << " sec.;" << " Efficiency = " << (oneThreadTime / executionTime) << endl;
    }

    if (N <= 5) {
        cout << endl << "Result matrix C: " << endl;
        printMatrix(c);
    }
}