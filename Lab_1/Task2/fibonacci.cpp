#include <iostream>
#include <sys/time.h>
using namespace std;

struct timeval {
    time_t tv_sec;
    suseconds_t tv_usec;
}

struct timezone {
    int tz_minuteswest;
    int tz_dsttime;
}


int main() {
    int n, firstTerm = 1, secondTerm = 1, nextTerm;
    cout << "Enter number of terms: ";
    cin >> n;

    cout << "Fibonacci Series: " << firstTerm << " + " << secondTerm << " + ";

    //measure the execution time of the following loop
    for (int i = 1; i <= n-2; ++i) {
        nextTerm = firstTerm + secondTerm;
        cout << nextTerm << " + ";
        firstTerm = secondTerm;
        secondTerm = nextTerm;
    }
    return 0;
}
