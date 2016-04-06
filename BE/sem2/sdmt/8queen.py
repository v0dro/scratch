import json

N = 8

def is_valid_position(board, row, col):
  # check columns on the left
  for i in xrange(0, col):
    if board[row][i] == 1:
      # print board
      return False

  # check left upper diagonal
  i = row
  j = col

  while i >= 0 and j >= 0:
    if board[i][j] == 1:
      return False

    i -= 1
    j -= 1

  # check left lower diagonal
  i = row
  j = col

  while i < N and j >= 0:
    if board[i][j] == 1:
      return False

    i += 1
    j -= 1

  return True

def queen_solve(board, col):
  if (col >= N):
    return True

  for i in xrange(0, N):
    if is_valid_position(board, i, col):
      board[i][col] = 1

      if queen_solve(board, col + 1):
        return True
      
      board[i][col] = 0

  return False

if __name__ == '__main__':
  board = []

  for i in xrange(0,N):
    board.append([0]*N)

  board[int(json.load(open('a.json'))['start'])][0] = 1
  queen_solve(board, 1)

  for i in xrange(0,N):
    print board[i]