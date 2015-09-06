// Name: Sameer Deshmukh
// Title: Informed search with the A Star algorithm
// Class: BE I
// Roll: 119

#include <iostream>
#include <vector>
#include <cmath>

#define GRID_SIZE 3
using namespace std;

class Grid;
class AStar;
void swap(int *num1, int* num2);
void pretty_print(Grid mat);
void copy_matrix(int clone[GRID_SIZE][GRID_SIZE], int original[GRID_SIZE][GRID_SIZE]);

class Grid
{ 
  void
  move_up(int current[GRID_SIZE][GRID_SIZE], int blank_row, int blank_col)
  {
    swap(&current[blank_row][blank_col], &current[blank_row-1][blank_col]);
  }

  void
  move_down(int current[GRID_SIZE][GRID_SIZE], int blank_row, int blank_col)
  {
    swap(&current[blank_row][blank_col], &current[blank_row+1][blank_col]);
  }

  void
  move_right(int current[GRID_SIZE][GRID_SIZE], int blank_row, int blank_col)
  {
    swap(&current[blank_row][blank_col], &current[blank_row][blank_col+1]);
  }

  void
  move_left(int current[GRID_SIZE][GRID_SIZE], int blank_row, int blank_col)
  {
    swap(&current[blank_row][blank_col], &current[blank_row][blank_col-1]);
  }

  int grid[GRID_SIZE][GRID_SIZE];

 public:
  Grid(int gr[GRID_SIZE][GRID_SIZE]) 
  { 
    copy_matrix(grid, gr);
  }

  Grid() {}

  void
  find(int element, int *row, int *col)
  {
    for (int i = 0; i < GRID_SIZE; ++i) {
      for (int j = 0; j < GRID_SIZE; ++j) {
        if (grid[i][j] == element) {
          *row = i;
          *col = j;
          break;
        }
      }
    }
  }

  vector<Grid>
  get_possible_positions()
  {
    vector<Grid> states;
    int mp_row, mp_col, possible_state[3][3];

    for (int i = 0; i < GRID_SIZE; ++i) {
      for (int j = 0; j < GRID_SIZE; ++j) {
        if (this->grid[i][j] == -1) {
          mp_row = i;
          mp_col = j;
          break;
        }
      }
    }

    if (mp_row == 0) {
      copy_matrix(possible_state, grid);
      move_down(possible_state, mp_row,mp_col);
      states.push_back(Grid(possible_state));

      if (mp_col == 0) { // top left corner
        copy_matrix(possible_state, grid);
        move_right(possible_state, mp_row, mp_col);
        states.push_back(Grid(possible_state));
        //right
      }
      else if (mp_col == 1) { // top edge
        copy_matrix(possible_state, grid);
        move_right(possible_state, mp_row, mp_col);
        states.push_back(Grid(possible_state));

        copy_matrix(possible_state, grid);
        move_left(possible_state, mp_row, mp_col);
        states.push_back(Grid(possible_state));
        //left and right
      }
      else if (mp_col == 2) { // top right corner
        copy_matrix(possible_state, grid);
        move_left(possible_state, mp_row, mp_col);
        states.push_back(Grid(possible_state));
        //left
      }
    }
    else if (mp_row == 1) { 
      copy_matrix(possible_state, grid);
      move_up(possible_state, mp_row, mp_col);
      states.push_back(Grid(possible_state));

      copy_matrix(possible_state, grid);
      move_down(possible_state, mp_row, mp_col);
      states.push_back(Grid(possible_state));

      if (mp_col == 1) { // center
        copy_matrix(possible_state, grid);
        move_left(possible_state, mp_row, mp_col);
        states.push_back(Grid(possible_state));

        copy_matrix(possible_state, grid);
        move_right(possible_state, mp_row, mp_col);
        states.push_back(Grid(possible_state));
        // left, right
      }
      else if (mp_col == 2) { // right edge
        copy_matrix(possible_state, grid);
        move_left(possible_state, mp_row, mp_col);
        states.push_back(Grid(possible_state));
        // left
      }
      else if (mp_col == 0) { // left edge
        copy_matrix(possible_state, grid);
        move_right(possible_state, mp_row, mp_col);
        states.push_back(Grid(possible_state));
        // right
      } 
    }
    else if (mp_row == 2) {
      copy_matrix(possible_state, grid);
      move_up(possible_state, mp_row, mp_col);
      states.push_back(Grid(possible_state));

      if (mp_col == 0) { // bottom left corner
        copy_matrix(possible_state, grid);
        move_right(possible_state, mp_row, mp_col);
        states.push_back(Grid(possible_state));
        // right
      }
      else if (mp_col == 1) { // bottom edge
        copy_matrix(possible_state, grid);
        move_left(possible_state, mp_row, mp_col);
        states.push_back(Grid(possible_state));

        copy_matrix(possible_state, grid);
        move_right(possible_state, mp_row, mp_col);
        states.push_back(Grid(possible_state));
        // right, left
      }
      else if (mp_col == 2) { // bottom right corner
        copy_matrix(possible_state, grid);
        move_left(possible_state, mp_row, mp_col);
        states.push_back(Grid(possible_state));
        // left
      }
    }

    return states;
  }

