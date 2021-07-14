import numpy as np
block = 1024
rank = 16

np.set_printoptions(linewidth=1000)

av = np.arange(0, block * rank).reshape(rank, block)
bu = np.arange(0, block * rank).reshape(block, rank)

print(av[:8, :])

print(av[:8, :] @ bu[:, :8])
print(av[8:, :] @ bu[:, 8:])
