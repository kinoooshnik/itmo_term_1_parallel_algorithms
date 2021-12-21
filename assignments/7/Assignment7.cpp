#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include "mpi.h"

using namespace std;

#define VECTOR_SIZE 1000000

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Status status;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int split_size;
    vector<int> a, b;
    int c = 0;
    if (rank == 0) {
        vector<int> a_original(VECTOR_SIZE, 1);
        vector<int> b_original(VECTOR_SIZE, 1);
        split_size = ceil((float) VECTOR_SIZE / (float) size);

        for (int i = 1; i < size; i++) {
            int start_offset = (int) (split_size * i);
            int end_offset = (int) min(split_size * (i + 1), (int) a_original.size());
            int process_split_size = end_offset - start_offset;
            MPI_Send(&process_split_size, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&a_original[start_offset], process_split_size, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&b_original[start_offset], process_split_size, MPI_INT, i, 0, MPI_COMM_WORLD);
        }

        a = vector<int>(a_original.begin(), a_original.begin() + split_size);
        b = vector<int>(b_original.begin(), b_original.begin() + split_size);
    } else {
        MPI_Recv(&split_size, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        a = vector<int>(split_size);
        b = vector<int>(split_size);
        MPI_Recv(&a[0], split_size, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        MPI_Recv(&b[0], split_size, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    }

    for (int i = 0; i < split_size; i++)
        c += a[i] * b[i];

    if (rank == 0) {
        vector<int> result(size);
        result[0] = c;
        for (int i = 1; i < size; i++) {
            MPI_Recv(&result[i], 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        }

        int sum = 0;
        for (auto &n: result)
            sum += n;

        cout << "Scalar product of a and b is " << sum << endl;
    } else {
        MPI_Send(&c, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}