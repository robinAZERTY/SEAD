import IntegralImage as ii
import numpy as np

class HessianPyramid(object):
    def __init__(self, original):
        '''
        original list[list]: 2d array containing original table data
        '''
        self.II = ii.IntegralImage(original)
    
    def __repr__(self):
        return '\n'.join([''.join(['{:4}'.format(item) for item in row]) for row in self.pyramid])
    
    def Hessian(self,x,y,size):
        #calcul de la matrice hessienne
        #size est la taille de la fenetre
        #x et y sont les coordonnées du centre de la fenetre
        pass

    
    def interrestPoint(self, det_threshold=0, edge_threshold=0):
        #detection des points d'interret
        #approximation du Dog par la difference de flou de moyenne (permet l'utilisation de filtre boite avec l'image integrale)
        #selection des points d'interret : 
        #l'extremum est-il sur un point de faible déterminant ? si oui, on ne le garde pas
        #l'extremum est-il sur une arête ? si oui, on ne le garde pas
        
        #on ne pourra decrire un blob que s'il est suffisament grand (10*10 pixels)
        tau0 = 9
        
        #ration entre la deux intensités de flou
        ratio=1.6
        
        #calcul du nombre de niveau de flou
        max_tau=min(self.II.width,self.II.height)
        S=int(np.log2(max_tau/tau0)/np.log2(ratio))+1
        
        #tailles des flous
        self.tau=[int((tau0*ratio**i)//2*2+1) for i in range(S)]
        
        #la difference entre deux flous de moyenne met en évidence les blobs qui ont une taille comprise entre la taille des deux flous
        self.blobSize=[(self.tau[i+1]+self.tau[i])//2 for i in range(S-1)]
        
        #on ne peut pas calculer le flou partout sans sortir de l'image
        border=[(self.tau[i]-1)//2 for i in range(S)]
        
        self.scaleSpace=[[[0 for _ in range(self.II.width)] for _ in range(self.II.height)] for _ in range(S-1)]
        
        print("calcul des flous")
        print("taille de l'image : ",self.II.width,"x",self.II.height)
        print("nombre de niveau de flou : ",S)
        print("tailles des flous : ",self.tau)
        print("tailles des blobs : ",self.blobSize)
        print("bordure : ",border)
        
        extremum=[]
        self.maxe=0
        avr=0
        for s in range(S-1):
            #pour chaque niveau, on sépare le calcul en plusieurs grilles, et on retient les extremums
            grid_size=self.tau[s+1]//4*2+1
            gridX=[i*grid_size for i in range(self.II.width//grid_size+1)]
            gridY=[i*grid_size for i in range(self.II.height//grid_size+1)]
            
            for x0 in gridX:
                for y0 in gridY:
                    #pour chaque grille
                    extremum_=(0,0,0,0)
                    for x in range(x0,x0+grid_size):
                        for y in range(y0,y0+grid_size):
                            
                            #on verifie qu'on ne va pas sortir de l'image
                            if x<border[s+1] or y<border[s+1] or x>=self.II.width-border[s+1] or y>=self.II.height-border[s+1]:
                                continue
                            

                            newVal=(self.II.avrBlur(x,y,self.tau[s+1]))/(self.II.avrBlur(x,y,self.tau[s])+1)
                            newVal=abs(newVal)
                            self.scaleSpace[s][y][x]=newVal
                            if newVal>extremum_[3]:
                                extremum_=(x,y,s,newVal)
                    
                    extremum.append(extremum_)
                    avr+=extremum[-1][3]
                    if extremum[-1][3]>self.maxe:
                        self.maxe=extremum[-1][3]
                    
        self.interrestPoint=[]
        avr/=len(extremum)
        #on ne garde que les points d'interret tres differents de la moyenne
        for e in extremum:
            if e[3]>avr*det_threshold:
                
                try:
                    if edge_threshold>0:
                        #sur une arête, la dérivée seconde dominante est vraiment plus grande que dans la direction perpendiculaire
                        h=(self.tau[e[2]+1]-self.tau[e[2]])//4+1
                        ddx=self.scaleSpace[e[2]][e[1]][e[0]+h]-2*self.scaleSpace[e[2]][e[1]][e[0]]+self.scaleSpace[e[2]][e[1]][e[0]-h]
                        ddy=self.scaleSpace[e[2]][e[1]+h][e[0]]-2*self.scaleSpace[e[2]][e[1]][e[0]]+self.scaleSpace[e[2]][e[1]-h][e[0]]
                        
                        dx=self.scaleSpace[e[2]][e[1]][e[0]+1]-self.scaleSpace[e[2]][e[1]][e[0]-1]
                        dy=self.scaleSpace[e[2]][e[1]+1][e[0]]-self.scaleSpace[e[2]][e[1]-1][e[0]]
                        
                        #dans la direction perpendiculaire
                        x1=-ddy
                        y1=ddx
                        norm=np.sqrt(x1**2+y1**2)
                        x1*=h/norm
                        y1*=h/norm
                        x1=round(x1)
                        y1=round(y1)
                        dd=self.scaleSpace[e[2]][e[1]+y1][e[0]+x1]-2*self.scaleSpace[e[2]][e[1]][e[0]]+self.scaleSpace[e[2]][e[1]-y1][e[0]-x1]
                        #print("extremum : ",e[0],e[1],e[2],e[3],ddx,ddy,dd)
                        #input()
                        normal=ddx**2+ddy**2
                        tangent=dd**2
                        if abs((tangent-normal)/normal)<edge_threshold and dx**2+dy**2<2:
                            new_ip=(e[0],e[1],e[2],e[3])
                            self.interrestPoint.append(new_ip)
                    else:
                        self.interrestPoint.append(e)
                except:
                    print("erreur : ",e[0],e[1],e[2],e[3])
        
        new_interrestPoint=[]        
        for ip in self.interrestPoint:
            #interpolation pour trouver la taille du blob
            if ip[2]>0 and ip[2]<S-2:
                x1,x2,x3=self.blobSize[ip[2]-1],self.blobSize[ip[2]],self.blobSize[ip[2]+1]
                y1,y2,y3=self.scaleSpace[ip[2]-1][ip[1]][ip[0]],self.scaleSpace[ip[2]][ip[1]][ip[0]],self.scaleSpace[ip[2]+1][ip[1]][ip[0]]
            elif ip[2]==0:
                x1,x2,x3=self.blobSize[0],self.blobSize[1],self.blobSize[2]
                y1,y2,y3=self.scaleSpace[0][ip[1]][ip[0]],self.scaleSpace[1][ip[1]][ip[0]],self.scaleSpace[2][ip[1]][ip[0]]
            else:
                x1,x2,x3=self.blobSize[S-3],self.blobSize[S-2],self.blobSize[S-1]
                y1,y2,y3=self.scaleSpace[S-3][ip[1]][ip[0]],self.scaleSpace[S-2][ip[1]][ip[0]],self.scaleSpace[S-1][ip[1]][ip[0]]
            #find a,b,c such that y=ax^2+bx+c with X=A^-1Y
            A=np.array([[x1**2,x1,1],[x2**2,x2,1],[x3**2,x3,1]])
            Y=np.array([y1,y2,y3])
            X=np.linalg.inv(A)@Y
            a,b,c=X
            
            #find the extremum of the parabola
            x=-b/(2*a)
            y=a*x**2+b*x+c
            
            new_interrestPoint.append((ip[0],ip[1],x,y))
        
        self.interrestPoint=new_interrestPoint
        