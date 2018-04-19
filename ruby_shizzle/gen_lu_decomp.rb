require 'numo/narray'
require 'numo/linalg'
Fiddle.dlopen("/usr/lib/libopenblas.so")
Numo::Linalg::Lapack.dlopen("/usr/lib/liblapacke.so.3")

N = 8
n = Numo::DFloat.new(N, N).seq

N.times do |i|
  N.times do |j|
    n[i,j] = (i + j).to_f
  end
end

puts n.inspect

lu, p = Numo::Linalg.lu_fact n

puts lu.inspect
