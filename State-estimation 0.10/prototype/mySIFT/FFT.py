"""Fast Fourier Transform tricks"""

from logging import warning
import numpy as np

"""
#################################
optimisation of the FFT algorithm
#################################

#to use the FFT, we need a length of signal that is a power of 2
#if the signal is not a power of 2, we need to pad it
#we can pad it with zeros, or with a linear function...
#but we can also use this space to make the signal periodic, and continus at the same time
#make it continus and periodic help to converge at low frequencies
#So we shouldn't have many coefficients in high frequencies (helpful to reduce the number of useful coefficient for compression)
"""



def get_polynomial_padding(signal,n,degree):
    #find coefficients of the polynomial to satisfy the boundary conditions (value and derivative)
    #useful to make periodic a signal
    
    x0=0
    x1=n

    y1=signal[0]
    y0=signal[-1]

    
    if degree==1:
        a=(y1-y0)/(x1-x0)
        ret=[a*x+y0 for x in range(n)]
        
    elif degree==3:
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
        #warning ("the length of the signal is already a power of 2, if it is not a periodic signal, possible difficulties to converge")
        return signal,N
    
    #find the next power of 2
    N2=2**int(np.ceil(log2_N))
    
    #number of element to add
    n=N2-N
    
    if n<0.1*N2:
        #warning ("the length of the signal is slightly lower than power of 2, possible difficulties to converge")
        pass
    
    pad=[]
    if padding=="zero":
        pad=[0]*n
    elif padding=="linear":
        pad=[signal[-1]]*n
    elif padding=="mirror":
        pad=signal[-n:][::-1]
    elif padding=="periodic":
        pad=signal[:n]
    #make it continus and periodic help to converge
    elif padding=="polynomial1":
        pad=get_polynomial_padding(signal,n,1)
    elif padding=="polynomial3":
        pad=get_polynomial_padding(signal,n,3)
        
    new_signal=np.concatenate((signal,pad))
    return new_signal,N2


    
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


"""
#################################
FFT algorithm
#################################
"""

def FFT(signal,padding="zero"):
    signal,N=pad(signal,padding)
    ret=FFT_pow_of_2(signal)/N
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
        return np.array(signal)
    else:
        even=FFT_pow_of_2(signal[::2])
        odd=FFT_pow_of_2(signal[1::2])
        T=[np.exp(-2j*np.pi*k/N)*odd[k] for k in range(N//2)]
        return np.array([even[k]+T[k] for k in range(N//2)]+[even[k]-T[k] for k in range(N//2)])


def IFFT(coeff,NN=None,domain="Complex"):
    """
    zero padding to the next power of 2
    """
    #if the signal is already a power of 2
    N=len(coeff)
    log2_N = np.log2(N)
        
    if log2_N%1==0:    
        ret=np.array(IFFT_pow_of_2(coeff))
        
        if NN is None:
            if domain=="Real":
                return ret.real
            return ret
        else:
            if domain=="Real":
                return ret[0:NN].real
            return ret[0:NN]
    
    #find the next power of 2
    N2=2**int(np.ceil(log2_N))
    #number of zero to add
    n=N2-N
    #zero padding
    pad=[0]*n
    
    coeff=np.concatenate((coeff,pad))
    ret=np.array(IFFT_pow_of_2(coeff))
    
    if NN is None:
        if domain=="Real":
            return ret.real
        return ret
    else:
        if domain=="Real":
            return ret[0:NN].real
        return ret[0:NN]

def IFFT_pow_of_2(coeff, mode="entier"):
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
