// Program for achieving highest flops/sec for matrix multiplication.
// Use SIMD, multi-threading, cache blocking, etc.

#include "config.hpp"

#define A(i,j) A[(i)*N + j]
#define B(i,j) B[(i)*N + j]
#define C(i,j) C[(i)*N + j]

void packB_KCxMC(double *packB, double *B, int , int , aux_t *);
void packA_NCxKC(double *packA, double *A, int , int , aux_t *);
void micro_kernel(double *A, double *B, double *C, aux_t *);

// XA - array of size NCxKC.
// XB - array of size KCxMC.
void macro_kernel(double *XA,
                  double *XB,
                  double *C,
                  int nc_min,
                  int kc_min,
                  int mc_min,
                  aux_t *aux)
{
  // inside this loop is a block of A of dim MC x KC and B of dim KC x NC.
  // now we need to iterate over the elements of these make it run.

  // the block of B is vertically divided into blocks of width (cols) NR, and the
  // block of A into blocks of length MR. The multiplication between these
  // blocks needs to be treated like a matrix multiplication by itself and
  // should therefore consume its own triple nested loop.

  // Imagine it this way: the nc block is the constraint of the matrix that
  // you need to work under. The topmost NC loop only iterates over that
  // much loop space. All the loops below need to do the full work of
  // multiplying the full matrix that falls under NC.

  // The names given to the blocks are very indicative. The m,n,k denote dims
  // of the main matrices. The mc,nc and kc denote the dims of the inner blocks
  // as made by the immediately inner loops and the mr, nr and kr denote the
  // innermost blocks where the actual matrix multiplication happens.

  // Now, since we make a jump of NC in the outermost loop, we need to compute
  // the result for every single row of C, just like we computed the result
  // for each row of C individually when the NC-like outermost loop jump
  // was only 4.

  // Below loops loop over the blocks of the inner blocks of B with dim KC x NR.
  // Treat this like the blocked matrix multiplication in the previous loops. These
  // also need to multiply the specific elements within them and compute a point-by-point
  // matrix product.

  // Note that the entire configuration changes when row-major is being done. Algorithms
  // must be entirely reworked for that purpose when changing
  #pragma omp parallel for
  for (int mr = 0; mr < nc_min; mr += MR) {
    int index_c = (aux->nc + mr)*ldc + aux->mc;
    for (int nr = 0; nr < mc_min; nr += NR) {
      aux->nr = nr;
      aux->mr = mr;
      micro_kernel(&XA[mr*aux->kc_min], &XB[nr*aux->kc_min],
                   &C[index_c + nr], aux);
    }
  }
}