  int
  displaced_tiles(Grid &goal)
  { 
    int h = 0;

    for (int i = 0; i < GRID_SIZE; ++i) {
      for (int j = 0; j < GRID_SIZE; ++j) {
        if (goal.get(i,j) != this->get(i,j)) {
          h++;
        }
      }
    }

    return h;
  }

  int
  compute_heuristic(Grid &start, Grid &goal, Grid parent)
  {
    int h = 0, m = 0;
    
    // calculate number of tiles out of place
    h = displaced_tiles(goal);

    // calculate manhattan distances
    int orig_row, orig_col, curr_row, curr_col;
    for (int i = 1; i < GRID_SIZE*GRID_SIZE; ++i) {
      this->find(i, &curr_row, &curr_col);
      parent.find(i, &orig_row, &orig_col);
      m += abs(curr_row - orig_row) + abs(curr_col - orig_col);
    }

    this->find(-1, &curr_row, &curr_col);
    parent.find (-1, &orig_row, &orig_col);
    m += abs(curr_row - orig_row) + abs(curr_col - orig_col);

    return h + m;
  }

  Grid
  copy()
  {  
    Grid g(grid);
    return g;
  }

  int
  get(int i, int j)
  {
    return this->grid[i][j];
  }

  bool
  operator== (Grid other)
  {
    bool equal = true;

    for (int i = 0; i < GRID_SIZE; ++i) {
      for (int j = 0; j < GRID_SIZE; ++j) {
        if (this->get(i,j) != other.get(i,j)) {
          equal = false;
          break;
        }
      }
    }

    return equal;
  }

  Grid
  operator= (Grid other) 
  {
    for (int i = 0; i < GRID_SIZE; ++i) {
      for (int j = 0; j < GRID_SIZE; ++j) {
        this->grid[i][j] = other.get(i,j);
      }
    }

    return *this;
  }
};

class AStar
{ 
  Grid *start, *goal;
  struct node {
    Grid parent,current;
    int f;
  };
  typedef struct node node;
  
  int
  get_min_index(vector<int> values)
  {
    int min = values[0], idx = 0;

    for (int i = 1; i < values.size(); ++i) {
      if (min > values[i]) {
        min = values[i];
        idx = i;
      }
    }

    return idx;
  };

  int
  min_heuristic_node(vector<node> list)
  {
    int min = list[0].f;
    int smallest_index = 0;

    for (int i = 0; i < list.size(); ++i) {
      if (min > list[i].f) {
        min = list[i].f;
        smallest_index = i;
      }
    }

    return smallest_index;
  };

