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
    
        
    def detect(self,preselection_treshold=None,stability_treshold=None):
       
       
        self.__extract_extrema()
        
        if preselection_treshold is None:
            preselection_minima = self.minimas
            preselection_maxima = self.maximas
        else:
            preselection_minima,preselection_maxima = self.__preselection(preselection_treshold)
        
        
        self.keypoints=preselection_minima+preselection_maxima
    
    
    
    def draw(self):
        copy = self.image.copy()
        for keypoint in self.keypoints:
            px,py,level,value,neighbour_config = keypoint
            originalX,originalY = self.LP.get_original_coordonate(px,py,level)
            size = self.LP.levelInfo[level][2]
            cv2.rectangle(copy,(round(originalX-size//2),round(originalY-size//2)),(round(originalX+size//2),round(originalY+size//2)),255,1)
            #cv2.circle(copy,(round(originalX),round(originalY)),round(size),255,1)
        return np.array(copy)