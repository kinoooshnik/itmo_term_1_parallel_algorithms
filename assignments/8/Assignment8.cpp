#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include "mpi.h"

using namespace std;

#define N_REPEATS 1000000
#define LATENCY_REPEATS 1000000

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Status status;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int n_elements = 1;
    while (n_elements <= 1000000) {
        if (rank == 0) {
            vector<char> some_data(n_elements);

            double time_start, time_finish;
            time_start = MPI_Wtime();
            for (int i = 0; i < N_REPEATS; i++) {
                MPI_Send(&some_data[0], n_elements, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
                MPI_Recv(&some_data[0], n_elements, MPI_CHAR, 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            }
            time_finish = MPI_Wtime();

            int L = n_elements * (int) sizeof(char);
            float R = (float) ((2 * (float) N_REPEATS * (float) L) / (time_finish - time_start));
            cout
                 << "Number of elements: " << n_elements
                 << ", Message size: " << (float) L << " byte"
                 << ", Speed: " << R / 1024 / 1024 << " Mbyte/s" << endl;
        } else if (rank == 1) {
            vector<char> some_data(n_elements);
            for (int i = 0; i < N_REPEATS; i++) {
                MPI_Recv(&some_data[0], n_elements, MPI_CHAR, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
                MPI_Send(&some_data[0], n_elements, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
            }
        }
        n_elements *= 10;
    }

    char c;
    if (rank == 0) {
        double time_start, time_finish;
        time_start = MPI_Wtime();
        for (int i = 0; i < LATENCY_REPEATS; i++) {
            MPI_Send(&c, 0, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(&c, 0, MPI_CHAR, 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        }
        time_finish = MPI_Wtime();

        float s = (float) (time_finish - time_start) / (2 * LATENCY_REPEATS);
        cout << "Latency: " << s << " sec" << endl;
    } else if (rank == 1) {
        for (int i = 0; i < LATENCY_REPEATS; i++) {
            MPI_Send(&c, 0, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
            MPI_Recv(&c, 0, MPI_CHAR, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        }
    }

    MPI_Finalize();
    return 0;
}