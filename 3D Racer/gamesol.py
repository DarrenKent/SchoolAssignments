from direct.showbase.ShowBase import ShowBase
from panda3d.core import loadPrcFileData
import sys
import worldsol as world
import carsol as car

class GameController(ShowBase):
    def __init__(self):
        loadPrcFileData('', 'fullscreen 0')
        loadPrcFileData('', 'win-size 800 600')
        loadPrcFileData('', 'window-title Racer')
        loadPrcFileData('', 'force-parasite-buffer #t')
        loadPrcFileData('', 'default-far 300000')
        loadPrcFileData('', 'sync-video #f') 
        ShowBase.__init__(self)
        base.win.setClearColor((0,0,0,1))
        base.disableMouse()
        
        base.setFrameRateMeter(True)
        
        self.SetupWorld()
        self.SetupCar()
        self.SetupKeys()
        
        taskMgr.add(self.GameLoop,'Game Loop')
        
    def GameLoop(self,task):
        self.world.WorldFrame()
        self.car.CarFrame()
        
        if(self.keyMap["accelerate"]!= 0):
            self.car.Accelerate()
        if(self.keyMap["deccelerate"]!=0):
            self.car.Deccelerate()
        if(self.keyMap["left"]!=0):
            self.car.TurnLeft()
        if(self.keyMap["right"]!=0):
            self.car.TurnRight()
        if(self.keyMap["left"]==0 and self.keyMap["right"]==0):
            self.car.StraightenOut()
            
        self.car.CollisionCheck()
        
        return task.cont
        
    def SetupWorld(self):
        self.world = world.World()
        
    def SetupCar(self):
        self.car = car.Car()
        
    def SetupKeys(self):
        self.keyMap = {
            'accelerate':0,
            'deccelerate':0,
            'left':0,
            'right':0
            }
        self.accept('escape',sys.exit)
        self.accept('w',self.setKey,['accelerate',1])
        self.accept('w-up',self.setKey,['accelerate',0])
        self.accept('s',self.setKey,['deccelerate',1])
        self.accept('s-up',self.setKey,['deccelerate',0])
        self.accept('a',self.setKey,['left',1])
        self.accept('a-up',self.setKey,['left',0])
        self.accept('d',self.setKey,['right',1])
        self.accept('d-up',self.setKey,['right',0])
        
    def setKey(self,key,value):
        self.keyMap[key] = value