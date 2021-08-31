#!/bin/python3

import math
import os
import random
import re
import sys

sys.setrecursionlimit(1000000)

#
# Complete the 'swapNodes' function below.
#
# The function is expected to return a 2D_INTEGER_ARRAY.
# The function accepts following parameters:
#  1. 2D_INTEGER_ARRAY indexes
#  2. INTEGER_ARRAY queries
#

def actual_tree_swap(tree, node, target_depth, depth, traversal):
    if node == -1:
        return

    if depth % target_depth == 0:
        tree[node][0], tree[node][1] = tree[node][1], tree[node][0]

    actual_tree_swap(tree, tree[node][0], target_depth, depth+1, traversal)
    traversal.append(node)
    actual_tree_swap(tree, tree[node][1], target_depth, depth + 1, traversal)

def swapNodes(indexes, queries):
    # Write your code here
    indexes.insert(0, [None, None])
    results = []

    for query in queries:
        temp = list()
        actual_tree_swap(indexes, 1, query, 1, temp)
        results.append(temp)

    return results

if __name__ == '__main__':
    fptr = open(os.environ['OUTPUT_PATH'], 'w')

    n = int(input().strip())

    indexes = []

    for _ in range(n):
        indexes.append(list(map(int, input().rstrip().split())))

    queries_count = int(input().strip())

    queries = []

    for _ in range(queries_count):
        queries_item = int(input().strip())
        queries.append(queries_item)

    result = swapNodes(indexes, queries)

    fptr.write('\n'.join([' '.join(map(str, x)) for x in result]))
    fptr.write('\n')

    fptr.close()
