#include <omp.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{
    double Speedup = 0;
        if (argc >= 2) {
            Speedup = atof(argv[1]);
        }

        float Fp = (4.0 / 3.0) * (1.0 - (1.0 / Speedup));
        printf("Parallel Fraction = %.2lf\n", Fp);

        return 0;
}