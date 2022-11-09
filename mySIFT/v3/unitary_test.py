import imageTest
import cv2
import numpy as np
import matplotlib.pyplot as plt
import integralImage as ii
import LaplacianPyramid as lp


img=imageTest.imageTest((200,300))
"""
img.randomBlob(3,30,10)
iiimg=ii.IntegralImage(img.table)

plt.figure(1)
plt.subplot(1,2,1)
plt.imshow(img.table)
plt.subplot(1,2,2)
plt.imshow(iiimg.table)
plt.show()
"""
LP = lp.LaplacianPyramid(img.table)
