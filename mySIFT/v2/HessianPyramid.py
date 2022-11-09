
import IntegralImage as ii
import numpy as np
import cv2
import time

class HessianPyramid(object):
    def __init__(self, original, tau0=11,ratio=1.6, stageShapeAccurate=3):
        '''
        original list[list]: 2d array containing original table data
        '''
        strart = time.time()
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
        self.stageShape=[[round((self.II.width-self.border[i+1]*2)/self.tau[i]*stageShapeAccurate),round((self.II.height-self.border[i+1]*2)/self.tau[i]*stageShapeAccurate)] for i in range(S-1)]        
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
            
        #pour chaque niveau, on va séparer le calcul en plusieurs sections afin d'y extraire leur extrema, qui seront des candidats pour les points d'interret   
        #les grilles on une taille de stageShapeAccurate pixels
        self.grid=[] #[[[[x1,x2],[x3,x4],...],[[y1,y2],[y3,y4],...]],...]
        for shape in self.stageShape:
            stageGridX=[]
            stageGridY=[]
            for i in range(shape[0]//stageShapeAccurate):
                a=i*stageShapeAccurate
                b=(i+1)*(stageShapeAccurate)-1
                stageGridX.append([a,b])
                
            if len(stageGridX)>0:
                if stageGridX[-1][1]<shape[0]-1:
                    stageGridX.append([stageGridX[-1][1]+1,shape[0]-1])
                elif stageGridX[-1][1]>shape[0]-1 and stageGridX[-1][0]<shape[0]-1:
                    stageGridX[-1][1]=shape[0]-1
                elif stageGridX[-1][0]>shape[0]-1 and stageGridX[-1][1]>shape[0]-1:
                    stageGridX.pop()
            else:
                stageGridX.append([0,shape[0]-1])
            
            
            for i in range(shape[1]//stageShapeAccurate):
                a=i*stageShapeAccurate
                b=(i+1)*(stageShapeAccurate)-1
                stageGridY.append([a,b])
            
            
            if len(stageGridY)>0:
                if stageGridY[-1][1]<shape[1]-1:
                    stageGridY.append([stageGridY[-1][1]+1,shape[1]-1])
                elif stageGridY[-1][1]>shape[1]-1 and stageGridY[-1][0]<shape[1]-1:
                    stageGridY[-1][1]=shape[1]-1
                elif stageGridY[-1][0]>shape[1]-1 and stageGridY[-1][1]>shape[1]-1:
                    stageGridY.pop()
            else:
                stageGridY.append([0,shape[1]-1])


            
            
            stageGrid=[stageGridX,stageGridY]
            self.grid.append(stageGrid)
                
            self.scaleFactor=[[(self.II.width-2*self.border[s+1])/self.stageShape[s][0],(self.II.height-2*self.border[s+1])/self.stageShape[s][1]] for s in range(S-1)]
                
        print('HessianPyramid init time :',round(time.time()-strart,4),'s')
        print("calcul des flous de moyenne")
        print("taille de l'image : ",self.II.width,"x",self.II.height)
        print("nombre de niveaux de flou : ",S)
        print("taille des flous : ",self.tau)
        print("taille des blobs : ",self.blobSize)
        print("taille des bords : ",self.border)
        print("taille des étages précis : ",self.stageShape)
        #print("taille des grilles : ",self.grid)

        
    
    def __repr__(self):
        return '\n'.join([''.join(['{:4}'.format(item) for item in row]) for row in self.pyramid])
    
    def update(self, original):
        '''
        original list[list]: 2d array containing original table data
        '''
        self.II.update(original)

    def isInPyramid(self,x,y,s):
        #on verifie que le point est dans la pyramide
        return (x>=0 and x<=self.stageShape[s][0]-1 and y>=0 and y<=self.stageShape[s][1]-1)
    
    def pyramid_Bilineaire_Interpolation(self,x,y,s):
        #donne la valeur de la pyramide à l'interieur d'un pixel ou même entre quatre pixels par interpolation bilineaire
        #on assume que (x,y et s) est dans la pyramide
        a=x%1
        b=y%1
        c=1-a
        d=1-b
        x1=int(x)
        x2=x1+1
        y1=int(y)
        y2=y1+1
        #on verifie que les points sont dans la pyramide
        if self.isInPyramid(x1,y1,s) and self.isInPyramid(x2,y1,s) and self.isInPyramid(x1,y2,s) and self.isInPyramid(x2,y2,s):
            return c*d*self.pyramid[s][y1][x1]+a*d*self.pyramid[s][y1][x2]+c*b*self.pyramid[s][y2][x1]+a*b*self.pyramid[s][y2][x2]
        else:
            return 0

    def space_scale_interpolation(self,x,y,s):
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
        if x-1<0:
            xm=True
        if x+1>=self.stageShape[s][0]:
            xp=True
        if y-1<0:
            ym=True
        if y+1>=self.stageShape[s][1]:
            yp=True
        
        if not(xm) and not(ym) and not(xp) and not(yp):
            XY=[[0,-1],[1,0],[1,1],[-1,1],[-1,0],[0,0]]
        elif xm and not(ym) and not(xp) and not(yp):
            XY=[[0,-1],[1,-1],[1,0],[1,1],[0,1],[0,0]]
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
            p[i]=[XY[i][0],XY[i][1],self.blobSize[s],self.pyramid[s][y+XY[i][1]][x+XY[i][0]]]
            
        #print(x,y,s)
        
        bsm=(s-1<0)
        bsp=(s+1>=len(self.pyramid))
        

        if not(bsm) and not(bsp):
            #wich pixel is the best at the next and previous level
            pixelSize=self.blobSize[s]/self.stageShapeAccurate #at this stage, the size of one pixel is equivalent to that in the initial image
            p[6]=[0,0,self.blobSize[s-1],self.pyramid[s-1][y][x]]
            p[7]=[0,0,self.blobSize[s+1],self.pyramid[s+1][y][x]]
        elif bsm and not(bsp):
            p[6]=[0,0,self.blobSize[s+1],self.pyramid[s+1][y][x]]
            p[7]=[0,0,self.blobSize[s+2],self.pyramid[s+2][y][x]]
        elif not(bsm) and bsp:
            p[6]=[0,0,self.blobSize[s-2],self.pyramid[s-2][y][x]]
            p[7]=[0,0,self.blobSize[s-1],self.pyramid[s-1][y][x]]
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

    def space_interpolation(self,x,y,s):
        #interpolation de la matrice hessienne pour trouver le maximum local 
        #étape 1 : choisir les 6 points autour du maximum local sans dépasser les bords de l'image
        #étape 2 : calculer la matrice A composé des x*x, y*y, x*y, x, y, 1 et Y composé des valeurs des points
        #étape 3 : X=A^-1*Y
        #étape 4 : résoudre le système
        xm=False
        xp=False
        ym=False
        yp=False
        if x-1<0:
            xm=True
        if x+1>=self.stageShape[s][0]:
            xp=True
        if y-1<0:
            ym=True
        if y+1>=self.stageShape[s][1]:
            yp=True
        
        if not(xm) and not(ym) and not(xp) and not(yp):
            XY=[[0,-1],[1,0],[1,1],[-1,1],[-1,0],[0,0]]
        elif xm and not(ym) and not(xp) and not(yp):
            XY=[[0,-1],[1,-1],[1,0],[1,1],[0,1],[0,0]]
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

        p=np.zeros((6,3))
        for i in range(len(XY)):
            p[i]=[XY[i][0],XY[i][1],self.pyramid[s][y+XY[i][1]][x+XY[i][0]]]

        A=np.zeros((6,6))
        Y=np.zeros((6,1)) 
        for i in range(6):
            A[i]=[XY[i][0]**2,XY[i][1]**2,XY[i][0]*XY[i][1],XY[i][0],XY[i][1],1]
            Y[i]=self.pyramid[s][y+XY[i][1]][x+XY[i][0]]
            
        try:
            X=np.linalg.inv(A).dot(Y)
        except:
            return "error inversion"
    
        #make shure all the values are numbers
        for i in range(6):
            if not(np.isfinite(X[i])):
                return "error inversion"
        
        tmp1=X[0]**2 - 2*X[0]*X[1] + X[1]**2 + X[3]**2
        if (tmp1<0):
            return "error inversion"
        tmp1=np.sqrt(tmp1)
        tmp2=-X[2]**2 + 4*X[0]*X[1]
        if (abs(tmp2)<1e-6 or not(np.isfinite(tmp2))):
            return "error inversion"
        
        a=float((X[0]+X[1]+tmp1)/2)
        b=float((X[0]+X[1]-tmp1)/2)
        x0=float((X[2]*X[4]-2*X[1]*X[3])/tmp2)
        y0=float((X[2]*X[3]-2*X[0]*X[4])/tmp2)
        

        if(abs(x0)>1 or abs(y0)>1):
            #sommet trop loin
            #return self.space_interpolation(round(x+x0),round(y+y0),s)
            return "maxima trop loin"
        
        
        if (abs(X[2])>1e-6):
            theta=float(np.arctan((X[1]-X[0]+tmp1)/X[2]))
        else:
            theta=0
        
        ct=float(np.cos(theta))
        st=float(np.sin(theta))
        z0=self.pyramid[s][y][x]-a*(ct*x0 + st*y0)**2 - b*(st*x0 - ct*y0)**2
        
        return [[x+x0,y+y0,s,z0],[theta,a,b]]
    
    def size_interpolation(self,extremum):
        x,y,s,z=extremum
        if not(self.isInPyramid(x,y,s)):
                return "erreur coordonnées"
        #on trouve les coordonnées dans l'image originale
        originalX,originalY=self.get_original_coordonate(x,y,s)
        
        #z=a*(x-x0)**2+c=(a)*x**2 + (-2*x0*a)*x + (x0**2*a+c) = A*x**2 + B*x + C
        
        """
        A=a
        B=-2*x0*a
        C=x0**2*a+c
        
        x0=-B/(2*a)
        c=C-x0**2*a
        """
        
        Y=np.zeros((3,1))
        A=np.zeros((3,3))
        p=np.zeros((3,2))
        #on test aussi ses 2 voisins au dessus et en dessous s'ils existent
        
        
        if s>0 and s<len(self.pyramid)-1:
            #coordonnées du pixel au dessus
            xm,ym=self.get_pyramid_coordonate(originalX,originalY,s-1)
            if not(self.isInPyramid(xm,ym,s-1)):
                return "erreur coordonnées"
            p[0]=[self.blobSize[s-1],self.pyramid_Bilineaire_Interpolation(xm,ym,s-1)]
            p[1]=[self.blobSize[s],z]
            xm,ym=self.get_pyramid_coordonate(originalX,originalY,s+1)

            if not(self.isInPyramid(xm,ym,s+1)):
                return "erreur coordonnées"
            p[2]=[self.blobSize[s+1],self.pyramid_Bilineaire_Interpolation(xm,ym,s+1)]
        elif s==0 and s+2<len(self.pyramid):
            p[0]=[self.blobSize[0],z]
            xm,ym=self.get_pyramid_coordonate(originalX,originalY,1)          
            #toujours dans l'image ?
            if not(self.isInPyramid(xm,ym,1)):
                return "erreur coordonnées"
            p[1]=[self.blobSize[1],self.pyramid_Bilineaire_Interpolation(xm,ym,1)]
            xm,ym=self.get_pyramid_coordonate(originalX,originalY,2)
            if not(self.isInPyramid(xm,ym,2)):
                return "erreur coordonnées"
            p[2]=[self.blobSize[2],self.pyramid_Bilineaire_Interpolation(xm,ym,2)]
        elif s==len(self.pyramid)-1 and s-2>=0:
            p[2]=[self.blobSize[s],z]
            xm,ym=self.get_pyramid_coordonate(originalX,originalY,s-1)
            if not(self.isInPyramid(xm,ym,s-1)):
                return "erreur coordonnées"
            p[1]=[self.blobSize[s-1],self.pyramid_Bilineaire_Interpolation(xm,ym,s-1)]
            xm,ym=self.get_pyramid_coordonate(originalX,originalY,s-2)
            if not(self.isInPyramid(xm,ym,s-2)):
                return "erreur coordonnées"
            p[0]=[self.blobSize[s-2],self.pyramid_Bilineaire_Interpolation(xm,ym,s-2)]
        else:
            return "error size interpolation"
        
        for i in range(3):
            A[i]=[p[i][0]**2,p[i][0],1]
            Y[i]=p[i][1]
        #print(extremum)
        #print(p)
        #input("")
        
        try :
            X=np.linalg.inv(A).dot(Y)
        except:
            return "error inversion"
        
        a=float(X[0])
        x0=-float(X[1])/(2*a)
        c=float(X[2])-a*x0**2
        
        if s>0 and s<len(self.pyramid)-1:
            if x0<self.blobSize[s-1] or x0>self.blobSize[s+1]:
                return "maxima trop loin"
        elif s==0 and s+2<len(self.pyramid):
            if x0<self.blobSize[0] or x0>self.blobSize[2]:
                return "maxima trop loin"
        elif s==len(self.pyramid)-1 and s-2>=0:
            if x0<self.blobSize[s-2] or x0>self.blobSize[s]:
                return "maxima trop loin"

            

        
        return [[originalX,originalY,x0,c],[float(a)]]  

        
        
        
        
    def get_original_coordonate(self,x,y,s):
        #but : convertir les coordonnées du point dans le repère de la pyramide vers le repère de l'image d'origine
        #entrée : x,y,s coordonnées du point dans le repère de la pyramide
        #sortie : x,y coordonnées du point dans le repère de l'image d'origine
        

        originalX=x*self.scaleFactor[s][0]+self.border[s+1]
        originalY=y*self.scaleFactor[s][1]+self.border[s+1]
        return (originalX,originalY)

    
    def get_pyramid_coordonate(self,x,y,s):
        #but : convertir les coordonnées du point dans le repère de l'image d'origine vers le repère de la pyramide
        #entrée : x,y,s coordonnées du point dans le repère de l'image d'origine
        #sortie : x,y coordonnées du point dans le repère de la pyramide
        

        pyramidX=(x-self.border[s+1])/self.scaleFactor[s][0]
        pyramidY=(y-self.border[s+1])/self.scaleFactor[s][1]
        return (pyramidX,pyramidY)
    
    def interrestPoint(self):
        #detection des points d'interret
        #approximation du Dog par la difference de flou de moyenne (permet l'utilisation de filtre boite avec l'image integrale)
        #selection des points d'interret : 
        #l'extremum est-il sur un point de faible déterminant ? si oui, on ne le garde pas
        #l'extremum est-il sur une arête ? si oui, on ne le garde pas
        start=time.time()
        self.complxity=0
        self.maxValue=0
        self.minValue=255
        """
        for s in range(len(self.pyramid)):
            for y in range(len(self.pyramid[s])):
                for x in range(len(self.pyramid[s][0])):
                    #calcul du pixel correspondant sur l'image originale
                    x0=int(x/self.stageShape[s][0]*(self.II.width-self.border[s+1]*2)+self.border[s+1])
                    y0=int(y/self.stageShape[s][1]*(self.II.height-self.border[s+1]*2)+self.border[s+1])
                    
                    self.pyramid[s][y][x]=self.II.avrBlur(x0,y0,self.tau[s+1])-(self.II.avrBlur(x0,y0,self.tau[s]))
                    if self.pyramid[s][y][x]>self.maxValue:
                        self.maxValue=self.pyramid[s][y][x]
                    if self.pyramid[s][y][x]<self.minValue:
                        self.minValue=self.pyramid[s][y][x]
                    self.complxity+=1
        """
        self.minimas=[]
        self.maximas=[]
        self.minValMinima=None
        self.maxValMinima=None
        self.minValMaxima=None
        self.maxValMaxima=None
        self.avrMinimasVal=0
        self.avrMaximasVal=0
        self.minval=0
        self.maxval=0
        
        for stage_index in range(len(self.grid)):
            maxima_candidate=[]
            minima_candidate=[]
            for gridX in self.grid[stage_index][0]:
                for gridY in self.grid[stage_index][1]:
                    for x in range(gridX[0],gridX[1]+1):
                        for y in range(gridY[0],gridY[1]+1):
                            #calcul du pixel correspondant sur l'image originale
                            xx,yy=self.get_original_coordonate(x,y,stage_index)
                            xx=round(xx)
                            yy=round(yy)
                            
                            self.pyramid[stage_index][y][x]=self.II.avrBlur(xx,yy,self.tau[stage_index])-(self.II.avrBlur(xx,yy,self.tau[stage_index+1]))
                            if self.pyramid[stage_index][y][x]>self.maxval:
                                self.maxval=self.pyramid[stage_index][y][x]
                            if self.pyramid[stage_index][y][x]<self.minval:
                                self.minval=self.pyramid[stage_index][y][x]
                            if x==gridX[0] and y==gridY[0]:
                                #calcul du premier pixel de la grille
                                #qu'on va comparer avec les autres pixels de la grille
                                maxima_grid_candidate=(x,y,stage_index,self.pyramid[stage_index][y][x])
                                minima_grid_candidate=(x,y,stage_index,self.pyramid[stage_index][y][x])
                            if self.pyramid[stage_index][y][x]>maxima_grid_candidate[3]:
                                maxima_grid_candidate=(x,y,stage_index,self.pyramid[stage_index][y][x])
                            if self.pyramid[stage_index][y][x]<minima_grid_candidate[3]:
                                minima_grid_candidate=(x,y,stage_index,self.pyramid[stage_index][y][x])
                           
        
                    
                    maxima_candidate.append(maxima_grid_candidate)
                    minima_candidate.append(minima_grid_candidate)
                   
                    

            for maxima in maxima_candidate:
                #test s'il est plus grand que ses voisins
                isRealMaxima=True
                for x in range(maxima[0]-1,maxima[0]+2):
                    for y in range(maxima[1]-1,maxima[1]+2):
                        #ne pas sortir de l'image
                        if x<0 or x>=self.stageShape[stage_index][0] or y<0 or y>=self.stageShape[stage_index][1]:
                            continue
                        if self.pyramid[stage_index][y][x]>maxima[3]:
                            isRealMaxima=False
                            break
                    if not(isRealMaxima):
                        break
                #on test aussi ses 2 voisins au dessus et en dessous s'ils existent
                originalX,originalY=self.get_original_coordonate(maxima[0],maxima[1],stage_index)
                
                if stage_index>0:
                    #coordonnées du pixel au dessus
                    xm,ym=self.get_pyramid_coordonate(originalX,originalY,stage_index-1)
                    #on test si le pixel existe à cette échelle
                    if self.isInPyramid(xm,ym,stage_index-1):
                        if self.pyramid_Bilineaire_Interpolation(xm,ym,stage_index-1)>maxima[3]:
                            isRealMaxima=False
                if stage_index<len(self.pyramid)-1:
                    #coordonnées du pixel en dessous
                    xm,ym=self.get_pyramid_coordonate(originalX,originalY,stage_index+1)
                    #on test si le pixel existe à cette échelle
                    if self.isInPyramid(xm,ym,stage_index+1):
                        if self.pyramid_Bilineaire_Interpolation(xm,ym,stage_index+1)>maxima[3]:
                            isRealMaxima=False
                    
                if isRealMaxima:
                    self.avrMaximasVal+=maxima[3]
                    self.maximas.append(maxima)
                   
                    if self.minValMaxima==None:
                        self.minValMaxima=maxima[3]
                    elif maxima[3]<self.minValMaxima:
                        self.minValMaxima=maxima[3]
                    if self.maxValMaxima==None:
                        self.maxValMaxima=maxima[3]
                    elif maxima[3]>self.maxValMaxima:
                        self.maxValMaxima=maxima[3]
                    
             
                                     
            for minima in minima_candidate:
                #test s'il est plus petit que ses voisins
                isRealMinima=True
                for x in range(minima[0]-1,minima[0]+2):
                    for y in range(minima[1]-1,minima[1]+2):
                        #ne pas sortir de l'image
                        if x<0 or x>=self.stageShape[stage_index][0] or y<0 or y>=self.stageShape[stage_index][1]:
                            continue
                        if self.pyramid[stage_index][y][x]<minima[3]:
                            isRealMinima=False
                            break
                    if not(isRealMinima):
                        break
                
                #on test aussi ses 2 voisins au dessus et en dessous s'ils existent
                originalX,originalY=self.get_original_coordonate(maxima[0],maxima[1],stage_index)
                if stage_index>0:
                    #coordonnées du pixel au dessus
                    xm,ym=self.get_pyramid_coordonate(originalX,originalY,stage_index-1)
                    #on test si le pixel existe à cette échelle
                    if self.isInPyramid(xm,ym,stage_index-1):
                        if self.pyramid_Bilineaire_Interpolation(xm,ym,stage_index-1)>maxima[3]:
                            isRealMaxima=False
                if stage_index<len(self.pyramid)-1:
                    #coordonnées du pixel en dessous
                    xm,ym=self.get_pyramid_coordonate(originalX,originalY,stage_index+1)
                    #on test si le pixel existe à cette échelle
                    if self.isInPyramid(xm,ym,stage_index+1):
                        if self.pyramid_Bilineaire_Interpolation(xm,ym,stage_index+1)>maxima[3]:
                            isRealMaxima=False
                        
                if isRealMinima:
                    self.avrMinimasVal+=minima[3]
                    self.minimas.append(minima)
                    if self.minValMinima==None:
                        self.minValMinima=minima[3]
                    elif minima[3]<self.minValMinima:
                        self.minValMinima=minima[3]
                    if self.maxValMinima==None:
                        self.maxValMinima=minima[3]
                    elif minima[3]>self.maxValMinima:
                        self.maxValMinima=minima[3]
                        
        self.avrMaximasVal/=len(self.maximas)          
        self.avrMinimasVal/=len(self.minimas) 
             
        #print("extrema found in "+str(round(time.time()-start,4))+"s")
        #print("len(self.minimas)="+str(len(self.minimas)))
        #print("len(self.maximas)="+str(len(self.maximas)))                                 

                            
    def select_interespoints(self, val_threshold=None, stability_threshold=None, edge_threshold=None):
        
        #print("selecting interespoints...")
        start=time.time()
        
        
        #premiere etape : seuiller les extrema
        if val_threshold!=None:
            new_minimas=[]
            new_maximas=[]
            #on ne garde que les extrema dont la valeur est comprise dans la bonne tranche
            minima_threshold=(self.minValMinima-self.maxValMinima)*val_threshold+self.maxValMinima
            for minima in self.minimas:
                if minima[3]<=minima_threshold:
                    new_minimas.append(minima)
                    
            maximas_threshold=(self.maxValMaxima-self.minValMaxima)*val_threshold+self.minValMaxima
            for maxima in self.maximas:
                if maxima[3]>=maximas_threshold:
                    new_maximas.append(maxima)
            self.maximas=new_maximas
            self.minimas=new_minimas
        
        if stability_threshold!=None and edge_threshold!=None:
            new_minimas=[]
            new_maximas=[] 
            #deuxieme etape : interpoler les extrema pour avoir une position plus precise et des informations sur leur stabilité
            for maxima in self.maximas:
                if self.stageShape[maxima[2]][0]<3 or self.stageShape[maxima[2]][1]<3:
                    continue
                else:
                    interp=self.space_interpolation(maxima[0],maxima[1],maxima[2])

                    if type(interp)==str:
                        continue
                    
                    coord=interp[0]
                    info=interp[1]
                    theta,a,b=(info[0],info[1],info[2])
                    stability=min(abs(a),abs(b))
                    if abs(a) > 1e-6 and abs(b) > 1e-6:
                        edge=min(abs(a)/abs(b),abs(b)/abs(a))
                    else:
                        edge=0
                    if stability>stability_threshold and edge>edge_threshold:
                        new_maximas.append(coord)
                        
            self.maximas=new_maximas
            for minima in self.minimas:
                if self.stageShape[minima[2]][0]<3 or self.stageShape[minima[2]][1]<3:
                    continue
                else:
                    interp=self.space_interpolation(minima[0],minima[1],minima[2])
                    if type(interp)==str:
                        continue
                    coord=interp[0]
                    info=interp[1]
                    theta,a,b=(info[0],info[1],info[2])
                    stability=min(abs(a),abs(b))
                    if abs(a) > 1e-6 and abs(b) > 1e-6:
                        edge=min(abs(a)/abs(b),abs(b)/abs(a))
                    else:
                        edge=0
                    if stability>stability_threshold and edge>edge_threshold:
                        new_minimas.append(coord)
                        
        
            self.minimas=new_minimas
        
        self.interespoints=[]
        for maxima in self.maximas:
            ip=self.size_interpolation(maxima)
            if type(ip)==str:
                #print(maxima,ip)
                continue
            self.interespoints.append(ip[0])
            #print(maxima,self.blobSize[maxima[2]],"->",ip[0])
        for minima in self.minimas:
            ip=self.size_interpolation(minima)
            if type(ip)==str:
                #print(minima,ip)
                continue
            self.interespoints.append(ip[0])
            #print(minima,self.blobSize[minima[2]],"->",ip[0])
            
                          
        #print("interespoints selected in "+str(round(time.time()-start,4))+"s")      
        #print("len(new_minimas)="+str(len(new_minimas)))
        #print("len(new_maximas)="+str(len(new_maximas)))
        
    def draw_interespoints(self, image, color=(255,0,0)):
        #image can be a resized version of the original image
        scaleFactorX=len(image[0])/self.II.width
        scaleFactorY=len(image)/self.II.height
        for ip in self.interespoints:
            try:
                x=round(ip[0]*scaleFactorX)
                y=round(ip[1]*scaleFactorY)
                l=round(ip[2]/2*scaleFactorX)
                L=round(ip[2]/2*scaleFactorY)
                startPoint=(x-l,y-L)
                endPoint=(x+l,y+L)
                cv2.rectangle(image,startPoint,endPoint,color,1)
            except:
                continue