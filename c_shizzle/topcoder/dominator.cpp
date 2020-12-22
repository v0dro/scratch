// Find out whether a node dominates another node using dfs
#include <iostream>
#include <vector>
using namespace std;

typedef pair<int, int> ii;      // pair of (neighbour, weight)
typedef vector<ii> vii;
typedef vector<int> vi;

#define VISITED 1
#define UNVISITED -1

int m = 7;
int w = 0;

vi tracker;

bool dfs(int start_node, int check_exists, vector<vector<ii>>& graph) {
  tracker[start_node] = VISITED;
  cout << "c: " << start_node << endl;
  if (start_node == check_exists) {
    return true;
  }
  for (int i = 0; i < graph[start_node].size(); ++i) {
    int cur_node = graph[start_node][i].first;
    if (tracker[cur_node] == UNVISITED) {
      return dfs(cur_node, check_exists, graph);
    }
  }
  return false;
}

// check if dominated node is still acceesible given that all edges of dominator
// have been cut off.
void dfs_cut_off(int start_node, int dominator, int dominated, vector<vector<ii>>& graph,
                 bool * found_dominator) {
  tracker[start_node] = VISITED;

  for (int i = 0; i < graph[start_node].size(); ++i) {
    int cur_node = graph[start_node][i].first;
    if (tracker[cur_node] == UNVISITED) {
      if (cur_node == dominator) {
        *found_dominator = true;
        break;
      }
      dfs_cut_off(cur_node, dominator, dominated, graph, found_dominator);
    }
  }
}

void dominator(int x, int y, vector<vector<ii>>& graph) {
  tracker.resize(m, UNVISITED);
  bool reachable = dfs(0, y, graph);

  if (reachable) {
    for (int i = 0; i < m; ++i) {
      tracker[i]= UNVISITED;
    }
    bool dominator = false;
    dfs_cut_off(0, x, y, graph, &dominator);
  }
}

int main(int argc, char* argv[]) {
  vector<vector<ii>> graph;
  graph.resize(m);
  
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

  dominator(1, 4, graph);
  
}
