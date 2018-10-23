import numpy as np
import scipy.linalg

np.set_printoptions(linewidth=300, precision=5, suppress=True)

def laplace1d(arr, M, N):
    num = np.zeros([M, N])
    for i in range(M):
        for j in range(N):
            num[i, j] = 1 / (abs(arr[i] - arr[j]) + 1e-3)

    return num
    
m = 16
n = 16
r = np.array([-0.00655826, -0.4015241 ,  1.42340937, -1.44202559, -0.06256317,
              0.72443302, -0.00993673,  0.35438382, -1.78246991,  0.87301446,
              0.09226022,  1.01042724, -0.34648705, -0.09347721,  0.1855797 ,
              0.09158122])
x = np.array([0.9294330700147447 ,0.6273750976863751 ,0.29107094771109343 ,
              0.6129116578392685 ,0.2743782418195698 ,0.030704261786309583,
              0.6439203565088071 ,0.3716898150386927] * 2)
x= x.transpose()
b = np.zeros(n)
print("random values for laplace:")
print(' ,'.join([str(e) for e in r]))

print("random values for RHS:")
print(' ,'.join([str(e) for e in x]))

q = laplace1d(r, m, n)
b += np.matmul(q, x)
p,l,u=scipy.linalg.lu(q)

print("X:")
print(x)
print("Q:")
print(q)
print("B:")
print(b)
print("P:")
print(p)
print("L:")
print(l)
print("U:")
print(u)

