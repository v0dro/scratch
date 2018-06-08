import numpy as np
import scipy.linalg as la
import pdb

np.set_printoptions(precision=1, linewidth=150)

def compute_lu(a, show=False):
    p, l, u = la.lu(a)
    computed = np.matmul(l, u)
    
    if show:
        print(p)
        print(l)
        print(u)
        print("\n")
        print(computed)

    return computed

def pivot_matrix(ipiv):
    pass
"""
Abs max element in the ith column.
"""
def idamax(panel, i, c):
    print(np.absolute(panel[:,i]))
    return np.argmax(np.absolute(panel[:,i]), axis=0)

"""
swap rows of the panel

panel - numpy array denoting the panel to be swapped
"""
def dswap(panel, new_row, original_row):
    t = np.copy(panel[new_row])
    panel[new_row] = panel[original_row]
    panel[original_row] = t
    return None

"""
Scale the i'th column of the panel w.r.t the pivoting element.
"""
def dscal(col, pivot):
    col[1:] = col[1:]/pivot

"""
Update trailing matrix.

mat - full matrix
"""
def dger(mat, i):
    mat[i+1:,i+1:] -= np.matmul(mat[i+1:, i], mat[i,i+1:].transpose())
    
"""
Compute LU decomposition of square matrix using right looking LU decomposition
technique. Operates on blocks.

Performs a partial pivoting. The pivot element is first swapped into the diagonal
row. After this, we perform a subtraction of an element in the row k with an
element in col k, and then divide by the pivot element.
"""
def right_looking_lu(mat):
    m = mat.shape[0]
    n = mat.shape[1]
    ipiv = np.arange(m)

    nb = 2

    # advance columns in blocks of nb
    for c in range(0, n, nb):
        # get a vertical panel of the matrix
        mxnb_panel = mat[c:m,c:nb+c]
        for i in range(0, nb):
            # dgetf2 part
            # --------------------------------------------------
            # find abs max element of the ith column
            new_row = idamax(mxnb_panel, i, c+i)
            dswap(mxnb_panel, new_row, i) # swap pivot row

            # column of size (c+i)*nb
            column = mxnb_panel[i:,i]
            pivot = column[0]
            dscal(column, pivot) # scale the ith column of the matrix
            
            dger(mat, c+i) # update trailing matrix
            # -------------------------------------------------
            
        
        
def main():
    a = np.arange(8*8).reshape(8,8).astype(float)
    print("original matrix : \n")
    print(a)
    print("\n")

    c  = compute_lu(a)
    rl = right_looking_lu(a)

    print("2norm subtraction : " + str(np.linalg.norm(c, 2) - np.linalg.norm(rl, 2)))

if __name__ == "__main__":
    main()
