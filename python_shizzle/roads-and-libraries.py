def make_adjacency_matrix(n, cities):
    pass

def get_road_cost(graph, city):
    pass

def roadsAndLibraries(n, c_lib, c_road, cities):
    if c_lib < c_road:
        return c_lib * n

    visited = set([])
    graph = make_adjacency_matrix(n, cities)
    total_cost = 0
    for city in range(1, n+1):
        if city not in visited:
            road_cost, visited_nodes = get_road_cost(graph, city)
            visited.update(visited_nodes)
            total_cost += road_cost + c_lib

    return total_cost