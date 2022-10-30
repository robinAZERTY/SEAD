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

from numpy import Inf,shape

#filename = "C:\\Users\\robin\\Desktop\\SEAD\\mySIFT\\IMG_20220825_132650.jpg"
#filename="C:\\Users\\robin\\Desktop\\SEAD\\mySIFT\\IMG_20220824_204150.jpg"
filename="C:\\Users\\robin\\Desktop\\SEAD\\mySIFT\\IMG_20220824_204247.jpg"
#filename="C:\\Users\\robin\\Desktop\\SEAD\\mySIFT\\IMG_20220822_212022.jpg"
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
H.interrestPoint(2,0.8)

print("Number of interrestPoint: ",len(H.interrestPoint))

#draw the interrest point in the image (rectangle with blob size)
for ip in H.interrestPoint:
    print(ip)
    x=ip[0]
    y=ip[1]
    size=round(ip[2])
    strenght=255-round((ip[3]-1)/(H.maxe-1)*255)
    print(strenght)
    cv2.rectangle(img,(x-size//2,y-size//2),(x+size//2,y+size//2),(strenght,strenght,strenght),1)

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
