Program getrf_test

!*****************************************************************************80
!
!! DGETRF_TEST tests DGETRF.
!
!  Discussion:
!
!    The problem is just an enlarged version of the
!    problem for n = 5, which is:
!
!    Matrix A is ( N -1 -1 -1 -1)    right hand side b is  (1)
!                (-1  N -1 -1 -1)                          (1)
!                (-1 -1  N -1 -1)                          (1)
!                (-1 -1 -1  N -1)                          (1)
!                (-1 -1 -1 -1  N)                          (1)
!
!    Solution is   (1)
!                  (1)
!                  (1)
!                  (1)
!                  (1)
!
!    For this problem, no pivoting is required.
!
!  Licensing:
!
!    This code is distributed under the GNU LGPL license.
!
!  Modified:
!
!    12 September 2006
!
!  Author:
!
!    John Burkardt
!
  implicit none

  integer ( kind = 4 ), parameter :: n = 8
  integer ( kind = 4 ), parameter :: lda = n
  integer I_FOR, J_FOR

  real ( kind = 8 ) a(lda,n)
  real ( kind = 8 ) b(n)
  integer ( kind = 4 ) i
  integer ( kind = 4 ) info
  integer ( kind = 4 ) ipiv(n)
  integer ( kind = 4 ) j

  write ( *, '(a)' ) ' '
  write ( *, '(a)' ) 'DGETRF_TEST'
  write ( *, '(a)' ) '  DGETRF factors a general matrix;'
  write ( *, '(a)' ) '  DGETRS solves a linear system;'
  write ( *, '(a)' ) '  For a double precision real matrix (D)'
  write ( *, '(a)' ) '  in general storage mode (GE):'
!
!  Assign values to matrix A and right hand side b.
!
  do i = 1, n
    do j = 1, n
      if ( i == j ) then
        a(i,j) = dble ( n )
      else
        a(i,j) = -1.0D+00
      end if
    end do
 end do

 a(1,1) = 0
 a(2,1) = 8
 a(3,1) = 16
 a(4,1) = 24
 a(5,1) = 32
 a(6,1) = 40
 a(7,1) = 48
 a(8,1) = 56
 
 a(1,2) = 1
 a(2,2) = 9
 a(3,2) = 17
 a(4,2) = 25
 a(5,2) = 33
 a(6,2) = 41
 a(7,2) = 49
 a(8,2) = 57
 
 a(1,3) = 2
 a(2,3) = 10
 a(3,3) = 18
 a(4,3) = 26
 a(5,3) = 34
 a(6,3) = 42
 a(7,3) = 50
 a(8,3) = 58

 a(1,4) = 3
 a(2,4) = 11
 a(3,4) = 19
 a(4,4) = 27
 a(5,4) = 35
 a(6,4) = 43
 a(7,4) = 51
 a(8,4) = 59
 
 a(1,5) = 4
 a(2,5) = 12
 a(3,5) = 20
 a(4,5) = 28
 a(5,5) = 36
 a(6,5) = 44
 a(7,5) = 52
 a(8,5) = 60
 
 a(1,6) = 5
 a(2,6) = 13
 a(3,6) = 21
 a(4,6) = 29
 a(5,6) = 37
 a(6,6) = 45
 a(7,6) = 53
 a(8,6) = 61
 
 a(1,7) = 6
 a(2,7) = 14
 a(3,7) = 22
 a(4,7) = 30
 a(5,7) = 38
 a(6,7) = 46
 a(7,7) = 54
 a(8,7) = 62
 
 a(1,8) = 7
 a(2,8) = 15
 a(3,8) = 23
 a(4,8) = 31
 a(5,8) = 39
 a(6,8) = 47
 a(7,8) = 55
 a(8,8) = 63
 
do I_FOR=1, 8
   write (*,"(100G15.5)") (a(I_FOR,J_FOR), J_FOR=1,8)
enddo
print*,"\n"

 b(1:n) = 1.0D+00
!
!  Factor the matrix.
!
 call dgetrf ( n, n, a, lda, ipiv, info )


do I_FOR=1, 8
   write (*,"(100G15.5)") (a(I_FOR,J_FOR), J_FOR=1,8)
enddo

 if ( info /= 0 ) then
    write ( *, '(a)' ) ' '
    write ( *, '(a,i8)' ) '  Matrix is singular, INFO = ', info
    return
 end if
!
!  Solve the linear system.
!
 call dgetrs ( 'n', n, 1, a, lda, ipiv, b, n, info )

 if ( info /= 0 ) then
    write ( *, '(a)' ) ' '
    write ( *, '(a,i8)' ) '  Solution procedure failed, INFO = ', info
    return
 end if

 ! call r8vec_print_some ( n, b, 1, 5, '  Partial solution (all should be 1)' )
End Program getrf_test
