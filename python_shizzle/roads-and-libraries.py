import copy

# https://www.cs.cmu.edu/afs/cs/academic/class/15210-f14/www/lectures/mst.pdf

# graph can have many spanning trees, but all have|V|vertices and|V|−1 edges.

def make_adjacency_list(n, cities):
    matrix = {}
    for i in range(n):
        matrix[i] = dict()

    for i, j in cities:
        matrix[i-1][j-1] = True
        matrix[j-1][i-1] = True

    return matrix

def path_exists(node1, node2, graph):
    return (node2 in graph[node1])

def get_road_cost(graph, city, c_road):
    visited = set([city])
    queue = [city]

    while queue:
        first = queue.pop()

        for c in graph[first].keys():
            if path_exists(first, c, graph) and not (c in visited):
                queue.insert(0, c)
                visited.add(c)

    return (len(visited) - 1) * c_road, visited

def roadsAndLibraries(n, c_lib, c_road, cities):
    if c_lib < c_road:
        return c_lib * n

    visited = set([])
    graph = make_adjacency_list(n, cities)
    total_cost = 0
    for city in range(0, n):
        if city not in visited:
            road_cost, visited_nodes = get_road_cost(graph, city, c_road)
            visited.update(visited_nodes)
            total_cost += road_cost + c_lib

    return total_cost


print(roadsAndLibraries(3, 2, 1, [[1,2], [3,1], [2, 3]]))

print(roadsAndLibraries(6, 3, 2, []))
