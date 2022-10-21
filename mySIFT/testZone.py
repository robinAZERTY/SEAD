from imgFT import*
import matplotlib.pyplot as plt

def set_axes_equal(ax):
    '''Make axes of 3D plot have equal scale so that spheres appear as spheres,
    cubes as cubes, etc..  This is one possible solution to Matplotlib's
    ax.set_aspect('equal') and ax.axis('equal') not working for 3D.

    Input
      ax: a matplotlib axis, e.g., as output from plt.gca().
    '''

    x_limits = ax.get_xlim3d()
    y_limits = ax.get_ylim3d()
    z_limits = ax.get_zlim3d()

    x_range = abs(x_limits[1] - x_limits[0])
    x_middle = np.mean(x_limits)
    y_range = abs(y_limits[1] - y_limits[0])
    y_middle = np.mean(y_limits)
    z_range = abs(z_limits[1] - z_limits[0])
    z_middle = np.mean(z_limits)

    # The plot bounding box is a sphere in the sense of the infinity
    # norm, hence I call half the max range the plot radius.
    plot_radius = 0.5*max([x_range, y_range, z_range])

    ax.set_xlim3d([x_middle - plot_radius, x_middle + plot_radius])
    ax.set_ylim3d([y_middle - plot_radius, y_middle + plot_radius])
    ax.set_zlim3d([z_middle - plot_radius, z_middle + plot_radius])
    
#buid a step function
def step(x,x1):
    if x < x1:
        return 0
    else:
        return 1
    

#signal = [step(x,32) for x in range(64)]
#signal = [np.sin(2+10*x/128*(2*np.pi)+1) for x in range(128)]
signal = [np.exp(-((x-64)/2)**2/2) for x in range(128)]
ft=FFT(signal,"mirror")



half=FT_get_half(ft)
#we can work with the half of the fourier transform
n=25#number of coefficient to keep
initailLength=len(half)
half=half[0:n-1]
half=np.concatenate((half,[0]*(initailLength-n+1)))
entier=FT_get_entier(half)

ift=IFFT(ft)
ift2=IFFT(entier)

#plot the signal
plt.subplot(2,1,1)
plt.plot(signal)
plt.plot(ift)
plt.plot(ift2)
plt.legend(["signal","FFT","FFT with half"])
plt.subplot(2,2,3)
plt.plot(np.abs(ft))
plt.plot(np.angle(ft))
plt.legend(["abs","angle"])
plt.subplot(2,2,4)
plt.plot(np.abs(entier))
plt.plot(np.angle(entier))
plt.legend(["abs","angle"])
plt.title("half reconstructed")


fig = plt.figure()
ax = plt.axes(projection ='3d')


ax.plot3D(np.real(ft),np.imag(ft),np.linspace(0, 1, len(ft)))
set_axes_equal(ax)
plt.show()

