import numpy as np
from sklearn.svm import SVC

X = np.array([[30,2,9], [21,1,1], [26,1,2], [28,0,10], [40,1,14], [35,2,10], 
  [27,0,6], [32,2,9], [45,0,17],[3,2,7]])
y = np.array([0,1,2,1,0,2,2,1,0,2])
clf = SVC()
clf.fit(X, y)

clf.predict([30,8,1])

# OUTPUT
# array([2])
        