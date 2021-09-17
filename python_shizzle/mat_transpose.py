import numpy as np

m = 30
n = 30

a = np.random.randn(m, n)
b = np.random.randn(m, n)
r = np.zeros((m, m))

for i in range(m):
    for j in range(m):
        for k in range(m):
            r[i, j] += a[i, k] * b[k, j]

assert np.allclose(r, a @ b)

r = np.zeros((m, m))
for i in range(m):
    for j in range(m):
        for k in range(m):
            r[i, j] += a[i, k] * b[j, k]

assert np.allclose(r, a @ b.T)
