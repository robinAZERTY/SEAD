"""
This version of the Laplacian Pyramid is like this:
each level of the pyramid is a 2D array of 8bit signed integers and the size decreases.
to build a level of the pyramid, we take the difference between two average filtered images (use of the integral image)
We have to care about boundary conditions. A lvl is a cropped and scaled version of the previous lvl.
"""
import integralImage as ii
from math import*

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
        self.maxima_candidates=[]
        self.minima_candidates=[]
        self.maxima_dynamic=[None,None]
        self.minima_dynamic=[None,None]
        self.integralImage=ii.IntegralImage(img)
        
        #max number of blur levels we can have
        NLevel=0
        maxBoxSize=min(self.integralImage.width,self.integralImage.height)
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
            xdim = round((self.integralImage.width - 2*boundary)/pixelFactor)
            ydim = round((self.integralImage.height - 2*boundary)/pixelFactor)
            #xdim and ydim must be greater or equal to 3
            xdim = max(3,xdim)
            ydim = max(3,ydim)
            #recompute the pixelFactor to have an integer number of pixels
            pixelFactorX = (self.integralImage.width - 2*boundary)/xdim
            pixelFactorY = (self.integralImage.height - 2*boundary)/ydim
            if pixelFactorX>self.blurInfo[NLevel-1][0] or pixelFactorX<self.blurInfo[NLevel][0]-self.blurInfo[NLevel-1][0]:
                #"Warning: pixelFactorX is not in the range of the previous and next blur, could be a problem when interpolating"
                break
            if pixelFactorY>self.blurInfo[NLevel-1][0] or pixelFactorY<self.blurInfo[NLevel][0]-self.blurInfo[NLevel-1][0]:
                #"Warning: pixelFactorY is not in the range of the previous and next blur, could be a problem when interpolating"
                break 

            self.levelInfo.append([xdim,ydim,blobSize,pixelFactorX,pixelFactorY])
            boxSize*=boxSizeRatio
            


        print("Number of levels: ",NLevel)
        print("Blur info (boxSize,boundary): ",self.blurInfo)
        print("Level info (xdim,ydim,blobSize,pixelFactorX,pixelFactorY): ",self.levelInfo)