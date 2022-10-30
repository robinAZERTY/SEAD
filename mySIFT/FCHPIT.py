    
"""
Only with this format : fourier coefficients halved, characterized by power, indexed and tresholded (FCHPIT)
"""

import FFT
from FFT import np

# take last element for sort
def takeLast(elem):
    return elem[-1]

# take first element for sort
def takeFirst(elem):
    return elem[0].real

def FCHPI_2_FCHPIT(FCHPI,treshold):
    #treshold the FCHPI (FCHPIT)
    
    if treshold==0:
        return FCHPI
    else:
        
        
        """
        power2=[]
        coeff2=[]
        freq2=[]
        treshold=treshold*power[0]
        for i in range(len(power)):
            if power[i]>treshold:
                power2.append(power[i])
                coeff2.append(coeff[i])
                freq2.append(freq[i])
          
        FCHPIT=[power2,np.array(coeff2),freq2]   
        """
        power,coeff,freq=FCHPI
        
        keepPower=[]
        keepCoeff=[]
        keepFreq=[]
        
        totalPower=np.sum(power)
        index_sorted=np.argsort(power)[::-1]
        P=0
        for i in index_sorted:
            P+=power[i]
            lossePower=totalPower-P
            if lossePower<totalPower*treshold:
                break
            else:
                keepPower.append(power[i])
                keepCoeff.append(coeff[i])
                keepFreq.append(freq[i])
                
        FCHPIT=[keepPower,np.array(keepCoeff),keepFreq]        
        return FCHPIT

    
def FC_2_FCHPIT(FT,treshold=0,domain="Complex"):
    #for the FT of a real function, the fourier coefficients are symetric so we only keep the first half
    #(FCH)
    if domain=="Real":
        FCH=FFT.FT_get_half(FT)
    else:
        FCH=FT
    
    #make the FCH characterized by power and indexed (FCHMI)
    
    power=[]
    coeff=[]
    freq=[]
    max_power=0
    for i in range(len(FCH)):
        p=np.sqrt(FCH[i].real**2+FCH[i].imag**2)*i
        power.append(p)
        coeff.append(FCH[i])
        freq.append(i)
        if p>max_power:
            max_power=p
            
    power[0]=max_power
    
    
    FCHPI=[power,np.array(coeff),freq]
    
    return FCHPI_2_FCHPIT(FCHPI,treshold)
        


def FCHPIT_2_FC(FCHPIT,N,domain="Complex"):
    if domain=="Real":
        FCH=[0j]*(N//2)
    
        for i in range(len(FCHPIT[2])):
            index=FCHPIT[2][i]
            FCH[index]=FCHPIT[1][i]
        FC=FFT.FT_get_entier(FCH)
        
    else:
        FCH=[0j]*N
        for i in range(len(FCHPIT[2])):
            index=FCHPIT[2][i]
            
            FCH[index]=FCHPIT[1][i]
        FC=FCH
    return FC


def compress(X,treshold=0,extrapolation="polynomial1",domain="Complex"):
    FT=FFT.FFT(X,extrapolation)
    FCHPIT=FC_2_FCHPIT(FT,treshold,domain)
    return FCHPIT

def decompress(FCHPIT,N,domain="Complex"):
    #next power of 2 of N
    #if N is already a power of 2
    if N & (N - 1) == 0:
        N2=N
    else:
        N2=2**int(np.ceil(np.log2(N)))
        
    FC=FCHPIT_2_FC(FCHPIT,N2,domain)
    X=FFT.IFFT(FC,N,domain)
    return X

def conv(X,Y):
    #Y is the filter (its FC as a list)
    #X is the signal (its FCHMIT)
    
    if len(Y)<X[-1][-1]:
        print("the filter is too short")
        return
    
    ret=[]
    
    for m,z,i in X:
        ii=int(i.real)
        y=float(Y[ii])
        ret.append([m*y,z*y,i])
        
    return np.array(ret)
    
def gauss_FC(N,sigma,lenght):
    x=np.linspace(0,N,N)
    y=np.exp(-x**2/(2*(N/sigma)**2))
    y=y[0:lenght]
    return y
