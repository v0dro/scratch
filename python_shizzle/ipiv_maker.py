"""
Python script to create pivot matrix from ipiv array
"""
import numpy as np

def make_pivot_mat(ipiv):
    p = np.eye(len(ipiv), dtype=float)
    
    for i, piv in np.ndenumerate(ipiv):
        if i != piv:
            t = np.copy(p[i,:])
            p[i,:] = p[piv,:]
            p[piv,:] = t

    return p

ipiv = np.array([7,3,7,6,5,5,7,7])
print(make_pivot_mat(ipiv))

