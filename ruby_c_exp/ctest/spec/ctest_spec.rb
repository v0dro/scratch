require 'ctest'

def creator
  mult = 0
  0.upto(500000) do
    mult += 4*4
  end

  puts mult
end
describe CTest do
  it "writes my first ext" do
    expect(CTest.hello_world).to eq('hello world!')
  end

  it "#creator" do
    # puts CTest.creator
    require 'benchmark'

    Benchmark.bm do |x|
      x.report { CTest.creator }
      x.report { creator }
    end
  end
end