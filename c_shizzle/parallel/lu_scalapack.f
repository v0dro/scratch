      SUBROUTINE PDGETRF( nrows_of_sub_A, ncols_of_sub_A, A,
     $     row_matrix_block, col_matrix_block,
     $     DESCA, IPIV, INFO )
*
*  -- ScaLAPACK routine (version 1.0) --
*     University of Tennessee, Knoxville, Oak Ridge National Laboratory,
*     and University of California, Berkeley.
*     February 28, 1995
*
*     .. Scalar Arguments ..
      INTEGER            IA, INFO, row_matrix_block, nrows_of_sub_A, ncols_of_sub_A
*     ..
*     .. Array Arguments ..
      INTEGER            DESCA( * ), IPIV( * )
      DOUBLE PRECISION   A( * )
*     ..
*
*  Purpose
*  =======
*
*  PDGETRF computes an LU factorization of a general M-by-N distributed
*  matrix sub( A ) = (IA:IA+M-1,col_matrix_block:col_matrix_block+N-1) using partial pivoting with
*  row interchanges.
*
*  The factorization has the form sub( A ) = P * L * U, where P is a
*  permutation matrix, L is lower triangular with unit diagonal ele-
*  ments (lower trapezoidal if m > n), and U is upper triangular
*  (upper trapezoidal if m < n). L and U are stored in sub( A ).
*
*  This is the right-looking Parallel Level 3 BLAS version of the
*  algorithm.
*
*  Notes
*  =====
*
*  A description vector is associated with each 2D block-cyclicly dis-
*  tributed matrix.  This vector stores the information required to
*  establish the mapping between a matrix entry and its corresponding
*  process and memory location.
*
*  In the following comments, the character _ should be read as
*  "of the distributed matrix".  Let A be a generic term for any 2D
*  block cyclicly distributed matrix.  Its description vector is DESCA:
*
*  NOTATION        STORED IN  EXPLANATION
*  --------------- ---------- ------------------------------------------
*  M_A    (global) DESCA( 1 ) The number of rows in the distributed
*                             matrix.
*  N_A    (global) DESCA( 2 ) The number of columns in the distributed
*                             matrix.
*  MB_A   (global) DESCA( 3 ) The blocking factor used to distribute
*                             the rows of the matrix.
*  NB_A   (global) DESCA( 4 ) The blocking factor used to distribute
*                             the columns of the matrix.
*  RSRC_A (global) DESCA( 5 ) The process row over which the first row
*                             of the matrix is distributed.
*  CSRC_A (global) DESCA( 6 ) The process column over which the first
*                             column of the matrix is distributed.
*  CTXT_A (global) DESCA( 7 ) The BLACS context handle, indicating the
*                             BLACS process grid A is distributed over.
*                             The context itself is global, but the handle
*                             (the integer value) may vary.
*  LLD_A  (local)  DESCA( 8 ) The leading dimension of the local array
*                             storing the local blocks of the distri-
*                             buted matrix A. LLD_A >= MAX(1,LOCp(M_A)).
*
*  Let K be the number of rows or columns of a distributed matrix,
*  and assume that its process grid has dimension p x q.
*  LOCp( K ) denotes the number of elements of K that a process
*  would receive if K were distributed over the p processes of its
*  process column.
*  Similarly, LOCq( K ) denotes the number of elements of K that a
*  process would receive if K were distributed over the q processes of
*  its process row.
*  The values of LOCp() and LOCq() may be determined via a call to the
*  ScaLAPACK tool function, NUMROC:
*          LOCp( M ) = NUMROC( M, MB_A, MYROW, RSRC_A, NPROW ),
*          LOCq( N ) = NUMROC( N, NB_A, MYCOL, CSRC_A, NPCOL ).
*
*  This routine requires square block decomposition ( MB_A = NB_A ).
*
*  Arguments
*  =========
*
*  M       (global input) INTEGER
*          The number of rows to be operated on, i.e. the number of rows
*          of the distributed submatrix sub( A ). M >= 0.
*
*  N       (global input) INTEGER
*          The number of columns to be operated on, i.e. the number of
*          columns of the distributed submatrix sub( A ). N >= 0.
*
*  A       (local input/local output) DOUBLE PRECISION pointer into the
*          local memory to an array of dimension (LLD_A, LOCq(col_matrix_block+N-1)).
*          On entry, this array contains the local pieces of the M-by-N
*          distributed matrix sub( A ) to be factored. On exit, this
*          array contains the local pieces of the factors L and U from
*          the factorization sub( A ) = P*L*U; the unit diagonal ele-
*          ments of L are not stored.
*
*  IA      (global input) INTEGER
*          A's global row index, which points to the beginning of the
*          submatrix which is to be operated on.
*
*  col_matrix_block      (global input) INTEGER
*          A's global column index, which points to the beginning of
*          the submatrix which is to be operated on.
*
*  DESCA   (global and local input) INTEGER array of dimension 8
*          The array descriptor for the distributed matrix A.
*
*  IPIV    (local output) INTEGER array, dimension ( LOCp(M_A)+MB_A )
*          This array contains the pivoting information.
*          IPIV(i) -> The global row local row i was swapped with.
*          This array is tied to the distributed matrix A.
*
*  INFO    (global output) INTEGER
*          = 0:  successful exit
*          < 0:  If the i-th argument is an array and the j-entry had
*                an illegal value, then INFO = -(i*100+j), if the i-th
*                argument is a scalar and had an illegal value, then
*                INFO = -i.
*
*  =====================================================================
*          > 0:  If INFO = K, U(IA+K-1,col_matrix_block+K-1) is exactly zero.
*                The factorization has been completed, but the factor U
*                is exactly singular, and division by zero will occur if
*                it is used to solve a system of equations.
*
*  =====================================================================
*
*     .. Parameters ..
      DOUBLE PRECISION   ONE
      PARAMETER          ( ONE = 1.0D+0 )
