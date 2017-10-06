# FMM code in Ruby
include Math

N = 100
LEVEL = 2

# Sort the morton indices and corresponding co-ordinates w.r.t the Morton index.
def sort_index_and_coords morton_index, x, y
  combined = morton_index.zip(x).zip(y).map!(&:flatten!)
  combined.sort_by! { |c| c[0] }
  combined = combined.transpose

  [combined[0], combined[1], combined[2]]
end

# Calculates the morton index for the given co-ordinates and the level.
# Works by first taking the Y co-ordinate, shifting it by 'level' bits, and taking the modulus
#   so the bit at that particular level is obtained. It does this same thing for X also.
#   After this, it will create a temp of (y << 1) | x. The index is first left shifted by 2. This
#   is done before the bitwise addition because in case of co-ords 0-1, shifting after addition leads
#   to higher result that expected.
def calculate_morton_index coords
  index = 0
  (LEVEL-1).downto(0) do |level|
    y = (coords[1] >> level) % 2
    x = (coords[0] >> level) % 2
    temp = (y << 1) | x
    index = index << 2
    index = index + temp
  end
  index
end

# 1: create a set of 100 points, each assigned with X and Y co-ordinates. Each number should be a rand between 0 and 1.

# u -> individual forces of target particles
# q -> individual forces of current particles
# x, y -> co ordinates of points
u, q, x, y = [], [], [], []

N.times do |i|
  x << rand
  y << rand
  q << 1.0
  u << 0.0
end

# Subdivide the points into their own cells and prepare a morton index for each of them.
# The end result of this step is a sorted index array that contains the morton index of each point and
#   X and Y arrays that correspond to the index
cell_coords = [0,0]
morton_index = []
N.times do |i|
  cell_coords[0] = (x[i] * (1 << LEVEL)).to_i
  cell_coords[1] = (y[i] * (1 << LEVEL)).to_i
  morton_index << calculate_morton_index(cell_coords)
end
morton_index, x, y = sort_index_and_coords morton_index, x, y

# P2M

N.times do |i|
  puts "shiz: #{morton_index[i]} #{(x[i] * 4).to_i} #{(y[i] * 4).to_i}"
end

# final step: check the results
result = []
N.times do |i|
  uid = 0.0
  N.times do |j|
    dx = x[i] - x[j]
    dy = y[i] - y[j]
    r = sqrt(dx*dx + dy*dy)
    uid += q[j] / r if r != 0
  end

  result << uid
end

# N.times do |i|
#   puts "#{x[i]} #{y[i]} #{result[i]}"
# end
