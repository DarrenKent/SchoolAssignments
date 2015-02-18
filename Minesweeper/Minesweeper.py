##                      ##
##   Minesweeper v1.0   ##
##      Coded by:       ##
##     Darren Kent      ##
##########################


#imports
import pygtk
import gtk
import random

#globals
GAME_SIZE = 10 #size of game in buttons (i.e. 10x10)
CELL_SIZE = 20 #how big each buttons should be (in pixels)
IMG_FLAG = 'flag.gif' #image to display for flags
IMG_BOMB = 'bomb.gif' #image to display for bombs
NUM_BOMBS = 10        #how many bombs should be on the screen
BOMB = 'bomb'


#classes
class Cell:
    def __init__(self):
        self.visibility = True
        self.contents = ""
    def setInvisible(self):
        self.visibility = False
    def getVisibility(self):
        return self.visibility
    def getContents(self):
        return self.contents
    def setContents(self,content):
        self.contents = content

class MinesweeperLogic:
    def __init__(self,size):
        self.size = size
        self.reset()
    def reset(self):
        self.bomb_locations = []
        for row in range(self.size):
            r = []
            for col in range(self.size):
                c = Cell()
                r.append(c)
            self.bomb_locations.append(r)
        self.setBombLocations()
        self.setBombCounters()
    def getBombLocations(self):
        return self.bomb_locations
    def setBombLocations(self):
        for i in range(NUM_BOMBS):
            setting = True
            while setting == True:
                r = random.randrange(GAME_SIZE)
                c = random.randrange(GAME_SIZE)
                if self.bomb_locations[r][c].getContents() == "":
                    self.bomb_locations[r][c].setContents(BOMB)
                    setting = False
    def setBombCounters(self):
        for r in range(GAME_SIZE):
            for c in range(GAME_SIZE):
                count = self.setBombCountNumbers(r,c)
                if self.bomb_locations[r][c].getContents() != BOMB:
                    self.bomb_locations[r][c].setContents(count)
    def setBombCountNumbers(self,row,col):
        count = 0
        count += self.isBomb(col+1,row)
        count += self.isBomb(col+1,row-1)
        count += self.isBomb(col+1,row+1)
        count += self.isBomb(col-1,row)
        count += self.isBomb(col-1,row+1)
        count += self.isBomb(col-1,row-1)
        count += self.isBomb(col,row-1)
        count += self.isBomb(col,row+1)
        return count
    def isBomb(self,col,row):
        if col < 0 or row < 0:
            return 0
        if col >= GAME_SIZE or row >= GAME_SIZE:
            return 0
        if self.bomb_locations[row][col].getContents() == BOMB:
            return 1
        else:
            return 0

    def checkCell(self,row,col):
            if row < 0 or col < 0:
                return 'nil'
            if col >= GAME_SIZE or row >= GAME_SIZE:
                return 'nil'
            if self.bomb_locations[row][col].getContents() > 0:
                return 'stop'
            if self.bomb_locations[row][col].getContents() == 0:
                return 'blank'
        
class myTable(gtk.Table):
    def __init__(self, xsize, ysize):
        gtk.Table.__init__(self, xsize, ysize, True)
        numcells = xsize * ysize
        self.xsize = xsize
        self.ysize = ysize
        self.cells = [] * numcells
        self.images = []
        self.labels = []

    def attachButton(self,button,row,column):
        self.attach(button,row,row+1,column,column+1)
        index = row * GAME_SIZE + column
        self.cells.append((button,index))

    def getRowColOfButton(self,button):
        for i in range(len(self.cells)):
            if self.cells[i][0] == button:
                buttonindex = i
                break
        index = self.cells[buttonindex][1]
        row = (index/GAME_SIZE)+1
        column = (index%GAME_SIZE)+1
        return (row,column)

    def attachImage(self,image):
        self.images.append(image)

    def attachLabel(self,label):
        self.labels.append(label)

    def clearImages(self):
        for i in range(len(self.images)):
            self.images[i] = ""

    def clearLabels(self):
        for i in range(len(self.labels)):
            self.labels[i] = ""
        
