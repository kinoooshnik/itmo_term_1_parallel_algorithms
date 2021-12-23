#include <iostream>
#include "mpi.h"

using namespace std;

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    char port_name[MPI_MAX_PORT_NAME];
    MPI_Open_port(MPI_INFO_NULL, port_name);
    cout << "Port name: " << port_name << endl;

    MPI_Comm intercomm;
    MPI_Comm_accept(port_name, MPI_INFO_NULL, 0, MPI_COMM_SELF, &intercomm);

    int message_to_client = 25;
    int message_from_client;
    MPI_Request requests[2];
    MPI_Status statuses[2];
    MPI_Irecv(&message_from_client, 1, MPI_INT, 0, 0, intercomm, &requests[0]);
    MPI_Isend(&message_to_client, 1, MPI_INT, 0, 0, intercomm, &requests[1]);

    cout << "Waiting for the client ..." << endl;
    MPI_Waitall(2, requests, statuses);

    cout << "Client connected" << endl;
    cout << "Server sent value: " << message_to_client << endl;
    cout << "Server got value: " << message_from_client << endl;

    MPI_Comm_free(&intercomm);
    MPI_Close_port(port_name);
    MPI_Finalize();
    return 0;
}