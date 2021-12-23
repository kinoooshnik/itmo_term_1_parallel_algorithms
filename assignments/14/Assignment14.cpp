#include <iostream>
#include <vector>
#include "mpi.h"

#define N 1000
#define N_FIRST_ELEMENTS_TO_PRINT 10

using namespace std;

void smod5(void *a, void *b, int *l, MPI_Datatype *type) {
    int i;
    for (i = 0; i < *l; i++)
        ((int *) b)[i] = (((int *) a)[i] + ((int *) b)[i]) % 5;
}

void custom_max(void *a, void *b, int *l, MPI_Datatype *type) {
    for (int i = 0; i < *l; i++) {
        if (((int *) a)[i] > ((int *) b)[i])
            ((int *) b)[i] = ((int *) a)[i];
    }
}

int main(int argc, char **argv) {
    int rank, size;
    int a[N], b[N], c[N];
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    for (int i = 0; i < N; i++) a[i] = i + rank + 1;

    cout << "Process: " << rank << ", first " << min(N_FIRST_ELEMENTS_TO_PRINT, N) << " elements of a: ";
    for (int i = 0; i < min(N_FIRST_ELEMENTS_TO_PRINT, N); i++) cout << a[i] << " ";
    cout << endl;

    MPI_Op op;
    MPI_Op_create(&custom_max, 1, &op);
    MPI_Reduce(&a, &b, N, MPI_INT, op, 0, MPI_COMM_WORLD);
    MPI_Reduce(&a, &c, N, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Op_free(&op);

    if (rank == 0) {
        cout << endl << "Result of custom max function" << endl;
        cout << "Process: " << rank << ", first " << min(N_FIRST_ELEMENTS_TO_PRINT, N) << " elements of b: ";
        for (int i = 0; i < min(N_FIRST_ELEMENTS_TO_PRINT, N); i++) cout << b[i] << " ";
        cout << endl;

        cout << "Result of MPI_MAX function" << endl;
        cout << "Process: " << rank << ", first " << min(N_FIRST_ELEMENTS_TO_PRINT, N) << " elements of c: ";
        for (int i = 0; i < min(N_FIRST_ELEMENTS_TO_PRINT, N); i++) cout << c[i] << " ";
        cout << endl;
    }

    MPI_Finalize();
}