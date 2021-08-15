#!/bin/python3

import math
import os
import random
import re
import sys

#
# Complete the 'isValid' function below.
#
# The function is expected to return a STRING.
# The function accepts STRING s as parameter.
#

def isValid(s):
    # Write your code here
    memo = dict()
    for c in s:
        if c in memo:
            memo[c] += 1
        else:
            memo[c] = 1

    vals = list(memo.values())

    u= sorted(list(vals))

    if len(set(u)) == 1:
        return "YES"

    if len(set(u)) > 2:
        return "NO"

    min_freq = vals.count(u[0])
    max_freq = vals.count(u[-1])

    if not (min_freq == 1 or max_freq == 1):
        return "NO"

# if the unique list of values is equals to 2
# and max value - min value == 1 and max occurrences is only one
# or min value - 1 == 0 and min occurrences is only one
# the string is valid in all other cases it is invalid

    if (u[-1] - u[0] == 1 and max_freq == 1) or ((u[-1] == 1 or u[0] == 1) and min_freq == 1):
        return "YES"

    return "NO"

if __name__ == '__main__':
    # fptr = open('out.txt', 'w')

    s = input()

    result = isValid(s)
    print("result: ", result)

    # fptr.write(result + '\n')

    # fptr.close()
