#include <iostream>
#include <cstdlib>
#include <omp.h>


using namespace std;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cout << "Invalid number of arguments" << endl;
        return 1;
    }

    int N = atoi(argv[1]);
    auto *a = new long[N];

    for (long int i = 0; i < N; i++) {
        a[i] = rand() / 100000;
    }
    if (N <= 10) {
        for (long int i = 0; i < N; i++) {
            cout << a[i] << " ";
        }
        cout << endl;
    }

    for (int NUM_THREADS = 1; NUM_THREADS <= 10; NUM_THREADS++) {
        long max_element = 0;
        double time = omp_get_wtime();
#pragma omp parallel shared(a) reduction (max:max_element) num_threads(NUM_THREADS)
#pragma omp for
        for (long int i = 0; i < N; i++) {
            if (a[i] > max_element) {
                max_element = a[i];
            }
        }
        cout << "Max element = " << max_element << "; Num threads = " << NUM_THREADS << "; Time = "
             << (omp_get_wtime() - time) << " sec." << endl;
    }
    delete[] a;
}