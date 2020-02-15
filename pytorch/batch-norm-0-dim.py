import torch

bn = torch.nn.BatchNorm1d(5)
x = torch.ones(0,5,9)
bn(x)
