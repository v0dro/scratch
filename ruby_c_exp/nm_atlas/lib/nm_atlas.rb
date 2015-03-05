require 'nm_atlas.so'
require 'nmatrix'

class NMatrix
  module CLAPACK_ATLAS
    def self.solve_gesv a, b
      __solve_gesv__ a, b
    end
  end
end