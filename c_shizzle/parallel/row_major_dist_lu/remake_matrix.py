# script for rewriting a matrix from files outputted by individual processes
# requires the user to specify dims of global matrix, matrix and processor block size,
#   and file names according to process number.
# assumes that contents are written in one long string with spaces between individual numbers.
# Currently works only for cyclic matrices, not block cyclic.

import numpy as np
import math
import sys
import argparse

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

def generate_file_dict(f_name, procs):
    d = {}
    for proc in range(0,procs):
        d[proc] = str(proc) + f_name + ".txt"

    return d

def main(args):
    n = 8
    nb = 4
    pb = 2
    major = args.major

    if args.default:
        input_files = generate_file_dict("input", 4)
        output_files = generate_file_dict("", 4)

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
    else:
        files = generate_file_dict(args.file, 4)
        print(files)
        print("printing " + args.file + "...")
        a = get_big_matrix(files, major, n, nb, pb, 4)
        print(a)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Remake matrix output by MPI processes.")
    parser.add_argument('--file', dest="file",
    help="file extension name to read.")
    parser.add_argument('--major', dest="major", default="col",\
    help="row or col major (default row)")
    parser.add_argument('--default', dest="default", help="run with defaults")
    args = parser.parse_args()

    main(args)
