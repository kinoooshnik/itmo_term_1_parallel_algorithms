#include <cstdlib>
#include <vector>
#include <iostream>
#include "mpi.h"

using namespace std;

#define MATRIX_SIZE 500

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

int main(int argc, char **argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    //matrix initialization by each process
    auto a = generateMatrix(MATRIX_SIZE);
    auto b = generateMatrix(MATRIX_SIZE);
    auto c = generateZeroMatrix(MATRIX_SIZE);

    MPI_Barrier(MPI_COMM_WORLD); //barrier process synchronization

    // start timing for each process
    double time_start, time_finish;
    time_start = MPI_Wtime();

    // matrix multiplication
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            for (int k = 0; k < MATRIX_SIZE; k++) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }

    }
    // end of timing
    time_finish = MPI_Wtime();

    // output the execution time of matrix multiplication at each process
    cout << "Proceess: " << rank << ", time spent: " << (time_finish - time_start) << endl;
    MPI_Finalize();
}