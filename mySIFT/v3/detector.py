import LaplacianPyramid as lp
import cv2
import numpy as np
class detector(object):
    def __init__(self,img,boxSizeRatio=1.4,boxSize0=9):
        self.maximas=[]
        self.minimas=[]
        self.keypoints=[]
        self.image=img
        self.LP = lp.LaplacianPyramid(img,boxSizeRatio,boxSize0)
        self.LP.buid()
        
        #cas où les bordures ne gênent pas
        self.__direct_neighbours = []
        self.__direct_neighbours.append([[-1,-1],[-1,0],[-1,1],[0,-1],[0,1],[1,-1],[1,0],[1,1]])
        for i in range(1,16):
            #cas où les bordures gênent
            #il y a 16 cas différents en comptant le cas où les bordures ne gênent pas
            #le mot binair (bxp,bxm,byp,bym)_2 donne le numéro du cas en base 10
            # ex : si i=5 alors bxp=0,bxm=1,byp=0,bym=1
            
            #conversion en binaire
            (bxp,bxm,byp,bym) = (i>>3&1,i>>2&1,i>>1&1,i&1)
            
            selected_neighbours=[]
            for neighbour in self.__direct_neighbours[0]:
                x,y = (neighbour[0],neighbour[1])
                if not(bxp and x>0) and not(bxm and x<0) and not(byp and y>0) and not(bym and y<0):
                    selected_neighbours.append(neighbour)
            
            self.__direct_neighbours.append(selected_neighbours)
                
        
    def update(self,img):
        self.keypoints=[]
        self.LP.update(img)
        self.image=img
        self.LP.buid()
        
    def __select_below(self,minimas,limit):
        selected=[]
        for minima in minimas:
            if minima[3]<=limit:
                selected.append(minima)
        return selected
    
    def __select_above(self,maximas,limit):
        selected=[]
        for maxima in maximas:
            if maxima[3]>=limit:
                selected.append(maxima)
        return selected
    
    def __nearest_neighbour(self,point):
        #return the coordinates of the nearest neighbour of the point (px,py) in the laplacian pyramid without go out of the image
        #we choose the neighbors so that they are distributed as equitably as possible
        px,py,level,_ = point
        neighbours=[point]
    
    def __border_test(self,point):
        #return (bxp,bxm,byp,bym) convert in base 10
        bxm=(point[0]<1)
        bxp=(point[0]>self.LP.levelInfo[point[2]][0]-2)
        bym=(point[1]<1)
        byp=(point[1]>self.LP.levelInfo[point[2]][1]-2)
        
        return (bxp<<3)+(bxm<<2)+(byp<<1)+bym
    
    def __get_below_and_above_neighbour(self,point):
        
        originalX,originalY = self.LP.get_original_coordonate(point[0],point[1],point[2])
        bsm=(point[2]<1)
        bsp=(point[2]>self.LP.NLevel-2)
        
        
        if bsp:
            above_neighbour = None
        else:
            #au dessus 
            x,y=self.LP.get_pyramid_coordonate(originalX,originalY,point[2]+1)
            x,y=(round(x),round(y))
            if x>=0 and x<self.LP.levelInfo[point[2]+1][0] and y>=0 and y<self.LP.levelInfo[point[2]+1][1]:
                above_neighbour = [x,y,point[2]+1,self.LP.table[point[2]+1][y][x]>point[3]]
            else:
                above_neighbour = None
        
        if bsm:
            below_neighbour = None
        else:
            #en dessous
            x,y=self.LP.get_pyramid_coordonate(originalX,originalY,point[2]-1)
            x,y=(round(x),round(y))
            if x>=0 and x<self.LP.levelInfo[point[2]-1][0] and y>=0 and y<self.LP.levelInfo[point[2]-1][1]:
                below_neighbour = [x,y,point[2]-1,self.LP.table[point[2]-1][y][x]>point[3]]
            else:
                below_neighbour = None
         
        
        return [below_neighbour,above_neighbour]
        #return [None,None]
    
    def __extract_maxima(self):
        self.maximas=[]

        self.maxima_dynamic=[self.LP.maxima_dynamic[1],self.LP.maxima_dynamic[0]]
        
        for potential_maxima in self.LP.maxima_candidates:
            neighbour_config = self.__border_test(potential_maxima)
            isRealMaxima=True
            for neighbour in self.__direct_neighbours[neighbour_config]:
                x,y = (potential_maxima[0]+neighbour[0],potential_maxima[1]+neighbour[1])
                if self.LP.table[potential_maxima[2]][y][x]>potential_maxima[3]:
                    isRealMaxima=False
                    break
            
            neighbour=self.__get_below_and_above_neighbour(potential_maxima)
            for nei in neighbour:
                if nei is not None:
                    if nei[3]>=potential_maxima[3]:
                        isRealMaxima=False
                        break
            
            if isRealMaxima:
                self.maximas.append([potential_maxima[0],potential_maxima[1],potential_maxima[2],potential_maxima[3],neighbour_config])
                self.LP.update_extrem_value(potential_maxima[3],self.maxima_dynamic)
                
    def __extract_minima(self):
        
        self.minimas=[]
        
        self.minima_dynamic=[self.LP.minima_dynamic[1],self.LP.minima_dynamic[0]]
        
        for potential_minima in self.LP.minima_candidates:
            neighbour_config = self.__border_test(potential_minima)
            isRealMinima=True
            for neighbour in self.__direct_neighbours[neighbour_config]:
                x,y = (potential_minima[0]+neighbour[0],potential_minima[1]+neighbour[1])
                if self.LP.table[potential_minima[2]][y][x]<potential_minima[3]:
                    isRealMinima=False
                    break
            
            neighbour=self.__get_below_and_above_neighbour(potential_minima)
            for nei in neighbour:
                if nei is not None:
                    if nei[3]<=potential_minima[3]:
                        isRealMinima=False
                        break
                        
            if isRealMinima:
                self.minimas.append([potential_minima[0],potential_minima[1],potential_minima[2],potential_minima[3],neighbour_config])
                self.LP.update_extrem_value(potential_minima[3],self.minima_dynamic)
                
    def __extract_extrema(self):
        #extract all the minima and maxima of the laplacian pyramid
        self.__extract_maxima()
        self.__extract_minima()

    def __preselection(self,val_treshold):

        minima_limit = (self.minima_dynamic[0]-self.minima_dynamic[1])*val_treshold+self.minima_dynamic[1]
        maxima_limit = (self.maxima_dynamic[1]-self.maxima_dynamic[0])*val_treshold+self.maxima_dynamic[0]
        
        preselection_minima = self.__select_below(self.minimas,minima_limit)
        preselection_maxima = self.__select_above(self.maximas,maxima_limit)
        
        return [preselection_minima,preselection_maxima]
    

    def __second_order_interpolation(self,point):
        #for test
        #return self.__first_order_interpolation(point)
        
        x,y,s,_,_ = point
        
        XY=[[0,-1],[1,0],[1,1],[-1,1],[-1,0],[0,0]]
        
        p=np.zeros((8,4))
        for i in range(len(XY)):
            p[i]=[XY[i][0],XY[i][1],self.LP.levelInfo[s][2],self.LP.table[s][y+XY[i][1]][x+XY[i][0]]]
            
        
        


        neighbour=self.__get_below_and_above_neighbour(point)
        if neighbour[0] is None or neighbour[1] is None:
            return self.__first_order_interpolation(point)
        
        #on projette les coordonnées des voisins en dessous et au dessus sur le niveau de la pyramide actuelle
        p6x,p6y=self.LP.get_original_coordonate(neighbour[0][0],neighbour[0][1],neighbour[0][2])
        p6x,p6y=self.LP.get_pyramid_coordonate(p6x,p6y,s)
        p7x,p7y=self.LP.get_original_coordonate(neighbour[1][0],neighbour[1][1],neighbour[1][2])
        p7x,p7y=self.LP.get_pyramid_coordonate(p7x,p7y,s)
        
        p[6]=[p6x-x,p6y-y,self.LP.levelInfo[s][neighbour[0][2]],neighbour[0][3]]
        p[7]=[p7x-x,p7y-y,self.LP.levelInfo[s][neighbour[1][2]],neighbour[1][3]]
        
        A=np.zeros((8,8))
        Y=np.zeros((8,1)) 
        for i in range(8):
            A[i]=[p[i][0]**2,p[i][1]**2,p[i][2]**2,p[i][0]*p[i][1],p[i][0],p[i][1],p[i][2],1]
            Y[i]=p[i][3]
            
        try:
            X=np.linalg.inv(A).dot(Y)
        except:
            return self.__first_order_interpolation(point)
        #as a list

        
        tmp1=np.sqrt(X[0]**2 - 2*X[0]*X[1] + X[1]**2 + X[3]**2)
        tmp2=-X[3]**2 + 4*X[0]*X[1]
        
        a=X[0]/2+X[1]/2+tmp1/2
        b=X[0]/2+X[1]/2-tmp1/2
        c=X[2]
        x0=-(2*X[1]*X[4] - X[3]*X[5])/tmp2
        y0=-(2*X[0]*X[5] - X[3]*X[4])/tmp2
        
        if (a>=0 or b>=0 or c>=0):
            return self.__first_order_interpolation(point)
        if(abs(x0)>1 or abs(y0)>1):
            return self.__first_order_interpolation(point)
        
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

        return [float(x+x0),float(y+y0),float(tau0),float(z0)]

          
        return interpolation
        
    def __first_order_interpolation(self,point):
        x,y,s,val,neighbour_config = point
        x,y = self.LP.get_original_coordonate(x,y,s)
        interpolation = [x,y,self.LP.levelInfo[s][2],val]
        return interpolation
        
        
    def __interpolation(self,point):
        x,y,s,val,neighbour_config = point
        bsm=(s<1)
        bsp=(s>self.LP.NLevel-2)
        
        if not(bsm) and not(bsp) and neighbour_config==0:
            return self.__second_order_interpolation(point)
        else:
            return self.__first_order_interpolation(point)
           
    def detect(self,preselection_treshold=None,stability_treshold=None):
       
       
        self.__extract_extrema()
        
        if preselection_treshold is None:
            preselection_minima = self.minimas
            preselection_maxima = self.maximas
        else:
            preselection_minima,preselection_maxima = self.__preselection(preselection_treshold)
        
        
        self.keypoints=preselection_minima+preselection_maxima
        
        for keypoint in self.keypoints:
            keypoint[0:4]=self.__interpolation(keypoint)
    
    
    
    def draw(self):
        copy = np.array(self.image.copy())

        for keypoint in self.keypoints:

            X,Y,size,power = (keypoint[0],keypoint[1],keypoint[2],keypoint[3])
            X,Y = (round(X),round(Y))

            cv2.rectangle(copy,(round(X-size//2),round(Y-size//2)),(round(X+size//2),round(Y+size//2)),255,1)

        return list(copy)