*     ..
*     .. Local Scalars ..
      CHARACTER          COLBTOP, COLCTOP, ROWBTOP
      INTEGER            I, ICOFF, ICTXT, IINFO, IROFF, J, JB, JN, MN,
     $                   MYCOL, MYROW, NPCOL, NPROW
*     ..
*     .. Local Arrays ..
      INTEGER            IDUM1( 1 ), IDUM2( 1 )
*     ..
*     .. External Subroutines ..
      EXTERNAL           BLACS_GRIDINFO, CHK1MAT, IGAMN2D, PCHK1MAT,
     $                   PTOPGET, PTOPSET, PDGEMM, PDGETF2,
     $                   PDLASWP, PDTRSM, PXERBLA
*     ..
*     .. External Functions ..
      INTEGER            ICEIL, INDXG2P, NUMROC
      EXTERNAL           ICEIL, INDXG2P, NUMROC
*     ..
*     .. Intrinsic Functions ..
      INTRINSIC          MAX, MIN, MOD
*     ..
*     .. Executable Statements ..
*
*     Get grid parameters
*
      ICTXT = DESCA( 7 ) ! set the context
      CALL BLACS_GRIDINFO( ICTXT, NPROW, NPCOL, MYROW, MYCOL )
*
*     Test the input parameters
*
      INFO = 0
      IF( NPROW.EQ.-1 ) THEN
         INFO = -607
      ELSE
         CALL CHK1MAT( nrows_of_sub_A, 1, ncols_of_sub_A, 2,
     $        row_matrix_block, col_matrix_block, DESCA, 6, INFO )
         IF( INFO.EQ.0 ) THEN
            IROFF = MOD( row_matrix_block-1, DESCA( 3 ) )
            ICOFF = MOD( col_matrix_block-1, DESCA( 4 ) )
            IF( IROFF.NE.0 ) THEN
               INFO = -4
            ELSE IF( ICOFF.NE.0 ) THEN
               INFO = -5
            ELSE IF( DESCA( 3 ).NE.DESCA( 4 ) ) THEN
               INFO = -604
            END IF
         END IF
         CALL PCHK1MAT( nrows_of_sub_A, 1, ncols_of_sub_A, 2, row_matrix_block, col_matrix_block,
     $        DESCA, 6, 0, IDUM1, IDUM2, INFO )
      END IF
