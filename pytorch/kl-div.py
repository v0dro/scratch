import torch
import torch.nn.functional as F
torch.set_printoptions(threshold=5000, profile="full",precision=20)


a = torch.tensor([[1.0,2,3], [5.0, 5.0, 5.0]])
b = torch.tensor([[1.0,2,3], [5.0, 5.0, 5.0]])

print(F.kl_div(F.log_softmax(a, 1), F.softmax(b, 1), reduction="none"))
print("----")
print(F.softmax(b, dim=1) * (F.log_softmax(b, dim=1) - F.log_softmax(a, dim=1)))
