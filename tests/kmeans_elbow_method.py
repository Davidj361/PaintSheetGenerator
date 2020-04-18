import numpy as np
from sklearn.cluster import KMeans
from sklearn.metrics import mean_squared_error
import matplotlib.pyplot as plt
import cv2 as cv

img = cv.imread('tree.jpg')
img_x = np.reshape(img,(img.shape[0]*img.shape[1],3))

mses = []
k_values = np.arange(2,11)
for k in k_values:
    kmeans = KMeans(k).fit(img_x)
    kmeans_y = []
    for label in kmeans.labels_:
        kmeans_y.append(kmeans.cluster_centers_[label])
    mses.append(mean_squared_error(img_x, kmeans_y))

cv.imshow('Input', img)
plt.plot(k_values,mses, 'bo-')
plt.show()
