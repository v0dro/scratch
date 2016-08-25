require_relative 'nm_atlas_clapack.so'
require 'benchmark'
# require 'nm_atlas_clapack/clapack'
# require 'nm_atlas_clapack/atlas'

a = Fact.new

Benchmark.bm do |x|
  x.report("a.factorial") do
    a.factorial(60)
  end
end

puts a.factorial(60)
