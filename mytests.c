#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/times.h> /* for times system call */
#include <sys/time.h>  /* for gettimeofday system call */
#include <unistd.h>
#include <error.h>
#include <pthread.h> // NEED THIS
#include "lab.h"

// function prototypes
int check_if_sorted(int A[], int n);
void generate_random_array(int A[], int n, int seed);
void serial_mergesort(int A[], int p, int r);
void merge(int A[], int p, int q, int r);
void insertion_sort(int A[], int p, int r);
double getMilliSeconds(void);
double runSerialTest(int seed, int n);
double runParallelTest(int seed, int n, int numThreads);
void runVariableThreadTest(int seed, int n);




/*
---------------------------------------------------------------------------
clock_t times(struct tms *buffer);
times() fills the structure pointed to by buffer with
time-accounting information.  The structure defined in
<sys/times.h> is as follows:
struct tms {
    clock_t tms_utime;       user time
    clock_t tms_stime;       system time
    clock_t tms_cutime;      user time, children
    clock_t tms_cstime;      system time, children
The time is given in units of 1/CLK_TCK seconds where the
value of CLK_TCK can be determined using the sysconf() function
with the agrgument _SC_CLK_TCK.
---------------------------------------------------------------------------
*/


float report_cpu_time(void)
{
	struct tms buffer;
	float cputime;

	times(&buffer);
	cputime = (buffer.tms_utime)/ (float) sysconf(_SC_CLK_TCK);
	return (cputime);
}


float report_sys_time(void)
{
	struct tms buffer;
	float systime;

	times(&buffer);
	systime = (buffer.tms_stime)/ (float) sysconf(_SC_CLK_TCK);
	return (systime);
}

double getMilliSeconds(void)
{
    struct timeval now;
    gettimeofday(&now, (struct timezone *)0);
    return (double) now.tv_sec*1000.0 + now.tv_usec/1000.0;
}


/*
 * generate_random_array(int A[], int n, int seed):
 *
 * description: Generate random integers in the range [0,RANGE]
 *              and store in A[1..n]
 */

#define RANGE 1000000

void generate_random_array(int A[], int n, int seed)
{
    int i;

	srandom(seed);
    for (i=1; i<=n; i++)
        A[i] = random()%RANGE;
}


/*
 * check_if_sorted(int A[], int n):
 *
 * description: returns TRUE if A[1..n] are sorted in nondecreasing order
 *              otherwise returns FALSE
 */

int check_if_sorted(int A[], int n)
{
	int i=0;

	for (i=1; i<n; i++) {
		if (A[i] > A[i+1]) {
			return FALSE;
		}
	}
	return TRUE;
}

double runSerialTest(int seed, int n) {

	int *A = (int *) malloc(sizeof(int) * (n+1)); // n+1 since we are using A[1]..A[n]

	// generate random input
	generate_random_array(A,n, seed);

	double start_time;
	double sorting_time;

	start_time = 0;
	sorting_time = 0;
	// sort the input (and time it)
	start_time = getMilliSeconds();
	mergesort(A,1,n, 1);
	sorting_time = getMilliSeconds() - start_time;

	// print results if correctly sorted otherwise cry foul and exit
	if (check_if_sorted(A,n)) {
		printf("Sorting %d elements in *serial* took %4.2lf seconds.\n", n,  sorting_time/1000.0);
	} else {
		printf("sorting failed!!!!\n");
		exit(EXIT_FAILURE);
	}
	free(A);
	return sorting_time/1000.0;
}

double runParallelTest(int seed, int n, int numThreads) {

	int *A = (int *) malloc(sizeof(int) * (n+1)); // n+1 since we are using A[1]..A[n]

	// generate random input
	generate_random_array(A,n, seed);

	double start_time;
	double sorting_time;

	// sort the input (and time it)
	start_time = getMilliSeconds();
	mergesort(A,1,n, numThreads);
	sorting_time = getMilliSeconds() - start_time;

	// print results if correctly sorted otherwise cry foul and exit
	if (check_if_sorted(A,n)) {
		printf("Sorting %d elements in *parallel* took %4.2lf seconds.\n", n,  sorting_time/1000.0);
	} else {
		printf("sorting failed!!!!\n");
		exit(EXIT_FAILURE);
	}

	free(A);
	return sorting_time/1000.0;
}

	void runMinTest(int seed, int numThreads) {
		int i;
		for (i = 1000000; i <= 100000000; i = i + 1000000) {
			double ser = runSerialTest(seed, i);
			double par = runParallelTest(seed, i, numThreads);
			double ratio = ser/par;
			printf("\nRATIO: %4.2lf (with %d input size)\n\n", ratio, i);
		}
	}

	void runVariableThreadTest(int seed, int n) {
		int i;
		for (i = 1; i <= 8; i++) {
			double ser = runSerialTest(seed, n);
			double par = runParallelTest(seed, n, i);
			double ratio = ser/par;
			printf("\nRATIO: %4.2lf (with %d thread(s))\n\n", ratio, i);
		}
	}

int main(int argc, char **argv) {

	if (argc < 3) { // there must be at least two command-line argument
			fprintf(stderr, "Usage: %s <num threads> <input size> [<seed>] \n", argv[0]);
			exit(1);
	}

	int numThreads;
	if (atoi(argv[1]) > 32) {
		fprintf(stderr, "Usage: %s <num threads> <input size> [<seed>]. Too many threads specified, defaulting to 32. \n", argv[0]);
		numThreads = 32;
	}
	else {
		numThreads = atoi(argv[1]);
	}

	int n = atoi(argv[2]);
	int seed = 1;
	if (argc == 4)
		seed = atoi(argv[3]);

	printf("Running %s with a size of %d using %d thread(s):\n", argv[0], n, numThreads);
	runSerialTest(seed, n);
	runParallelTest(seed, n, numThreads);
	//runMinTest(seed, numThreads);
	//runVariableThreadTest(seed, n);



	exit(EXIT_SUCCESS);
}
