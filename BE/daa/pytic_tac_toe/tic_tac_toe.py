from Tkinter import *
from Tkconstants import *
import tkMessageBox
import copy

# Defaults: Human is ALWAYS 'X' and machine is ALWAYS 'O'

GRID_SIZE = 3

class Grid(object):
  def __init__(self, player_state, **kwargs):
    if kwargs.has_key('without_buttons') and kwargs['without_buttons'] == True:
      self.grid = player_state
    else:
      self.grid = []
      for i in xrange(0,GRID_SIZE):
        self.grid.append([])
        for j in xrange(0,GRID_SIZE):
          self.grid[i].append(player_state[i*GRID_SIZE + j]["text"])

  @classmethod
  def without_buttons(cls, buttons):
    Grid(buttons, without_buttons=True)

  def is_over(self):
    # Check if O or X win
    if self.o_wins() or self.x_wins():
      return True
    # Check for all boxes full
    all_full = True
    for row in xrange(0,GRID_SIZE):
      for col in xrange(0,GRID_SIZE):
        if self.grid[row][col] == "":
          all_full = False

    if all_full:
      return True

    return False

  def check_win(self, sym):
    # check rows
    if (self.grid[0][0] == self.grid[0][1] == self.grid[0][2] == sym) or (self.grid[1][0] == self.grid[1][1] == self.grid[1][2] == sym) or (self.grid[2][0] == self.grid[2][1] == self.grid[2][2] == sym):
      return True

    # check cols
    if (self.grid[0][0] == self.grid[1][0] == self.grid[2][0] == sym) or (self.grid[0][1] == self.grid[1][1] == self.grid[2][1] == sym) or (self.grid[0][2] == self.grid[1][2] == self.grid[2][2] == sym):
      return True

    # check diag
    if (self.grid[0][0] == self.grid[1][1] == self.grid[2][2] == sym):
      return True

    # check other diag
    if (self.grid[2][0] == self.grid[1][1] == self.grid[0][2] == sym):
      return True

    return False

  def o_wins(self):
    return self.check_win('O')

  def x_wins(self):
    return self.check_win('X')

  def get_all_possible_states(self, move_symbol):
    possible_states = []

    for i in xrange(0, GRID_SIZE):
      for j in xrange(0, GRID_SIZE):
        if self.grid[i][j] == "":
          temp = copy.deepcopy(self.grid)
          temp[i][j] = move_symbol
          possible_states.append(Grid(temp,without_buttons=True))
    
    return possible_states

  def __str__(self):
    return ' '.join([str(item) for item in self.grid])

  def to_list(self):
    return self.grid

class MiniMax(object):
  def __init__(self):
    pass

  def compute(self, button_list):
    game = Grid(button_list)
    recent_move = 'X'
    self._actual_compute(game, recent_move,0)

    if self.chosen_grid.o_wins():
      print "O has won"
    elif self.chosen_grid.x_wins():
      print "X has won"

    return self.chosen_grid

  def _score(self, game, depth, recent_move):
      if game.o_wins():
        return 10 - depth
      elif game.x_wins():
        return depth - 10
      else:
        return 0

  def _actual_compute(self, game, recent_move,depth):
    if game.is_over():
      return self._score(game, depth, recent_move)

    depth += 1
    scores = []
    moves  = []

    next_move = ('O' if recent_move == 'X' else 'X')
    all_possible_states = game.get_all_possible_states(next_move)

    for state in all_possible_states:
      scores.append(self._actual_compute(state, next_move, depth))
      moves.append(state)

    if recent_move == 'X':
      # Computer move so maximize scores.
      max_score_index = scores.index(max(scores))
      self.chosen_grid = moves[max_score_index]
      return scores[max_score_index]
    else:
      # Human move so minimize scores.
      min_score_index = scores.index(min(scores))
      self.chosen_grid = moves[min_score_index]
      return scores[min_score_index]

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
    if button['text'] == 'X' or button['text'] == 'O':
      return True

    return False

  def _update_buttons(self, solution_from_bot):
    lst = solution_from_bot.to_list()

    k = 0
    for row in xrange(0,GRID_SIZE):
      for col in xrange(0,GRID_SIZE):
        self.button_list[k]['text'] = lst[row][col]
        k += 1

  def _report_change_to_minimax(self,event):
    button = event.widget
    if not self._check_for_already_set(button):
      button["text"] = "X"
      solution_from_bot = self.minimax.compute(self.button_list)
      self._update_buttons(solution_from_bot)

x = GUI()
