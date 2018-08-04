"""
Python implementation of a basic trsm solver for lower triangular matrices.
"""
import numpy as np

def trsm(n, b):
    for col_num in range(0, b.shape[1]):
        b_col = b[:,col_num]
        for i in range(0, len(b_col)):
            for j in range(0, i):
                b_col[i] = b_col[i] - b_col[j]*n[i,j]
            b_col[i] = b_col[i]/n[i,i]

    return b

def main():
    n = np.array([
        15,0,0,0,0,0,
        2,14,0,0,0,0,
        2,3,12,0,0,0,
        2,3,4,164,0,0,
        2,3,4,5,165,0,
        2,3,4,5,6,111
    ]).reshape(6,6)
    b = np.array([
        1.0,2,3,4,5,6,
        1,2,3,4,5,6
    ]).reshape(2,6).transpose()
    x = np.matmul(np.linalg.inv(n), b)
    
    x1 = trsm(n,b)

    print("x: ", x)
    print("x1: ", x1)
    
    
if __name__ == "__main__":
    main()
