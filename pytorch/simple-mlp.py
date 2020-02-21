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

flat_images = images.flatten(start_dim=1)

w1 = torch.randn(784, 1)
w2 = torch.randn(256, 1)
w3 = torch.randn(10, 1)

flat_images = images.flatten(start_dim=1)

hidden_input = flat_images.mm(w1)
feature_vector = hidden_input.sum().repeat(256).reshape(1,256)
feature_vector.mm(w2)
hidden_output = torch.nn.functional.sigmoid(feature_vector.mm(w2))

final_input = hidden_output.sum().repeat(10).reshape(1, 10)
output = final_output.mm(w3)

print(output.shape)
print(hidden_input.shape)
print(hidden_output.shape)
    
print(h64.shape)

