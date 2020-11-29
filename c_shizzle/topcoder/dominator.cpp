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
bool dfs_cut_off(int start_node, int dominator, int dominated, vector<vector<ii>>& graph) {
  tracker[start_node] = VISITED;

  cout << "c1: " << start_node << endl;
  
  for (int i = 0; i < graph[start_node].size(); ++i) {
    int cur_node = graph[start_node][i].first;
    cout << "cn: " << cur_node << endl;
    if (tracker[cur_node] == UNVISITED) {
      if (cur_node == dominator) {
        return true;
      }
      return dfs_cut_off(cur_node, dominator, dominated, graph);
    }
  }
}

void dominator(int x, int y, vector<vector<ii>>& graph) {
  tracker.resize(m, UNVISITED);
  bool reachable = dfs(0, y, graph);

  if (reachable) {
    tracker.resize(m, UNVISITED);
    int dominator = dfs_cut_off(0, x, y, graph);
    cout << "dom: " << dominator << endl;
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
