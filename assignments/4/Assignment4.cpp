#include <iostream>
#include <vector>
#include "mpi.h"

using namespace std;

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Status status;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        vector<int> messages(size - 1);
        int messageCount = 0;
        int messageFromChildProcess;
        for (int i = 1; i < size; i++) {
            MPI_Recv(&messageFromChildProcess, 1, MPI_INT, MPI_ANY_SOURCE,
                     MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            messages[messageCount] = messageFromChildProcess;
            messageCount++;
        }
        int oddProduct = 1;
        for (int message: messages) {
            if (message % 2 == 1) {
                oddProduct *= message;
            }
        }
        cout << "Product of odd numbers got from child processes is " << oddProduct << endl;
    } else MPI_Send(&rank, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    MPI_Finalize();
    return 0;
}