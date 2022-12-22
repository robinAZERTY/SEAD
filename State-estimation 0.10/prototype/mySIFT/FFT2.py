from tkinter import CENTER
from turtle import distance
import FCHPIT
import numpy as np

"""image fourier transform"""


def FFT2(image,padding="polynomial1"):
    
    FT1=[]
    for line in image:
        FT1.append(FCHPIT.FFT.FFT(line,padding))
    
    FT1=np.transpose(FT1)
    FT2=[]
    for col in FT1:
        FT2.append(FCHPIT.FFT.FFT(col,padding))
    FT2=np.transpose(FT2)
    return FT2
        
    
def IFFT2(FT2,NN=None):
    FT1=[]
    for line in FT2:
        FT1.append(FCHPIT.FFT.IFFT(line,NN=NN[1]))
    
    FT1=np.transpose(FT1)
    FT2=[]
    for col in FT1:
        FT2.append(FCHPIT.FFT.IFFT(col,NN=NN[0],domain="Real"))
    FT2=np.transpose(FT2)
    return FT2


def FCHPI2_2_FCHPIT2(FCHPI2,treshold):
    #treshold the FCHPI2 (FCHPIT2)
    
    if treshold==0:
        return FCHPI2
    
    Power,Coeff,Freq=FCHPI2
    
    maxPower=Power[0]
    totalPower=np.sum(Power)
    
    keepPower=[]
    keepCoeff=[]
    keepFreq=[]
    
    index_sorted=np.argsort(Power)[::-1]
    P=0
    for i in index_sorted:
        P+=Power[i]
        lossePower=totalPower-P
        if lossePower<totalPower*treshold:
            break
        else:
            keepPower.append(Power[i])
            keepCoeff.append(Coeff[i])
            keepFreq.append(Freq[i])
    
    FCHPIT2=[keepPower,keepCoeff,keepFreq]
    
    return FCHPIT2

def FC2_2_FCHPIT2(FT2,treshold=0,domain="Complex"):
    #for the FT of a real function, the fourier coefficients are symetric so we only keep the first half
    #(FCH)
    if domain=="Real":
        FCH2=FCHPIT.FFT.FT_get_half(FT2)
    else:
        FCH2=FT2
    
    #make the FCH characterized by power and indexed (FCHMI)
    
    power=[]
    coeff=[]
    freq=[]
    max_power=0
    for i in range(len(FCH2)):
        p=np.sqrt(FCH2[i].real**2+FCH2[i].imag**2)*i
        power.append(p)
        coeff.append(FCH2[i])
        freq.append(i)
        if p>max_power:
            max_power=p
            
    power[0]=max_power
    FCHPI2=[power,np.array(coeff),freq]
    return FCHPI2_2_FCHPIT2(FCHPI2,treshold)

def FCHPIT2_2_FC2(FCHPIT2,N,domain="Complex"):
    if domain=="Real":
        FCH2=[0j]*(N//2)
    
        for i in range(len(FCHPIT2[2])):
            index=FCHPIT2[2][i]
            FCH2[index]=FCHPIT2[1][i]
        FC2=FCHPIT.FFT.FT_get_entier(FCH2)
        
    else:
        FCH2=[0j]*N
        for i in range(len(FCHPIT2[2])):
            index=FCHPIT2[2][i]
            
            FCH2[index]=FCHPIT2[1][i]
        FC2=FCH2
    return FC2


    

def IMG_compress(image,treshold):
    """_summary_
    just a fast fourier transform algorithm
    _parameters_
    image : the image to transform (2D array)
    _return_
    the fourier transform of the image
    """
    
    #padding
    i2=[]
    for line in image:
        d,_=FCHPIT.FFT.pad(line,"polynomial1")
        i2.append(d)
    i2=np.transpose(i2)
    i3=[]
    for line in i2:
        d,_=FCHPIT.FFT.pad(line,"polynomial1")
        i3.append(d)
    i3=np.transpose(i3)
    image_padded=i3

    
    #for each line
    ret=[]
    tfLine=[]#fourier transform of the lines
    
    for line in (image_padded):
        tfLine.append(FCHPIT.compress(line,domain="Real")[1])#give complex number
 
    
    
    tfLineT=np.transpose(tfLine)
    #half tfLineT
    
    #tfLineT=tfLineT[0:len(tfLineT)//2]
    tf=[]
    
    #for each column
    for col in tfLineT:      
        tf.append(FCHPIT.compress(col,0,domain="Complex")[1])
 
    tf=np.transpose(tf)
    #we can't take only the half of the tf because we do the FT on a complex Series
    
            
    return FCHPIT2,(len(tf),len(tf[0]))

def IMG_decompress(FCHPIT2,FTH_shape,final_img_shape):
    """_summary_
    just a fast fourier transform algorithm
    _parameters_
    image : the image to transform (2D array)
    _return_
    the fourier transform of the image
    """
    #array of complex number of size FTH_shape
    #create a new array from the shape of FCHPIT2
    FCHPIT2array = np.zeros(FTH_shape, dtype=complex)

    #fill the array with the values of FCHPIT2
    for i in range(len(FCHPIT2[0])):
        FCHPIT2array[FCHPIT2[2][i][0],FCHPIT2[2][i][1]]=FCHPIT2[1][i]
    
    FCHPIT2array=np.transpose(FCHPIT2array)
    mirror=FCHPIT2array[::-1]
    mirror=np.concatenate(([[0]*len(mirror[0])],mirror))
    
    mirror=np.conjugate(mirror)
    
    FT=np.concatenate((FCHPIT2array,mirror))
    FT=FT[:-1]
    
    FT=np.transpose(FT)
    

    
    img=[]
    for line in FT:
        img.append(FCHPIT.decompress(line,final_img_shape[1],domain="Complex"))
    
    igmT=np.transpose(img)
    img2=[]
    for line in igmT:
        img2.append(FCHPIT.decompress(line,final_img_shape[0],domain="Real"))
        

    img2=np.transpose(img2)
    
    return img2