  bool
  node_with_same_position_and_better_f_is_in_list(node current_node, vector<node> list)
  {
    for (int i = 0; i < list.size(); ++i) {
      if (current_node.current == list[i].current) {
        if (current_node.f >= list[i].f) {
          return true;
        }
      }
    }

    return false;
  }

  void
  print_list(vector<node> list)
  {
    for (int i = 0; i < list.size(); ++i) {
      pretty_print(list[i].current);
      cout << "\n";
    }
  }

 public:
  AStar()
  { 
    int st[GRID_SIZE][GRID_SIZE] = {
      1,8,2,
      -1,4,3,
      7,6,5
    };
    start = new Grid(st);

    int en[GRID_SIZE][GRID_SIZE] = {
      1,2,3,
      4,5,6,
      7,8,-1   
    };
    goal = new Grid(en);
  };

  void
  compute()
  { 
    Grid current = start->copy();
    vector<Grid> successors;
    vector<node> open_list, closed_list;
    node *curr = new node();
    int ex = 0;

    curr->current = current;
    curr->f = 0;
    open_list.push_back(*curr);

    while (open_list.size() != 0) {
      int smallest_f_node_index = min_heuristic_node(open_list);
      node current_node = open_list[smallest_f_node_index];
      open_list.erase(open_list.begin() + smallest_f_node_index); // erase element with smallest heuristic
      // get all possible positions of the current node
      successors = current_node.current.get_possible_positions();

      for (int i = 0; i < successors.size(); ++i) {
        if (successors[i] == *goal) {
          ex = 1;
        }
        else {
          node *successor_node    = new node();
          successor_node->current = successors[i];
          successor_node->f       = successors[i].compute_heuristic(*start, *goal, current_node.current);
          successor_node->parent  = current_node.current;

          if (node_with_same_position_and_better_f_is_in_list(*successor_node, open_list)) {
            continue;
          }
          else if (node_with_same_position_and_better_f_is_in_list(*successor_node, closed_list)) {
            continue;
          }
          else {
            open_list.push_back(*successor_node);
          }
        }
      }

      closed_list.push_back(current_node);
      if (ex == 1) break;
    }

    print_list(closed_list);
    pretty_print(*goal);
  }
};

// GLOBAL FUNCTIONS
void
swap(int *num1, int* num2)
{
  int temp;

  temp  = *num1;
  *num1 = *num2;
  *num2 = temp;
}

void
pretty_print(Grid mat)
{
  for (int i = 0; i < GRID_SIZE; ++i) {
    for (int j = 0; j < GRID_SIZE; ++j) {
      cout << mat.get(i,j) << " ";
    }
    cout << "\n";
  }
}

void
copy_matrix(int clone[GRID_SIZE][GRID_SIZE], int original[GRID_SIZE][GRID_SIZE])
{
  for (int i = 0; i < GRID_SIZE; ++i) {
    for (int j = 0; j < GRID_SIZE; ++j) {
      clone[i][j] = original[i][j];
    }
  }
}

int
main()
{
  AStar a;
  a.compute();

  return 0;
}

// OUTPUT

// 1 8 2 
// -1 4 3 
// 7 6 5 

// 1 8 2 
// 4 -1 3 
// 7 6 5 

// 1 -1 2 
// 4 8 3 
// 7 6 5 

// 1 2 -1 
// 4 8 3 
// 7 6 5 

// 1 2 3 
// 4 8 -1 
// 7 6 5 

// 1 2 3 
// 4 8 5 
// 7 6 -1 

// 1 2 3 
// 4 -1 8 
// 7 6 5 

// 1 2 3 
// 4 8 5 
// 7 -1 6 

// 1 2 3 
// 4 -1 5 
// 7 8 6 

// 1 2 3 
// 4 5 -1 
// 7 8 6 

// 1 2 3 
// 4 5 6 
// 7 8 -1 
