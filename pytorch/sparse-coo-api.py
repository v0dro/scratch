import torch
import random

size = (100, 100)
dims = list(range(100))
choices_x = random.sample(dims, 10)
choices_y = random.sample(dims, 10)

indices = torch.tensor([choices_x, choices_y])
nnz = 10
values = torch.randn(10)
x1 = torch.sparse_coo_tensor(indices, values, size)

print("x1:")
print(x1)

indices = torch.tensor([choices_x, choices_x])
values = torch.randn(10)
x2 = torch.sparse_coo_tensor(indices, values, size)
print("x2:")
print(x2)

y = x1 + x2
print(y)
