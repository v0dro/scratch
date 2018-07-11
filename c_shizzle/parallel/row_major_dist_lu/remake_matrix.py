# script for rewriting a matrix from files outputted by individual processes
# requires the user to specify dims of global matrix, matrix and processor block size,
#   and file names according to process number.
# assumes that contents are written in one long string with spaces between individual numbers.
# Currently works only for cyclic matrices, not block cyclic.

import numpy as np
import math
import sys

np.set_printoptions(precision=5, linewidth=150, suppress=True)

def convert_to_number_list(string):
    return [float(s) for s in string.split(' ')]

"""
Create a big matrix from a bunch of files that have been written by MPI processes.
Works for block cyclic square matrices.

files     -- Dict of process number and file name.
major     -- String denoting storage format. Can be "row" or "col".
n         -- Number of rows and cols of the global matrix.
nb        -- Number of rows and cols of the matrix block.
pb        -- Number of rows and cols of the process block.
num_procs -- Number of processes that output this matrix.
"""
def get_big_matrix(files, major, n, nb, pb, num_procs):
    matrix = np.zeros([n,n])
    width = math.sqrt(num_procs)
    nb_r = int(math.sqrt(nb))
    nb_c = int(math.sqrt(nb))
    
    for proc, file_name in files.items():
        f = open(file_name, "r").read().rstrip()
        arr = convert_to_number_list(f)
        myrow = int(proc // width)
        mycol = int(proc - (myrow * width))

        if major == "col":
            for c in range(nb_c):
                for r in range(nb_r):
                    for pc in range(pb):
                        for pr in range(pb):
                            glob_row = int(myrow*pb + pr + pc*nb)
                            glob_col = int(c*nb + mycol*pb + r)   
                            index = pr + pc*pb + (r + c*pb)*nb
                            matrix[glob_row, glob_col] = arr[index]
        elif major == "row":
            for r in range(nb_r):
                for c in range(nb_c):
                    for pr in range(pb):
                        for pc in range(pb):
                            glob_row = int(mycol*pb + pr + pc*nb)
                            glob_col = int(c*nb + myrow*pb + r)   
                            index = pr + pc*pb + (r + c*pb)*nb
                            matrix[glob_col, glob_row] = arr[index]
        
    return matrix

def main(major):
    n = 8
    nb = 4
    pb = 2
    
    input_files = {
        0 : "0input.txt",
        1 : "1input.txt",
        2 : "2input.txt",
        3 : "3input.txt"
    }
    
    output_files = {
        0 : "0.txt",
        1 : "1.txt",
        2 : "2.txt",
        3 : "3.txt"
    }

    num_procs = len(input_files.keys())

    print("printing input ... \n")
    print(get_big_matrix(input_files, major, n, nb, pb, num_procs))
    
    print("printing output ... \n")
    output = get_big_matrix(output_files, major, n, nb, pb, num_procs)
    print(output)

    print("product of lower and upper ... \n")
    upper = np.triu(output)
    lower = np.tril(output)
    np.fill_diagonal(lower, 1)
    print(np.matmul(lower, upper))

if __name__ == "__main__":
    major = sys.argv[1] if len(sys.argv) > 1 else None
    if major is None:
        major = "col"

    main(major)
