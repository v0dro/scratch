import torch
import torchvision.datasets as datasets
import torchvision
import numpy as np

mnist_trainset = datasets.MNIST(root='./data', train=True, download=True,
                                transform=torchvision.transforms.ToTensor())
trainloader = torch.utils.data.DataLoader(mnist_trainset, batch_size=64,
                                          shuffle=True)
print(trainloader)

dataiter = iter(trainloader)
images, labels = next(dataiter)
print(images.shape)
print(labels.shape)
