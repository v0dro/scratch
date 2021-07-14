import math
import os
import random
import re
import sys
import copy
import bisect
import functools

# Complete the findShortest function below.

#
# For the weighted graph, <name>:
#
# 1. The number of nodes is <name>_nodes.
# 2. The number of edges is <name>_edges.
# 3. An edge exists between <name>_from[i] to <name>_to[i].
#
#
INFINITY = 999999

@functools.total_ordering
class Node:
    def __init__(self, _n, _w):
        self.node = _n
        self.weight = _w

    def __eq__(self, other):
        if isinstance(other, int):
            return self.node == other
        else:
            return self.node == other.node

    def __lt__(self, value):
        if isinstance(value, int):
            return self.node < value
        else:
            return self.node < value.node

    def __str__(self):
        return "<" + str(self.node) + "," + str(self.weight) + ">"

    def __repr__(self):
        return "<" + str(self.node) + "," + str(self.weight) + ">"

class Graph:
    def __init__(self, num_nodes):
        self.data = {}
        for i in range(num_nodes):
            self.data[i] = list()

    def edge_exists(self, from_node, to_node):
        graph_list = self.data[from_node]
        index = bisect.bisect_left(graph_list, to_node)
        if index < len(graph_list):
            return True
        else:
            return False

    def insert_edge(self, from_node, to_node, _weight=1):
        """
        Undirected graph so store edges twice.
        """
        other_node = Node(to_node, _weight)
        if from_node in self.data:
            bisect.insort(self.data[from_node], other_node)
        else:
            self.data[from_node] = [other_node]

        other_node = Node(from_node, _weight)
        if to_node in self.data:
            bisect.insort(self.data[to_node], other_node)
        else:
            self.data[to_node] = [other_node]

    def edge_length(self, from_node, to_node):
        if from_node == to_node:
            return 0
        if not self.edge_exists(from_node, to_node):
            return INFINITY

        graph_list = self.data[from_node]
        index = bisect.bisect_left(graph_list, to_node)

        if index < len(graph_list):
            return graph_list[index].weight

    def update_edge(self, from_node, to_node, new_weight):
        graph_list = self.data[from_node]
        index = bisect.bisect_left(graph_list, to_node)
        if index < len(graph_list): # update weight
            graph_list[index].weight = new_weight
        else:                   # create new weight between nodes
            self.insert_edge(from_node, to_node, new_weight)

        graph_list = self.data[to_node]
        index = bisect.bisect_left(graph_list, from_node)
        if index < len(graph_list): # update weight
            graph_list[index].weight = new_weight
        else:                   # create new weight between nodes
            self.insert_edge(to_node, from_node, new_weight)

    def __str__(self):
        return str(self.data)


def findShortest(graph_nodes, graph_from, graph_to, ids, val):
    if ids.count(val) == 1:
        return -1

    graph = Graph(graph_nodes)

    for i in range(len(graph_from)):
        from_node = graph_from[i] - 1
        to_node = graph_to[i] - 1
        graph.insert_edge(from_node, to_node)
    print(graph)

    values_connected_nodes = []
    for i, id in enumerate(ids):
        if id == val:
            values_connected_nodes.append(i)

    for k in range(graph_nodes):
        for i in range(graph_nodes):
            for j in range(graph_nodes):
                if i != j:
                    new_weight = graph.edge_length(i, k) + graph.edge_length(k, j)
                    old_weight = graph.edge_length(i, j)

                    print("min weight: ", min(new_weight, old_weight))
                    graph.update_edge(i, j, min(new_weight, old_weight))
                    # if old_weight > new_weight:
                    #     print("i: ", i, " j: ", j, " k: ", k, " graph: ", graph)
                    #     graph.update_edge(i, j, new_weight)
    print(graph)

    min_length = INFINITY
    for i in values_connected_nodes:
        if graph[val-1][i] < min_length and i != val - 1:
            min_length = graph[val-1][i]

    return min_length

graph_nodes = 5
graph_from = [1, 1, 2, 3]
graph_to = [2, 3, 4, 5]
ids = [1, 2, 3, 3, 2]
val = 2

findShortest(graph_nodes, graph_from, graph_to, ids, val)
