import numpy as np

a = np.random.randn(50, 50) + 50 * np.eye(50)
b = np.ones((50,1))
k = b

for m in range(10):
    print(m)
