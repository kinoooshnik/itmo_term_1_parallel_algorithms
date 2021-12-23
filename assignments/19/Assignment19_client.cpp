#include <iostream>
#include <cstring>
#include "mpi.h"

using namespace std;

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    char port_name[MPI_MAX_PORT_NAME];
    strcpy(port_name, argv[1]);
    cout << "Attempt to connect to " << port_name << " port" << endl;

    MPI_Comm intercomm;
    MPI_Comm_connect(port_name, MPI_INFO_NULL, 0, MPI_COMM_SELF, &intercomm);
    cout << "Server connection" << endl;

    int message_to_server = 42;
    int message_from_server;
    MPI_Request requests[2];
    MPI_Status statuses[2];
    MPI_Irecv(&message_from_server, 1, MPI_INT, 0, 0, intercomm, &requests[0]);
    MPI_Isend(&message_to_server, 1, MPI_INT, 0, 0, intercomm, &requests[1]);
    MPI_Waitall(2, requests, statuses);
    cout << "Client sent value: " << message_to_server << endl;
    cout << "Client got value: " << message_from_server << endl;

    MPI_Finalize();
    return 0;
}