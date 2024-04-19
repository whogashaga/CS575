#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define CSV
#define NUMT 4

int	NowYear = 2024;		// 2024- 2029
int	NowMonth = 0;		// 0 - 11

float NowPrecip;		// inches of rain per month
float NowTemp;          // fahrenheit temperature this month
float NowHeight  = 5.;	// grain height in inches
int NowNumDeer   = 2;	// number of deer in the current population

const float GRAIN_GROWS_PER_MONTH   = 12.0;
const float ONE_DEER_EATS_PER_MONTH = 1.0;

const float AVG_PRECIP_PER_MONTH    = 7.0;	// average
const float AMP_PRECIP_PER_MONTH    = 6.0;	// plus or minus
const float RANDOM_PRECIP           = 2.0;	// plus or minus noise

const float AVG_TEMP    = 60.0;	// average
const float AMP_TEMP    = 20.0;	// plus or minus
const float RANDOM_TEMP = 10.0;	// plus or minus noise

const float MIDTEMP   = 40.0;
const float MIDPRECIP = 10.0;

omp_lock_t Lock;
volatile int NumInThreadTeam;
volatile int NumAtBarrier;
volatile int NumGone;

float Ranf(float low, float high)
{
    float r = (float)rand();       // 0 - RAND_MAX
    float t = r / (float)RAND_MAX; // 0. - 1.
    return low + t * (high - low);
}

float SQR(float x)
{
    return x * x;
}

// specify how many threads will be in the barrier:
//	(also init's the Lock)
void InitBarrier(int n)
{
    NumInThreadTeam = n;
    NumAtBarrier = 0;
    omp_init_lock(&Lock);
}

void WaitBarrier()
{
    omp_set_lock(&Lock);
    {
        NumAtBarrier++;
        if (NumAtBarrier == NumInThreadTeam)
        {
            NumGone = 0;
            NumAtBarrier = 0;
            // let all other threads get back to what they were doing
            // before this one unlocks, knowing that they might immediately
            // call WaitBarrier( ) again:
            while (NumGone != NumInThreadTeam - 1)
                ;
            omp_unset_lock(&Lock);
            return;
        }
    }
    omp_unset_lock(&Lock);

    while (NumAtBarrier != 0)
        ; // this waits for the nth thread to arrive

#pragma omp atomic
    NumGone++; // this flags how many threads have returned
}

void Watcher()
{
    while (NowYear < 2030)
    {
        // DoneComputing barrier:
        WaitBarrier();

        // DoneAssigning barrier:
        WaitBarrier();

        float ang = (30. * (float)NowMonth + 15.) * (M_PI / 180.); // angle of earth around the sun

        float temp = AVG_TEMP - AMP_TEMP * cos(ang);
        NowTemp = temp + Ranf(-RANDOM_TEMP, RANDOM_TEMP);
        float centigrade = (5. / 9.) * (NowTemp - 32);

        float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin(ang);
        NowPrecip = precip + Ranf(-RANDOM_PRECIP, RANDOM_PRECIP);
        if (NowPrecip < 0.)
            NowPrecip = 0.;
        float precipCm = (NowPrecip < 0.) ? 0. : (NowPrecip * 2.54);

        fprintf(stderr, "%2d, %d - Temp: %5.2lf °C, Precip: %5.2lf cm | Grain: %6.2lf cm, Deers: %2d\n",
                NowMonth, NowYear, centigrade, precipCm, NowHeight*2.54, NowNumDeer);

        if (NowMonth < 11) {
            NowMonth++;
        }
        else {
            NowMonth = 0;
            if (NowYear < 2030)
                NowYear++;
        }
        // DonePrinting barrier:
        WaitBarrier();
    }
}

void Deer()
{
    while (NowYear < 2030)
    {
        // compute a temporary next-value for this quantity
        // based on the current state of the simulation:
        int nextNumDeer = NowNumDeer;
        int carryingCapacity = (int)(NowHeight);
        if (nextNumDeer < carryingCapacity)
            nextNumDeer++;
        else if (nextNumDeer > carryingCapacity)
            nextNumDeer--;

        // DoneComputing barrier:
        WaitBarrier();

        NowNumDeer = (nextNumDeer < 0) ? 0 : nextNumDeer;
        // DoneAssigning barrier:
        WaitBarrier();

        // DonePrinting barrier:
        WaitBarrier();
    }
}

void Grain()
{
    while (NowYear < 2030)
    {
        // compute a temporary next-value for this quantity
        // based on the current state of the simulation:
        float tempFactor = exp(-SQR((NowTemp - MIDTEMP) / 10.));
        float precipFactor = exp(-SQR((NowPrecip - MIDPRECIP) / 10.));

        float nextHeight = NowHeight;
        nextHeight += tempFactor * precipFactor * GRAIN_GROWS_PER_MONTH;
        nextHeight -= (float)NowNumDeer * ONE_DEER_EATS_PER_MONTH;

        // DoneComputing barrier:
        WaitBarrier();

        NowHeight = (nextHeight < 0.) ? 0. : nextHeight;
        // DoneAssigning barrier:
        WaitBarrier();

        // DonePrinting barrier:
        WaitBarrier();
    }
}

void MyAgent()
{
    while (NowYear < 2030)
    {
        // compute a temporary next-value for this quantity
        // based on the current state of the simulation:

        // DoneComputing barrier:
        WaitBarrier();

        // DoneAssigning barrier:
        WaitBarrier();

        // DonePrinting barrier:
        WaitBarrier();
    }
}

int main(int argc, char *argv[])
{
#ifdef _OPENMP
#ifndef CSV
    fprintf(stderr, "OpenMP is supported -- version = %d\n", _OPENMP);
#endif
#else
    fprintf(stderr, "No OpenMP support!\n");
    return 1;
#endif

    omp_set_num_threads(NUMT); // same as # of sections
    InitBarrier(NUMT);

#pragma omp parallel sections
    {
        #pragma omp section
        {
            Deer();
        }

        #pragma omp section
        {
            Grain();
        }

        #pragma omp section
        {
            Watcher();
        }

        #pragma omp section
        {
            MyAgent(); // your own
        }
    } // implied barrier -- all functions must return in order
      // to allow any of them to get past here
}