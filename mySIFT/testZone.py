from imgFT import*
import matplotlib.pyplot as plt

N=2**12
cut=N//10
t=np.linspace(0,1,N)
t=t[0:N-cut]
N=len(t)
w=2*np.pi*t
signal = np.cos(w*1.8)+0.5*np.cos(5*w)-1
#add noise
signal+=np.random.normal(0,0.0,N)
#signal=np.exp(-((t-0.5)/0.03)**2/0.1)
ft=FFT(signal,padding="polynomial")
#hft=FT_get_half(ft)
#ft2=FT_get_entier(hft)

#ift=depad(IFFT(ft2),N)
ift=IFFT(ft)
N2=len(ift)
print(N,N2)
fte=compress(ft,1)

ftd=decompress(fte,N2)
ift2=IFFT(ftd)




#plot the signal
plt.figure()
plt.subplot(2,1,1)
plt.plot(signal)
plt.plot(ift)
plt.plot(ift2)
plt.legend(["true","ifft k= "+str(len(ft)),"compressed k = "+str(len(fte))])
plt.subplot(2,1,2)
plt.plot(abs(ft))
plt.plot(abs(ftd))
plt.legend(["original","decompressed"])

plt.show()