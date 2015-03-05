class NMatrix
  module CLAPACK
    class << self
      def solve n, nrhs, a, lda, ipiv, b, ldb
        raise TypeError, "Must be either float or complex dtype" if 
          !(a.float_dtype? or a.complex_dtype?)
          
        __solve__ n, nrhs, a, lda, ipiv, b, ldb
      end
    end
  end
end


class NMatrix
  module CLAPACK
    module UsableFunctions
      def solve b
        n    = self.nrows
        nrhs = b.ncols
        lda  = a.ncols
        ldb  = b.nrows
        ipiv = NMatrix.new([1,n], dtype: :int32)

        NMatrix::CLAPACK.solve n, nrhs, self, lda, ipiv, b, ldb
      end
    end 
  end
end