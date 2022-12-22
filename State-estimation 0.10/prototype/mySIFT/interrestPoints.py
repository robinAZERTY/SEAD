"""here, are functions to find the interrest points of a picture
    for every function, img is in gray scale 0 to 255
    convolution(img,array) : apply a convolution to a picture
    gaussianMask(tau) : create a gaussian mask
    sampling(img,x,y) : sample the picture at the point (x,y) with bilinear interpolation or not
    resize(img,factor) : resize the picture by a factor
    gaussianPyramid(img,tau0,r) : create a gaussian pyramid of a picture
    laplacianPyramid(Gp,tau0,r) : create a laplacian pyramid of a picture from a gaussian pyramid
    selectExtrema(Lp) : select the extremas of a laplacian pyramid
    gradient(img) : return the gradient of a picture
"""
from math import *
import re
from turtle import shape

from numpy import Inf,shape

def convolution(img,mask):
    """apply a convolution to a picture
    img : the picture (y by x array)
    array : the array to apply
    do not use numpy
    """
    
    #the convolution array must be a square array (n by n)
    #and n must be odd
    convDim=shape(mask)
    if not(len(convDim)==2 and convDim[0]==convDim[1] and convDim[0]%2==1):
        return "error : the convolution array must be a square array (n by n) and n must be odd but the shape is currently "+str(convDim)

    #convolutionArray size
    n=convDim[0]

    #the img must be an X by Y array
    imgDim=shape(img)
    if not(len(imgDim)==2):
        return "error : the img must be an X by Y array"
    
    #img size
    (Ylen,Xlen)=imgDim
    
    
    #create the array to return : array of the same size as img
    res=[[0 for i in range(Xlen)] for j in range(Ylen)]
    
    for y in range(Ylen):
        for x in range(Xlen):
            #for each pixel of the img
            for i in range(n):
                for j in range(n):
                    #if the convolution array at (ij) is not nan just skip
                    if not(isnan(mask[i][j])):
                        #for each pixel of the convolution array
                        xx=x+i-n//2
                        yy=y+j-n//2
                        #if we are out of the img, do a symetry
                        if xx<0:
                            xx=-xx
                        if yy<0:
                            yy=-yy
                        if xx>=Xlen:
                            xx=Xlen-(xx-Xlen)-1
                        if yy>=Ylen:
                            yy=Ylen-(yy-Ylen)-1
                        #add the value of the pixel of the img multiplied by the value of the pixel of the convolution array
                        Mw=mask[j][i]
                        IMGp=img[yy][xx]
                        res[y][x]+=IMGp*Mw
    return res

