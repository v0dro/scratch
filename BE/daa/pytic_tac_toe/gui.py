from Tkinter import *
from Tkconstants import *
import tkMessageBox
import logic

class MiniMax(object):
  def __init__(self):
    pass

  def compute(self, pressed_button_name, button_list):
    pass

class GUI(object):
  """GUI"""

  def __init__(self):
    self.app = Tk()
    self.minimax = MiniMax()
    buttonframe = LabelFrame(self.app)
    buttonframe.grid(row=0)

    tagframe = LabelFrame(self.app)
    tagframe.grid(row=1)
    tag = Label(tagframe, text="Inside the buttonframe")

    self.button_list = []
    for x in xrange(0,9):
      button = Button(buttonframe,text="",name=str(x),height="10",width="10")
      button.bind('<Button-1>', self._report_change_to_minimax)
      self.button_list.append(button)

    k = 0
    for i in xrange(0,3):
      for j in xrange(0,3):
        self.button_list[k].grid(row=i, column=j)
        k += 1

    self.app.mainloop()

  def _check_for_already_set(self, button):
    pass

  def _update_buttons(self, solution_from_bot):
    pass

  def _report_change_to_minimax(self,event):
    button = event.widget
    self._check_for_already_set(button)
    button["text"] = "X"
    solution_from_bot = self.minimax.compute(button._name, self.button_list)
    self._update_buttons(solution_from_bot)

x = GUI()

# www.ayatists.in
