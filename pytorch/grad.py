import torch

x = torch.randn(2, 3, 1, 1).double().requires_grad_()

y = x ** 2
y.backward()
print(y.grad)
