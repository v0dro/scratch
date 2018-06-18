import numpy as np
import scipy.linalg as la
import pdb

np.set_printoptions(precision=3, linewidth=150, suppress=True)

"""
Accept pivot array ipiv and create a pivot matrix.
"""
def create_pivot_matrix(ipiv):
    s = ipiv.shape[0]
    p = np.zeros([s, s])
    for index, x in np.ndenumerate(ipiv):
        p[x, index] = 1

    return p
        
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
    return np.argmax(np.absolute(panel[i:,i]), axis=0)

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
def dger(mat, c,i,nb):
    # print(str(mat[c+i+1:,c+i+1:c+i+nb].shape))
    # print(str(mat[c+i+1:, c+i]))
    # print(str(mat[c+i,c+i+1:c+i+nb]))
    # print(str(mat[c+i+1:, c+i] * mat[c+i,c+i+1:c+i+nb]))
    mat[c+i+1:,c+i+1:c+i+nb] -= np.matmul(mat[c+i+1:, c+i:c+i+1], mat[c+i:c+i+1,c+i+1:c+i+nb])

def pivot_panel(panel, ipiv, k1, k2, c, nb, n):
    for index, x in np.ndenumerate(ipiv):
        i = index[0] 
        if x != i + k1: # because index will start from 0
            t = np.copy(panel[i])
            panel[i] = panel[x]
            panel[x] = t    
"""
Apply row interchanges to the left and right of the panel. Row interchanges
start from row k1 of matrix mat and go on until row k2. The ipiv array is
used for this purpose.
"""
def dlaswp(mat, ipiv, k1, k2, c, nb, n):
    # case 1 : only pivot right panel
    if c == 0:
        pivot_panel(mat[:,nb:n], ipiv, k1, k2, c, nb, n)
        
    # case 2 : only pivot left panel
    if c == n-nb:
        pivot_panel(mat[:,0:n-nb], ipiv, k1, k2, c, nb, n)

    # case 3 : pivot both left and right panel
    if c > 0 and c < n - nb:
        pivot_panel(mat[:, 0:c], ipiv, k1, k2, c, nb, n) # pivot left
        pivot_panel(mat[:, c+nb:n], ipiv, k1, k2, c, nb, n) # pivot right

"""
Update the pivot array. ipiv(i) signifies that the row at index i has been
swapped with that at index ipiv(i).
"""
def update_pivot_array(ipiv, new, original):
    ipiv[original] = new

"""
Multiply the L11 inverse with the U panel.
"""
def dtrsm(mat, c, nb, n):
    L11 = np.tril(mat[c:c+nb,c:c+nb])
    np.fill_diagonal(L11, 1)
    L11_inv = np.linalg.inv(L11)
    mat[c:c+nb,c+nb:n] = np.matmul(L11_inv, mat[c:c+nb,c+nb:n])


def dgemm(mat, c, nb, n):
    L21 = mat[c+nb:n,c:c+nb]
    U12 = mat[c:c+nb,c+nb:n]
    mat[c+nb:n,c+nb:n] -= np.matmul(L21, U12)

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
            print("new row " + str(new_row))
            if c+i+new_row != c+i:
                # original row is first or second of this panel
                dswap(mxnb_panel, new_row, i)
                update_pivot_array(ipiv, c+i+new_row, c+i)
            
            column = mxnb_panel[i:,i]
            pivot = column[0]
            dscal(column, pivot)

            """
            Update the rest of the vertical panel.
            """
            dger(mat, c, i, nb)
            print("interm matrix...")
            print(mat)
            # -------------------------------------------------
        print("C = ", c+i+1)
        print(ipiv + 1)
        print(mat)
        """
        This function updates the panels to the left and right of the current
        vertical panel with the pivoting updates applied in the previous step.
        """
        k1 = c
        k2 = n
        dlaswp(mat, ipiv, k1, k2, c, nb, n)
        """
        This function calculates the U panel in the upper part of the matrix
        my multiplying it with the inverse of the L11 part of the A11 panel
        of the overall matrix.
        """
        dtrsm(mat, c, nb, n)
        """
        Update the rest of the matrix with the updated L and U.
        """
        dgemm(mat, c, nb, n)

    l = np.tril(mat)
    np.fill_diagonal(l, 1)
    u = np.triu(mat)

    print(l)
    print(u)
    print("ipiv : ")
    print(ipiv+1)
    
    return np.matmul(l, u)
        
def main():
    a = np.arange(8*8).reshape(8,8).astype(float)
    print("original matrix : \n")
    print(a)
    print("\n")

    c  = compute_lu(a, True)
    rl = right_looking_lu(a)

    print(rl)
    print("2norm subtraction : " + str(np.linalg.norm(c, 2) - np.linalg.norm(rl, 2)))

if __name__ == "__main__":
    main()