def gaussianMask(tau,size=(0,0),nTau=2,digit=None):
    """ create a gaussian mask
    tau : the parameter of the gaussian blur
    """
    
    if size==(0,0):
        #the size of the convolution array is 2*nTau*tau+1
        n=ceil(2*nTau*tau)+1
        m=n
    else:
        #n and m must be odd
        (m,n)=size
        if not(n%2==1 and m%2==1):
            return "error : size(n and m) must be odd"
        
    #create the convolution array
    mask=[[0 for i in range(n)] for j in range(m)]
    
    #weight cumulator
    weight=0
    for i in range(n):
        for j in range(m):
            distanceSq=(i-n//2)**2+(j-n//2)**2
            val=1/(2*pi*tau**2)*exp(-distanceSq/(2*tau**2))
            mask[j][i]=val
            weight+=val
    
    #normalize the convolution array and round it
    for i in range(n):
        for j in range(m):
            mask[j][i]/=weight
            if(digit!=None):
                mask[i][j]=round(mask[i][j],digit)
            
    return mask

def Laplacien_Mask(tau,size=(0,0),nTau=2,digit=None):
    """ create a gaussian mask
    tau : the parameter of the gaussian blur
    """
    
    if size==(0,0):
        #the size of the convolution array is 2*nTau*tau+1
        n=ceil(2*nTau*tau+1)
        m=n
    else:
        #n and m must be odd
        (m,n)=size
        if not(n%2==1 and m%2==1):
            return "error : size(n and m) must be odd"
        
    #create the convolution array
    mask=[[0 for i in range(n)] for j in range(m)]
    
    #weight cumulator
    weight=0
    for i in range(n):
        for j in range(m):
            x=(i-n//2)**2+(j-n//2)**2
            val=((x**2-tau**2)/(tau**5*sqrt(2*pi)))*exp(-(x**2)/(2*tau**2))
            #val=(-x/(tau**3*sqrt(2*pi)))*exp(-(x**2)/(2*tau**2))
            mask[j][i]=val
            weight+=val
    
    #normalize the convolution array and round it
    for i in range(n):
        for j in range(m):
            mask[j][i]/=weight
            if(digit!=None):
                mask[i][j]=round(mask[i][j],digit)
            
    return mask

def sampling(img,x,y,interpolation='NEAREST'):
    """_summary_
    sample the picture at the point (x,y) with bilinear interpolation
    img : the picture (y by x array)
    x : the x coordinate of the point to sample, can be float, must be normalize
    y : the y coordinate of the point to sample, can be float, must be normalize
    interpolation : 'NEAREST','BILLINEAR','IN_AERA'
    """
    #the img must be an X by Y array
    imgDim=shape(img)
    if not(len(imgDim)==2):
        return "error : the img must be an X by Y array"
    
    if not(interpolation in ['NEAREST','BILLINEAR','IN_AERA']):
        return "error : interpolation must be 'NEAREST','BILLINEAR','IN_AERA'"
    
    if not(x<=1 and x>=0 and y<=1 and y>=0):
        return "error : x and y must be normalize"
    
    #img size
    (Xlen,Ylen)=imgDim
    
    if interpolation=='NEAREST':
        #nearest interpolation
        return img[round(y*Ylen)][round(x*Xlen)]

    elif interpolation=='BILINEAR':
        #bilinear interpolation
        #get the 4 nearest pixels
        x1=round(x*Xlen)
        y1=round(y*Ylen)
        x2=x1+1
        y2=y1+1
        #if we are out of the img, do a symetry
        if x1<0:
            x1=-x1
        if y1<0:
            y1=-y1
        if x2>=Xlen:
            x2=Xlen-(x2-Xlen)-1
        if y2>=Ylen:
            y2=Ylen-(y2-Ylen)-1
        #compute the weight of each pixel
        w1=(x2-x*Xlen)*(y2-y*Ylen)
        w2=(x*Xlen-x1)*(y2-y*Ylen)
        w3=(x2-x*Xlen)*(y*Ylen-y1)
        w4=(x*Xlen-x1)*(y*Ylen-y1)
        #return the weighted sum of the 4 pixels
        return img[y1][x1]*w1+img[y1][x2]*w2+img[y2][x1]*w3+img[y2][x2]*w4
    
    elif interpolation=='IN_AERA':
        #in aera interpolation
        #average the value of the in aera pixels
        #scale should be given and can be a float
        return "error : IN_AERA is not implemented"


def resize(img,factor,interpolation='NEAREST'):
    """_summary_
    resize the picture by a factor
    img : the picture (y by x array)
    factor : the factor to resize the picture
    interpolation : 'NEAREST','BILLINEAR','IN_AERA'
    """
    #the img must be an X by Y array
    imgDim=shape(img)
    if not(len(imgDim)==2):
        return "error : the img must be an X by Y array"
    
    #img size
    (Xlen,Ylen)=imgDim
    
    #create the array to return
    res=[[0 for i in range(round(Xlen*factor))] for j in range(round(Ylen*factor))]
    
    for y in range(round(Ylen*factor)):
        for x in range(round(Xlen*factor)):
            #for each pixel of the img
            res[y][x]=sampling(img,x/factor,y/factor,interpolation)
    return res

def gaussianPyramid(img,tau0,r,N=Inf):
    """_summary_
    create a gaussian pyramid of a picture
    img : the picture (y by x array)
    tau0 : the parameter of the gaussian blur
    r : the ratio between two consecutive equivalent gaussian blur    
    
    the initail img (i0) is blured with a gaussian blur of parameter tau0 : i0b
    i0b is resized by a s factor : i1
    i1 is blured with a gaussian blur of parameter tau0 : i1b
    ...
    
    
    so we have :
    to find the tau to apply to the gaussian blur at the level i to keep the r ratio between two consecutive equivalent gaussian blur :
    tau_eq : equivalent tau is independent to scale in the pyramid (the size of tau in pixel in the initial img)
    tau_eq_i = tau_applied_i*scale_i
    
    we want 
    tau_eq_i/tau_eq_(i-1)=r
    tau_eq_0=tau0
    
    so find scale_i and tau_applied_i
    """
    initialShape=shape(img)
    if not(len(initialShape)==2):
        return "error : the img must be an X by Y array"
    
    tau_eq=[]
    scale=[]
    
    Pyr=[]#contains [img,blur] at each level
    
    N_max=ceil(log(min(initialShape[0],initialShape[1])/tau0)/log(r))
    if N>N_max:
        N=N_max
    
    N=int(N)
    
    gaussMask=gaussianMask(tau0)
    relativeRescale=1/r
    
    for i in range(N):
        tau_eq.append(tau0*r**i)
        scale.append(1/(r**i))
        print("level "+str(i)+" : tau_eq="+str(tau_eq[i])+" scale="+str(scale[i]))
        if i==0:
            Pyr.append([img,convolution(img,gaussMask)])
        else:
            resized=resize(Pyr[-1][1],relativeRescale,'BILINEAR')
            Pyr.append([resized,convolution(resized,gaussMask)])
    
    return Pyr,tau_eq,scale

import cv2
cap = cv2.VideoCapture(0)
mode='gray'

def get_frame():
    _, frame = cap.read()
    #reverse the frame
    frame = cv2.flip(frame,1)
    if(mode=="gray"):
        frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    return frame

import numpy as np

def integrate(img):
    
    

    choosenType=np.int32

    ret=np.zeros(img.shape,dtype=choosenType)
    for y in range(len(img)):
        for x in range(len(img[0])):
            if y==0 and x==0:
                ret[y][x]=img[y][x]
            elif y==0:
                ret[y][x]=ret[y][x-1]+img[y][x]
            elif x==0:
                ret[y][x]=ret[y-1][x]+img[y][x]
            else:
                ret[y][x]=ret[y-1][x]+ret[y][x-1]-ret[y-1][x-1]+img[y][x]
    return ret

def derive(img_inte):
    
    ret=np.zeros(img_inte.shape,dtype=np.int32)
    
    for y in range(len(img_inte)):
        for x in range(len(img_inte[0])):
            if y==0 and x==0:
                ret[y][x]=img_inte[y][x]
            elif y==0:
                ret[y][x]=img_inte[y][x]-img_inte[y][x-1]
            elif x==0:
                ret[y][x]=img_inte[y][x]-img_inte[y-1][x]
            else:
                ret[y][x]=img_inte[y][x]-img_inte[y-1][x]-img_inte[y][x-1]+img_inte[y-1][x-1]
    return ret

def avrBlur(itegrateIMG,winSize):
    #winSize must be odd
    if winSize%2==0 or winSize<3:
        raise "error : winSize must be odd and superior to 2"
    ret=np.zeros(itegrateIMG.shape,dtype=np.int32)
    
    #compute a average blur from an integrate image
    #ignore the border
    border=(winSize-1)//2
    print(border)
    for y in range(border,len(itegrateIMG)-border):
        for x in range(border,len(itegrateIMG[0])-border):
            ret[y][x]=itegrateIMG[y+border][x+border]-itegrateIMG[y+border][x-border-1]-itegrateIMG[y-border-1][x+border]+itegrateIMG[y-border-1][x-border-1]
            ret[y][x]=ret[y][x]//(winSize**2)

                
    return ret

