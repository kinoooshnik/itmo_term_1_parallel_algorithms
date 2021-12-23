#include <iostream>
#include "mpi.h"

using namespace std;

int main(int argc, char **argv) {
    int size, rank1, rank2, slave_size;
    MPI_Status status;
    MPI_Comm intercomm;
    char slave[10] = "./slave.o";
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_spawn(slave, MPI_ARGV_NULL, 3, MPI_INFO_NULL, 0, MPI_COMM_SELF, &intercomm, MPI_ERRCODES_IGNORE);
    MPI_Recv(&rank1, 1, MPI_INT, 0, 0, intercomm, &status);
    MPI_Recv(&rank2, 1, MPI_INT, 1, 1, intercomm, &status);
    cout << "Slaves " << rank1 << " and " << rank2 << " are working" << endl;
    MPI_Recv(&slave_size, 1, MPI_INT, 2, MPI_ANY_TAG, intercomm, &status);
    cout << "Number of running processes: " << slave_size << endl;
    MPI_Finalize();
    return 0;
}