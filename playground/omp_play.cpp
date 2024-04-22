#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <cstdio>


int main(int argc, char* argv[])
{
    #ifdef   _OPENMP
	    fprintf( stderr, "OpenMP version %d is supported here\n", _OPENMP );
    #else
        fprintf( stderr, "OpenMP is not supported here - sorry!\n" );
        exit( 0 );
    #endif

    omp_set_num_threads( 2 );
    #pragma omp parallel
    {
        #pragma omp single
        {
            #pragma omp task
                printf("A\n" );
            #pragma omp taskwait

            #pragma omp task
                printf("B\n" );
            #pragma omp taskwait
        }
        
    }
    return 0;
}