*
      IF( INFO.NE.0 ) THEN
! xerbla is an error handler for lapack routines.
! called if an input parameter is wrong
         CALL PXERBLA( ICTXT, 'PDGETRF', -INFO )
         RETURN
      END IF
*
*     Quick return if possible
*
      IF( DESCA( 1 ).EQ.1 ) THEN ! if elements in block is 1
         IPIV( 1 ) = 1
         RETURN
      ELSE IF( nrows_of_sub_A.EQ.0 .OR. ncols_of_sub_A.EQ.0 ) THEN
         RETURN
      END IF
*
*     Split-ring topology for the communication along process rows
*
      CALL PTOPGET( 'Broadcast', 'Rowwise', ROWBTOP )
      CALL PTOPGET( 'Broadcast', 'Columnwise', COLBTOP )
      CALL PTOPGET( 'Combine', 'Columnwise', COLCTOP )
      CALL PTOPSET( 'Broadcast', 'Rowwise', 'S-ring' )
      CALL PTOPSET( 'Broadcast', 'Columnwise', ' ' )
      CALL PTOPSET( 'Combine', 'Columnwise', ' ' )
*
*     Handle the first block of columns separately
*
      MN = MIN( nrows_of_sub_A, ncols_of_sub_A )
      ! ICEIL - compute the ceiling of the division of two integers.
      JN = MIN( ICEIL( col_matrix_block, DESCA( 4 ) )*DESCA( 4 ),
     $     col_matrix_block+MN-1 )
      
      nrows_matrix_sub_block = JN - col_matrix_block + 1 ! first run: col_matrix_block = 1. JN = 
*
*     Factor diagonal and subdiagonal blocks and test for exact
*     singularity.
*
*     Calculate LU decomposition of the L00, L01 and U00 step.
*     Reduction is done later.   
*
      CALL PDGETF2( nrows_of_sub_A, nrows_matrix_sub_block, A, row_matrix_block,
     $     col_matrix_block, DESCA, IPIV, INFO )
*
      IF( nrows_matrix_sub_block+1.LE.ncols_of_sub_A ) THEN
*
*        Apply interchanges to columns col_matrix_block+nrows_matrix_sub_block:col_matrix_block+N-1.
*
         CALL PDLASWP( 'Forward', 'Rows', ncols_of_sub_A-nrows_matrix_sub_block, A,
     $        row_matrix_block, col_matrix_block+nrows_matrix_sub_block, DESCA,
     $        row_matrix_block, row_matrix_block+nrows_matrix_sub_block-1, IPIV )
*
*        Compute block row of U.
*
         CALL PDTRSM( 'Left', 'Lower', 'No transpose', 'Unit',
     $        nrows_matrix_sub_block,
     $        ncols_of_sub_A-nrows_matrix_sub_block, ONE, A,
     $        row_matrix_block, col_matrix_block, DESCA, A,
     $        row_matrix_block, col_matrix_block + nrows_matrix_sub_block,
     $        DESCA )
*
         IF( nrows_matrix_sub_block+1.LE.nrows_of_sub_A ) THEN
*
*           Update trailing submatrix.
*
            CALL PDGEMM( 'No transpose', 'No transpose',
     $           nrows_of_sub_A - nrows_matrix_sub_block,
     $           ncols_of_sub_A - nrows_matrix_sub_block,
     $           nrows_matrix_sub_block, -ONE, A,
     $           row_matrix_block + nrows_matrix_sub_block,
     $           col_matrix_block, DESCA, A, row_matrix_block,
     $           col_matrix_block+nrows_matrix_sub_block, DESCA,
     $           ONE, A, row_matrix_block+nrows_matrix_sub_block,
     $           col_matrix_block+nrows_matrix_sub_block, DESCA )
