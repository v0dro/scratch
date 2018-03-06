require 'nmatrix'

class BlockMatrix
  def initialize mat, block
    @matrix = mat
    @block = block
  end

  def block_copy
  end
end

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

size = 8
n = NMatrix.zeros(size)
a = NMatrix.random(size)
b = NMatrix.random(size)

(0...size).step(4) do |i|
  ii = i + 4
  (0...size).step(4) do |j|
    jj = j + 4
    (0...size).step(4) do |k|
      kk = k + 4
      n[i...ii,j...jj] =
        n[i...ii, j...jj] + a[i...ii, k...kk].dot(b[k...kk,j...jj])
    end
  end
end

def mult a, b, c,block
  
end
