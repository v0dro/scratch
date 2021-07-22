import heapq
from collections import defaultdict

class Graph:
    def __init__(self, n):
        self.nodes = n
        self.data = {}
        for _n in range(n):
            self.data[_n] = list()

    def connect(self, node1, node2):
        self.data[node1].append(node2)
        self.data[node2].append(node1)

    def find_all_distances(self, s):
        visited = set()
        costs = defaultdict(lambda: float('inf'))
        queue = list()
        priority_queue = []

        queue.append(s)
        visited.add(s)
        heapq.heappush(priority_queue, (0, s))
        costs[s] = 0

        while priority_queue:
            shortest_path, origin_node = heapq.heappop(priority_queue)

            for dest_node in self.data[origin_node]:
                if not dest_node in visited:
                    path_length = costs[origin_node] + 6
                    if path_length < costs[dest_node]:
                        visited.add(dest_node)
                        costs[dest_node] = path_length
                        heapq.heappush(priority_queue, (path_length, dest_node))

        for node in range(self.nodes):
            if costs[node] == float('inf'):
                costs[node] = -1

        output = ""
        for node in range(self.nodes):
            if node != s:
                output += str(costs[node]) + " "

        print(output)

t = int(input())
for i in range(t):
    n,m = [int(value) for value in input().split()]
    graph = Graph(n)
    for i in range(m):
        x,y = [int(x) for x in input().split()]
        graph.connect(x-1,y-1)
    s = int(input())
    graph.find_all_distances(s-1)

# Expected:
# 6 6 6 6 12 6 12 6 12 12 6 6 6 6 6 12 12 6 6 6 6 12 6 12 6 12 6 12 12 12 12 6 12 12 6 12 12 6 12 6 12 6 12 12 6 6 12 6 6 6 6 12 12 12 12 6 6 6 12 6 6 12 12 12 12 12 12 6 6

# Real:
# 6 6 6 6 12 6 30 6 18 12 6 6 6 6 6 12 18 6 6 6 6 24 6 12 6 12 6 12 18 12 12 6 12 12 6 12 18 6 12 6 12 6 12 24 6 6 12 6 6 6 6 12 18 12 12 6 6 6 12 6 6 24 18 18 12 18 12 6 6
