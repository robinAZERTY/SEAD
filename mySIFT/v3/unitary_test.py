import imageTest
from detector import detector
import matplotlib.pyplot as plt
import numpy as np
import cv2

#img=imageTest.imageTest((400,550))
#img.randomBlob(10,50,40)

#open C:\Users\robin\Desktop\SEAD\mySIFT\v3\IMG_20200725_162220.jpg
img=cv2.imread('C:\\Users\\robin\\Desktop\\SEAD\\mySIFT\\v3\\IMG_20200725_162220.jpg')
img=cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
img=cv2.resize(img,(img.shape[1]//10,img.shape[0]//10))

my_d=detector(img)
my_d.detect(0.25)
print(len(my_d.keypoints))
draw=my_d.draw()
my_d.LP.show()
plt.figure()
plt.imshow(draw,cmap='gray')
plt.show()


"""LP = lp.LaplacianPyramid(img)

LP.buid()
LP.show()
print("len(LP.maxima_candidates)=",len(LP.maxima_candidates))
print("len(LP.minima_candidates)=",len(LP.minima_candidates))
print("LP.dynamic=",LP.dynamic)

plt.figure()
plt.subplot(1,2,1)
plt.imshow(img,cmap='gray')
plt.title("original image")
plt.subplot(1,2,2)
plt.imshow(LP.II.table,cmap='gray')
plt.title("Integral image")
plt.show()"""