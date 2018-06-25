import os
from os import listdir

def really_get_contents(s, indent, path):
    contents = listdir(path)
    
    for content in contents:
        s += "-"*indent + str(content) + "\n"
        if os.path.isdir(os.path.join(path, content)):
            s = really_get_contents(s, indent+2, os.path.join(path, content))
            
    return s

"""
Print the directory contents recursively of every directory specified in path.
"""
def print_directory_contents(path):
    s = ""
    indent = 0
    return really_get_contents(s, indent, path)

s = print_directory_contents("/home/1/17M38101/gitrepos/hpc_lecture/")
print(s)
