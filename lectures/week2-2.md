## Apr.10 2024

### Project1
1. swap between cores since there is not enough hardware.

### moores.law.and.multicore.pptx
  1. “Clock speed doubles every 1.5 years.” is the output of Moore's law, not equivalence.
  2. Page 6, Why does clock speed level out? PowerConsumption is proportional to the square of ClockSpeed
  3. Page 16, 17, **States** are **Threads**
  4. Page 18, 8 threads does not cut the elapsed time to half since there are only 4 cores. 8 Threads have to swap between only 4 cores.

### funcitonal.decomposition.pptx
  1. **Strats from page 6, highly related to Project2**
  2. Page 10, cannot use ` #pragma omp barrier` because those threads are not in the same memory

### twoassemblycodes.pptx
  1. play with some bugs in the function **WaitBarrier()**

### hyperthreading.and.almost.amdahl.pptx
  1. Page 4, the mathematic equation for Hyperthreading SpeedUp
  2. Amdahl's law: Do more things
