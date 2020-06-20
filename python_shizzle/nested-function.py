import torch
from IPython import get_ipython
ipython = get_ipython()

def bar(meth):
    ipython.magic("timeit -o meth()")

def foo(arg1):
    t = torch.ones(arg1)
    
    def inner_foo():
        t + arg1
        return 0

    bar(inner_foo)

foo(100)
