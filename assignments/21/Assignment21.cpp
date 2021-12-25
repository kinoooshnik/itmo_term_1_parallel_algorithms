#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include "mpi.h"


#define BUFSIZE 50
#define FILENAME "file.txt"
#define LINE_NUMBER 1

using namespace std;


string gen_random(const int len) {
    static const char alphanum[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
    string tmp_s;
    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i) {
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    return tmp_s;
}

int generate_file(const string &file_name) {
    ofstream myfile;
    myfile.open(file_name);
    for (int i = 0; i < LINE_NUMBER; i++) {
        myfile << gen_random(180) << endl;
    }
    return 0;
}


int main(int argc, char **argv)
{
    generate_file(FILENAME);
	int rank;
	MPI_Init(&argc, &argv);
	MPI_File fh;
	char buf[BUFSIZE];
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_File_open(MPI_COMM_WORLD, FILENAME, MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);
	MPI_File_set_view(fh, rank * BUFSIZE, MPI_CHAR, MPI_CHAR, "native", MPI_INFO_NULL);
	MPI_File_read_all(fh, buf, BUFSIZE, MPI_CHAR, MPI_STATUS_IGNORE);
    cout << "Process: " << rank << ", buf: " << buf << endl;
	MPI_File_close(&fh);
	MPI_Finalize();
}