import math

import pygame.draw

from point import Point
import config

class Shape:
    def __init__(self,position,rotation,color):
        self.position = position
        self.rotation = rotation
        self.color = color
        self.dx = 0
        self.dy = 0
    def conatins(self,point):
        raise NotImplementedError()
    def get_points(self):
        raise NotImplementedError()
    def paint(self,surface):
        raise NotImplementedError()
    def game_logic(self,keys,newkeys):
        raise NotImplementedError()
    def move(self):
        raise NotImplementedError()
    def accelerate(self,acceleration):
        self.dx = self.dx + acceleration * math.cos(math.radians(self.rotation))
        self.dy = self.dy + acceleration * math.sin(math.radians(self.rotation))    

class Polygon(Shape):
    def __init__(self, shape, position, rotation, color):
        Shape.__init__(self,position,rotation,color)
        self.active = True
        self.cache_points = (None, None, None)
        self.dx = 0
        self.dy = 0

        # first, we find the shape's top-most left-most boundary, its origin
        (origin_x, origin_y) = (shape[0].x, shape[0].y)
        for p in shape:
            if p.x < origin_x:
                origin_x = p.x
            if p.y < origin_y:
                origin_y = p.y

        # then, we orient all of its points relative to its origin
        shifted = []
        for p in shape:
            shifted.append(Point(p.x - origin_x, p.y - origin_y))

        # now shift them all based on the center of gravity
        self.shape = shifted
        self.center = self._findCenter()
        self.shape = []
        for p in shifted:
            self.shape.append(Point(p.x - self.center.x, p.y - self.center.y))

    # apply the rotation and offset to the shape of the polygon
    def getPoints(self):
        (oldrotation, oldposition, oldpoints) = self.cache_points
        if oldrotation == self.rotation and oldposition == self.position:
            return oldpoints

        angle = math.radians(self.rotation)
        sin = math.sin(angle)
        cos = math.cos(angle)
        points = []

        for p in self.shape:
            x = p.x * cos - p.y * sin + self.position.x
            y = p.x * sin + p.y * cos + self.position.y
            points.append(Point(x, y))

        self.cache_points = (self.rotation, self.position, points)
        return points

    # test if the given point is inside this polygon
    def contains(self, point):
        points = self.getPoints()
        crossingNumber = 0

        for i in range(len(points)):
            j = (i + 1) % len(points)

            if     (((points[i].x < point.x and point.x <= points[j].x) or
                    (points[j].x < point.x and point.x <= points[i].x)) and
                   (point.y > points[i].y + (points[j].y - points[i].y) /
                    (points[j].x - points[i].x) * (point.x - points[i].x))):
                crossingNumber += 1

        return crossingNumber % 2 == 1

    def _findArea(self):
        shape = self.shape
        sum = 0.0

        for i in range(len(shape)):
            j = (i + 1) % len(self.shape)
            sum += shape[i].x * shape[j].y - shape[j].x * shape[i].y
        return abs(0.5 * sum)

    def _findCenter(self):
        shape = self.shape
        (sum_x, sum_y) = (0.0, 0.0)

        for i in range(len(shape)):
            j = (i + 1) % len(self.shape)

            sum_x += (shape[i].x + shape[j].x) * \
                     (shape[i].x * shape[j].y - shape[j].x * shape[i].y)
            sum_y += (shape[i].y + shape[j].y) * \
                     (shape[i].x * shape[j].y - shape[j].x * shape[i].y)

        area = self._findArea()
        return Point(abs(sum_x / (6.0 * area)), abs(sum_y / (6.0 * area)))

    def is_active(self):
        return self.active

    def set_inactive(self):
        self.active = False
        
    def MoveObject(self,x,y,half_size):
        if y > config.SCREEN_Y+half_size:
            newposition = Point(x,0)
            return newposition
        elif y < 0:
            newposition = Point(x,config.SCREEN_Y+half_size)
            return newposition
        if x > config.SCREEN_X+half_size:
            newposition = Point(0,y)
            return newposition
        elif x < 0:
            newposition = Point (config.SCREEN_X+half_size,y)
            return newposition
        else:
            newposition = Point(x,y)
            return newposition


    def move(self,obj):
        if obj == 'ship':
            half_size = config.HALF_SHIP
        if obj == 'rock':
            half_size = config.HALF_ROCK
        x = self.position.x + self.dx
        y = self.position.y + self.dy
        self.position = self.MoveObject(x,y,half_size)

    def rotate(self,degrees):
        if self.rotation + degrees < 360.00 and self.rotation + degrees > 0.0:
            self.rotation += degrees
        if self.rotation + degrees >= 360.00:
            self.rotation = (self.rotation+degrees) - 360.00
        if self.rotation + degrees <= 0.0:
            self.rotation = (self.rotation+degrees) + 360.00

    def paint(self,surface):
        if self.active == True:
            points = self.getPoints()
            newpoints = []
            for i in points:
                newpoint = i.pair()
                newpoints.append(newpoint)
            pygame.draw.polygon(surface,self.color,newpoints,0)

    def intersect(self,other_polygon):
        for i in other_polygon.getPoints():
            collided = self.contains(i)
            if collided == True:
                return True
        for i in self.getPoints():
            collided = other_polygon.contains(i)
            if collided == True:
                return True
        return False

    def accelerate(self,acceleration):
        self.dx = self.dx + acceleration * math.cos(math.radians(self.rotation))
        self.dy = self.dy + acceleration * math.sin(math.radians(self.rotation))


