from panda3d.core import AmbientLight,Spotlight
import random

class World:
    def __init__(self):
        self.SetupWorld()
        self.SetupLights()
        
    def SetupWorld(self):
        self.world = loader.loadModel('Models/track.bam')
        self.world.reparentTo(render)
        self.skybox = loader.loadModel('Models/skybox.bam')
        self.skybox.reparentTo(render)
        self.skybox.setScale(3)
        
        self.displayBushes()
        self.displayRocks()
        self.displayCactus()
        
    def SetupLights(self):
        
        self.skyboxLight = AmbientLight('skybox light')
        self.skyboxLightNode = render.attachNewNode(self.skyboxLight)
        self.skyboxLight.setColor((1,1,1,1))
        self.skybox.setLight(self.skyboxLightNode)
        
        self.sunLight = AmbientLight('world light')
        self.sunLightNode = render.attachNewNode(self.sunLight)
        self.sunLight.setColor((.4,.4,.4,1))
        self.world.setLight(self.sunLightNode)

        self.world.setShaderAuto()
        
        self.spot = Spotlight('World spot')
        self.spot.setColor((.7,.7,.7,1))
        self.spot.getLens().setFov(180)
        self.spot.getLens().setNearFar(1,10000000)
        self.spot.setShadowCaster(True)
        self.spotNode = render.attachNewNode(self.spot)
        self.spotNode.setPos(-10000,-10000,500000)
        self.spotNode.lookAt(self.world)
        render.setLight(self.spotNode)
        
    def WorldFrame(self):
        self.skybox.setH(self.skybox.getH()+(.5 * globalClock.getDt()))
        
    def generateBushes(self,x,y,num):
        for i in range(num):
            xnew = random.randint(x-2500,x+2500)
            ynew = random.randint(y-2500,y+2500)
            bushInstance = self.world.attachNewNode('bush')
            bushInstance.setPos(xnew,ynew,0)
            bushInstance.setTwoSided(True)
            self.bush.instanceTo(bushInstance)
            
    def displayBushes(self):
        self.bush = loader.loadModel('Models/bush1.bam')
        self.generateBushes(-17000,-40000,5)
        self.generateBushes(9000,-40000,5)
        self.generateBushes(-26000,4000,5)
        self.generateBushes(-70000,2500,5)
        self.generateBushes(-28000,30000,5)
        self.generateBushes(-51000,46000,5)
        self.generateBushes(17000,18000,5)
        
    def generateRock(self,x,y,num):
        for i in range(num):
            xnew = random.randint(x-2500,x+2500)
            ynew = random.randint(y-2500,y+2500)
            type = random.randint(1,3)
            h = random.randint(0,180)
            rockInstance = self.world.attachNewNode('rock')
            rockInstance.setPos(xnew,ynew,0)
            rockInstance.setH(h)
            if type == 1:   
                self.rock1.instanceTo(rockInstance)
            elif type == 2:   
                self.rock2.instanceTo(rockInstance)
            elif type == 3:   
                self.rock3.instanceTo(rockInstance)
                
    def displayRocks(self):
        self.rock1 = loader.loadModel('Models/rock1.bam')
        self.rock2 = loader.loadModel('Models/rock2.bam') 
        self.rock3 = loader.loadModel('Models/rock3.bam')       
        self.generateRock(28000,-30000,2)
        self.generateRock(-14000,34000,2)
        self.generateRock(-89000,3500,2)
        self.generateRock(-22000,-42000,2)
        self.generateRock(-4000,7000,2)
 
    def generateCactus(self,x,y,num):
        for i in range(num):
            xnew = random.randint(x-2500,x+2500)
            ynew = random.randint(y-2500,y+2500)
            h = random.randint(0,180)
            cactusInstance = self.world.attachNewNode('cactus')
            cactusInstance.setPos(xnew,ynew,0)
            cactusInstance.setH(h)
            self.cactus.instanceTo(cactusInstance)

    def displayCactus(self):
        self.cactus = loader.loadModel('Models/cactus.bam')
        self.generateCactus(9000,-47000,3)
        self.generateCactus(-49000,38000,3)
        self.generateCactus(-80000,-21000,3)