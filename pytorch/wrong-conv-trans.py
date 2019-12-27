import torch

y = torch.ones(0,1,1)
d1 = torch.nn.ConvTranspose1d(1, 1, 3)
d1.weight.data = torch.ones(1,1,3)
d1(y)
