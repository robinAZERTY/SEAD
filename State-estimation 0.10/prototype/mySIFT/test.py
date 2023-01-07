
from PIL import Image
import FFT2
import numpy as np
import matplotlib.pyplot as plt

filename=r"C:\Users\\robin\\Documents\\SEAD\\State-estimation 0.10\\prototype\\mySIFT\\IMG_20220824_204247.jpg"

img  = Image.open(filename) 
#convert the image to grayscale
img = img.convert('L')

#convert the image to numpy array
img=np.array(img)
f=FFT2.FFT2(img,"zeros")

plt.imshow(f)
plt.show()
