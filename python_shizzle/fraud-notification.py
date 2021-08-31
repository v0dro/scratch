#!/bin/python3

import math
import os
import random
import re
import bisect
import sys
from statistics import median

#
# Complete the 'activityNotifications' function below.
#
# The function is expected to return an INTEGER.
# The function accepts following parameters:
#  1. INTEGER_ARRAY expenditure
#  2. INTEGER d
#

def activityNotifications(expenditure, d):
    # Write your code here
    notifications = 0
    max_count = 201
    median = None

    if d == len(expenditure):
        return 0

    counts = [0] * max_count
    for j in range(0, d):
        counts[expenditure[j]] += 1

    for i in range(d, len(expenditure)):
        nums = 0
        for c in range(max_count):
            nums += counts[c]
            if d % 2 == 1:
                if nums > int(d/2):
                    median = c
                    break
            else:
                if nums > int(d/2):
                    median = c
                    median1 = 0
                    for c1 in range(c, max_count):
                        nums += counts[c1]
                        if nums > int(d/2) + 1:
                            median1 = c1
                            break
                    median = (median + median1) / 2
                    break

        if expenditure[i] >= 2 * median:
            notifications += 1

        counts[expenditure[i-d]] -= 1
        counts[expenditure[i]] += 1

    return notifications

if __name__ == '__main__':

    first_multiple_input = input().rstrip().split()

    n = int(first_multiple_input[0])

    d = int(first_multiple_input[1])

    expenditure = list(map(int, input().rstrip().split()))

    result = activityNotifications(expenditure, d)

    print("result: ", result)
