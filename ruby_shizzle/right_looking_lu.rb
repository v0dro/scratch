require 'numo/narray'
require 'numo/linalg'
Fiddle.dlopen("/usr/lib/libopenblas.so")
Numo::Linalg::Lapack.dlopen("/usr/lib/liblapacke.so")

def lu_decompose matrix
  nrows = matrix.shape[0]
  ncols = matrix.shape[1]
  
  nrows.times do |j|
    (j...ncols).each do |i| # lower
      a = 0
      (0...j).each do |k|
        a = a + matrix[i,k]*matrix[k,j]
      end
      matrix[i,j] = matrix[i,j] - a
    end
    
    (j...ncols).each do |i| # upper
      a = 0.0
      (0...j).each do |k|
        a = a + matrix[j,k]*matrix[k,i]
      end
      matrix[j,i] = (matrix[j,i] - a) / matrix[j,j]
    end
  end

  matrix
end

N = 8
BLK_SIZE = 2
n = Numo::NArray[[56, 57, 58, 59, 60, 61, 62, 63],
                 [48, 49, 50, 51, 52, 53, 54, 55],
                 [40, 41, 42, 43, 44, 45, 46, 47],
                 [32, 33, 34, 35, 36, 37, 38, 39],
                 [24, 25, 26, 27, 28, 29, 30, 31],
                 [16, 17, 18, 19, 20, 21, 22, 23],
                 [8, 9, 10, 11, 12, 13, 14, 15],
                 [0, 1, 2, 3, 4, 5, 6, 7]]
n = Numo::DFloat.cast n
n_dup = n.dup
NBLOCKS = N / BLK_SIZE

(0..NBLOCKS).step(BLK_SIZE) do |k|
  lu, p = Numo::Linalg.lu_fact n[k...(k+BLK_SIZE), k...(k+BLK_SIZE)]
  l = lu.tril
  u = lu.triu
  BLK_SIZE.times { |i| l[i,i] = 1.0 }

  # distribute L and U blocks amongst the row and col.
  l_inv = Numo::Linalg.inv l
  u_inv = Numo::Linalg.inv u
  ((k+BLK_SIZE)..NBLOCKS).step(BLK_SIZE) do |i|
    n[k...(k+BLK_SIZE), i...(i+BLK_SIZE)] = l_inv.dot(n[k...(k+BLK_SIZE), i...(i+BLK_SIZE)])
    n[i...(i+BLK_SIZE), k...(k+BLK_SIZE)] = n[i...(i+BLK_SIZE), k...(k+BLK_SIZE)].dot(u_inv)
  end

  ((k+BLK_SIZE)..NBLOCKS).step(BLK_SIZE) do |i|
    ((k+BLK_SIZE)..NBLOCKS).step(BLK_SIZE) do |j|
      n[i...(i+BLK_SIZE), j...(j+BLK_SIZE)] -=
        n[i...(i+BLK_SIZE), k...(k+BLK_SIZE)].dot(n[k...(k+BLK_SIZE), j...(j+BLK_SIZE)])
    end
  end
end

n_dup = Numo::NArray[[2,-1,2],
                     [-4,6,3],
                     [-4,-2,8]]
n_dup = Numo::DFloat.cast n_dup
p Numo::Linalg.lu_fact(n_dup)

puts "-------"

p lu_decompose(n_dup)


