#include <omp.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

// #ifndef NUMT
// #define NUMT 1	// number of threads to use -- do once for 1 and once for 4
// #endif

#ifndef SIZE
#define SIZE 56789	// array size -- you get to decide
#endif

#define NUMTRIES 20	// how many times to run the timing to get reliable timing data

float A[SIZE];
float B[SIZE];
float C[SIZE];

int main(int argc, char* argv[])
{
#ifdef   _OPENMP
	fprintf( stderr, "OpenMP version %d is supported here\n", _OPENMP );
#else
	fprintf( stderr, "OpenMP is not supported here - sorry!\n" );
	exit( 0 );
#endif
        
        int NUMT = 1;
        if (argc >= 2) {
                NUMT = atoi( argv[1] ); // inject number of threads as an argument from bash script
        }

        // initialize the arrays:
	for( int i = 0; i < SIZE; i++ )
	{
		A[i] = 1.;
		B[i] = 2.;
	}

        omp_set_num_threads( NUMT );

        double maxMegaMults = 0.;

        for( int t = 0; t < NUMTRIES; t++ )
        {
                double time0 = omp_get_wtime( );

                #pragma omp parallel for
                for( int i = 0; i < SIZE; i++ )
                {
                        C[i] = A[i] * B[i];
                }

                double time1 = omp_get_wtime( );
                double megaMults = (double)SIZE/(time1-time0)/1000000.;
                if( megaMults > maxMegaMults ) {
                        maxMegaMults = megaMults;
                }
                
        }

        fprintf(stderr, "For %d threads, Peak Performance = %.2lf MegaMults/Sec\n", NUMT, maxMegaMults);
        // note: %lf stands for "long float", which is how printf prints a "double"
	//        %d stands for "decimal integer", not "double"
        
        // Speedup = (Peak performance for 4 threads) / (Peak performance for 1 thread)
	
        cout << maxMegaMults << endl;
        return 0;
}