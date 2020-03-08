from torch import nn
import torch.nn.functional as F

class Network(nn.Module):
    def __init__(self):
        super().__init__()
        
        self.hidden1 = nn.Linear(786, 128)
        self.hidden2 = nn.Linear(128,64)
        self.output = nn.Linear(64,10)

    def forward(self, x):
        self.relu1 = F.relu(self.hidden1(x))
        self.relu2 = F.relu(self.hidden2(x))
        self.probabilities = F.softmax(self.output(x), dim=1)

        return self.probabilities

    
