# FMM code in Ruby
include Math

N = 100
LEVEL = 3

def get_coords morton_index
  level = 0
  x, y = 0, 0
  while morton_index > 0
    y += (morton_index & 1) << level
    morton_index >>= 1
    x += (morton_index & 1) << level
    morton_index >>= 1
    level += 1
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
def calculate_morton_index coords, lvl
  index = 0
  coords = coords.dup
  0.upto(lvl-1) do |level|
      index += (coords[1] & 1) << (2*level)
      coords[1] >>= 1
      index += (coords[0] & 1) << (2*level + 1)
      coords[0] >>= 1
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

# Create an array that points to the cells in the top level that are non-empty.
def calculate_non_empty_cells morton_index
  non_empty_cells = []
  ic = -1
  morton_index.each do |m|
    if ic != m
      ic = m
      non_empty_cells << m
    end
  end
  non_empty_cells
end

# u -> individual forces of target particles
# q -> individual forces of current particles
# x, y -> co ordinates of points
u, q = [], []
x = [0.17280346741136454,0.19115832834401236,0.7345312180072571,0.22560823230249472,0.4183481071265637,0.4047061625817101,0.47374126530527305,0.3257656381156552,0.8708332649320957,0.823947137207495,0.5429211004029263,0.9406693380393317,0.3217285026333436,0.04949142903620796,0.46497223740313964,0.9463026812914828,0.970518265107997,0.3330349673804196,0.8090731528518333,0.45604083652238947,0.11567562359095918,0.9674207796389165,0.46869632661239635,0.9414846321685119,0.08023256929635059,0.5240716834510082,0.6960286062890801,0.7999409478234238,0.5704194349813656,0.9910910773689678,0.7024507167754349,0.26682714693002296,0.707222504801747,0.2576434670296329,0.9191937080396094,0.2913808977769444,0.2469866919609952,0.7688689599797462,0.6872629023953133,0.444311195531766,0.1195744829572638,0.8219112660934099,0.4347967133696745,0.42811329517150454,0.6226077824577202,0.09812096699988604,0.8565672814518892,0.3239637700085993,0.4465774687857481,0.8126202775235158,0.8528033270146841,0.46855650356828904,0.011430166360157368,0.053380606127747754,0.2095451452041761,0.16324917150006235,0.7152996144385662,0.6982752249215572,0.8681078877237871,0.8254643899525447,0.8399587967762273,0.6278184307567404,0.816520892181849,0.6638061944997199,0.9503188681595904,0.2919594574984573,0.5236742729386399,0.45002944570342684,0.966571170068425,0.5295618791588087,0.12450275719737414,0.6865796793168119,0.025638588153187203,0.8493800650472368,0.8804374834268026,0.6039665094740906,0.9427831092651285,0.5412477392245699,0.9689182434945669,0.30198125483096816,0.031892589754185074,0.8441119591345674,0.5045398647767546,0.7014029142381129,0.8111972811031151,0.4720241250308256,0.5228395672741267,0.39258558734412197,0.5985502078985594,0.5384406154501119,0.0729817825447322,0.0398378678673208,0.7514107529286894,0.4922358842589628,0.19898542664629149,0.2553791681856413,0.05958218725901776,0.8169602586890503,0.3632040502778112,0.9316539883403074]

y = [0.25013380515409545,0.8105290654776193,0.31725098545584685,0.8872170255712382,0.21869009521813176,0.615867640678987,0.04538620075637667,0.5388987104291129,0.5429111570690346,0.05781336833640838,0.8251503914819508,0.5560895227350965,0.7650894991397028,0.27014455798872017,0.8744569729062712,0.04392368814291636,0.44847625598521235,0.329640361447467,0.9424282484927913,0.552775550332451,0.7214696167260293,0.783250905637877,0.03480724402572177,0.7955021130479697,0.9894799271392315,0.8005242296546681,0.4219479522707924,0.5102991780656028,0.6179692806234449,0.5792095402732351,0.32125265698738803,0.5586919210252812,0.5723098733123593,0.9495476178245952,0.1627835413470109,0.9647535503637135,0.46902723083750286,0.1111736653772144,0.5376416645352932,0.23476448565890673,0.43967920449293063,0.7178688907751729,0.842300925313458,0.5326070226087067,0.4889104444290212,0.7197070126518983,0.24237020468250048,0.46353094765971437,0.42489192391730635,0.996248986542843,0.4922164406268552,0.5769653980403654,0.9598291103158854,0.49954280648760063,0.3635622819821338,0.4060541313342747,0.9228075447754257,0.43525878083496994,0.15120035494952744,0.03333637452576044,0.033120998627662046,0.028803386567355593,0.763714396821058,0.8077786532863226,0.4494698261748855,0.6618061641362114,0.22444665584861467,0.12617960612817514,0.452451044657295,0.7773681363737674,0.44352706948038323,0.4184556338327198,0.2581389673436083,0.21792143960308208,0.9548890924289672,0.838413949876847,0.713444474485892,0.7851761341929943,0.9059653508430493,0.4908373355833979,0.7401002144876803,0.41349256617809516,0.3390437797834398,0.5645584576620254,0.5583700254001736,0.04776686894398552,0.0421315402578486,0.4842761410151576,0.15063217995932243,0.6132140165103924,0.9223203080209729,0.8947755376275118,0.8461834673524026,0.7813877821052738,0.9133729544286112,0.7377271845706209,0.960432744734166,0.43881921424525117,0.7826509384987466,0.6441386836437457]

