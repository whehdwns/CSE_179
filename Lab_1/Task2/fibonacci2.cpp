#include <bits/stdc++.h> 
#include <sys/time.h> 
using namespace std; 

// A sample function whose time taken to 
// be measured 
void fun() 
{ 
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
} 

int main() 
{ 

	struct timeval start, end; 

	// start timer. 
	gettimeofday(&start, NULL); 

	// sync the I/O of C and C++. 
	ios_base::sync_with_stdio(false); 

	fun(); 

	// stop timer. 
	gettimeofday(&end, NULL); 

	// Calculating total time taken by the program. 
	double time_taken; 

	time_taken = (end.tv_sec - start.tv_sec) * 1e6; 
	time_taken = (time_taken + (end.tv_usec - start.tv_usec)) * 1e-6; 

	cout << "Time taken by program is : " << fixed 
		<< time_taken << setprecision(6); 
	cout << " sec" << endl; 
	return 0; 
} 
