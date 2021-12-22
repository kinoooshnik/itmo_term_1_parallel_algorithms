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

    // Cascade
    int split_size;
    vector<int> original_array(VECTOR_SIZE, 1);
    vector<int> a;
    double time_start, time_finish;
    if (rank == 0) {
        cout << "Cascade way" << endl;
        time_start = MPI_Wtime();
        split_size = ceil((float) VECTOR_SIZE / (float) size);

        for (int i = 1; i < size; i++) {
            int start_offset = (int) (split_size * i);
            int end_offset = (int) min(split_size * (i + 1), (int) original_array.size());
            int process_split_size = end_offset - start_offset;
            MPI_Send(&process_split_size, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&original_array[start_offset], process_split_size, MPI_INT, i, 0, MPI_COMM_WORLD);
        }

        a = vector<int>(original_array.begin(), original_array.begin() + split_size);
    } else {
        MPI_Recv(&split_size, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        a = vector<int>(split_size);
        MPI_Recv(&a[0], split_size, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    }

    int sum = 0;
    for (int i = 0; i < split_size; i++)
        sum += a[i];

    int receive_sum;
    for (int cascade_step = 2; cascade_step <= size; cascade_step *= 2) {
        if (rank % cascade_step == 0) {
            if (rank + cascade_step / 2 < size) {
                MPI_Recv(&receive_sum, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
                cout << "Precess " << rank << " got from process " << status.MPI_SOURCE << " number " << receive_sum << " and sum it with local number " << sum << " and got " << sum + receive_sum << endl;
                sum += receive_sum;
            }
        } else {
            MPI_Send(&sum, 1, MPI_INT, rank - rank % cascade_step, 0, MPI_COMM_WORLD);
            break;
        }
    }

    if (rank == 0){
        time_finish = MPI_Wtime();
        cout << "Vector sum: " << sum << ", time spent: " << (time_finish - time_start) << endl;
    }

    // Reduce
    if (rank == 0) {
        cout << endl << "Reduce way" << endl;
        time_start = MPI_Wtime();
        split_size = ceil((float) VECTOR_SIZE / (float) size);

        for (int i = 1; i < size; i++) {
            int start_offset = (int) (split_size * i);
            int end_offset = (int) min(split_size * (i + 1), (int) original_array.size());
            int process_split_size = end_offset - start_offset;
            MPI_Send(&process_split_size, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&original_array[start_offset], process_split_size, MPI_INT, i, 0, MPI_COMM_WORLD);
        }

        a = vector<int>(original_array.begin(), original_array.begin() + split_size);
    } else {
        MPI_Recv(&split_size, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        a = vector<int>(split_size);
        MPI_Recv(&a[0], split_size, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    }

    sum = 0;
    for (int i = 0; i < split_size; i++) {
        sum += a[i];
    }

    int total_sum = 0;
    MPI_Reduce(&sum, &total_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    if (rank == 0){
        time_finish = MPI_Wtime();
        cout << "Vector sum: " << total_sum << ", time spent: " << (time_finish - time_start) << endl;
    }

    MPI_Finalize();
    return 0;
}