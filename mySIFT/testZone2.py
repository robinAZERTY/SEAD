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
    return frame

# Use the above statement within try block, as it can 
# raise an IOError if file cannot be found, 
# or image cannot be opened.

#rognage de l'image au centre pour avoir une image carrée au dimension de la plus grande 
#dimension de l'image
"""
width, height = img.size
if width>height:
    img=img.crop((width//2-height//2,0,width//2+height//2,height))
else:
    img=img.crop((0,height//2-width//2,width,height//2+width//2))

print("Image size: ", np.shape(img))
"""


#resize the image keeping the aspect ratio
width=480
height=int(width*img.size[1]//img.size[0])
img = img.resize((width, height))
#convert the image to a list 
img=np.array(img)


print("New Image size: ", shape(img))
print("Image type: ", type(img))

H=hp.HessianPyramid(img)
#H.interrestPoint(2,0.8)
H.interrestPoint(1.5,space_stability_threshold=2)

print("Number of interrestPoint: ",len(H.interrestPoint))
print("complexity: ",H.complexity)

#draw the interrest point in the image (rectangle with blob size)
for ip in H.interrestPoint:
    point,info=ip
    x=int(point[0])
    y=int(point[1])
    size=int(point[2])
    a=2*size/(1+info[4][0]/info[5][0])
    b=2*size-a
    theta=info[0][0]
    print("x: ",x," y: ",y," size: ",size, " theta: ",theta," a: ",a," b: ",b)
    #cv2.rectangle(img,(x-size//2,y-size//2),(x+size//2,y+size//2),(255,255,255),1)
    #draw a ellipse
    cv2.ellipse(img, (int(x),int(y)),(int(a),int(b)), theta/(np.pi)*180, 0, 360, 0, 1)

plt.figure(1)
plt.imshow(img,cmap='gray')
plt.show()
plt.figure(2)
i=0
while True :
    plt.clf()
    plt.imshow(H.scaleSpace[i],cmap='gray',vmin=0,vmax=H.maxe)
    plt.title("blobSize "+str(H.blobSize[i]))
    plt.colorbar()
    #waite for + or - key
    key = plt.ginput(1, timeout=-1)
    if key[0][0]<H.II.width//2 and i>0:
        i-=1
    elif key[0][0]>H.II.width//2 and i<len(H.scaleSpace)-1:
        i+=1

"""
while True:
    img=get_frame()
    #img=cv2.resize(img,(300,200))
    
    H=hp.HessianPyramid(img)
    H.interrestPoint(1.5)
    
    for ip in H.interrestPoint:
        point,info=ip
        print("point: ",point)
        try:
            x=point[0][0]
            y=point[1][0]
            theta=info[0][0]
            aa=info[4][0]
            bb=info[5][0]
            size=point[2][0]
        except:
            x=point[0]
            y=point[1]
            theta=info[0]
            aa=info[4]
            nb=info[5]
            size=point[2]
            
        x=int(x)
        y=int(y)
        size=int(size)
        a=2*size/(1+aa/bb)
        b=2*size-a
        cv2.ellipse(img, (int(x),int(y)),(int(a),int(b)), theta/(np.pi)*180, 0, 360, 0, 1)
    
    #show img
    cv.imshow("img",img)
    
    if cv.waitKey(1) == ord('q'):
        break
"""