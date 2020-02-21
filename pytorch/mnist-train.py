import torch
import torchvision.datasets as datasets
import torchvision
import numpy as np

def activation(x):
    return 1 / (1 + torch.exp(-x))

def softmax(out):
    return torch.exp(out) / torch.sum(torch.exp(out), dim=1).view(-1,1)

mnist_trainset = datasets.MNIST(root='./data', train=True, download=True,
                                transform=torchvision.transforms.ToTensor())
trainloader = torch.utils.data.DataLoader(mnist_trainset, batch_size=64,
                                          shuffle=True)

dataiter = iter(trainloader)
images, labels = next(dataiter)

flat_images = images.flatten(start_dim=1)

w1 = torch.randn(784, 256)
b1 = torch.randn(256)

w2 = torch.randn(256, 10)
b2 = torch.randn(10)

h1 = torch.mm(flat_images, w1) + b1
output = softmax(activation(torch.mm(h1, w2) + b2))
print(output.sum(dim=1))
