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
def idamax(panel, i):
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
As per the SCALAPACK code, this divides the column below the diagonal
element by the pivot to achieve the scaling and does not make those
elements 0, unlike what is specified in the article in mendeley.
"""
def dscal(col, pivot):
    col[1:] = col[1:]/pivot

"""
Update trailing matrix. Multiply the pivot row with pivot column and
subtract the product from the trailing matrix.

mat - full matrix.
i - Indices to update.
"""
def dger(mat, i):
    mat[i+1:,i+1:] -= np.matmul(mat[i+1:, i], mat[i,i+1:].transpose())

"""
Apply row interchanges to the left and right of the panel. Row interchanges
start from row k1 of matrix mat and go on until row k2. The ipiv array is
used for this purpose.
"""
def dlaswp(mat, ipiv, k1, k2, c, nb):
    if k1 == 0: # 
        
    else:
            
    # right panel. Always swap.

"""
Update the pivot array. ipiv(i) signifies that the row at index i has been
swapped with that at index ipiv(i).
"""
def update_pivot_array(ipiv, original, new):
    ipiv[original] = new

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

    for c in range(0, n, nb):
        mxnb_panel = mat[c:m,c:nb+c]
        """
        Think of it this way - you're sending the panel into the below loop
        and all the work being done inside the below loop is strictly in the
        panel and nowhere else (apart from the grid update). So all the indices
        should be wrt this panel only.
        """
        for i in range(0, nb):
            # dgetf2 part
            # --------------------------------------------------
            new_row = idamax(mxnb_panel, i)
            dswap(mxnb_panel, new_row, i)
            update_pivot_array(ipiv, c+i, new_row)
            
            column = mxnb_panel[i:,i]
            pivot = column[0]
            dscal(column, pivot)
            
            dger(mat, c+i)
            # -------------------------------------------------
        """
        This function updates the panels to the left and right of the current
        vertical panel with the pivoting updates applied in the previous step.
        """
        k1 =
        dlaswp(mat, ipiv, k1, k2, c, nb)

            
        
        
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
