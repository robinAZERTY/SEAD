"""
This version of the Laplacian Pyramid is like this:
each level of the pyramid is a 2D array of 8bit signed integers and the size decreases.
to build a level of the pyramid, we take the difference between two average filtered images (use of the integral image)
We have to care about boundary conditions. A lvl is a cropped and scaled version of the previous lvl.
"""
import integralImage as ii
from math import*
import numpy as np
import matplotlib.pyplot as plt

def round_odd(x):
    return 2*floor(x/2.0)+1

def round_even(x):
    return 2*round(x/2.0)

class LaplacianPyramid(object):
    """summary
    variables:
        self.integralImage : integral image of the original image
        self.blurInfo : list of list [boxSize,border] for each level of the pyramid
        self.levelInfo : list of list [xdim,ydim,blobSize,pixelFactor] for each level of the pyramid
        
        self.maxima_candidates : list of list [x,y,level] for each maxima candidate
        self.minima_candidates : list of list [x,y,level] for each minima candidate
        self.maxima_dynamic : [zmin,zmax]
        self.minima_dynamic : [zmin,zmax]
    """
    def __init__(self, img, boxSizeRatio=1.4, boxSize0=9) -> None:
        """
        img : original image
        boxSize0 : size of the box for the first level of the pyramid
        accuracy_2d = 3 : we allocate 3 pixels for a blob at any given level of the pyramid
        """
        
        #initializing variables
        self.blurInfo=[]
        self.levelInfo=[]
        self.dynamic=[0,0]
        self.maxima_candidates=[]
        self.minima_candidates=[]
        self.maxima_dynamic=[None,None]
        self.minima_dynamic=[None,None]
        self.II=ii.IntegralImage(img)
        self.table=[]
        
        #max number of blur levels we can have
        NLevel=0
        maxBoxSize=min(self.II.width,self.II.height)
        boxSize=boxSize0
        #initail blur
        if boxSize>maxBoxSize:
            return
        Nlevel=1
        self.blurInfo.append([boxSize,boxSize//2])
        
        boxSize*=boxSizeRatio
        while boxSize<=maxBoxSize:
            NLevel+=1
            #find the nearest odd number of boxSize
            trueBoxSize = round_odd(boxSize)
            boundary = trueBoxSize//2
            self.blurInfo.append([trueBoxSize,boundary])
            
            #levelInfo
            blobSize = self.blurInfo[NLevel-1][0]
            pixelFactor = blobSize/2.0
            xdim = round((self.II.width - 2*boundary)/pixelFactor)
            ydim = round((self.II.height - 2*boundary)/pixelFactor)
            #xdim and ydim must be greater or equal to 3
            xdim = max(3,xdim)
            ydim = max(3,ydim)
            #recompute the pixelFactor to have an integer number of pixels
            pixelFactorX = (self.II.width - 2*boundary)/xdim
            pixelFactorY = (self.II.height - 2*boundary)/ydim

            """
            if pixelFactorX>self.blurInfo[NLevel-1][0] or pixelFactorX<self.blurInfo[NLevel][0]-self.blurInfo[NLevel-1][0]:
                #"Warning: pixelFactorX is not in the range of the previous and next blur, could be a problem when interpolating"
                break
            if pixelFactorY>self.blurInfo[NLevel-1][0] or pixelFactorY<self.blurInfo[NLevel][0]-self.blurInfo[NLevel-1][0]:
                #"Warning: pixelFactorY is not in the range of the previous and next blur, could be a problem when interpolating"
                break 
            """

            self.levelInfo.append([xdim,ydim,blobSize,pixelFactorX,pixelFactorY])
            self.table.append([[0 for i in range(xdim)] for j in range(ydim)])
            boxSize*=boxSizeRatio
            


        print("Number of levels: ",NLevel)
        print("Blur info (boxSize,boundary): ",self.blurInfo)
        print("Level info (xdim,ydim,blobSize,pixelFactorX,pixelFactorY): ",self.levelInfo)
    
    def update(self,img):
        
        if len(img)!=self.II.width or len(img[0])!=self.II.height:
            self.__init__(img)
        else:
            self.II.update(img)   
    
    def __get_original_coordonate(self,x,y,s):
        #but : convertir les coordonnées du point dans le repère de la pyramide vers le repère de l'image d'origine
        #entrée : x,y,s coordonnées du point dans le repère de la pyramide
        #sortie : x,y coordonnées du point dans le repère de l'image d'origine
        
        originalX=x*self.levelInfo[s][3]+self.blurInfo[s+1][1]
        originalY=y*self.levelInfo[s][4]+self.blurInfo[s+1][1]
        return [originalX,originalY]

    
    def __get_pyramid_coordonate(self,x,y,s):
        #but : convertir les coordonnées du point dans le repère de l'image d'origine vers le repère de la pyramide
        #entrée : x,y,s coordonnées du point dans le repère de l'image d'origine
        #sortie : x,y coordonnées du point dans le repère de la pyramide
        
        pyramidX=(x-self.blurInfo[s+1][1])/self.levelInfo[s][3]
        pyramidY=(y-self.blurInfo[s+1][1])/self.levelInfo[s][4]
        return [pyramidX,pyramidY]
    
    def __compute_filter(self,s,x,y):
        """_summary_

        Args:
            s (float): lvl in the pyramid
            x (float): x in the pyramid
            y (float): y in the pyramid
        """
        originalCoord=self.__get_original_coordonate(x,y,s)
        originalCoord[0]=round(originalCoord[0])
        originalCoord[1]=round(originalCoord[1])
        a1=self.II.avrBlur(originalCoord[1],originalCoord[0],self.blurInfo[s][0])
        a2=self.II.avrBlur(originalCoord[1],originalCoord[0],self.blurInfo[s+1][0])
        self.table[s][y][x]=a1-a2
    
    def __update_extrem_value(self,val,dynamic):
        if val<dynamic[0]:
            dynamic[0]=val
        if val>dynamic[1]:
            dynamic[1]=val
        #return [minval,maxval]
        
    def __compute_filter_and_find_extrema(self,s,x,y,potentialMinima,potentialMaxima):
        self.__compute_filter(s,x,y)
        if(self.table[s][y][x]>potentialMaxima[3]):
            potentialMaxima=[x,y,s,self.table[s][y][x]]
        elif(self.table[s][y][x]<potentialMinima[3]):
            potentialMinima=[x,y,s,self.table[s][y][x]]
    
    def __compute_filter_in_range(self,s,xmin,xmax,ymin,ymax):

        self.__compute_filter(s,xmin,ymin)
        potentialMinima=[xmin,ymin,s,self.table[s][ymin][xmin]]
        potentialMaxima=potentialMinima
        for x in range(xmin+1,xmax):
            for y in range(ymin,ymax):
                self.__compute_filter_and_find_extrema(s,x,y,potentialMinima,potentialMaxima)
        
        for y in range(ymin+1,ymax):
            self.__compute_filter_and_find_extrema(s,xmin,y,potentialMinima,potentialMaxima)
        
        self.__update_extrem_value(potentialMaxima[3],self.dynamic)
        self.__update_extrem_value(potentialMinima[3],self.dynamic)
        self.maxima_candidates.append(potentialMaxima)
        self.minima_candidates.append(potentialMinima)
            
    def buid(self):
        
        for s in range(len(self.levelInfo)):
            nx=self.levelInfo[s][0]//3
            ny=self.levelInfo[s][1]//3
            rx=self.levelInfo[s][0]%3
            ry=self.levelInfo[s][1]%3
            X0=0
            Y0=0
            for x0 in range(0,nx):
                X0=x0*3
                for y0 in range(0,ny):
                    Y0=y0*3
                    self.__compute_filter_in_range(s,X0,X0+3,Y0,Y0+3)

            X0+=3
            Y0+=3
            X0=min(X0,self.levelInfo[s][0]-1)
            Y0=min(Y0,self.levelInfo[s][1]-1)
            for y0 in range(ny):
                Y00=y0*3
                X00=X0
                self.__compute_filter_in_range(s,X00,X00+rx,Y00,Y00+3)
            for x0 in range(nx):
                X00=x0*3
                Y00=Y0
                self.__compute_filter_in_range(s,X00,X00+3,Y00,Y00+ry)
            
            self.__compute_filter_in_range(s,X0,X0+rx,Y0,Y0+ry)
                        
    def show(self):
        plt.figure()
        n=ceil(np.sqrt(len(self.levelInfo)))
        m=ceil(len(self.levelInfo)/n)
        for i in range(len(self.levelInfo)):
            plt.subplot(n,m,i+1)
            plt.imshow(self.table[i],cmap='gray',vmin=self.dynamic[0],vmax=self.dynamic[1])
            plt.title("level "+str(i))     
        