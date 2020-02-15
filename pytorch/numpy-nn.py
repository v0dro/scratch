import numpy as np

weights = np.array([[1],[2],[3],[4]])
features1 = np.array([1, 2, 3, 4])
features2 = np.array([5,6,7,8])
bias = 8

h1 = np.matmul(features1, weights) + bias
h2 = np.matmul(features2, weights) + bias

weights_hidden = np.array([[10], [20]])
hidden_features = np.array([h1[0], h2[0]])
output_bias = 10

output = np.matmul(hidden_features, weights_hidden) + output_bias

print(hidden_features) 
print(output)