class MinesweeperGUI:
    def __init__(self):
        self.buttons = []
        self.createWindow()
        self.createMenu()
        self.winbar = gtk.InfoBar()
        self.loosebar = gtk.InfoBar()
        label = gtk.Label('You Win!')
        self.winbar.add(label)
        self.main_vbox.pack_start(self.winbar)
        label2 = gtk.Label('You landed on a bomb, Game Over!')
        self.loosebar.add(label2)
        self.main_vbox.pack_start(self.loosebar)
        self.logic = MinesweeperLogic(GAME_SIZE)
        self.createGame()
        self.win.show_all()
        self.winbar.hide()
        self.loosebar.hide()

    def placeImagesAndLabels(self):
        for r in range(GAME_SIZE):
            for c in range(GAME_SIZE):
                if self.logic.bomb_locations[r][c].getContents() == BOMB:
                    self.table.attachImage(self.getSmallImage(IMG_BOMB))
                    self.table.attach(self.getSmallImage(IMG_BOMB),r,r+1,c,c+1)
                elif self.logic.bomb_locations[r][c].getContents() > 0:
                    label = gtk.Label(self.logic.bomb_locations[r][c].getContents())
                    self.table.attachLabel(label)
                    self.table.attach(label,r,r+1,c,c+1)

    def getSmallImage(self,filename):
        image = gtk.Image()
        pb = gtk.gdk.pixbuf_new_from_file(filename)
        pb.scale_simple(CELL_SIZE,CELL_SIZE,gtk.gdk.INTERP_BILINEAR)
        image.set_from_pixbuf(pb)
        return image     

    def createGame(self):
        self.table = myTable(GAME_SIZE,GAME_SIZE)
        for h in range(GAME_SIZE):
            for v in range(GAME_SIZE):
                button = gtk.Button()
                button.set_size_request(CELL_SIZE,CELL_SIZE)
                self.buttons.append(button)
                button.connect('button_press_event',self.clicked_handler)
                self.table.attachButton(button,h,v)
        self.indexButtons()
        self.placeImagesAndLabels()
        self.main_vbox.pack_start(self.table)          
        
    def createMenu(self):
        self.menu = gtk.Menu()
        self.menu.add(self.createMenuItem('New Game',self.restart_handler))
        self.menu.add(self.createMenuItem('Solve',self.solve_handler))
        self.menu.add(self.createMenuItem('Quit',self.destroy_handler))
        self.root_menu = gtk.MenuItem('Game')
        self.root_menu.set_submenu(self.menu)
        self.menu_bar = gtk.MenuBar()
        self.menu_bar.add(self.root_menu)

        self.diff = gtk.Menu()
        self.diff.add(self.createMenuItem('Easy',self.easy_handler))
        self.diff.add(self.createMenuItem('Medium',self.med_handler))
        self.diff.add(self.createMenuItem('Hard',self.hard_handler))
        self.diff_menu = gtk.MenuItem('Difficulty')
        self.diff_menu.set_submenu(self.diff)
        self.menu_bar.add(self.diff_menu)

