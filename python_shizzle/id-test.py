import numpy as np
from numpy.linalg import matrix_rank

N = 100
mat = np.zeros((N, N), dtype=float)
vec = np.sort(np.random.randn(N))

for i in range(100):
    for j in range(N):
        mat[i, j] = 1 / float(vec[i] - vec[j] + 1000)

print(mat)
print(matrix_rank(mat))
