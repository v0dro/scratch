# FMM code in Ruby
include Math

N = 100
LEVEL = 3

def get_coords morton_index
  x, y = 0, 0
  (LEVEL-1).downto(0) do |level|
    temp = morton_index >> level*2
    y = y << 1 | temp % 2
    x = x << 1 | (temp >> 1) % 2
  end

  [x,y]
end

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

# Calculates the offset array for noting the places in the morton index array where a new
#   index begins. We append N at the end of this array so that the last element in the morton_index
#   array is also factored in.
def calculate_offset_array morton_index
  offsets = [0]
  c = morton_index[0]
  offset_i = 1
  morton_index.each_with_index do |mi, i|
    if c != mi
      c = mi
      offsets[offset_i] = i
      offset_i += 1
    end
  end
  offsets << N

  offsets
end

def calculate_level_offsets
  level_offsets = [0]
  memo = 0
  0.upto(LEVEL-1) do |l|
    memo += 2**l * 2**l
    level_offsets << memo
  end

  level_offsets
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

# If there are some boxes that do not contain any particle, the morton index for that box will not exist.
# Therefore, the morton_index array can be smaller than N.
cell_coords = [0,0]
morton_index = []
N.times do |i|
  cell_coords[0] = (x[i] * (1 << LEVEL)).to_i
  cell_coords[1] = (y[i] * (1 << LEVEL)).to_i
  morton_index << calculate_morton_index(cell_coords)
end
morton_index, x, y = sort_index_and_coords morton_index, x, y

# Offsets will be used later since it is not an important feature for implementation of FMM at this scale.
# The offsets should be stored so that we can effectively jump between non-empty indices.
# We also need arrays for storing the multipoles and the local expansions.
offsets = calculate_offset_array morton_index

# Number of multipoles = total number of cells for all levels. Add +1 so that the 0th level multipole
#   can also be factored into the multipoles array. This has no real advantage but it keeps the code
#   simple since that way there will no exceptions and it will blend properly with the offsets.
TOTAL_CELLS = (1..LEVEL).inject(0) { |memo, l| memo += (2**l) * (2**l); memo } + 1
multipoles = Array.new TOTAL_CELLS, 0
locals = Array.new TOTAL_CELLS, 0

# Then we need level offsets for determining the places in the multipole array where the level changes.
level_offsets = calculate_level_offsets

# P2M
#   The P2M step basically takes the current particles in the domain and lumps them together for each
#   cell as multipoles. For this purpose, it needs to loop over each particle inside each cell, add their
#   forces and store it in the multipoles array starting from the offset for the highest level.
offsets.each_cons(2).with_index do |arr, offset_index|
  a = arr[0]
  b = arr[1]
  a.upto(b-1) do |index|
    multipoles[level_offsets[LEVEL] + morton_index[index]] += q[index]
  end
end
# M2M
#   M2M basically lumps together the already formed multipoles into several other multipoles. It does this
#   and stores the multipoles in the multipoles array. I need to access the mutipoles of each level 3 cell,
#   take a sum of the mutipoles that are part of the larger level 2 cells and store the sum inside that.
#   The multipole calculation must be done for each and every level other than the topmost.

#   We do not need to store the morton indexes of the lower levels (meaning the ones with lesser cells)
#   since the index can be easily found out with child/4.
(LEVEL).downto(1) do |level|
  total_cells = 2**level * 2**level
  total_cells.times do |idx|
    child_index = level_offsets[level] + idx
    parent_index = level_offsets[level-1] + idx / 4
    multipoles[parent_index] += multipoles[child_index]
  end
end

# M2L
#  The M2L step expands on the previous multipoles and creates local interactions. The 'locals' array
#  comes into play over here. It basically takes the morton index of a cell, figures out the co-ordinates
#  of the cell, and checks whether the other cell being considered is near or far. These two cells need
#  to be far from each other, however, their parents must be close to each other (abs distance must be
#  <= 1). So we also factor in the index of their parents into the calculation. We consider the condition
#  that the neihbours of the parents should not be considered because we want to consider only those red
#  boxes that are not too far (more than one parent box) from the blue box.

#  Q -> How do you determine which cells are to be considered for evaluation?
#  A -> For each level, iterate over the number of cells that can be present in that level. Get the
#       co-ordinates of the number. Then, in an inner loop, iterate over all the other cells of the level.

# Formula to use:
#   force = m / sqrt(dx*dx + dy*dy)

N.times do |i|
  puts "shiz: #{offsets[i]} #{morton_index[i]} #{(x[i] * 8).to_i} #{(y[i] * 8).to_i}"
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
