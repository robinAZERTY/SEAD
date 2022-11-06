
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
            
        print(x,y,s)
        
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
        self.extremum=[]
        self.minimas=[]
        self.maximas=[]
        self.avrMinimasVal=0
        self.avrMaximasVal=0
        
        for stage_index in range(len(self.grid)):
            maxima_candidate=[]
            minima_candidate=[]
            for gridX in self.grid[stage_index][0]:
                for gridY in self.grid[stage_index][1]:
                    #calcul du premier pixel de la grille
                    self.pyramid[stage_index][gridY[0]][gridX[0]]=self.II.avrBlur(gridX[0],gridY[0],self.tau[stage_index])-(self.II.avrBlur(gridX[0],gridY[0],self.tau[stage_index+1]))
                    #qu'on va comparer avec les autres pixels de la grille
                    maxima_grid_candidate=(gridX[0],gridY[0],stage_index,self.pyramid[stage_index][gridY[0]][gridX[0]])
                    minima_grid_candidate=(gridX[0],gridY[0],stage_index,self.pyramid[stage_index][gridY[0]][gridX[0]])
                    
                    for x in range(gridX[0]+1,gridX[1]):
                        for y in range(gridY[0]+1,gridY[1]):
                            self.pyramid[stage_index][y][x]=self.II.avrBlur(x,y,self.tau[stage_index])-(self.II.avrBlur(x,y,self.tau[stage_index+1]))
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
                if isRealMaxima:
                    self.avrMaximasVal+=maxima[3]
                    self.maximas.append(maxima)
                    
             
                                     
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
                if isRealMinima:
                    self.avrMinimasVal+=minima[3]
                    self.minimas.append(minima)  
        self.avrMaximasVal/=len(self.maximas)          
        self.avrMinimasVal/=len(self.minimas) 
             
        print("extrema found in "+str(round(time.time()-start,4))+"s")
        print("len(self.minimas)="+str(len(self.minimas)))
        print("len(self.maximas)="+str(len(self.maximas)))                                 

                            
    def select_interespoints(self, val_threshold=None, edge_threshold=-1, space_stability_threshold=-1, scale_stability_threshold=-1, homogeneity_threshold=-1):
        
        print("selecting interespoints...")
        
        
        new_minimas=[]
        new_maximas=[]
        #premiere etape : seuiller les extrema
        if val_threshold!=None:
            #on ne garde que les extrema dont la valeur est assez eloignee de sa moyenne
            for minima in self.minimas:
                if minima[3]<self.avrMinimasVal-val_threshold:
                    new_minimas.append(minima)
            for maxima in self.maximas:
                if maxima[3]>self.avrMaximasVal+val_threshold:
                    new_maximas.append(maxima)
        else:
            new_minimas=self.minimas
            new_maximas=self.maximas
            
        #deuxieme etape : interpoler les extrema pour avoir une position plus precise et des informations sur leur stabilité
        for maxima in new_maximas:
            if self.stageShape[maxima[2]][0]<3 or self.stageShape[maxima[2]][1]<3:
                continue
            else:
                interp=self.interpolation(maxima[0],maxima[1],maxima[2])
                
        print("len(new_minimas)="+str(len(new_minimas)))
        print("len(new_maximas)="+str(len(new_maximas)))
        print("maxima : ",new_maximas)
        print("minima : ",new_minimas)