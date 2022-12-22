"""
An integral image is a data structure that allows for fast computation of the sum of pixels in a rectangular region of an image.
Each pixel in the integral image contains the sum of all pixels in the original image above and to the left of it.
See https://en.wikipedia.org/wiki/Summed-area_table
"""

class IntegralImage(object):
    """summary
    variables:
        self.width
        self.height
        self.table
    
    methods:
        __init__(self, original) : initialize variables and compute integral image
        self.update(original) : update integral image from a new original image (performances are better if the size of the original image is the same)
        self.get(x,y) : return the value of the integral image at position x,y (or 0 if out of range)
        self.box_sum(x,y,w,h) : return the sum of the box of size w,h at position x,y
        self.avrBlur(x,y,size) : return the average of the box of size size*size at position x,y
        self.avrBlur_fast(x0,y0,size,squaredSize) : return the average of the box of size size*size at position x,y (faster than avrBlur but need to compute squaredSize before)
        self.__biliniear_interpolation(x,y) : return the bilinear interpolation of the integral image at position x,y
        self.subpixel_box_sum(x,y,w,h) :
        self.avrBlur_subpixel(x,y,size) : 
        self.avrBlur_fast_subpixel(x0,y0,size,squaredSize) 
        
        
    """   
                 
    def __init__(self, original):
        '''
        original list[list]: 2d array containing original table data
        '''
        self.width = len(original[0])
        self.height = len(original)
        
        #initializing table
        self.table = [ [ 0 for y in range(self.width) ] for x in range(self.height) ]
        self.__compute(original)


    def update(self, original):
        if self.width != len(original[0]) or self.height != len(original):
            self.__init__(original)
        else:
            self.__compute(original)
    
    
    def __compute(self,original):
        # building integral
        summation=0
        for x in range(0, self.width):
            summation += original[0][x]
            self.table[0][x] = summation
            
        for y in range(1, self.height):
            summation = 0
            for x in range(0, self.width):
                summation += original[y][x]
                self.table[y][x] = summation + self.table[y-1][x]
                
    def get(self, x, y):
        ''' Returns value of cell at x, y
        x (int): x position of cell
        y (int): y position of cell
        All rows/columns are zero-indexed from top-left
        If requested cell is out of range 0 will be returned
        Returns int
        '''
        if x < 0 or y < 0 or x >= self.width or y >= self.height:
            return 0
        else:
            return self.table[y][x]


    """
    warning
    from here, there are no overflow check (for speed)
    so we use self.table[y][x] instead of self.get(x,y)
    When using this functions, be sure that x,y are in the image
    """
    
    def __biliniear_interpolation(self, x, y):
        "just an biliniear interpolation function for aproximate values between pixels on the integral image"
        
        x0 = int(x)
        y0 = int(y)
        x1 = x0 + 1
        y1 = y0 + 1
        return self.table[y0][x0]*(x1-x)*(y1-y) + self.table[y0][x1]*(x-x0)*(y1-y) + self.table[y1][x0]*(x1-x)*(y-y0) + self.table[y1][x1]*(x-x0)*(y-y0)
       
       
    def box_sum(self, x, y, w, h):
        ''' Sums a box at x, y with width w and height h
        x(int), y(int): x position of cell (left top corner)
        w(int): width of box to sum
        h(int): height of box to sum
        Returns int
        '''
        yy = y + h
        xx = x + w
        return self.table[xx][yy] - self.table[xx][y] - self.table[x][yy] + self.table[x][y] 
    
    def subpixel_box_sum(self, x, y, w, h):
        ''' Sums a box at x, y with width w and height h
        x(float), y(float): x position of cell (left top corner)
        w(float): width of box to sum
        h(float): height of box to sum
        compute the bilinear interpolation for the 4 points
        return float
        usefull for subpixel but slower (maybe about 4x)
        '''
        yy = y + h
        xx = x + w
        return self.__biliniear_interpolation(xx,yy) - self.__biliniear_interpolation(xx,y) - self.__biliniear_interpolation(x,yy) + self.__biliniear_interpolation(x,y)
        
        
    """
    x,y :center
    """
    def avrBlur(self,x,y,size):
        return self.box_sum(x-size//2,y-size//2,size,size)/(size*size)
    
    def avrBlur_subpixel(self,x,y,size):
        return self.subpixel_box_sum(x-size/2,y-size/2,size,size)/(size*size)
   
    """
    x0,y0 :top left corner
    if you have already compuded top left corner and size*size you can use this 2 functions
    """ 
    def avrBlur_fast(self,x0,y0,size,squaredSize):
        return self.box_sum(x0,y0,size,size)/squaredSize
    
    def avrBlur_fast_subpixel(self,x0,y0,size,squaredSize):
        return self.subpixel_box_sum(x0,y0,size,size)/squaredSize
    
    def __repr__(self):
        return '\n'.join([''.join(['{:4}'.format(item) for item in row]) for row in self.table])