// g++ -Xpreprocessor -fopenmp  -I/opt/homebrew/Cellar/libomp/14.0.0/include -L/opt/homebrew/Cellar/libomp/14.0.0/lib -lomp  *.cpp -o main

#include <iostream>
#include <omp.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
using namespace std;

// The "state" of the system consists of the following global variables
int	NowYear;		// 2022 - 2027
int	NowMonth;		// 0 - 11

float NowPrecip;	// inches of rain per month
float NowTemp;		// temperature this month
float NowHeight;	// grain height in inches
int	NowNumDeer;		// number of deer in the current population
// own choice
int NowNumMonster;

// Your basic time step will be one month. Interesting parameters that you need are:
const float GRAIN_GROWS_PER_MONTH =	9.0;
const float ONE_DEER_EATS_PER_MONTH = 1.0;

const float AVG_PRECIP_PER_MONTH = 7.0;	    // average
const float AMP_PRECIP_PER_MONTH = 6.0;	    // plus or minus
const float RANDOM_PRECIP =	2.0;	        // plus or minus noise

const float AVG_TEMP = 60.0;	    // average
const float AMP_TEMP = 20.0;	    // plus or minus
const float RANDOM_TEMP = 10.0;	    // plus or minus noise

const float MIDTEMP = 40.0;
const float MIDPRECIP =	10.0;

// Put this at the top of your program to make it a global
unsigned int seed = 0;

omp_lock_t	Lock;
int		NumInThreadTeam;
int		NumAtBarrier;
int		NumGone;

float SQR(float x)
{
    return x * x;
}

float Ranf(unsigned int *seedp, float low, float high)
{
    float r = (float) rand_r(seedp);  // 0 - RAND_MAX
    return(low + r * (high - low) / (float)RAND_MAX);
}


int Ranf(unsigned int *seedp, int ilow, int ihigh)
{
    float low = (float)ilow;
    float high = (float)ihigh + 0.9999f;

    return (int)(Ranf(seedp, low, high));
}

// specify how many threads will be in the barrier:
// also init's the Lock
void InitBarrier(int n)
{
	NumInThreadTeam = n;
	NumAtBarrier = 0;
	omp_init_lock(&Lock);
}

// have the calling thread wait here until all the other threads catch up:
void WaitBarrier( )
{
    omp_set_lock(&Lock);
    {
        NumAtBarrier++;
        if(NumAtBarrier == NumInThreadTeam){
            NumGone = 0;
            NumAtBarrier = 0;
            // let all other threads get back to what they were doing
			// before this one unlocks, knowing that they might immediately
			// call WaitBarrier( ) again:
            while(NumGone != NumInThreadTeam-1);
            omp_unset_lock(&Lock);
            return;
        }
    }
    omp_unset_lock(&Lock);

    while(NumAtBarrier != 0);	// this waits for the nth thread to arrive

    #pragma omp atomic
    NumGone++;			// this flags how many threads have returned
}

void Monster() {
    while(NowYear < 2028)
    {
        // compute a temporary next-value for this quantity
        // based on the current state of the simulation:
        int nextNumMonster = NowNumMonster;

        if (nextNumMonster >= (NowNumDeer / 2)){
            nextNumMonster--;
        }else if (nextNumMonster < (NowNumDeer / 2)){
            nextNumMonster++;
        }

        if (nextNumMonster < 0) nextNumMonster = 0;

        // DoneComputing barrier:
        WaitBarrier();
        NowNumMonster = nextNumMonster;

        // DoneAssigning barrier:
        WaitBarrier();

        // DonePrinting barrier:
        WaitBarrier();
    }
}

void Deer() {
    while(NowYear < 2028)
    {
        // compute a temporary next-value for this quantity
        // based on the current state of the simulation:
        int nextNumDeer = NowNumDeer;
        int carryingCapacity = (int)(NowHeight);
        if(nextNumDeer < carryingCapacity)
                nextNumDeer++;
        else
            if(nextNumDeer > carryingCapacity)
                nextNumDeer--;
        if(nextNumDeer < 0) nextNumDeer = 0;

        // DoneComputing barrier:
        WaitBarrier();
        NowNumDeer = nextNumDeer;

        // DoneAssigning barrier:
        WaitBarrier();

        // DonePrinting barrier:
        WaitBarrier();
    }
}

void Grain() {
    while(NowYear < 2028)
    {
        // compute a temporary next-value for this quantity
        // based on the current state of the simulation:
        float tempFactor = exp(-SQR((NowTemp - MIDTEMP) / 10.));
        float precipFactor = exp(-SQR((NowPrecip - MIDPRECIP) / 10.));
        float nextHeight = NowHeight;
        nextHeight += tempFactor * precipFactor * GRAIN_GROWS_PER_MONTH;
        nextHeight -= (float)NowNumDeer * ONE_DEER_EATS_PER_MONTH;
        if(nextHeight < 0.) nextHeight = 0.; 

        // DoneComputing barrier:
        WaitBarrier();
        NowHeight = nextHeight;

        // DoneAssigning barrier:
        WaitBarrier();

        // DonePrinting barrier:
        WaitBarrier();
    }
}

void Watcher() {
    while(NowYear < 2028)
    {
        // compute a temporary next-value for this quantity
        // based on the current state of the simulation:

        // DoneComputing barrier:
        WaitBarrier();

        // DoneAssigning barrier:
        WaitBarrier();

        float ang = (30.*(float)NowMonth + 15.) * (M_PI / 180.);

        float temp = AVG_TEMP - AMP_TEMP * cos(ang);
        NowTemp = temp + Ranf(&seed, -RANDOM_TEMP, RANDOM_TEMP);

        float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin(ang);
        NowPrecip = precip + Ranf(&seed, -RANDOM_PRECIP, RANDOM_PRECIP);

        if(NowPrecip < 0.){
            NowPrecip = 0.;
        }

        NowMonth++;
        if (NowMonth == 12){
            NowMonth = 0;
            NowYear++;
        }

        float C_temperature = (5. / 9.) * (NowTemp - 32);
		float P_Height = NowPrecip * 2.54;
		float G_Height = NowHeight * 2.54;
		cout << NowMonth << "," << NowYear << "," << NowNumMonster << "," 
            << NowNumDeer << "," << G_Height << "," << C_temperature 
            << "," << P_Height << endl;

        // DonePrinting barrier:
        WaitBarrier();
    }
}

int main () {

    // starting date and time:
    NowMonth =    0;
    NowYear  = 2022;

    // starting state (feel free to change this if you want):
    NowNumDeer = 1;
    NowHeight =  1.;

    float ang = (30.*(float)NowMonth + 15.) * (M_PI / 180.);

    float temp = AVG_TEMP - AMP_TEMP * cos(ang);
    NowTemp = temp + Ranf(&seed, -RANDOM_TEMP, RANDOM_TEMP);

    float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin(ang);
    NowPrecip = precip + Ranf(&seed, -RANDOM_PRECIP, RANDOM_PRECIP);

    if(NowPrecip < 0.) NowPrecip = 0.;

    omp_set_num_threads(4);	// same as # of sections
    InitBarrier(4);

    cout << "Year,Month,NumMonster,NumDeer,G_Height,Temperature,P_Height" << endl;

    #pragma omp parallel sections
    {
        #pragma omp section
        {
            Deer( );
        }

        #pragma omp section
        {
            Grain( );
        }

        #pragma omp section
        {
            Watcher( );
        }

        #pragma omp section
        {
            Monster();
        }
    }       // implied barrier -- all functions must return in order
            // to allow any of them to get past here
    return 0;
}