from Tkinter import *
from Tkconstants import *
import tkMessageBox
import copy

# Defaults: Human is ALWAYS 'X' and machine is ALWAYS 'O'

GRID_SIZE = 3

class Grid(object):
  def __init__(self, player_state, **kwargs):
    if kwargs['without_buttons'] == True:
      self.grid = player_state
    else:
      self.grid = []
      for i in xrange(0,3):
        grid.append([])
        for j in xrange(0,3):
          grid[i].append(player_state[i*GRID_SIZE + j]["text"])

  @classmethod
  def without_buttons(cls, buttons):
    Grid(buttons, without_buttons=True)

  def is_over():
    for row in xrange(0,3):
      for col in xrange(0,3):
        if self.grid[row][col] != "":
          return False

    return True

  def get_all_possible_states():
    possible_states = []

    for i in xrange(0,3):
      for j in xrange(0,3):
        if self.grid[i][j] == "":
          temp = copy.copy(self.grid)
          temp[i][j] = "O"
          possible_states.append(Grid.without_buttons(temp))

    return possible_states

class MiniMax(object):
  def __init__(self):
    pass

  def compute(self, pressed_button_name, button_list):
    game = Grid(button_list)

    _actual_compute(game)

  def _score(self, game):
    arr = []
    # check horizontal winning combinations
    for row in xrange(0,GRID_SIZE):
      for col in xrange(0,GRID_SIZE):
        arr.append(self.grid[row][col])

      if len(set(arr)) == 1:
        return True

  def _actual_compute(self, game):
    if game.is_over():
      return _score(game)

    scores = []
    moves = []

    all_possible_states = game.get_all_possible_states()
    for state in all_possible_states:
      _actual_compute(state)   


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
