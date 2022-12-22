#buid a class to create a image object wich contains "blobs" like circles, squares, etc.
#img is 2d array of 8bit integers
import cv2
import numpy as np


class imageTest(object):
    def __init__(self,shape):
        self.shape=shape
        self.table = [ [ 0 for y in range(self.shape[0]) ] for x in range(self.shape[1]) ]
        self.blobs=[]


    def addBlob(self,pos,shape,figure="ellipse",brightness=255):
        #figure possible values: ellipse and rectangle
        if pos[0]<0 or pos[1]<0 or pos[0]>=self.shape[0] or pos[1]>=self.shape[1]:
            raise ValueError("position out of range")
        npimg=np.array(self.table)
        if figure=="ellipse":
            self.blobs.append([pos,shape,figure,brightness])
            cv2.ellipse(npimg, pos, shape, 0, 0, 360, brightness, -1)
        elif figure=="rectangle":
            self.blobs.append([pos,shape,figure,brightness])
            p1=(pos[0]-shape[0],pos[1]-shape[1])
            p2=(pos[0]+shape[0],pos[1]+shape[1])
            cv2.rectangle(npimg, p1,p2, brightness, -1)
        else:
            raise ValueError("figure must be 'ellipse' or 'rectangle'")
        
        self.table=npimg.tolist()
        
    def randomBlob(self,n,maxSize=20,minSize=1):
        for i in range(n):
            pos=(np.random.randint(0,self.shape[0]),np.random.randint(0,self.shape[1]))
            shape=(np.random.randint(minSize,maxSize),np.random.randint(minSize,maxSize))
            figure=np.random.choice(["ellipse","rectangle"])
            brightness=np.random.randint(0,255)
            self.addBlob(pos,shape,figure,brightness)