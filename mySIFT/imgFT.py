"""Fourier transform of an image.
"""
import numpy as np

def FFT(signal,padding="zero"):
    """
    zero padding to the next power of 2
    """
    
    #if the signal is already a power of 2
    N=len(signal)
    log2_N= np.log2(N)
        
    if log2_N%1==0:
        ret=np.array(FFT_pow_of_2(signal))/N
        return ret
    
    #find the next power of 2
    N2=2**int(np.ceil(log2_N))
    #number of zero to add
    n=N2-N
    #zero padding
    if padding=="zero":
        pad=[0]*n
    elif padding=="mirror":
        pad=signal[::-1][0:n]
    elif padding=="linear":
        pad=[signal[-1]]*n
    elif padding=="periodic":
        pad=signal[:n]

    else:
        raise ValueError("padding not recognized")
    
    signal=signal+pad
    ret=np.array(FFT_pow_of_2(signal))/N2
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