from Tkinter import *
from Tkconstants import *
import tkMessageBox

class GUI(object):
  """GUI"""

  def __init__(self):
    self.app = Tk()
    labelframe = LabelFrame(self.app, text="Lets play tic tac toe")
    labelframe.pack(fill="both", expand="yes")

    left = Label(labelframe, text="Inside the LabelFrame")
    left.pack()

    def action_for_x(event):
      name = event.widget
      name1 = name._name
      event.widget["text"] = str(name1)

    button = Button(labelframe,text="",name="b1HELLOE")
    button.pack()
    button.bind('<Button-1>',action_for_x)

    self.app.mainloop()

x = GUI()
