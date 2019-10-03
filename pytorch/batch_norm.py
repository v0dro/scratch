import torch

bn = torch.nn.BatchNorm2d(3).double()
x = torch.randn(2, 3, 1, 1).double().requires_grad_()

def f(x):
  bn.running_mean.data.fill_(0)
  bn.running_var.data.fill_(1)
  bn.eval(); y0 = bn(x)
  bn.train(); unused = bn(x)  # running buffers are changed in-place here
#  print(unused)
  return y0

def g(x):
  bn.running_mean.data.fill_(0)
  bn.running_var.data.fill_(1)
  bn.eval(); y0 = bn(x)
  bn.running_mean = bn.running_mean.clone()
  bn.running_var = bn.running_var.clone()
  bn.train(); unused = bn(x)
#  print(unused)
  return y0

torch.autograd.gradcheck(g, (x,))  # passes
print("\n\n>>>> ")
torch.autograd.gradcheck(f, (x,))  # fails gradcheck
