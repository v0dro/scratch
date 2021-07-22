#!/bin/python3

import math
import os
import random
import re
import sys
from collections import deque

#
# Complete the 'isBalanced' function below.
#
# The function is expected to return a STRING.
# The function accepts STRING s as parameter.
#

def isBalanced(s):
    # Write your code here
    stack = deque()

    for char in s:
        if char == '{' or char == '(' or char == '[':
            stack.append(char)
        else:
            if stack:
                if char == '}' and stack[-1] == '{':
                    stack.pop()
                elif char == ')' and stack[-1] == '(':
                    stack.pop()
                elif char == ']' and stack[-1] == '[':
                    stack.pop()
                else:
                    return "NO"
                    break
            else:
                return "NO"
                break

    # NOTE TO SELF: Whenever you encounter any data structure in the future,
    # think about how it will change depending on all sorts of exteme inputs
    # throughout the algorithm.
    if stack:
        return "NO"

    return "YES"


if __name__ == '__main__':
    fptr = open(os.environ['OUTPUT_PATH'], 'w')

    t = int(input().strip())

    for t_itr in range(t):
        s = input()

        result = isBalanced(s)

        fptr.write(result + '\n')

    fptr.close()
