require "nmatrix"
require 'nm_clapack'


describe NMatrix::CLAPACK do
  context "#solve_gesv" do
    [:float32, :float64].each do |dtype|
      it "solves the equation" do
        a = NMatrix.new([3,3], [76, 25, 11,
                                27, 89, 51,
                                18, 60, 32], dtype: dtype)
        b = NMatrix.new([3,1], [10,
                                 7,
                                43], dtype: dtype)

        expect(NMatrix::CLAPACK.solve_gesv(a,b)).to eq(
          NMatrix.new([3,1], 
            [-0.661082, 
              9.456125, 
            -16.014625], dtype: dtype)
        )
      end
    end
  end

  context "benchmark" do
    it "benchmarks CLAPACK with openBLAS" do
      require 'benchmark'

      Benchmark.bm do |x|
        x.report do
          10000.times do
            a = NMatrix.new([3,3], [76, 25, 11,
                                    27, 89, 51,
                                    18, 60, 32], dtype: :float64)
            b = NMatrix.new([3,1], [10,
                                     7,
                                    43], dtype: :float64)
            NMatrix::CLAPACK.solve_gesv(a,b)
          end
        end
      end
    end
  end
end