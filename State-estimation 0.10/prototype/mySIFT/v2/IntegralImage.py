class IntegralImage(object):
    def __init__(self, original):
        '''
        original list[list]: 2d array containing original table data
        '''
        self.width = len(original[0])
        self.height = len(original)
        """
        self.table = [[0 for _ in range(self.width)] for _ in range(self.height)]

        for y in range(self.height):
            for x in range(self.width):
                self.table[y][x] = original[y][x] + self.get(x - 1, y) + self.get(x, y - 1) - self.get(x - 1, y - 1)
        """
        self.table = [ [ 0 for y in range(self.width) ] for x in range(self.height) ]
        

        # building integral image to speed up block sum computation
        summation=0
        for x in range(0, self.width):
            summation += original[0][x]
            self.table[0][x] = summation
            
        for y in range(1, self.height):
            summation = 0
            for x in range(0, self.width):
                summation += original[y][x]
                self.table[y][x] = summation + self.table[y-1][x]


    def update(self, original):
        if self.width != len(original[0]) or self.height != len(original):
            self.__init__(original)
        else:

            # building integral image to speed up block sum computation
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

    def box_sum(self, x, y, w, h):
        ''' Sums a box at x, y with width w and height h
        x (int): x position of cell
        y (int): y position of cell (left top corner)
        w (int): width of box to sum
        h (int): height of box to sum
        Returns int
        '''
        yy = y + h - 1
        xx = x + w - 1
        return self.get(xx, yy) - self.get(xx, y - 1 ) - self.get(x-1, yy) + self.get(x-1, y - 1)
    
    def avrBlur(self,x,y,size):
        """
        x,y :center
        """
        return self.box_sum(x-size//2,y-size//2,size,size)/(size*size)
    
    
    def __repr__(self):
        return '\n'.join([''.join(['{:4}'.format(item) for item in row]) for row in self.table])