N.times do |i|
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
  morton_index << calculate_morton_index(cell_coords, LEVEL)
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
multipoles = Array.new TOTAL_CELLS, 0.0
locals = Array.new TOTAL_CELLS, 0.0

# Then we need level offsets for determininpg the places in the multipole array where the level changes.
level_offsets = calculate_level_offsets
non_empty_cells = calculate_non_empty_cells morton_index
nleaf = non_empty_cells.size

# P2M
#   The P2M step basically takes the current particles in the domain and lumps them together for each
#   cell as multipoles. For this purpose, it needs to loop over each particle inside each cell, add their
#   forces and store it in the multipoles array starting from the offset for the highest level.
# offsets.each_cons(2) do |a, b|
#  a.upto(b-1) do |index|
#    multipoles[level_offsets[LEVEL] + morton_index[index]] += q[index]
#  end
# end
ix = [0,0]
nx = 2**LEVEL
N.times do |n|
  ix[0] = (x[n] * nx).to_i
  ix[1] = (y[n] * nx).to_i
  j = calculate_morton_index ix, LEVEL
  multipoles[level_offsets[LEVEL] + j] += q[n]
end

# M2M
#   M2M basically lumps together the already formed multipoles into several other multipoles. It does this
#   and stores the multipoles in the multipoles array. I need to access the mutipoles of each level 3 cell,
#   take a sum of the mutipoles that are part of the larger level 2 cells and store the sum inside that.
#   The multipole calculation must be done for each and every level other than the topmost.

#   We do not need to store the morton indexes of the lower levels (meaning the ones with lesser cells)
#   since the index can be easily found out with child/4.
(LEVEL).downto(3) do |level|
  nleaf.times do |nl|
    idx = non_empty_cells[nl]
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
puts "level offsets: #{level_offsets}"
(2).upto(LEVEL) do |level|
  nx = 2**level
  total_cells = nx * nx

  total_cells.times do |i|
    ix = get_coords i
    total_cells.times do |j|
      jx = get_coords j
      # Figure out if the outer cells are far enough to be eligible for a multipole
      #   and close enough to be within their parents scope.
      if (ix[0] / 2 - jx[0] / 2).abs <= 1 && (ix[1] / 2 - jx[1] / 2).abs <= 1
        if (ix[0] - jx[0]).abs > 1 || (ix[1] - jx[1]).abs > 1
          dx = (ix[0] - jx[0]).to_f / nx
          dy = (ix[1] - jx[1]).to_f / nx
          r  = sqrt(dx*dx + dy*dy)
          i = calculate_morton_index ix, LEVEL
          j = calculate_morton_index jx, LEVEL
          locals[level_offsets[level] + i] += multipoles[level_offsets[level] + j] / r
        end
      end
    end
  end
end

# L2L
#  This step further expands on the already expanded local interactions and adds the sums of their
#  parents into the children. This is a local to local expansion hence it will only apply from level 2
#  onwards.
3.upto(LEVEL) do |level|
  nleaf.times do |cell|
    j = non_empty_cells[cell]
    locals[level_offsets[level] + j] += locals[level_offsets[level-1] + j/4]
  end
end

# L2P
#  This level creates interactions between the actual points and the topmost local expansions (which now
#  have been added with the bottom level expansions).

#  We use the offsets array in this case (not used here) so that we get direct access to the places where
#  elements from one cell end and the other begin. In case we are storing only non-empty cells (which is
#  not the case in the current code) it will help us to iterate over the cells which only contain some
#  particles.
nleaf.times do |leaf|
  (offsets[leaf]).upto(offsets[leaf+1]-1) do |j|
    u[j] += locals[level_offsets[LEVEL] + non_empty_cells[leaf]]
  end
end

# P2P
#  In this step we only calculate interactions between neighbour particles since we had not factored those
#  into the previous steps. This calculation is done directly between the particles.
ix  = [0,0]
jx = [0,0]
nx = 2**LEVEL
nleaf.times do |ic|
  ix = get_coords non_empty_cells[ic]
  nleaf.times do |jc|
    jx = get_coords non_empty_cells[jc]
    if (ix[0] - jx[0]).abs <= 1 && (ix[1] - jx[1]).abs <= 1
      offsets[ic].upto(offsets[ic+1]-1) do |i|
        offsets[jc].upto(offsets[jc+1]-1) do |j|
          dx = x[i] - x[j]
          dy = y[i] - y[j]
          r = sqrt(dx*dx + dy*dy)
          if r != 0
            u[i] += q[j] / r
          end
        end
      end
    end
  end
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
end