// multiply micro-panels of size MR x KC and KC x NR.
void micro_kernel(double *A, register double *B, double *C, aux_t *aux)
{
  register double *B_ptr, *C_ptr, *A_ptr, *A_temp, *C_temp, *C_temp_p4;
  register int lead_c = ldc;
  register int lead_c_p4 = lead_c + 4;
  register int lead_c_2 = 2*ldc;
  register int lead_c_2_p4 = lead_c_2 + 4;
  register int lead_c_3 = 3*ldc;
  register int lead_c_3_p4 = lead_c_3 + 4;
  register int inc_C = MR_INCR*lead_c;
  register int inc_A = (MR_INCR - 1)*aux->kc_min;

  C_ptr = C;
  B_ptr = B;
  A_ptr = A;
  // AVX2 container for A, B and C.
  __m256d A_avx0, A_avx1, A_avx2, A_avx3;
  __m256d B_avx0, B_avx1;
  __m256d C_avx000, C_avx001,
    C_avx100, C_avx101,
    C_avx200, C_avx201,
    C_avx300, C_avx301;
  // format - instruction source_op dest_op
  
  for (int i = 0; i < MR; i += MR_INCR) {
    B_ptr = B;
    C_temp = C_ptr;
    C_temp_p4 = C_ptr + 4;
    // For each completion of the below two nested loops, B is scanned from top to bottom.

    // Also notice that C stays inside the same place in memory throughout the loop.
    // Thus it can be a good candidate for assigning into registers.
    // __asm__ volatile("vmovapd (%1), %%ymm0  \n\t"
    //                  "    \n\t"
    //                  :  // output
    //                  : "r" (C_ptr), "r" (lead_c), "r" (lead_c_p4),
    //                    "r" (lead_c_2), "r" (lead_c_2_p4), "r" (lead_c_3),
    //                    "r" (lead_c_3_p4)// input
    //                  ); // clobber

    
    
    C_avx000 = _mm256_load_pd(C_temp);
    C_avx001 = _mm256_load_pd(C_temp_p4);
    C_temp += lead_c;
    C_temp_p4 += lead_c;
    
    C_avx100 = _mm256_load_pd(C_temp);
    C_avx101 = _mm256_load_pd(C_temp_p4);
    C_temp += lead_c;
    C_temp_p4 += lead_c;
    
    C_avx200 = _mm256_load_pd(C_temp);
    C_avx201 = _mm256_load_pd(C_temp_p4);
    C_temp += lead_c;
    C_temp_p4 += lead_c;
    
    C_avx300 = _mm256_load_pd(C_temp);
    C_avx301 = _mm256_load_pd(C_temp_p4);

    // For every iteration of k, B_ptr is incremented once by NR.
    //   Thus the whole array is scanned.
    // For every iteration of k, A_ptr is incremented once by 1.
    //   Thus one row of A_ptr is scanned.
    for (int k = 0; k < aux->kc_min; k += 1) {
      A_temp = A_ptr;
      A_avx0 = _mm256_broadcast_sd(A_temp);
      A_temp += aux->kc_min;
      A_avx1 = _mm256_broadcast_sd(A_temp);
      A_temp += aux->kc_min;
      A_avx2 = _mm256_broadcast_sd(A_temp);
      A_temp += aux->kc_min;
      A_avx3 = _mm256_broadcast_sd(A_temp);
      
      B_avx0 = _mm256_load_pd(B_ptr);
      B_avx1 = _mm256_load_pd(B_ptr + 4);
      
      // A0 * NR0
      C_avx000 = _mm256_fmadd_pd(A_avx0, B_avx0, C_avx000);
      C_avx001 = _mm256_fmadd_pd(A_avx0, B_avx1, C_avx001);

      // A4 * NR0
      C_avx100 = _mm256_fmadd_pd(A_avx1, B_avx0, C_avx100);
      C_avx101 = _mm256_fmadd_pd(A_avx1, B_avx1, C_avx101);

      // A8 * NR0
      C_avx200 = _mm256_fmadd_pd(A_avx2, B_avx0, C_avx200);
      C_avx201 = _mm256_fmadd_pd(A_avx2, B_avx1, C_avx201);

      // A12 * NR0
      C_avx300 = _mm256_fmadd_pd(A_avx3, B_avx0, C_avx300);
      C_avx301 = _mm256_fmadd_pd(A_avx3, B_avx1, C_avx301);
      
      B_ptr += NR;
      A_ptr++;
    }

    _mm256_store_pd(C_ptr            , C_avx000);
    _mm256_store_pd(C_ptr + 4        , C_avx001);
    _mm256_store_pd(C_ptr + lead_c      , C_avx100);
    _mm256_store_pd(C_ptr + lead_c_p4  , C_avx101);
    _mm256_store_pd(C_ptr + lead_c_2    , C_avx200);
    _mm256_store_pd(C_ptr + lead_c_2_p4, C_avx201);
    _mm256_store_pd(C_ptr + lead_c_3    , C_avx300);
    _mm256_store_pd(C_ptr + lead_c_3_p4, C_avx301);
    
    C_ptr += inc_C;
    A_ptr += inc_A;
  }
}

