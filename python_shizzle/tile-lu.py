import numpy as np

m = 30
n = 30
mb = 10
nb = 10
mt = int(m / mb)
nt = int(n / nb)

for k in range(nt):
    print("GETRF(A(",k,",",k,"), L(", k, "," , k, "), U(", k, ",", k, "))")
    for j in range(k+1, nt):
        print("  GESSM(A(",k , "," , j, ")," , "L(", k, "," , k, "), U(", k, ",", j, "))")
    for i in range(k+1, nt):
        print("  TSTRF(U(",k,",",k,"), A(", i, ",", k,"))")
        for j in range(k+1, nt):
            print("    SSSSM(U(",k,",",j,"), A(", i, ",", j, "), L(", i, ",", k, "))")


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
