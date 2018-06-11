# HPC lecture assingment

Run command `bash tsubame_run.sh` to call a script that compiles the program and sends the executable
to a single f-node of TSUBAME.

# Author

Sameer Deshmukh.
17M38101

# Note

THIS USES DOUBLES.

I have not used BLISlab. This code has been written by me for better understanding of
the topic. You can find the major functionality in the `main.cpp` file.

# Benchmarks

O0 optimizations : \n
N = 16. time: 1.69277e-05 s. Gflops: 0.48394
N = 32. time: 2.90871e-05 s. Gflops: 2.2531
N = 64. time: 9.10759e-05 s. Gflops: 5.75661
N = 128. time: 0.000359058 s. Gflops: 11.6814
N = 256. time: 0.00142002 s. Gflops: 23.6295
N = 512. time: 0.00633001 s. Gflops: 42.4068
N = 1024. time: 0.0338309 s. Gflops: 63.477
N = 2048. time: 0.229985 s. Gflops: 74.7
N = 4096. time: 1.57101 s. Gflops: 87.4846
N = 8192. time: 11.4372 s. Gflops: 96.1351

# YMM register notes

I have manually written ASM for speeding up SIMD. The YMM register-to-variable mappings are:
* `ymm0` - `C_avx000`
* `ymm1` - `C_avx001`
* `ymm2` - `C_avx100`
* `ymm3` - `C_avx101`
* `ymm4` - `C_avx200`
* `ymm5` - `C_avx201`
* `ymm6` - `C_avx300`
* `ymm7` - `C_avx301`

* `ymm8` - `A_avx0`
* `ymm9` - `A_avx1`
* `ymm10` - `A_avx2`
* `ymm11` - `A_avx3`

* `ymm12` - `B_avx0`
* `ymm13` - `B_avx1`
