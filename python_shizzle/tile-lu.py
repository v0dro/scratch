import numpy as np
from scipy.linalg import lu, solve_triangular, lu_factor

np.set_printoptions(precision=2, linewidth=250, suppress=True)

m = 15
n = 15
mb = 5
nb = 5
mt = int(m / mb)
nt = int(n / nb)

matrix = np.random.randn(m, n)
matrix_copy = np.copy(matrix)


for k in range(nt):    
    print("GETRF(A(",k,",",k,"), L(", k, "," , k, "), U(", k, ",", k, "))")
    for j in range(k+1, nt):
        print("  GESSM(A(",k , "," , j, ")," , "L(", k, "," , k, "), U(", k, ",", j, "))")
    for i in range(k+1, nt):
        print("  TSTRF(U(",k,",",k,"), A(", i, ",", k,"))")
        for j in range(k+1, nt):
            print("    SSSSM(U(",k,",",j,"), A(", i, ",", j, "), L(", i, ",", k, "))")

def A(irow, icol):
    if irow >= mt:
        raise(RuntimeError("irow cannot be ", irow))

    if icol >= nt:
        raise(RuntimeError("icol cannot be ", icol))
    
    rstart = irow * mb
    cstart = icol * nb
    
    return matrix[rstart:rstart+mb, cstart:cstart+nb]

def getrf(A_kk):
    P_kk, L_kk, U_kk = lu(A_kk, overwrite_a=True)
    return P_kk, L_kk, U_kk

def tstrf(U_kk, A_ik):
    stacked = np.concatenate((U_kk, A_ik), axis=0)
    P_ik, L_ik, U_kk = lu(stacked)

    return P_ik, L_ik, U_kk
    
def gessm(L_kk, P_kk, A_kj):
    pivoted = np.matmul(P_kk, A_kj)
    for i in range(A_kj.shape[0]):
        for j in range(A_kj.shape[1]):
            A_kj[i,j] = pivoted[i,j]

    U_kj = solve_triangular(L_kk, A_kj, overwrite_b=True)

    return U_kj

def ssssm(L_ik, P_ik, U_kj, A_ij):
    stacked  = np.concatenate((U_kj, A_ij), axis=0)
    pivoted = solve_triangular(L_ik, np.matmul(P_ik, stacked))
    
    return pivoted

print(matrix)

for k in range(nt):
    P_kk, L_kk, U_kk = getrf(A(k, k))
    for j in range(k+1, nt):
        U_kj = gessm(L_kk, P_kk, A(k,j))
    for i in range(k+1, nt):
        P_ik, L_ik, U_kk = tstrf(U_kk, A(i,k))
        for j in range(k+1, nt):
            pass
            # stack_kj = ssssm(L_ik, P_ik, U_kj, A(i,j))

print(matrix)

print("matrix copy:")
lu_factor(matrix_copy, overwrite_a=True)
# print("LU --->")
# print(lu)
print("matrix copy --->")
print(matrix_copy)


# GETRF(A( 0 , 0 ), L( 0 , 0 ), U( 0 , 0 ))
#   GESSM(A( 0 , 1 ), L( 0 , 0 ), U( 0 , 1 ))
#   GESSM(A( 0 , 2 ), L( 0 , 0 ), U( 0 , 2 ))
#   TSTRF(U( 0 , 0 ), A( 1 , 0 ))
#     SSSSM(U( 0 , 1 ), A( 1 , 1 ), L( 1 , 0 ))
#     SSSSM(U( 0 , 2 ), A( 1 , 2 ), L( 1 , 0 ))
#   TSTRF(U( 0 , 0 ), A( 2 , 0 ))
#     SSSSM(U( 0 , 1 ), A( 2 , 1 ), L( 2 , 0 ))
#     SSSSM(U( 0 , 2 ), A( 2 , 2 ), L( 2 , 0 ))
# GETRF(A( 1 , 1 ), L( 1 , 1 ), U( 1 , 1 ))
#   GESSM(A( 1 , 2 ), L( 1 , 1 ), U( 1 , 2 ))
#   TSTRF(U( 1 , 1 ), A( 2 , 1 ))
#     SSSSM(U( 1 , 2 ), A( 2 , 2 ), L( 2 , 1 ))
# GETRF(A( 2 , 2 ), L( 2 , 2 ), U( 2 , 2 ))
