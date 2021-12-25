#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include "mpi.h"

#define BUFSIZE 100
#define FILENAME "file.txt"
#define LINE_NUMBER 10

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
        myfile << gen_random(rand() % 40 + 1) << endl;
    }
    return 0;
}


int main(int argc, char **argv) {
    generate_file(FILENAME);

    int num, sum;
    MPI_Init(&argc, &argv);
    MPI_Status status;
    MPI_File fh;
    char buf[BUFSIZE];

    MPI_File_open(MPI_COMM_WORLD, FILENAME, MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);
    MPI_File_set_view(fh, 0, MPI_CHAR, MPI_CHAR, "native", MPI_INFO_NULL);
    sum = 0;
    do {
        MPI_File_read(fh, buf, BUFSIZE, MPI_CHAR, &status);
        MPI_Get_count(&status, MPI_CHAR, &num);
        cout << "buf: " << buf << endl;
        sum += num;
    } while (num >= BUFSIZE);
    MPI_File_close(&fh);

    cout << "Number of read symbols: " << sum << endl;

    MPI_Finalize();
    if (argc > 1 && (string) argv[1] == "--remove-file") {
        remove(FILENAME);
    }
}