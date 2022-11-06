#open the image IMG_20220825_132650.jpg
from turtle import width
from PIL import Image
import matplotlib.pyplot as plt
import numpy as np
import IntegralImage as ii
from math import *
from turtle import shape
import cv2
import HessianPyramid as hp
import time

from numpy import Inf,shape,size

import cv2 as cv
import matplotlib.pyplot as plt

cap = cv.VideoCapture(0)
cap.set(3, 720)
cap.set(4, 480)

def get_frame():
    _, frame = cap.read()
    #reverse the frame
    frame = cv.flip(frame,1)
    #gray
    frame = cv.cvtColor(frame, cv.COLOR_BGR2GRAY)
    
    
    #resize the image keeping the aspect ratio
    width=240
    height=int(width*len(frame[0])//len(frame))
    frame = cv2.resize(frame,(height,width))
    #convert the image to a list 
    frame=frame
    
    return frame


filename="C:\\Users\\robin\\Desktop\\SEAD\\mySIFT\\v2\\IMG_20200725_162220.jpg"
#filename="D:\documents\github\SEAD\SEAD\mySIFT\\v2\IMG_20200725_162220.jpg"

#img  = Image.open(path)     
# On successful execution of this statement,
# an object of Image type is returned and stored in img variable)


try: 
    img  = Image.open(filename) 
    #convert the image to grayscale
    img = img.convert('L')
    #resize the image by a factor
    originalSize=img.size
    ratioShape=originalSize[0]/originalSize[1]
    factor=0.05
    img = img.resize((int(img.size[0]*factor),int(img.size[1]*factor)))
    img = np.array(img)
    print("Image opened successfully")
except IOError:
    print("Error in opening the image file")
    exit()
    




start_time = time.time()

#img=get_frame()
H=hp.HessianPyramid(img,tau0=9,ratio=1.4,stageShapeAccurate=3)
H.interrestPoint()
H.select_interespoints(0.1,1,0.1)
newSize=720
cv.imshow("img",cv2.resize(img,(round(newSize*ratioShape),newSize),interpolation=cv2.INTER_NEAREST))
"""
plt.figure(1)
for i in range(len(H.pyramid)):
    plt.subplot(3,3,i+1)
    plt.imshow(H.pyramid[i],cmap='gray',vmin=H.minval,vmax=H.maxval)
plt.show() 
"""
originalIMG=img
while True:
    #img=get_frame()
    img=originalIMG.copy()
    H.update(img)
    H.interrestPoint()
    H.select_interespoints(0.5,1,0.2)
    H.draw_interespoints(img)
    dt = time.time() - start_time
    start_time = time.time()
    frameRate = 1/dt
    
    img=cv2.putText(img, str(int(frameRate)), (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2, cv2.LINE_AA)
    newSize=720
    cv.imshow("img",cv2.resize(img,(round(newSize*ratioShape),newSize),interpolation=cv2.INTER_NEAREST))
    """
    plt.figure(1)
    plt.clf()
    for i in range(len(H.pyramid)):
        plt.subplot(3,3,i+1)
        plt.imshow(H.pyramid[i],cmap='gray',vmin=H.minValue,vmax=H.maxValue)
    plt.pause(0.01)
    """
    if cv.waitKey(1) == ord('q'):
        break
    