void matmul(double *A, double *B, double *C, aux_t *aux)
{
  double *packA, *packB;
  packA = malloc_aligned(NC, KC, sizeof(double));
  packB = malloc_aligned(KC, MC, sizeof(double));
  // each iteration of this loop advances the rows of A and C.
  for (int nc = 0; nc < N; nc += NC) {  // like i.
    aux->nc = nc;
    int nc_min = std::min((N-nc), NC);
    
    // each iteration of this advances the cols of A and rows of B.
    for (int kc = 0; kc < N; kc += KC) { // like k.
      int kc_min = std::min((N-kc), KC);
      aux->kc = kc;
      aux->kc_min = kc_min;
      packA_NCxKC(packA, A, nc_min, kc_min, aux);

      // each iteration advances the cols of B and C.
      for (int mc = 0; mc < N; mc += MC) { // like j.
        aux->mc = mc;
        int mc_min = std::min(N-mc, MC);
        aux->mc_min = mc_min;

        packB_KCxMC(packB, B, kc_min, mc_min, aux);
        macro_kernel(packA, packB, &C[nc*nc_min], nc_min, kc_min, mc_min, aux);
      }
    }
  }
}

void packA_NCxKC(double *packA, double *A, int nc_min, int kc_min, aux_t *aux)
{
  double *packA_temp = packA;
  double *A_ptr;

  for (int n = aux->nc; n < aux->nc + nc_min; ++n) {
    A_ptr = &A(n,aux->kc);
    for (int k = 0; k < kc_min; ++k) {
      *(packA_temp++) = *(A_ptr++);
    }
  }
}

void packB_KCxMC(double *packB, double *B, int kc_min, int mc_min, aux_t *aux)
{
  double *packB_temp = packB, *temp;

  for (int m = aux->mc; m < aux->mc + mc_min; m += NR) {
    for (int k = aux->kc; k < aux->kc + kc_min; k++) {
      temp = &B(k,m);
      *(packB_temp++) = *(temp);
      *(packB_temp++) = *(temp + 1);
      *(packB_temp++) = *(temp + 2);
      *(packB_temp++) = *(temp + 3);
      
      *(packB_temp++) = *(temp + 4);
      *(packB_temp++) = *(temp + 5);
      *(packB_temp++) = *(temp + 6);
      *(packB_temp++) = *(temp + 7);
    }
  }
}

int main(int argc, char ** argv)
{
  if (argc != 2) {
    std::cout << "provide nrows and ncols" << std::endl;
    exit(1);
   }

  if (MC < MR) {
    std::cout << "wrong config: MC should be greater than MR." << std::endl;
    exit(1);
  }
  
  N = atoi(argv[1]); M = N;
  
  lda = ldb = ldc = N;
  double *A, *B, *C;
  double start, stop;
  aux_t aux;
  A = (double*)malloc(N * N * sizeof(double));
  B = (double*)malloc(N * N * sizeof(double));
  C = malloc_aligned(N, N, sizeof(double));
  generate_data(A, B, C, N);
  
  start = get_time();
  matmul(A, B, C, &aux);
  stop = get_time();

  std::cout << "N = " << N << ". time: " << stop - start << " s. Gflops: " <<
    2.*N*N*N/(stop-start)/1e9 << std::endl;

  double *D = (double*)calloc(sizeof(double), N*N);
  reset_matrix(D, N, 0);
  start = get_time();
  for (int i=0; i<N; i++) {
    for (int j=0; j<N; j++) {
      for (int k=0; k<N; k++) {
        D[i*N + j] += A[i*N + k] * B[k*N + j];
      }
    }
  }
  stop = get_time();

  std::cout << "N = " << N << ". time: " << stop - start << " s. Gflops: " <<
    2.*N*N*N/(stop-start)/1e9 << std::endl;


  double error = 0;

  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      error += C[i*N + j] - D[i*N + j];
    }
  }

  std::cout << "error: " << error/N/N << std::endl;
  return 0;
}
