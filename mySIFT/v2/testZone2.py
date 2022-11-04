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

from numpy import Inf,shape,size


#filename="D:\\documents\\github\\SEAD\\SEAD\\mySIFT\\IMG_20220824_204247.jpg"
filename="D:\documents\github\SEAD\SEAD\mySIFT\IMG_20200725_162220.jpg"

#img  = Image.open(path)     
# On successful execution of this statement,
# an object of Image type is returned and stored in img variable)


try: 
    img  = Image.open(filename) 
    #convert the image to grayscale
    img = img.convert('L')
    print("Image opened successfully")
except IOError:
    print("Error in opening the image file")
    exit()
    

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
    width=200
    height=int(width*len(frame[0])//len(frame))
    frame = cv2.resize(frame,(height,width))
    #convert the image to a list 
    frame=frame
    
    return frame

import time

start_time = time.time()

img=get_frame()
H=hp.HessianPyramid(img,9,1.4)

while True:
    img=get_frame()

    
    
    H.update(img)
    H.interrestPoint(1.5)
    dt = time.time() - start_time
    start_time = time.time()
    frameRate = 1/dt
    
    img=cv2.putText(img, str(int(frameRate)), (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2, cv2.LINE_AA)
    cv.imshow("img",cv2.resize(img,(720,480),interpolation=cv2.INTER_AREA))
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
