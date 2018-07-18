import numpy as np
import scipy.linalg as la
import pdb

np.set_printoptions(precision=3, linewidth=250, suppress=True)

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
def idamax(mat, i):
    return np.argmax(np.absolute(mat[i:,i]), axis=0)

"""
swap rows of the panel

panel - numpy array denoting the panel to be swapped
"""
def dswap(mat, new_row, original_row, block, nb):
    t = np.copy(mat[new_row:new_row+1, block:block+nb])
    mat[new_row:new_row+1, block:block+nb] = mat[original_row:original_row+1, block:block+nb]
    mat[original_row:original_row+1, block:block+nb] = t
    return None

"""
Scale the i'th column of the panel w.r.t the pivoting element.
As per the SCALAPACK code, this divides the column below the diagonal
element by the pivot to achieve the scaling and does not make those
elements 0, unlike what is specified in the article in mendeley.
"""
def dscal(mat, pivot, pos):
    mat[pos+1:, pos] = mat[pos+1:, pos]/pivot

"""
Update trailing vertical matrix panel. Multiply the pivot row with pivot column and
subtract the product from the trailing matrix.

mat - full matrix.
block - block iteration
i - iteration within the block
nb - block size
"""
def dger(mat, block, i, nb):
    mat[block+i+1:,block+i+1:block+nb] -= np.matmul(mat[block+i+1:,block+i:block+i+1],
                                                    mat[block+i:block+i+1, block+i+1:block+nb])

"""
panel is a matrix slice that contains all the rows but only the specific columns that
need pivoting. This is done to keep uniformity with the pivot array.
"""
def pivot_panel(panel, ipiv, k1, k2, block, nb, n):
    for i in range(k1,k2):
        x = ipiv[i]
        if x != i:
            t = np.copy(panel[i:i+1])
            panel[i:i+1] = panel[x:x+1]
            panel[x:x+1] = t
"""
Apply row interchanges to the left and right of the panel. Row interchanges
start from row k1 of matrix mat and go on until row k2. The ipiv array is
used for this purpose.
"""
def dlaswp(mat, ipiv, k1, k2, block, nb, n):
    # case 1 : only pivot right panel
    if block == 0:
        pivot_panel(mat[:,nb:n], ipiv, k1, k2, block, nb, n)
        
    # case 2 : only pivot left panel
    if block == n-nb:
        pivot_panel(mat[:,0:n-nb], ipiv, k1, k2, block, nb, n)

    # case 3 : pivot both left and right panel
    if block > 0 and block < n - nb:
        pivot_panel(mat[:, 0:block], ipiv, k1, k2, block, nb, n) # pivot left
        pivot_panel(mat[:, block+nb:n], ipiv, k1, k2, block, nb, n) # pivot right

"""
Update the pivot array. ipiv(i) signifies that the row at index i has been
swapped with that at index ipiv(i).
"""
def update_pivot_array(ipiv, new, original):
    ipiv[original] = new

"""
Multiply the L11 inverse with the U panel.
"""
def dtrsm(mat, block, nb, n):
    L11 = np.tril(mat[block:block+nb,block:block+nb])
    np.fill_diagonal(L11, 1)
    L11_inv = np.linalg.inv(L11)
    mat[block:block+nb,block+nb:n] = np.matmul(L11_inv, mat[block:block+nb,block+nb:n])


def dgemm(mat, block, nb, n):
    L21 = mat[block+nb:n,block:block+nb]
    U12 = mat[block:block+nb,block+nb:n]
    mat[block+nb:n,block+nb:n] -= np.matmul(L21, U12)

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

    nb = 4

    for block in range(0, n, nb):
        # iterations within a block of size nb.
        for i in range(0, nb):
            # dgetf2 part
            # --------------------------------------------------
            # get index of max element in column block+i
            new_row = idamax(mat, block+i) + i + block
            if block+i != new_row:
                # original row is block+i
                dswap(mat, new_row, block+i, block, nb)
                update_pivot_array(ipiv, new_row, block+i)
            
            pivot = mat[block+i,block+i]
            dscal(mat, pivot, block+i)

            """
            Update the rest of the vertical panel.
            """
            dger(mat, block, i, nb)
            print(mat)
            # -------------------------------------------------
        """
        This function updates the panels to the left and right of the current
        vertical panel with the pivoting updates applied in the previous step.
        """
        k1 = block
        k2 = n
        dlaswp(mat, ipiv, k1, k2, block, nb, n)
        """
        This function calculates the U panel in the upper part of the matrix
        my multiplying it with the inverse of the L11 part of the A11 panel
        of the overall matrix.
        """
        dtrsm(mat, block, nb, n)
        """
        Update the rest of the matrix with the updated L and U.
        """
        dgemm(mat, block, nb, n)

    l = np.tril(mat)
    np.fill_diagonal(l, 1)
    u = np.triu(mat)
    
    return np.matmul(l, u)
        
def main():
    a = np.power(np.arange(8*8).reshape(8,8).astype(float), 2)
    #a = np.arange(8*8).reshape(8,8).astype(float)
    print("original matrix : \n")
    print(a)
    
    c  = compute_lu(a, False)
    rl = right_looking_lu(a)
    
    print("python LU")
    print(rl)
    print("custom LU")
    print(c)
    print("2norm subtraction : " + str(np.linalg.norm(c, 2) - np.linalg.norm(rl, 2)))

if __name__ == "__main__":
    main()
