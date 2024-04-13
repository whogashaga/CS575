## Apr.8 2024

### openmp.pptx
1. Page 18 should be `int me = omp_get_thread_num( );`

### trapezoid.pptx
1. Three solutions:
   - Page 5: `#pragma omp atomic`
   - Page 6: `#pragma omp critical`
   - Page 7: `reduction(+:sum)`
2. Page 10: Two Reasons **why Reduction is so much better**
   -  Reduction secretly creates a temporary private variable for each thread’s running **sum**. There will be no conflicts for each thread.
   -  Reduction automatically creates a binary tree structure, like this, to add the N running sums in **log2N** time instead **N** time.

### speedups.and.amdahls.law.pttx
1. Page 9: The Parallel Fraction equation

