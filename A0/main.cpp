#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cout << "Invalid number of arguments: " << argc << endl;
        return 1;
    }
    unsigned long len = strlen(argv[1]);
    if (len == 0) {
        cout << 0 << endl;
        return 0;
    }
    bool inSpaces = true;
    int numWords = 0;
    for (unsigned long i = 0; i < strlen(argv[1]) - 1; i++) {
        if (argv[1][i] == ' ' || argv[1][i] == '\n' || argv[1][i] == '\t') {
            inSpaces = true;
        } else if (inSpaces) {
            numWords++;
            inSpaces = false;
        }
    }
    cout << numWords << endl;
    return 0;
}
