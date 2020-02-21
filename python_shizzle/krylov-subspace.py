import numpy as np

A = np.random.randn(50, 50) + 50 * np.eye(50)
v = np.ones((50,1))

for j in np.arange(10):
    print(j)
    w = np.matmul(A, v)
    print(w)
