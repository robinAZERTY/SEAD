import FCHPIT
import numpy as np

import matplotlib.pyplot as plt

N=2**7-20

t=np.linspace(0,1,N)
w=2*np.pi*t
tau=12


noise=np.random.normal(0,0.05,N)
signal=12*np.sinc(w-w[len(w)//2-1])+255-255*t+20*np.sinc(np.pi*(t*4)**2)*(t+1)**3+noise
signal=(np.sin(3*w)+1j*np.cos(3*w))*(t)


FT=FCHPIT.compress(signal,0.01,"polynomial3","Complex")
print(len(FT[0]))
XX=FCHPIT.decompress(FT,N,"Complex")

ft=FCHPIT.FFT.FFT(signal)
xx=FCHPIT.FFT.IFFT(ft,N,"Complex")





fig = plt.figure()
ax = plt.axes(projection='3d')
ax.plot3D(range(len(signal)),signal.real,signal.imag,'-')
ax.plot3D(range(len(XX)),XX.real,XX.imag,'-')
ax.set_title('Signal compress & decompress')

fig = plt.figure()
ax = plt.axes(projection='3d')
ax.plot3D(t,signal.real,signal.imag,'-')
ax.plot3D(t,xx.real,xx.imag,'-')
ax.set_title('Signal FTT & IFFT')

plt.show()