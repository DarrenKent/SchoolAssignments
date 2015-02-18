from game import *
from polygon import *
import config
import point
import pygame
import random

class Asteroids(Game):
    def __init__(self,name,screen_x, screen_y, frames_per_second):
        Game.__init__(self,name,screen_x,screen_y)
        self.fps = frames_per_second
        start = Point(config.SCREEN_X/2,config.SCREEN_Y/2) 
        self.ship = Ship(start,config.SHIP_INITIAL_DIRECTION,config.SHIP_COLOR)
        self.asteroids = []
        self.stars = []
        self.score = 0
        for i in range(config.STAR_COUNT):
            star = Star()
            self.stars.append(star)
        for i in range(config.ROCK_COUNT):
            x = random.randrange(0,config.SCREEN_X)
            y = random.randrange(0,config.SCREEN_Y)
            rotation = random.uniform(0.0,359.99)
            rotation_speed = random.uniform(config.ROCK_MIN_ROTATION_SPEED,config.ROCK_MAX_ROTATION_SPEED)
            direction = random.randint(0,1)
            if direction == 0:
                rotation_speed *= -1
            placement = Point(x,y)
            speed = random.randrange(config.ROCK_MIN_SPEED,config.ROCK_MAX_SPEED)
            rock = Rock(placement,rotation,config.ROCK_COLOR,rotation_speed,speed)
            self.asteroids.append(rock)
        self.nose = self.ship.getPoints()[0]
        self.bullet = Bullet(self.nose,config.BULLET_RADIUS,self.ship.rotation,config.BULLET_COLOR)
            
    def game_logic(self,keys,newkeys):
        self.nose = self.ship.getPoints()[0]
        self.ship.game_logic(keys,newkeys)
        for s in self.stars:
            s.twinkle()
        for i in self.asteroids:
            i.game_logic(keys,newkeys)
            collision = self.ship.intersect(i)
            if i.is_active() == True and self.ship.is_active() == True:
                if collision == True:
                    self.ship.set_inactive()
            collision2 = self.bullet.intersect(i)
            if i.is_active() == True and self.bullet.is_active() == True:
                if collision2 == True:
                    i.set_inactive()
                    self.bullet.set_inactive()
                    self.score += 100
                    print self.score
        if pygame.K_SPACE in newkeys and self.ship.is_active() == True:
            self.bullet.fire(self.nose,self.ship.rotation)
        self.bullet.game_logic(keys,newkeys)
    def paint(self,surface):
        surface.fill(config.BACKGROUND_COLOR)
        for s in self.stars:
            s.paint(surface)
        self.ship.paint(surface)
        for i in self.asteroids:
            i.paint(surface)
        self.bullet.paint(surface)

class Rock(Polygon):
    def __init__(self,position,rotation,color,rotation_speed,speed):
        self.rotation_speed = rotation_speed
        self.outline = self.createOutline()
        self.shape = []
        for i in self.outline:
            a = point.Point(i[0],i[1])
            self.shape.append(a)        
        Polygon.__init__(self,self.shape,position,rotation,color)
        self.accelerate(speed)
    def createOutline(self):
        base = 360/config.ROCK_POLYGON_SIZE
        pointList = []
        for i in range(config.ROCK_POLYGON_SIZE):
            pointList.append(i*base)
        outline = []
        for i in pointList:
            radius = random.uniform(config.ROCK_MIN_RADIUS,config.ROCK_MAX_RADIUS)
            radians = math.radians(i)
            x = math.cos(radians)*radius
            y = math.sin(radians)*radius
            outline.append([x,y])
        return outline
    
    def game_logic(self,keys,newkeys):
        if self.active == True:
            self.rotate(self.rotation_speed)
            self.move('rock')
        
class Ship(Polygon):
    def __init__(self,position,rotation,color):
        self.outline = [[30, 17], [29, 16], [26, 15], [20, 12], [15, 8], [12, 4],
                        [7, 0], [6, 1], [4, 1], [7, 4], [8, 4], [7, 9],
                        [5, 10], [1, 9], [0, 9], [5, 12], [5, 14], [6, 15],
                        [6, 16], [5, 16], [5, 18], [6, 19], [6, 20], [5, 20],
                        [5, 22], [0, 25], [1, 26], [5, 24], [7, 26], [8, 30],
                        [7, 31], [4, 33], [6, 33], [7, 34], [12, 31], [15, 26],
                        [20, 22], [26, 19], [29, 18], [30, 17]]
        self.half_ship = 12
        self.ship_shape = []
        for i in self.outline:
            a = point.Point(i[0],i[1])
            self.ship_shape.append(a)
        Polygon.__init__(self,self.ship_shape,position,rotation,color)
        
    def game_logic(self,keys,newkeys):
        if self.active == True:
            if pygame.K_LEFT in keys:
                self.rotate(-config.SHIP_ROTATION_RATE)
            if pygame.K_RIGHT in keys:
                self.rotate(+config.SHIP_ROTATION_RATE)
            if pygame.K_UP in keys:
                self.accelerate(config.SHIP_ACCELERATION_RATE)
            if pygame.K_DOWN in keys:
                self.accelerate(-config.SHIP_ACCELERATION_RATE)
            self.move('ship')

class Bullet(Circle):
    def __init__(self,position,radius,rotation,color):
        Circle.__init__(self,position,radius,rotation,color)
        self.active = False
    def game_logic(self,keys,newkeys):
        if self.active == False:
            return
        else:
            if self.position.x > config.SCREEN_X-1 or self.position.x < 1:
                self.active = False
            if self.position.y > config.SCREEN_Y-1 or self.position.y < 1:
                self.active = False
            self.move()
    def fire(self,position,rotation):
        self.position = position
        self.rotation = rotation
        self.active = True
        self.dx = 0
        self.dy = 0
        self.accelerate(config.BULLET_SPEED)

class Star(Circle):
    def __init__(self):
        x = random.randint(0,config.SCREEN_X)
        y = random.randint(0,config.SCREEN_Y)
        self.light = random.randint(0,255)
        self.position = Point(int(x),int(y))
        self.color = (self.light,self.light,self.light)
        self.radius = config.STAR_RADIUS
        Circle.__init__(self,self.position,self.radius,0.0,self.color)
    def twinkle(self):
        r = random.randint(0,config.STAR_TWINKLE_SPEED)
        a = random.choice([-1,1])
        if self.light + r*a > 255:
            self.light = 255
        elif self.light + r*a < 0:
            self.light = 0
        else:
            self.light = (self.light+r*a)
        self.color = (self.light,self.light,self.light)
        
def main():
    a = Asteroids(config.TITLE,config.SCREEN_X,config.SCREEN_Y,config.FRAMES_PER_SECOND)
    a.main_loop()

main()
