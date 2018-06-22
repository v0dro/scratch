import os
from os import listdir

def really_get_contents(s, indent, path):
    contents = listdir(path)
    print(path)
    for content in contents:
        if os.path.isdir(path + content):
            s += really_get_contents(s, indent+2, path + content + "/")
        else:
            s += "-"*indent + str(content) + "\n"
    return s

"""
Print the directory contents recursively of every directory specified in path.
"""
def print_directory_contents(path):
    s = ""
    indent = 0
    return really_get_contents(s, indent, path)
    #return s

print_directory_contents("/home/sameer/gitrepos/")