class Circle(Shape):
    def __init__(self,position,radius,rotation,color):
        Shape.__init__(self,position,rotation,color)
        self.radius = radius
        self.active = True
    def paint(self, surface):
        if self.active == True:
            pygame.draw.circle(surface,self.color,(int(self.position.x),int(self.position.y)),int(self.radius))
    def MoveObject(self,x,y):
        if y > config.SCREEN_Y:
            newposition = Point(x,0)
            return newposition
        elif y < 0:
            newposition = Point(x,config.SCREEN_Y)
            return newposition
        if x > config.SCREEN_X:
            newposition = Point(0,y)
            return newposition
        elif x < 0:
            newposition = Point (config.SCREEN_X,y)
            return newposition
        else:
            newposition = Point(x,y)
            return newposition
    def move(self):
        x = self.position.x + self.dx
        y = self.position.y + self.dy
        self.position = self.MoveObject(x,y)
    def rotate(self,degrees):
        if self.rotation + degrees < 360.00 and self.rotation + degrees > 0.0:
            self.rotation += degrees
        if self.rotation + degrees >= 360.00:
            self.rotation = (self.rotation+degrees) - 360.00
        if self.rotation + degrees <= 0.0:
            self.rotation = (self.rotation+degrees) + 360.00
    def accelerate(self,acceleration):
        self.dx = self.dx + acceleration * math.cos(math.radians(self.rotation))
        self.dy = self.dy + acceleration * math.sin(math.radians(self.rotation))
    def is_active(self):
        return self.active
    def set_inactive(self):
        self.active = False
    def getPoints(self):
        base = 360/config.BULLET_POINT_COUNT
        points = []
        for i in range(config.BULLET_POINT_COUNT):
            points.append(i*base)
        outline = []
        for i in points:
            radius = config.BULLET_RADIUS
            radians = math.radians(i)
            x = math.cos(radians)*radius
            y = math.sin(radians)*radius
            outline.append([x,y])
        shape = []
        for i in outline:
            a = Point(i[0]+self.position.x,i[1]+self.position.y)
            shape.append(a)
        return shape
    def contains(self,point):
        dist_x = self.position.x - point.x
        dist_y = self.position.y - point.y
        return dist_x*dist_x + dist_y*dist_y <= self.radius*self.radius
        
    def intersect(self,other_polygon):
        for i in other_polygon.getPoints():
            collided = self.contains(i)
            if collided == True:
                return True
        for i in self.getPoints():
            collided = other_polygon.contains(i)
            if collided == True:
                return True
        return False            
        
