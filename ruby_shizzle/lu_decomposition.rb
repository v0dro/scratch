require 'numo/narray'
require 'solid_assert'
SolidAssert.enable_assertions
include Numo

def pp mat
  all = ""
  
  mat.shape[0].times do |i|
    str = ""
    mat.shape[1].times do |j|
      str << " #{'%0.2f' % mat[i,j]} "
    end
    str << "\n"
    all << str
  end

  puts all
end

# Compute LU factorization of matrix and return lower and upper matrix.
def lu_decompose matrix
  nrows = matrix.shape[0]
  ncols = matrix.shape[1]
  
  nrows.times do |i|
    (i...ncols).each do |j|
      if j >= i # upper
        sum = 0.0
        i.times do |k|
          sum += matrix[k,j]*matrix[i,k]
        end
        matrix[i,j] = matrix[i,j] - sum
      else # lower
        sum = 0.0
        j.times do |k|
          sum += matrix[k,j]*matrix[i,k]
        end
        matrix[i,j] = (matrix[i,j] - sum)/matrix[j,j]
      end
    end
  end

  matrix
end

def lu_decomp_in_place n
  nrows = n.shape[0]
  ncols = n.shape[1]
end

def test_lu
  a = NArray[
    [1.0,2.0,3.0,4.0],
    [5.0,6.0,3.0,1.0],
    [4.0,3.0,5.0,2.0],
    [1.0,4.0,3.0,5.0]
  ]
  lu = lu_decompose a
  pp lu
  # assert ((l.dot(u) - a).matrix_norm / a.matrix_norm) < 0.00005
end

test_lu

