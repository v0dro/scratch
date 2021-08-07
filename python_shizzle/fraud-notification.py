import math
import os
import random
import re
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

# https://www.programiz.com/dsa/counting-sort
def activityNotifications(expenditure, d):
    # Write your code here
    notifications = 0
    if d == len(expenditure):
        return 0

    counts = [0] * 201

    for i in range(d, len(expenditure)):
        for j in range(i-d, i):
            counts[expenditure[j]] += 1




    print(counts)


    # for i in range(d, len(expenditure)):
    #     median_price = median(expenditure[i-d:i])

    #     if expenditure[i] >= 2 * median_price:
    #         notifications += 1

    # return notifications