##        #Debug Menu
##        self.debug = gtk.Menu()
##        self.debug.add(self.createMenuItem('Contents',self.printMap))
##        self.debug.add(self.createMenuItem('Visible',self.printVisible))
##        self.debug_menu = gtk.MenuItem('Debug')
##        self.debug_menu.set_submenu(self.debug)
##        self.menu_bar.add(self.debug_menu)
        
        self.main_vbox.pack_start(self.menu_bar)
        
    def createMenuItem(self,title,handler):
        menubutton = gtk.MenuItem(title)
        menubutton.connect('activate',handler)
        return menubutton

    def createWindow(self):
        self.win = gtk.Window()
        self.win.set_title('Minesweeper')
        self.win.connect('delete_event',self.delete_handler)
        self.win.connect('destroy',self.destroy_handler)
        self.win.set_resizable(False)
        self.main_vbox = gtk.VBox()
        self.win.add(self.main_vbox)

    def delete_handler(self,widget,event,data=None):
        return False

    def destroy_handler(self,widget,data=None):
        gtk.main_quit()

    def easy_handler(self,widget,data=None):
        global GAME_SIZE,NUM_BOMBS
        GAME_SIZE = 10
        NUM_BOMBS = 10
        self.logic.size = GAME_SIZE
        self.restart_handler(widget)

    def med_handler(self,widget,data=None):
        global GAME_SIZE,NUM_BOMBS
        GAME_SIZE = 16
        NUM_BOMBS = 40
        self.logic.size = GAME_SIZE
        self.restart_handler(widget)

    def hard_handler(self,widget,data=None):
        global GAME_SIZE,NUM_BOMBS
        GAME_SIZE = 22
        NUM_BOMBS = 99
        self.logic.size = GAME_SIZE
        self.restart_handler(widget)        

    def restart_handler(self,widget,data=None):
        children = self.table.get_children()
        for i in range(len(children)):
            if children[i].get_name() == 'GtkLabel' or 'GtkImage':
                self.table.remove(children[i])
        self.logic.reset()
        self.table.clearImages()
        self.table.clearLabels()
        self.clearFlagsAndHideButtons()
        self.createGame()
        self.updateDisplay()
        self.win.show_all()
        self.loosebar.hide()
        self.winbar.hide()

    def solve_handler(self,widget,data=None):
        self.clearFlagsAndHideButtons()

    def clicked_handler(self,widget,event,data=None):
        if event.button == 1:
            if widget.get_image() == None or widget.get_image().get_visible() == False:
                self.processCells(widget)
                widget.hide()
                self.checkWin()
        if event.button == 3:
            self.toggleFlag(widget)

    def clearFlagsAndHideButtons(self):
        for i in range(len(self.buttons)):
            if self.buttons[i].get_image() != None:
                self.buttons[i].get_image().set_visible(False)
            self.buttons[i].hide()

    def toggleFlag(self,button):
        if button.get_image() == None:
            button.set_image(self.getSmallImage(IMG_FLAG))
        elif button.get_image().get_visible() == False:
            button.get_image().set_visible(True)
        else:
            button.get_image().set_visible(False)

    def updateDisplay(self):
        for r in range(GAME_SIZE):
            for c in range(GAME_SIZE):
                if self.logic.bomb_locations[r][c].getVisibility() == False:
                    self.logic.bomb_locations[r][c].setInvisible()         
        
    def processCells(self,button):
        r = self.table.getRowColOfButton(button)[0]
        c = self.table.getRowColOfButton(button)[1]
        r = r-1
        c = c-1
        if self.logic.bomb_locations[r][c].getContents() == BOMB:
            for i in range(len(self.buttons)):
                self.buttons[i].hide()
            self.loosebar.show()
            print 'You landed on a bomb, Game over!'
        else:
            base = self.logic.checkCell(r,c)
            if base != 'stop':
                self.clearSpaces(r,c)
                                
    def run(self):
        gtk.main()

    def indexButtons(self):
        self.buttonindex = []
        temp = []
        for i in range(GAME_SIZE):
            if temp != []:
                self.buttonindex.append(temp)
            temp = []
            for h in range(GAME_SIZE):
                temp.append(self.table.cells[h+i*GAME_SIZE][0])
        self.buttonindex.append(temp)

    def clearSpaces(self,r,c):
            pos1 = self.logic.checkCell(r-1,c)
            pos2 = self.logic.checkCell(r+1,c)
            pos3 = self.logic.checkCell(r,c-1)
            pos4 = self.logic.checkCell(r,c+1)
            pos5 = self.logic.checkCell(r-1,c-1)
            pos6 = self.logic.checkCell(r-1,c+1)
            pos7 = self.logic.checkCell(r+1,c-1)
            pos8 = self.logic.checkCell(r+1,c+1)
            if pos1 != 'nil':
                if self.buttonindex[r-1][c].get_visible() == True:
                    self.buttonindex[r-1][c].hide()
                    if pos1 == 'blank':
                        self.clearSpaces(r-1,c)
            if pos2 != 'nil':
                if self.buttonindex[r+1][c].get_visible() == True:
                    self.buttonindex[r+1][c].hide()
                    if pos2 == 'blank':
                        self.clearSpaces(r+1,c)
            if pos3 != 'nil':
                if self.buttonindex[r][c-1].get_visible() == True:
                    self.buttonindex[r][c-1].hide()
                    if pos3 == 'blank':
                        self.clearSpaces(r,c-1)
            if pos4 != 'nil':
                if self.buttonindex[r][c+1].get_visible() == True:
                    self.buttonindex[r][c+1].hide()
                    if pos4 == 'blank':
                        self.clearSpaces(r,c+1)
            if pos5 != 'nil':
                if self.buttonindex[r-1][c-1].get_visible() == True:
                    self.buttonindex[r-1][c-1].hide()
                    if pos5 == 'blank':
                        self.clearSpaces(r-1,c-1)
            if pos6 != 'nil':
                if self.buttonindex[r-1][c+1].get_visible() == True:
                    self.buttonindex[r-1][c+1].hide()
                    if pos6 == 'blank':
                        self.clearSpaces(r-1,c+1)
            if pos7 != 'nil':
                if self.buttonindex[r+1][c-1].get_visible() == True:
                    self.buttonindex[r+1][c-1].hide()
                    if pos7 == 'blank':
                        self.clearSpaces(r+1,c-1)
            if pos8!= 'nil':
                if self.buttonindex[r+1][c+1].get_visible() == True:
                    self.buttonindex[r+1][c+1].hide()
                    if pos8 == 'blank':
                        self.clearSpaces(r+1,c+1)
                        
    def checkWin(self):
        win = self.checkForWin()
        if win == True:
            for b in range(len(self.buttons)):
                if self.buttons[b].get_visible() == True:
                    if self.buttons[b].get_image() == None:
                        self.buttons[b].set_image(self.getSmallImage(IMG_FLAG))
                    else:
                        self.buttons[b].get_image().set_visible(True)                    
            self.winbar.show()
            print 'You Won'
                       
    def checkForWin(self):
        count = 0
        for i in range(GAME_SIZE):
            for x in range(GAME_SIZE):
                if self.buttonindex[x][i].get_visible() == True:
                    count += 1
                    if self.logic.bomb_locations[x][i].getContents() != BOMB:
                        return False
        if count > 0:
            return True
        return False
    
    # Debug Functions
    def printMap(self,widget,data=None):
        for i in range(GAME_SIZE):
            print
            for x in range(GAME_SIZE):
                if self.logic.bomb_locations[x][i].getContents() == BOMB:
                    print 'B',
                else:
                    print self.logic.bomb_locations[x][i].getContents(),
        print

    def printVisible(self,widget,data=None):
        for i in range(GAME_SIZE):
            print
            for x in range(GAME_SIZE):
                if self.buttonindex[x][i].get_visible() == True:
                    print 'T',
                else:
                    print 'F',
        print

                        
m = MinesweeperGUI()
m.run()

