import imageTest
from detector import detector
import matplotlib.pyplot as plt
import numpy as np
import cv2 as cv
import time

video_or_image = 1

cap = cv.VideoCapture(0)
cap.set(3, 720)
cap.set(4, 480)

def get_frame():
    _, frame = cap.read()
    #reverse the frame
    frame = cv.flip(frame,1)
    #gray
    frame = cv.cvtColor(frame, cv.COLOR_BGR2GRAY)
    
    
    #resize the image keeping the aspect ratio
    width=150
    height=int(width*len(frame[0])//len(frame))
    frame = cv.resize(frame,(height,width))
    #convert the image to a list 
    #frame=frame.tolist()
    
    return frame

prev_fps = 20

if video_or_image == 0:
    my_d=detector(get_frame())
    print("img dim : ",my_d.LP.II.width,my_d.LP.II.height)
    print("level info: ",my_d.LP.levelInfo)
    
    while True:
        t0=time.time()
        my_d.update(get_frame())
        my_d.detect(0.25)
        t1=time.time()
        dt=t1-t0
        fps=prev_fps*0.98+1/dt*0.02
        prev_fps=fps
        draw=my_d.draw()
        draw=cv.resize(draw,(720,480),interpolation=cv.INTER_NEAREST)
        cv.putText(draw, "FPS: "+str(round(fps)), (50, 100), cv.FONT_HERSHEY_SIMPLEX, 3, 0, 5)
        cv.imshow('frame',draw)
        if cv.waitKey(1) == ord('q'):
            break
else:
    img = imageTest.imageTest((300,300))
    img.randomBlob(3,30,25)
    my_d=detector(img.table)
    my_d.detect(0.25)
    draw=my_d.draw()
    
    plt.figure()
    plt.subplot(1,2,1)
    plt.imshow(img.table,cmap='gray')
    plt.title("original image")
    plt.subplot(1,2,2)
    plt.imshow(draw,cmap='gray')
    plt.title("detected image")
    plt.show()   
    
    

        


"""LP = lp.LaplacianPyramid(img)

LP.buid()
LP.show()
print("len(LP.maxima_candidates)=",len(LP.maxima_candidates))
print("len(LP.minima_candidates)=",len(LP.minima_candidates))
print("LP.dynamic=",LP.dynamic)

plt.figure()
plt.subplot(1,2,1)
plt.imshow(img,cmap='gray')
plt.title("original image")
plt.subplot(1,2,2)
"""