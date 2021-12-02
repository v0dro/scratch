import numpy as np

m = np.random.randn(100, 100)
u, s, vt = np.linalg.svd(m)
