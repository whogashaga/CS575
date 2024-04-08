### Introduction
A great use for parallel programming is identical operations on large arrays of numbers.

### Requirements
1. Pick an array size to do the arithmetic on. Something like 16384 (16K) will do. Don't pick something too huge, as your machine may not allow you to use that much memory. Don't pick something too small, as the overhead of using threading might dominate the parallelism gains.
2. Using OpenMP, pairwise multiply two large floating-point arrays, putting the results in another array. Do this in a for-loop. Be sure to use the **#pragma omp parallel for** in the line before the for-loop.
`C[ i ] = A[ i ] * B[ i ];`
3. Do this for one thread and do this for four threads:
```cpp
#define NUMT 1
and
#define NUMT 4
```
4. Use omp_set_num_threads(NUMT); to set the number of threads to use.
5. Time the two runs using two calls to omp_get_wtime();. Convert the timing results into "Mega-Multiplies per Second".
6. What speedup, S, are you seeing when you move from 1 thread to 4 threads?<br>
**`S = (Execution time with one thread) / (Execution time with four threads) = (Performance with four threads) / (Performance with one thread)`**

7. This number should be greater than 1.0 . If not, be sure you are using the correct numerator and denominator.<br>
  If your 1-thread-to-4-threads speedup is S, compute the parallel fraction:<br>
  `float Fp = (4./3.)*( 1. - (1./S) );` <br>
  Don't worry what this means just yet. This will become more meaningful soon. <br>
  You must use only the 1-thread-to-4-threads speedup with this equation. The numbers in this equation depend on that.

8. Your written commentary (turned in as a PDF file) should include:
  - Tell what machine you ran this on
  - What performance results did you get?
  - What was your 1-thread-to-4-thread speedup?
  - Your 1-thread-to-4-thread speedup should be less than 4.0. Why do you think it is this way?
  - What was your Parallel Fraction, Fp? (Hint: it should be less than 1.0, but not much less.)
