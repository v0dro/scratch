# https://ryankresse.com/batchnorm-dropout-and-eval-in-pytorch/

import torch

bn = torch.nn.BatchNorm2d(3).double()
x = torch.randn(2, 3, 1, 1).double().requires_grad_()
i = 0

# The version numbers of the running_mean and running_var
# tensors become 1 after the bn(x) function call.

# After calling the .clone() method, the version is reset to 0.

# The reason why the gradcheck for f fails is because the running
# buffers get changed in-place in spite of having version = 1, and
# that should not be happening since applying BenchNorm in eval followed
# by train should apply the function only once on the tensor.

# The reason why the gradcheck for g passes is because the version numbers
# are reset to 0 and the train mode does not apply the BN function to the tensor
# (or applies it in a way that it is supposed to when in train mode).

# The buffers are not supposed to change during the eval mode since batchnorm
# uses constant mean and stddev values when the eval mode is set. However in
# function f, they are changing during the eval mode, which should not happen.

# THIS CASE FAILS.

# It happens because the call to bn(x) bumps the version counter to 1 in
# the eval mode, which it is not supposed to do.

# Essentially, if version > 0 then wrong_update_happens_in_train_mode
def failing(x):
  bn.running_mean.data.fill_(0)
  bn.running_var.data.fill_(1)
  bn.eval(); y0 = bn(x)
  print("failing mean: " + str(bn.running_mean._version))
  print("failing var: " + str(bn.running_var._version))
  bn.running_mean = bn.running_mean.clone()
  bn.running_var  = bn.running_var.clone()
  bn.train(); unused = bn(x)  # running buffers are changed in-place here

  print("failing mean: " + str(bn.running_mean._version))
  print("failing var: " + str(bn.running_var._version))
#  print("y0: " + str(unused))
  
  return y0

torch.autograd.gradcheck(failing, (x,))  # passes

# THIS CASE PASSES

# The reason why this case passes is because the clone sets the version counter
# back to zero and therefore the mean does not get wrongly updated after the
# call to bn(x) after bn.train(). Therefore the gradcheck passes.
# def passing(x):
#   bn.running_mean.data.fill_(0)
#   bn.running_var.data.fill_(1)
#   bn.eval(); y0 = bn(x)
  
#   bn.running_mean = bn.running_mean.clone()
#   bn.running_var = bn.running_var.clone()

#   bn.train(); unused = bn(x)

#   print("passing mean: " + str(bn.running_mean))
#   print("passing var: " + str(bn.running_var))
#   print("y0: " + str(y0))

#   return y0

# torch.autograd.gradcheck(failing, (x,))  # fails gradcheck

# COMMENT ON GITHUB

# Here's my understanding of this issue:
# Basically, the `unused = bn(x)` line in the `f` function is modifying
# `bm.running_mean` and `bm.running_var` when it is not actually supposed
# to do so.

# The difference between the state of the `bm` after the `bn.eval(); y0 = bn(x)`
# in `f` and `g` (in `g` after cloning) is that 

# SOLUTION

# 1. Find the place where running_mean and running_var are updated in the forward pass.
