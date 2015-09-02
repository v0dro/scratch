#include <iostream>
#include <vector>
#include <cmath>

#define GRID_SIZE 3
using namespace std;

class Grid;
void swap(int *num1, int* num2);
void pretty_print(Grid mat);
void copy_matrix(int clone[GRID_SIZE][GRID_SIZE], int original[GRID_SIZE][GRID_SIZE]);
void init_matrix(int state[GRID_SIZE][GRID_SIZE], int value);

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
  compute_heuristic(Grid &start, Grid &goal)
  {
    int h=0, m = 0;
    
    // calculate number of tiles out of place
    h = displaced_tiles(goal);

    // calculate manhattan distances
    int orig_row, orig_col, curr_row, curr_col;
    for (int i = 1; i < GRID_SIZE*GRID_SIZE; ++i) {
      this->find(i, &curr_row, &curr_col);
      goal.find(i, &orig_row, &orig_col);
      m += abs(curr_row - orig_row) + abs(curr_col - orig_col);
    }

    this->find(-1, &curr_row, &curr_col);
    goal.find (-1, &orig_row, &orig_col);
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
        if (this->get(i,j) == other.get(i,j)) {
          equal = false;
          break;
        }
      }
    }

    return equal;
  }
};

class AStar
{ 
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

  Grid *start, *goal;
  
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
    vector<Grid> positions, open_list, close_list;
    vector<int>  heuristic_values;
    int min_index, value, steps = 0;
    int ii = 0;

    do
    {
      positions = current.get_possible_positions();

      for (int i = 0; i < positions.size(); ++i) {
        heuristic_values.push_back(positions[i].compute_heuristic(*start, *goal));
      }

      min_index = get_min_index(heuristic_values);
      value     = heuristic_values[min_index];
      heuristic_values.clear();
      current   = positions[min_index].copy();
      pretty_print(current);
      positions.clear(); ++ii;
    } while(current.displaced_tiles(*goal) != 0);

    cout << "heuristic_values " << min_index << "\n";

    pretty_print(current);
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

void
init_matrix(int state[GRID_SIZE][GRID_SIZE], int value)
{
  for (int i = 0; i < GRID_SIZE; ++i) {
    for (int j = 0; j < GRID_SIZE; ++j) {
      state[i][j] = value;
    }
  }
};

int
main()
{
  AStar a;
  a.compute();
}
