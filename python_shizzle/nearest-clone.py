import math
import os
import random
import re
import sys
import copy
import bisect
import functools
import heapq
from collections import defaultdict

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

class UndirectedGraph:
    def __init__(self, num_nodes):
        self.data = {}

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

    def connected_nodes(self, node):
        return self.data[node]

    def __str__(self):
        return str(self.data)

def shortest_path(start_node, graph):
    visited = set()
    queue = list()              # priority queue of lengths
    parents_map = dict()        # hash showing a map of the edges
    # length from the start node until various nodes in the graph
    node_costs = defaultdict(lambda : float('inf'))
    node_costs[start_node] = 0

    heapq.heappush(queue, (0, start_node))

    while queue:
        _, node = heapq.heappop(queue)
        visited.add(node)

        for _n in graph.connected_nodes(node):
            n = _n.node
            w = _n.weight

            if n in visited:
                continue

            new_cost = node_costs[node] + w

            if new_cost < node_costs[n]:
                parents_map[n] = node
                node_costs[n] = new_cost
                heapq.heappush(queue, (new_cost, n))

    return (parents_map, node_costs)


def findShortest(graph_nodes, graph_from, graph_to, ids, val):
    if ids.count(val) == 1:
        return -1

    graph = UndirectedGraph(graph_nodes)
    for i in range(len(graph_from)):
        graph.insert_edge(graph_from[i], graph_to[i])

    start_node = 1
    end_node = 5
    shortest_path(start_node, graph)

    node_maps = {}
    node_costs = {}
    memo = {}
    useful_nodes = [i+1 for i, node in enumerate(ids) if node == val]

    for node in useful_nodes:
        node_map, node_cost = shortest_path(node, graph)
        node_maps[node] = node_map
        node_costs[node] = node_cost

    min_path = float('inf')
    for source in useful_nodes:
        for end in useful_nodes:
            if source != end:
                cost = node_costs[source][end]
                if cost < min_path:
                    min_path = cost

    return min_path

graph_to = [2, 3, 4, 5]
ids = [1, 2, 3, 3, 2]
val = 2

findShortest(graph_nodes, graph_from, graph_to, ids, val)
