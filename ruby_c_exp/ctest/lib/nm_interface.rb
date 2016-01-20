module CTest
  class << self
    def creator
      n = NMatrix.new([2,2], [2.2, 3.4, 4.5, 6.4], dtype: :float64)
      __creator__ [1,2,3]
    end
  end  
end
