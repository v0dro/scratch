#include <iostream>
#include <vector>
using namespace std;

// from the ICPC book section 4.2.1

// The section also adds that using a backtracking technique where
// the visited nodes are marked unvisited, it can be possible to
// change the time of DFS but backtracking has exponential cost.

// Using an adjacency list has the advantage that the DFS can
// be done in O(V+E) time.

typedef pair<int, int> ii;      // pair of (neighbour, weight)
typedef vector<ii> vii;
typedef vector<int> vi;

#define VISITED 1
#define UNVISITED -1

vi tracker;

void dfs(int node, vector<vii>& graph) {
  tracker[node] = VISITED;
  cout << "n: " << node << endl;

  for (int i = 0; i < graph[node].size(); i++) {
    ii new_node = graph[node][i];
    if (tracker[new_node.first] == UNVISITED) {
      dfs(new_node.first, graph);
    }
  }
}

int main(int argc, char *argv[]) {
  int m = 7;
  int w = 0;
  vector<vii> graph(m);
  tracker.resize(m, UNVISITED);

  graph[0].push_back(ii(2, w));
  graph[0].push_back(ii(1, w));

  graph[1].push_back(ii(0, w));
  graph[1].push_back(ii(2, w));
  graph[1].push_back(ii(4, w));

  graph[2].push_back(ii(0, w));
  graph[2].push_back(ii(1, w));
  graph[2].push_back(ii(5, w));

  graph[4].push_back(ii(1, w));

  graph[5].push_back(ii(2, w));
  graph[5].push_back(ii(6, w));

  graph[6].push_back(ii(5, w));

  dfs(0, graph);
}
