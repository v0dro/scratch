require 'nm_lapacke_atlas.so'
require 'nmatrix'

class NMatrix
  module LAPACKE_ATLAS
    def self.solve_gesv a, b
      __solve_gesv__ a, b
    end
  end
end