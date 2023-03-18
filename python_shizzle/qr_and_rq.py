import numpy as np
import scipy.linalg
import sys

np.set_printoptions(threshold=sys.maxsize)

n = 50
rank = 10

a = np.random.randn(n, n)
u, s, vt = np.linalg.svd(a)

u = u[:, :rank]
s = s[:rank]
vt = vt[:rank, :]
a = u @ np.diag(s) @ vt

q, r, ipiv = scipy.linalg.qr(a, pivoting=True)
_r, _q= scipy.linalg.qr(r)

print(np.linalg.norm(q @ _r @ _q - a[:, ipiv]))
