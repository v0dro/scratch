import numpy as np
import scipy.linalg as la

np.set_printoptions(precision=5, linewidth=150)
a = np.arange(8*8).reshape(8,8).transpose()
print(a)
p, l, u = la.lu(a)

print("\n")
print(p)
print(l)
print(u)

