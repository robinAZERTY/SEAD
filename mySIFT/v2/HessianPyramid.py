from sklearn.metrics import homogeneity_score
from sklearn.preprocessing import scale
import IntegralImage as ii
import numpy as np
import cv2
import time

class HessianPyramid(object):
    def __init__(self, original, tau0=11,ratio=1.6, stageShapeAccurate=3):
        '''
        original list[list]: 2d array containing original table data
        '''
        self.II = ii.IntegralImage(original)
        #on ne pourra decrire un blob que s'il est suffisament grand (10*10 pixels)
        #ration entre la deux intensités de flou

        
        #calcul du nombre de niveau de flou
        max_tau=min(self.II.width,self.II.height)
        S=int(np.log2(max_tau/tau0)/np.log2(ratio))+1
        
        #tailles des flous
        self.tau=[int((tau0*ratio**i)//2*2+1) for i in range(S)]
        #on ne peut pas calculer le flou partout sans sortir de l'image
        self.border=[(self.tau[i]-1)//2 for i in range(S)]
        
        self.stageShapeAccurate=stageShapeAccurate
        #stageShapeAccurate on calcul seulement n pixels sur la taille d'un blob dans son étage précis
        self.stageShape=[(round((self.II.width-self.border[i+1]*2)/self.tau[i]*stageShapeAccurate),round((self.II.height-self.border[i+1]*2)/self.tau[i]*stageShapeAccurate)) for i in range(S-1)]        
        #on supprime les niveaux qui ne sont pas assez grands pour contenir des pixels
        for shape in self.stageShape:
            if shape[0]<1 or shape[1]<1:
                index=self.stageShape.index(shape)
                self.border.pop(index)
                self.tau.pop(index)
                self.stageShape.remove(shape)
                S-=1
         
        #la difference entre deux flous de moyenne met en évidence les blobs qui ont une taille comprise entre la taille des deux flous
        self.blobSize=[(self.tau[i+1]+self.tau[i])//2 for i in range(S-1)]
        
        self.pyramid=[]#pyramide de flous de moyenne
        #chaque niveeau est une image de taille plus petite que l'image d'avant pour 2 raisons :
        #1) il y a des bords qui ne peuvent pas etre calculés
        #2) on reduit la resolution pour gagner du temps de calcul, on peut se le permettre car les blobs sont plus gros
        
        for shape in self.stageShape:
            self.pyramid.append(np.zeros((shape[1],shape[0])))   
            
        print("calcul des flous de moyenne")
        print("taille de l'image : ",self.II.width,"x",self.II.height)
        print("nombre de niveaux de flou : ",S)
        print("taille des flous : ",self.tau)
        print("taille des blobs : ",self.blobSize)
        print("taille des bords : ",self.border)
        print("taille des étages précis : ",self.stageShape)

        
    
    def __repr__(self):
        return '\n'.join([''.join(['{:4}'.format(item) for item in row]) for row in self.pyramid])
    
    def update(self, original):
        '''
        original list[list]: 2d array containing original table data
        '''
        self.II.update(original)

    def interpolation(self,x,y,s):
        #interpolation de la matrice hessienne pour trouver le maximum local 
        #étape 1 : choisir les 8 points autour du maximum local sans dépasser les bords de l'image
        #étape 2 : calculer la matrice A composé des x*x, y*y s*s x*y x y s 1 et Y composé des valeurs des points
        #étape 3 : X=A^-1*Y
        #étape 4 : résoudre le système
        
        
        #avec bordure
        xm=False
        xp=False
        ym=False
        yp=False
        if x-1<self.border[s]:
            xm=True
        if x+1>=self.II.width-self.border[s]:
            xp=True
        if y-1<self.border[s]:
            ym=True
        if y+1>=self.II.height-self.border[s]:
            yp=True
        
        if not(xm) and not(ym) and not(xp) and not(yp):
            XY=[[0,-1],[1,0],[1,1],[-1,1],[-1,0],[0,0]]
        elif xm and not(ym) and not(xp) and not(yp):
            XY=[[0,-1],[1 -1],[1,0],[1,1],[0,1],[0,0]]
        elif not(xm) and xp and not(ym) and not(yp):
            XY=[[-1,-1],[0,-1],[0,0],[0,1],[-1,1],[-1,0]]
        elif not(xm) and not(xp) and ym and not(yp):
            XY=[[-1,0],[-1,1],[0,1],[1,1],[1,0],[0,0]]
        elif not(xm) and not(xp) and not(ym) and yp:
            XY=[[-1,-1],[-1,0],[0,0],[1,0],[1,-1],[0,-1]]
        elif not(xm) and xp and not(ym) and yp:
            XY=[[-2,0],[-1,0],[0,0],[-1,-1],[0,-1],[0,-2]]
        elif xm and not(ym) and not(xp) and yp:
            XY=[[0,0],[1,0],[2,0],[1,-1],[0,-1],[0,-2]]
        elif xm and not(xp) and ym and not(yp):
            XY=[[0,0],[0,1],[0,2],[1,1],[1,0],[2,0]]
        elif not(xm) and xp and ym and not(yp):
            XY=[[-2,0],[-1,0],[0,0],[-1,1],[0,1],[0,2]]
        else:
            return "space bordure error"
        
        p=np.zeros((8,4))
        for i in range(len(XY)):
            p[i]=[XY[i][0],XY[i][1],self.blobSize[s],self.scaleSpace[s][y+XY[i][1]][x+XY[i][0]]]
        
        bsm=(s-1<0)
        bsp=(s+1>=len(self.scaleSpace))
        

        if not(bsm) and not(bsp):
            p[6]=[0,0,self.blobSize[s-1],self.scaleSpace[s-1][y][x]]
            p[7]=[0,0,self.blobSize[s+1],self.scaleSpace[s+1][y][x]]
        elif bsm and not(bsp):
            p[6]=[0,0,self.blobSize[s+1],self.scaleSpace[s+1][y][x]]
            p[7]=[0,0,self.blobSize[s+2],self.scaleSpace[s+2][y][x]]
        elif not(bsm) and bsp:
            p[6]=[0,0,self.blobSize[s-2],self.scaleSpace[s-2][y][x]]
            p[7]=[0,0,self.blobSize[s-1],self.scaleSpace[s-1][y][x]]
        else:
            return "scale bordure error"
            

        
        A=np.zeros((8,8))
        Y=np.zeros((8,1)) 
        for i in range(8):
            A[i]=[p[i][0]**2,p[i][1]**2,p[i][2]**2,p[i][0]*p[i][1],p[i][0],p[i][1],p[i][2],1]
            Y[i]=p[i][3]
            
        try:
            X=np.linalg.inv(A).dot(Y)
        except:
            return "error inversion"
        #as a list

        
        tmp1=np.sqrt(X[0]**2 - 2*X[0]*X[1] + X[1]**2 + X[3]**2)
        tmp2=-X[3]**2 + 4*X[0]*X[1]
        
        a=X[0]/2+X[1]/2+tmp1/2
        b=X[0]/2+X[1]/2-tmp1/2
        c=X[2]
        x0=-(2*X[1]*X[4] - X[3]*X[5])/tmp2
        y0=-(2*X[0]*X[5] - X[3]*X[4])/tmp2
        
        if (a>=0 or b>=0 or c>=0):
            return "pas de maximum local"
        if(abs(x0)>1 or abs(y0)>1):
            return "maxima trop loin"
        
        tau0=-X[6]/(2*X[2])
        
        if (abs(X[3])>1e-6):
            theta=np.arctan((X[1]-X[0]+tmp1)/X[3])
        else:
            theta=0
        
        ct=np.cos(theta)
        st=np.sin(theta)
        z0=p[0][3]-a*(ct*(p[0][0] - x0) + st*(p[0][1] - y0))**2 - b*(ct*(p[0][1] - y0) - st*(p[0][0] - x0))**2 - c*(p[0][3] - tau0)**2
        

        
        homogeneity=max(abs(a/b),abs(b/a))
        space_stability=min(abs(a),abs(b))#closer to 0, less stable
        scale_stability=c#closer to 0, less stable
        
        return ([x+x0,y+y0,tau0,z0],[theta,homogeneity,space_stability,scale_stability,a,b])


    
    def interrestPoint(self, det_threshold=-1, edge_threshold=-1, space_stability_threshold=-1, scale_stability_threshold=0, homogeneity_threshold=0):
        #detection des points d'interret
        #approximation du Dog par la difference de flou de moyenne (permet l'utilisation de filtre boite avec l'image integrale)
        #selection des points d'interret : 
        #l'extremum est-il sur un point de faible déterminant ? si oui, on ne le garde pas
        #l'extremum est-il sur une arête ? si oui, on ne le garde pas
        start=time.time()
        self.complxity=0
        self.maxValue=0
        self.minValue=255
        for s in range(len(self.pyramid)):
            for y in range(len(self.pyramid[s])):
                for x in range(len(self.pyramid[s][0])):
                    #calcul du pixel correspondant sur l'image originale
                    x0=int(x/self.stageShape[s][0]*(self.II.width-self.border[s+1]*2)+self.border[s+1])
                    y0=int(y/self.stageShape[s][1]*(self.II.height-self.border[s+1]*2)+self.border[s+1])
                    self.pyramid[s][y][x]=self.II.avrBlur(x0,y0,self.tau[s+1])/(self.II.avrBlur(x0,y0,self.tau[s])+1)
                    if self.pyramid[s][y][x]>self.maxValue:
                        self.maxValue=self.pyramid[s][y][x]
                    if self.pyramid[s][y][x]<self.minValue:
                        self.minValue=self.pyramid[s][y][x]
                    self.complxity+=1