require 'nm_lapacke.so'
require 'nmatrix'

class NMatrix
  module LAPACKE
    def self.solve_gesv a, b
      b_cln = b.clone
      __solve_gesv__ a, b_cln
    end
  end
end