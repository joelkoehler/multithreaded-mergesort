# Multithreaded Merge Sort

This project implements a multithreaded version of the
merge sort algorithm using the pthread library.

## Learning Objectives

- Take a singled threaded algorithm and safely convert it to threaded
  to see a performance gain.
- To gain more experience writing concurrent code.
- Explore the pthread library

## Make Concurrent

Convert the serial mergesort code to use multiple threads using the
pthread library. Your program should limit the number of threads it
uses via a command line argument (for example, this could be the total
number of threads or number of levels before cutting off new thread
generation). You must get a speedup of at least 2 with 4 or more cores
to get full credit on this project.  Use n = 100,000,000 elements for
your testing.

NOTE: Just because you see a speedup of 2 does not guarantee full
credit. You can easily hardcode the number of threads and hardcode the
branches to get a speedup of 2. Your code MUST work with a variable
number of threads!  Simply getting a speedup of 2 does not indicate
that you did the assignment correctly.

You will need to update mytests.c to accept a number of threads a
command line argument. The updated version of mytests should include
timing results for both the serial and parallel versions of merge
sort.

## Implementation Analysis

Create a file named speedup.pdf (in the root directory) that analyses
the following two aspects of your mergesort solution.

**Efficiency** - Analyse the thread efficiency by examining how the
threaded implementation’s performance compares to the single threaded
implementation. Include a chart showing the speedups obtained with
varying number of threads (from 1 to 8). The speedups should relative
to the serial mergesort. Please also note the number of cores on the
system that you are testing.  All tests cases should be run with 100
million elements. Please discuss whether the speedup increases
linearly as the number of threads are increased.

**Effectiveness** - Analyse the effectiveness of your threaded
implementation by comparing sorting time of your threaded solution to
the single threaded solution. Run both the serial mergesort and
threaded mergesort with elements in the range from 1,000,000 to
100,000,000, incrementing by 1M each time.  The threaded version of
merge sort should be run with 5 threads for all test cases.  Plot the
timing results in a line chart (google sheets or excel) and compare
the results. Please discuss whether the threaded version always
performs better than the serial version.
