"""
Simple test script for verifying matmul involving blockwise distribution
of numbers rather than row or col wise
"""

import numpy as np

temp_row = np.array([
    1,2,
    3,4,
    5,6,
    7,8
])

temp_col = np.array([
    1,2,3,4,
    5,6,7,8
])

# nb = 4
# num_procs = 2
# dim = nb / num_procs

dim = 2
mp = 2

for i in range(0,dim):
    for proc in range(0, dim*dim*mp, dim*dim):
        index = i*dim + proc

        for j in range(index, index+dim):
            print("j: ", j, "indx: ", index)
                
