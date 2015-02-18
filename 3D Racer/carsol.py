from panda3d.core import Spotlight, AmbientLight
from panda3d.core import CollisionTraverser, CollisionSphere, CollisionHandlerQueue, CollisionHandlerPusher, CollisionNode, BitMask32
from panda3d.core import Material
import math

class Car:
    def __init__(self):
        self.acceleration = 0
        
        self.SetupCar()
        self.SetupCamera()
        self.SetupLights()
        self.SetupCollisions()
        
    def SetupCar(self):
        self.car = loader.loadModel("Models/camaro.bam")
        self.car.setPos(-11551,-35356,0)
        self.car.setH(90)
        self.car.reparentTo(render)
        self.carMat = Material()
        self.carMat.setSpecular((.8,.7,.8,1))
        self.carMat.setShininess(20.0)
        self.car.setMaterial(self.carMat)
        
        self.DriverFrontTire = loader.loadModel("Models/tire.bam")
        self.DriverFrontTire.reparentTo(self.car)
        self.DriverFrontTire.setPos(-70,122,28)
        self.DriverRearTire = loader.loadModel("Models/tire.bam")
        self.DriverRearTire.reparentTo(self.car)
        self.DriverRearTire.setPos(-70,-110,28)
        self.PassFrontTire = loader.loadModel("Models/tire.bam")
        self.PassFrontTire.reparentTo(self.car)
        self.PassFrontTire.setPos(70,122,28)
        self.PassFrontTire.setH(180)
        self.PassRearTire = loader.loadModel("Models/tire.bam")
        self.PassRearTire.reparentTo(self.car)
        self.PassRearTire.setPos(70,-110,28)
        self.PassRearTire.setH(180)
        
    def SetupCamera(self):
        base.camera.reparentTo(self.car)
        base.camera.setPos(0,-1000,300)
        self.cameraTarget = self.car.attachNewNode("Camera Target")
        self.cameraTarget.setPos(0,0,200)
        base.camera.lookAt(self.cameraTarget)
        
    def SetupLights(self):
        self.spot = Spotlight('car spot')
        self.spot.setColor((.45,.25,.25,1))
        self.spot.getLens().setFov(40)
        self.spot.getLens().setNearFar(500,3000)
        self.spot.setShadowCaster(True)
        self.spotNode = self.car.attachNewNode(self.spot)
        self.spotNode.setPos(2000*math.cos(math.radians(90)),2000*math.sin(math.radians(90)),2000)
        self.spotNode.lookAt(self.car)
        render.setLight(self.spotNode)
        
        self.sunLight = AmbientLight('world light')
        self.sunLightNode = render.attachNewNode(self.sunLight)
        self.sunLight.setColor((.4,.4,.4,1))
        self.car.setLight(self.sunLightNode)
        
    def SetupCollisions(self):
        base.cTrav = CollisionTraverser()
        self.rearDecColSphere = self.car.attachNewNode(CollisionNode('rearSphere'))
        self.rearDecColSphere.node().addSolid(CollisionSphere(0,-130,80,100))
        self.rearDecColSphere.node().setIntoCollideMask(BitMask32.allOff())
        self.frontDecColSphere = self.car.attachNewNode(CollisionNode('frontSphere'))
        self.frontDecColSphere.node().addSolid(CollisionSphere(0,130,80,100))
        self.frontDecColSphere.node().setIntoCollideMask(BitMask32.allOff())
        self.frontHandler = CollisionHandlerQueue()
        self.rearHandler = CollisionHandlerQueue()
        base.cTrav.addCollider(self.rearDecColSphere,self.rearHandler)
        base.cTrav.addCollider(self.frontDecColSphere,self.frontHandler)
        self.rearColSphere = self.car.attachNewNode(CollisionNode('rearSphere'))
        self.rearColSphere.node().addSolid(CollisionSphere(0,-130,80,100))
        self.rearColSphere.node().setIntoCollideMask(BitMask32.allOff())
        self.frontColSphere = self.car.attachNewNode(CollisionNode('frontSphere'))
        self.frontColSphere.node().addSolid(CollisionSphere(0,130,80,100))
        self.frontColSphere.node().setIntoCollideMask(BitMask32.allOff())
        self.pusher = CollisionHandlerPusher()
        self.pusher.addCollider(self.rearColSphere,self.car)
        self.pusher.addCollider(self.frontColSphere,self.car)
        base.cTrav.addCollider(self.rearColSphere,self.pusher)
        base.cTrav.addCollider(self.frontColSphere,self.pusher)
        
        #self.frontDecColSphere.show()
        #self.rearDecColSphere.show()
        
    def CarFrame(self):            
        if self.acceleration > 0:
            self.acceleration -= 25
            self.Forward()
            
        if self.acceleration < 0:
            self.acceleration += 10
            self.Reverse()
            
    def Forward(self):
        self.car.setY(self.car, +self.acceleration  * globalClock.getDt())
        self.DriverFrontTire.setP(self.DriverFrontTire.getP() + self.acceleration * 20 * globalClock.getDt())
        self.DriverRearTire.setP(self.DriverRearTire.getP() + self.acceleration * 20 * globalClock.getDt())
        self.PassFrontTire.setP(self.PassFrontTire.getP() - self.acceleration * 20 * globalClock.getDt())
        self.PassRearTire.setP(self.PassRearTire.getP() - self.acceleration * 20 * globalClock.getDt())
        
    def Reverse(self):
        self.car.setY(self.car, +self.acceleration  * globalClock.getDt())
        self.DriverFrontTire.setP(self.DriverFrontTire.getP() - self.acceleration * 20 * globalClock.getDt())
        self.DriverRearTire.setP(self.DriverRearTire.getP() - self.acceleration * 20 * globalClock.getDt())
        self.PassFrontTire.setP(self.PassFrontTire.getP() + self.acceleration * 20 * globalClock.getDt())
        self.PassRearTire.setP(self.PassRearTire.getP() + self.acceleration * 20 * globalClock.getDt())
            
    def Accelerate(self):
        if self.acceleration < 0:
            self.acceleration += 100
        elif self.acceleration < 8000:
            self.acceleration += 50
        
    def Deccelerate(self):
        if self.acceleration > 0:
            self.acceleration -= 200
        elif self.acceleration > -3000:
            self.acceleration -= 20
            
    def applyTurnFriction(self):
        if self.acceleration > 0:
            self.acceleration -= 20
        if self.acceleration < 0:
            self.acceleration += 5
            
    def rotateCarSpot(self):
        angle = abs(int(self.car.getH()))
        self.spotNode.setX(2000*math.cos(math.radians(angle)))
        self.spotNode.setY(2000*math.sin(math.radians(angle)))
        self.spotNode.lookAt(self.car)
            
    def TurnLeft(self):
        self.DriverFrontTire.setH(20)
        self.PassFrontTire.setH(200)
        if self.acceleration != 0:
            self.applyTurnFriction()
            self.car.setH(self.car.getH() +50*globalClock.getDt())
            self.rotateCarSpot()
        
    def TurnRight(self):
        self.DriverFrontTire.setH(-20)
        self.PassFrontTire.setH(-200)
        if self.acceleration != 0:
            self.applyTurnFriction()
            self.car.setH(self.car.getH() -50*globalClock.getDt())
            self.rotateCarSpot()
        
    def StraightenOut(self):
        self.DriverFrontTire.setH(0)
        self.PassFrontTire.setH(180)
        
    def CollisionCheck(self):
        if self.frontHandler.getNumEntries() > 0:
                self.acceleration -= 50
                if self.acceleration <= 0:
                    self.acceleration = 0
        elif self.rearHandler.getNumEntries() > 0:
                self.acceleration += 50
                if self.acceleration >= 0:
                    self.acceleration =0