*
         END IF
      END IF
*
*     Loop over the remaining blocks of columns.
*
! DESCA(4) blocking factor for distributing columns of the matrix.
! This allows the matrix blocks to be iterated over in a digonal block-by-diagonal block
! manner. All the processes iterate in this manner.
      DO 10 J = JN+1, col_matrix_block+MN-1, DESCA( 4 )
         ! This MIN prolly exists cuz the last process might have lesser columns of data.
         nrows_matrix_sub_blocks = MIN( MN-J+col_matrix_block, DESCA( 4 ) ) ! nrows_matrix_sub_block
         I = row_matrix_block + J - col_matrix_block
*
*        Factor diagonal and subdiagonal blocks and test for exact
*        singularity.
*
*        This is applying pivoting factorization again to the matrix.
         CALL PDGETF2( nrows_of_sub_A - J + col_matrix_block,
     $        nrows_matrix_sub_blocks, A, I, J, DESCA, IPIV, IINFO )
*
         IF( INFO.EQ.0 .AND. IINFO.GT.0 )
     $      INFO = IINFO + J - col_matrix_block
*

*        Apply interchanges to columns col_matrix_block:J-col_matrix_block.
*
         CALL PDLASWP( 'Forward', 'Rowwise', J-col_matrix_block, A,
     $        row_matrix_block, col_matrix_block, DESCA, I,
     $        I+nrows_matrix_sub_block-1, IPIV )
*
         IF( J-col_matrix_block+nrows_matrix_sub_blocks + 1.LE.N ) THEN
*
*           Apply interchanges to columns J+nrows_matrix_sub_block:col_matrix_block+N-1.
*
         CALL PDLASWP( 'Forward', 'Rowwise',
     $        ncols_of_sub_A - J - nrows_matrix_sub_block + col_matrix_block,
     $        A, row_matrix_block, J+nrows_matrix_sub_blocks,
     $        DESCA, I, I+nrows_matrix_sub_block-1, IPIV )
*
*           Compute block row of U.
*
         CALL PDTRSM( 'Left', 'Lower', 'No transpose', 'Unit',
     $        nrows_matrix_sub_block,
     $        ncols_of_sub_A - J - nrows_matrix_sub_block + col_matrix_block,
     $        ONE, A, I, J, DESCA, A, I,
     $        J + nrows_matrix_sub_blocks, DESCA )
*
            IF( J-col_matrix_block+nrows_matrix_sub_blocks+1.LE.nrows_of_sub_A ) THEN
*
*              Update trailing submatrix.
*
*     multiplication to update the trailing matrix.
*     
               CALL PDGEMM( 'No transpose', 'No transpose',
     $              nrows_of_sub_A - J - nrows_matrix_sub_blocks + col_matrix_block,
     $              ncols_of_sub_A - J - nrows_matrix_sub_blocks + col_matrix_block,
     $              nrows_matrix_sub_blocks,
     $              -ONE, A, I + nrows_matrix_sub_blocks, J, DESCA,
     $              A, I, J + nrows_matrix_sub_blocks, DESCA, ONE, A,
     $              I + nrows_matrix_sub_blocks, J+nrows_matrix_sub_blocks,
     $              DESCA )
*
            END IF
         END IF
*
   10 CONTINUE ! loop ends here
*
      IF( INFO.EQ.0 )
     $   INFO = MN + 1
      CALL IGAMN2D( ICTXT, 'Rowwise', ' ', 1, 1, INFO, 1, IDUM1, IDUM2,
     $              -1, -1, MYCOL )
      IF( INFO.EQ.MN+1 )
     $   INFO = 0
*
      CALL PTOPSET( 'Broadcast', 'Rowwise', ROWBTOP )
      CALL PTOPSET( 'Broadcast', 'Columnwise', COLBTOP )
      CALL PTOPSET( 'Combine', 'Columnwise', COLCTOP )
*
      RETURN
*
*     End of PDGETRF
*
      END
