import torch
import numpy as np

input = torch.tensor(np.random.rand(2,2))
input2 = torch.tensor(np.random.rand(2,0))

torch.orgqr(input, input2)
