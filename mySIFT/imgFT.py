"""Fourier transform of an image.
"""
from posixpath import pardir
import numpy as np

def get_polynomial_padding(signal,n):
    #useful to make periodic a signal
    
    x0=0
    x1=n

    y1=signal[0]
    y0=signal[-1]
    dy1=signal[1]-y1
    dy0=y0-signal[-2] 
    
    Y=np.array([y0,y1,dy0,dy1])
    A=np.array([[x0**3,x0**2,x0,1],[x1**3,x1**2,x1,1],[3*x0**2,2*x0,1,0],[3*x1**2,2*x1,1,0]])
    X=np.linalg.solve(A,Y)
    a,b,c,d=X
    
    ret=[a*(i+1)**3+b*(i+1)**2+c*(i+1)+d for i in range(n)]
    
    return ret

def pad(signal,padding="zero"):
    N=len(signal)
    log2_N= np.log2(N)
    
    if log2_N%1==0:
        return signal,N
    
    #find the next power of 2
    N2=2**int(np.ceil(log2_N))
    
    #number of element to add
    n=N2-N
    
    padding_switcher={
            "zero":[0]*n,
            "linear":[signal[-1]]*n,
            "mirror":signal[-n:][::-1],
            "periodic":signal[:n],
            "polynomial":get_polynomial_padding(signal,n)
            }
    
    pad=padding_switcher[padding]
    new_signal=np.concatenate((signal,pad))
    return new_signal,N2




    
    
    

def depad(signal,N):
    n=len(signal)
    if n==N:
        return signal
    
    nl=(n-N)//2
    if N%2==0:
        nr=nl
    else:
        nr=nl-1
        
    return signal[nl:n-nr]
    

def FFT(signal,padding="zero"):
    signal,N=pad(signal,padding)
    ret=np.array(FFT_pow_of_2(signal))/N
    return ret

def FT_get_half(coeff):
    l=len(coeff)
    if l%2==0:
        return coeff[:l//2]
    else:
        raise ValueError("the length of the coefficient must be even")

def FT_get_entier(coeff):
    #concatenate coeff and its symetric
    mirror=coeff[::-1]
    #mirror=mirror[1:]
    mirror=np.concatenate(([0],mirror))
    #take the conjugate
    mirror=np.conjugate(mirror)
    ret= np.concatenate((coeff,mirror))
    #pop the last element
    ret=ret[:-1]
    return ret

    

def FFT_pow_of_2(signal):
    """_summary_
    just a fast fourier transform algorithm
    _parameters_
    signal : the signal to transform
    n : number of coefficient to return
    _return_
    the fourier transform of the signal
    """
    N=len(signal)
    if N==1:
        return signal
    else:
        even=FFT_pow_of_2(signal[::2])
        odd=FFT_pow_of_2(signal[1::2])
        T=[np.exp(-2j*np.pi*k/N)*odd[k] for k in range(N//2)]
        return [even[k]+T[k] for k in range(N//2)]+[even[k]-T[k] for k in range(N//2)]


def IFFT(coeff):
    """
    zero padding to the next power of 2
    """
    
    #if the signal is already a power of 2
    N=len(coeff)
    log2_N= np.log2(N)
        
    if log2_N%1==0:    
        ret=np.array(IFFT_pow_of_2(coeff))
        return ret
    
    #find the next power of 2
    N2=2**int(np.ceil(log2_N))
    #number of zero to add
    n=N2-N
    #zero padding
    pad=[0]*n
    
    coeff=np.concatenate((coeff,pad))
    ret=np.array(IFFT_pow_of_2(coeff))
    return ret

def IFFT_pow_of_2(coeff):
    """_summary_
    just a fast fourier transform algorithm
    _parameters_
    coeff : the coefficient to transform
    n : the lenght of the signal to return
    _return_
    the signal
    _return_
    the fourier transform of the signal
    """
    N=len(coeff)
    if N==1:
        return coeff
    else:
        even=IFFT(coeff[::2])
        odd=IFFT(coeff[1::2])
        T=[np.exp(2j*np.pi*k/N)*odd[k] for k in range(N//2)]
        return [(even[k]+T[k]) for k in range(N//2)]+[(even[k]-T[k]) for k in range(N//2)]
    
    
    
"""
Only with half of the coefficient
"""


    
def FT_list(ft):
    ret=[np.abs(ft),ft,[i for i in range(len(ft))]]
    #transpose the matrix
    ret=np.transpose(ret)
    return ret

    
def FT_e_sum(fte1,fte2):
    return [fte1[0]*fte2[0],fte1[1]+fte2[1]]

# take second element for sort
def takeFirst(elem):
    return elem[0]



def compress(ft,treshold_coeff):
    hft=FT_get_half(ft)#ok
    ft_l=FT_list(hft)
    if treshold_coeff==0:
        return ft_l
    #compute the average
    avg=np.average(ft_l[:,0])
    #keep only the coefficient above the treshold
    tres=avg*treshold_coeff
    ft_c=ft_l[ft_l[:,0]>tres]
    return ft_c

def decompress(ft_c,N):
    half=[0j+0]*(N//2)
    for _,z,i in ft_c:
        index=int(i.real)
        half[index]=z
    ret=FT_get_entier(half)
    return ret