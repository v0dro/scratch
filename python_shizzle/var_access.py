class A(object):
  """docstring for A"""
  def __init__(self, arg):
    self.this_var = arg


a=A(23)
a.this_var = 3435
print a.this_var