import imageTest
import LaplacianPyramid as lp
import matplotlib.pyplot as plt
import numpy as np

img=imageTest.imageTest((400,550))
img.randomBlob(10,50,40)

LP = lp.LaplacianPyramid(img.table)

LP.buid()
LP.show()
print("len(LP.maxima_candidates)=",len(LP.maxima_candidates))
print("len(LP.minima_candidates)=",len(LP.minima_candidates))

plt.figure()
plt.subplot(1,2,1)
plt.imshow(img.table,cmap='gray')
plt.title("original image")
plt.subplot(1,2,2)
plt.imshow(LP.II.table,cmap='gray')
plt.title("Integral image")
plt.show()