import os
import sys
import tempfile
import torch
import torch.distributed as dist
import torch.nn as nn
import torch.optim as optim
import torch.multiprocessing as mp
from torch.nn.parallel import DistributedDataParallel as DDP

def setup(rank, world_size):
    os.environ['MASTER_ADDR'] = 'localhost'
    os.environ['MASTER_PORT'] = '12355'

    # initialize the process group
    dist.init_process_group("gloo", rank=rank, world_size=world_size)

def cleanup():
    dist.destroy_process_group()

m = nn.SyncBatchNorm(100)
ranks = list(range(8))
r1, r2 = ranks[:4], ranks[4:]
process_groups = [torch.distributed.new_group(pids) for pids in [r1, r2]]
process_group = process_groups[0 if dist.get_rank() <= 3 else 1]
m = nn.BatchNorm3d(100, affine=False, process_group=process_group)
input = torch.randn(20, 100, 35, 45, 10)
output = m(input)
