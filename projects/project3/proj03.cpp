#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <string>
using namespace std;

// setting the number of threads:
#ifndef NUMT
#define NUMT		    1
#endif

// setting the number of capitals we want to try:
#ifndef NUMCAPITALS
#define NUMCAPITALS	5
#endif


// maximum iterations to allow looking for convergence:
#define MAXITERATIONS	100

// how many tries to discover the maximum performance:
#define NUMTIMES	20

#define CSV

struct city
{
	std::string	name;
	float		longitude;
	float		latitude;
	int		capitalnumber;
	float		mindistance;
};

#include "UsCities.data"

// setting the number of cities we want to try:
#define NUMCITIES 	( sizeof(Cities) / sizeof(struct city) )

// define object 'capital'
struct capital
{
	std::string	name;
	float		longitude;
	float		latitude;
	float		longsum;
	float		latsum;
	int		numsum;
};

// declare object array of 'capital' with size NUMCAPITALS
struct capital	Capitals[NUMCAPITALS];


float
Distance( int city, int capital )
{
	float dx = Cities[city].longitude - Capitals[capital].longitude;
	float dy = Cities[city].latitude  - Capitals[capital].latitude;
	return sqrtf( dx*dx + dy*dy );
}


int
main( int argc, char *argv[ ] )
{
	// #ifdef _OPENMP
	// 	fprintf( stderr, "OpenMP is supported -- version = %d\n", _OPENMP );
	// #else
	//         fprintf( stderr, "No OpenMP support!\n" );
	//         return 1;
	// #endif

	// make sure we have the data correctly:
	// for( int i = 0; i < NUMCITIES; i++ )
	// {
	// 	fprintf( stderr, "%3d  %8.2f  %8.2f  %s\n", i, Cities[i].longitude, Cities[i].latitude, Cities[i].name.c_str() );
	// }

	omp_set_num_threads(NUMT); // set the number of threads to use in parallelizing the for-loop:`

	// seed the capitals:
	// (this is just picking initial capital cities at uniform intervals)
	for( int k = 0; k < NUMCAPITALS; k++ )
	{
		int cityIndex = k * (NUMCITIES-1) / (NUMCAPITALS-1);
		Capitals[k].longitude = Cities[cityIndex].longitude;
		Capitals[k].latitude  = Cities[cityIndex].latitude;
		// fprintf(stderr, "%d. longitude: %f, latitude: %f\n", k, Capitals[k].name.c_str(), Capitals[k].longitude, Capitals[k].latitude);
	}

	double time0, time1;
	for( int n = 0;  n < MAXITERATIONS; n++ )
	{
		// reset the summations for the capitals:
		for( int k = 0; k < NUMCAPITALS; k++ )
		{
			Capitals[k].longsum = 0.;
			Capitals[k].latsum  = 0.;
			Capitals[k].numsum = 0;
		}

		time0 = omp_get_wtime( );

        	// the #pragma goes here -- you figure out what it needs to look like:
		#pragma omp parallel for default(none), shared(Capitals, Cities)
		for( int i = 0; i < NUMCITIES; i++ )
		{
			int capitalnumber = -1;
			float mindistance = 1.e+37;
			// fprintf(stderr, "= = = = %d. Distance Calculation = = = =\n", i);
			for( int k = 0; k < NUMCAPITALS; k++ )
			{
				float dist = Distance( i, k );
				// fprintf(stderr, "dist: %f\n", dist);
				if( dist < mindistance )
				{
					mindistance = dist;
					Cities[i].mindistance = dist;
					Cities[i].capitalnumber = k;
				}
			}

			int capitalNum = Cities[i].capitalnumber;
			// fprintf(stderr, "~ ~ ~ ~ ~CapitalNumber: %d\n", i, capitalNum);
			// this is here for the same reason as the Trapezoid noteset uses it:
			#pragma omp critical
			{
				Capitals[capitalNum].longsum += Cities[i].longitude;
				Capitals[capitalNum].latsum += Cities[i].latitude;
				Capitals[capitalNum].numsum++;
			}
		}
		time1 = omp_get_wtime( );

		// fprintf(stderr, "\n = = = = Averaging Long and Lat = = = = \n");
		// get the average longitude and latitude for each capital:
		for( int k = 0; k < NUMCAPITALS; k++ )
		{
			Capitals[k].longitude = Capitals[k].longsum / Capitals[k].numsum;
			Capitals[k].latitude = Capitals[k].latsum / Capitals[k].numsum;
			// fprintf(stderr, "Centroid %d. longitude: %f, latitude: %f\n", k, Capitals[k].longitude, Capitals[k].latitude);
		}
	}

	double megaCityCapitalsPerSecond = (double)NUMCITIES * (double)NUMCAPITALS / ( time1 - time0 ) / 1000000.;


	// figure out what actual city is closest to each capital:
	// this is the extra credit:
	for( int k = 0; k < NUMCAPITALS; k++ )
	{
		// fprintf(stderr, "%d. closest: %s\n", k, Capitals[k].closestCity.c_str());
	}

	// print the longitude-latitude of each new capital city:
	// you only need to do this once per some number of NUMCAPITALS -- do it for the 1-thread version:
	if( NUMT == 1 )
	{
		for( int k = 0; k < NUMCAPITALS; k++ )
		{
			float dist = 1.e+37;
			for (int i = 0; i < NUMCITIES; i++)
			{
				int capitalNum = Cities[i].capitalnumber;
				float minDis = Cities[i].mindistance;
				if (capitalNum == k && minDis < dist)
				{
					dist = minDis;
					Capitals[k].name = Cities[i].name;
				}
			}
			//if you did the extra credit, use this fprintf instead:
			fprintf( stderr, "\t%3d:  %8.2f , %8.2f , %s\n", k, Capitals[k].longitude, Capitals[k].latitude, Capitals[k].name.c_str() );
		}
	}
#ifdef CSV
        fprintf(stderr, "%2d , %4d , %4d , %8.3lf\n", NUMT, NUMCITIES, NUMCAPITALS, megaCityCapitalsPerSecond );
#else
        fprintf(stderr, "%2d threads : %4d cities ; %4d capitals; megatrials/sec = %8.3lf\n",
                NUMT, NUMCITIES, NUMCAPITALS, megaCityCapitalsPerSecond );
#endif
	}