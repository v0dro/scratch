require 'nm_clapack.so'

class NMatrix
  module CLAPACK
    class << self
      def solve_gesv a, b
        a_trans = a.transpose
        b_cln = b.clone
        __solve_gesv__ a_trans, b_cln
      end
    end
  end
end