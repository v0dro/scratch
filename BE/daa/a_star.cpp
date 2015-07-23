#include <iostream>
using namespace std;

class AStar
{
  int start[3][3], goal[3][3];
  void
  init_state(int state[3][3])
  {
    for (int i = 0; i < 3; ++i) {
      for (int j = 0; j < 3; ++j) {
        state[i][j] = 0;
      }
    }
  };

  void
  copy_matrix(int clone[3][3], int original[3][3])
  {
    for (int i = 0; i < 3; ++i) {
      for (int j = 0; j < 3; ++j) {
        clone[i][j] = original[i][j];
      }
    }
  }

  int
  heuristic_value_for(int current[3][3])
  {

  }

  void
  restore_to_original(int current[3][3])
  {

  }

  void
  choose_matrix_with_least_heuristic_and_overwrite_current(
    int current[3][3], 
    int orig_blank_row, 
    int orig_blank_col, 
    char least_state)
  {
    
  }

  void
  move_up(int current[3][3])
  {

  }

  void
  move_down(int current[3][3])
  {

  }

  void
  move_right(int current[3][3])
  {

  }

  void
  move_left(int current[3][3])
  {

  }

  int
  apply_middle(int current[3][3])
  {
    int heuristic_value, blank_row=1, blank_col=1, temp;
    char least_state;

    move_up(current);
    heuristic_value = heuristic_value_for(current);
    restore_to_original(current, blank_row, blank_col);
    least_state = 'U';

    move_down(current);
    temp = heuristic_value_for(current)
    if (temp < heuristic_value) {
      heuristic_value = temp;
      least_state = 'D';
    }
    restore_to_original(current, blank_row, blank_col);

    move_right(current);
    temp = heuristic_value_for(current);
    if (temp < heuristic_value) {
      heuristic_value = temp;
      least_state = 'R';
    }
    restore_to_original(current, blank_row, blank_col);

    move_left(current);
    temp = heuristic_value_for(current);
    if (temp < heuristic_value) {
      heuristic_value = temp;
      least_state = 'L';
    }
    restore_to_original(current, blank_row, blank_col);

    choose_matrix_with_least_heuristic_and_overwrite_current(
      current, blank_row, blank_col, least_state);

    return heuristic_value;
  } 

  int
  apply_edge(int current[3][3])
  {
    
  }

  int
  apply_corner(int corner[3][3])
  {

  }

  char
  type_of_case(int current[3][3])
  {
    // check whether blank is present in middle, corner or edge.
  }

  int
  compute_heuristic(int current[3][3])
  {
    // based on whether it is a edge, middle or corner case, change the matrix
    // once, then compute the heuristic function and store it in a value.
    char mat_case = type_of_case(current);

    if (mat_case == 'M') {
      return apply_middle(current);
    }
    else if (mat_case == 'E') {
      return apply_edge(current);
    }
    else if (mat_case == 'C') {
      return apply_corner(current);
    }
    else {
      cout << "Cannot recognize state " << mat_case << endl;
    }




    // compute value of heuristic for each iteration of current and choose the
    // matrix that yields the least value.

    // over write current by storing the chosen matrix in it and return the
    // calculated heuristic value.
  }

 public:
  AStar()
  {
    init_state(start);
    init_state(goal);
  };

  void
  accept_start()
  {
    int test[9] = {1,2,3,8,6,-1,7,5,4};
    for (int i = 0; i < 3; ++i) {
      for (int j = 0; j < 3; ++j) {
        start[i][j] = test[i*j]
      }
    }
  };

  void
  accept_goal()
  {
    int test[9] = {1,2,3,4,5,6,7,8,-1};
    for (int i = 0; i < 3; ++i) {
      for (int j = 0; j < 3; ++j) {
        goal[i][j] = test[i*j];
      }
    }
  }

  void
  compute()
  { 
    int current[3][3], heuristic_value;
    copy_matrix(current, this->start);

    while (heuristic_value = compute_heuristic(current)) {
      pretty_print(heuristic_value, current);
    }
    // i need to run the current matrix through the heuristic function
    // and choose the matrix that returns the least heuristic value.
    // I also need to know the value of the heuristic and the matrix that
    // is finally chosen by the algorithm.
    // This chosen matrix then needs to be saved in the current matrix.
    // have a current 3x3 matrix that stores the current state
    // 
  }
};

int
main()
{
  // accept start 3x3 matrix.
  // accept goal 3x3 matrix.
  // print states and reach goal state.
}