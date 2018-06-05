# script for rewriting a matrix from files outputted by individual processes
# requires the user to specify dims of global matrix, matrix and processor block size,
#   and file names according to process number.
# assumes that contents are written in one long string with spaces between individual numbers.
# Currently works only for cyclic matrices, not block cyclic.

import numpy as np
import math

np.set_printoptions(precision=5, linewidth=150)

def convert_to_number_list(string):
    return [float(s) for s in string.split(' ')]

def get_big_matrix(files, major, n, nb, process_block_r, process_block_c, num_procs):
    matrix = np.zeros([n,n])
    width = math.sqrt(num_procs)
    for proc, file_name in files.items():
        f = open(file_name, "r").read().rstrip()
        arr = convert_to_number_list(f)
        myrow = int(proc // width)
        mycol = int(proc - (myrow * width))
        
        for r in range(nb):
            for c in range(nb):
                glob_row = int(myrow*nb + r)
                glob_col = int(mycol*nb + c)
                if major == "row":
                    index = r*nb + c
                elif major == "col":
                    index = r + c*nb

                matrix[glob_row, glob_col] = arr[index]

    return matrix

def main():
    major = "col"
    n = 8
    nb = 4
    process_block_c = 4
    process_block_r = 4
    
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
    print(get_big_matrix(input_files, major, n, nb, process_block_r, process_block_c, num_procs))
    print("printing output ... \n")
    output = get_big_matrix(output_files, major, n, nb, process_block_r, process_block_c, num_procs)
    print(output)
    print("\n")
    upper = np.triu(output)
    lower = np.tril(output)
    np.fill_diagonal(lower, 1)
    print(np.matmul(lower, upper))

if __name__ == "__main__":
    main()
