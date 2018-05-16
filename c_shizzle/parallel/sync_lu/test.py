import numpy as np
import scipy.linalg as la

l =  [[0,1,8,9],
      [4, 5, 12, 13],
      [32, 33, 40, 41],
      [36, 37,  44,  45]]
a = np.array(l)
p, l, u = la.lu(a)

print(p)
print(l)
print(u)

