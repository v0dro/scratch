# Class for holding multi index on Vector and DataFrame.
class OldMultiIndex
  include Enumerable

  def each(&block)
    to_a.each(&block)
    self
  end

  def map(&block)
    to_a.map(&block)
  end

  attr_reader :relation_hash
  attr_reader :size
  attr_reader :values
  
  # Initialize a MultiIndex by passing a tuple of indexes. The order assigned
  # to the multi index corresponds to the position of the tuple in the array
  # of tuples.
  # 
  # Although you can create your own hierarchially indexed Vectors and DataFrames,
  # this class currently contains minimal error checking and is mainly used 
  # internally for summarizing, splitting and grouping of data.
  # 
  # == Arguments
  # 
  # * +source+ - The array of arrays from which the multi index is to be created.
  # 
  # == Usage
  # 
  #   tuples = [:a,:a,:b,:b].zip([:one,:two,:one,:two])
  #   #=> [[:a, :one], [:a, :two], [:b, :one], [:b, :two]]
  #   Daru::MultiIndex.new(tuples)
  def initialize source
    @relation_hash = {}
    @size = source.size
    values = Array.new(source.size) { |i| i } if values.nil?
    create_relation_hash source, values
    @relation_hash.freeze
    @values = values
  end

  def [] *indexes
    indexes.flatten!
    location = indexes[0]

    if location.is_a?(Symbol)
      result = read_relation_hash @relation_hash, indexes, 0
      return nil if result.nil?
      result.is_a?(Integer) ? result : OldMultiIndex.new(*make_tuples(result))
    else
      case location
      when Integer
        self[@relation_hash.keys[location]]
      when Range
        first = location.first
        last  = location.last

        hsh = {}
        first.upto(last) do |index|
          key = @relation_hash.keys[index]
          hsh[key] = read_relation_hash(@relation_hash, [key], 0)
        end
        OldMultiIndex.new(*make_tuples(hsh))
      end
    end
  end

  def + other
    other.flatten!
    tuples = to_a
    raise ArgumentError, "Incomplete tuple #{other}" unless 
      tuples.all? { |t| t.size == other.size }

    OldMultiIndex.new(tuples << (other))
  end

  def empty?
    @relation_hash.empty?
  end

  # Compare two MultiIndex objects for equality based on the contents of their
  # relation hashes. Does not take object_id into account.
  def == other
    return false if size != other.size
    deep_compare @relation_hash, other.relation_hash
  end

  # Convert a MultiIndex back to tuples (array of arrays). Will retain the 
  # order of creation.
  def to_a
    make_tuples(@relation_hash)[0]
  end

  # Completely duplicate a MultiIndex object and its contents.
  def dup
    OldMultiIndex.new to_a
  end
  
  # Check whether a tuple or identifier number exists in the multi index. 
  # The argument *tuple* can either a complete or incomplete tuple, or a number.
  def include? tuple
    tuple = [tuple] unless tuple.is_a?(Array)
    tuple.flatten!
    !!read_relation_hash(@relation_hash, tuple, 0)
  end

  # Obtain the tuple that correponds with the indentifier number.
  # 
  # == Arguments
  # 
  # * +key+ - A number for which the tuple is to be obtained.
  # 
  # == Usage
  #   
  #   mi.key(3) #=> [:a,:two,:baz]
  def key key
    tuple = find_tuple_for(@relation_hash, key)
    tuple.empty? ? nil : tuple
  end

  def size
    to_a.size
  end

 private

  # Deep compare two hashes
  def deep_compare this, other
    if this == other
      return true if this.is_a?(Integer) and other.is_a?(Integer)
      this.each_key do |key|
        deep_compare this[key], other[key]
      end
    else
      return false
    end
    true
  end

  # Create tuples out of the relation hash based on the order of the identifier
  # numbers. Returns an array of arrays containing the tuples and another
  # containing their corresponding index numbers.
  def make_tuples relation_hash
    tuples = []
    new_vals = []
    values.each do |number|
      tuple = find_tuple_for(relation_hash, number)
      unless tuple.empty?
        tuples << tuple
        new_vals << number
      end
    end
    [tuples,new_vals]
  end

  # Finds and returns a single tuple for a particular identifier number
  def find_tuple_for relation_hash, number
    tuple = []
    search_for_number number, relation_hash, tuple
    tuple.reverse
  end

  # Search for a number and store its corresponding tuple in *tuple*. Returns 
  # true if the number is successfully found.
  def search_for_number number, relation_hash, tuple
    found = false
    relation_hash.each_key do |key|
      value = relation_hash[key]
      if value.is_a?(Hash)
        if search_for_number(number, value, tuple)
          tuple << key
          found = true
        end
      elsif value == number
        tuple << key
        found = true
      end
    end

    found
  end

  # Read the relation hash and return a sub-relation hash or the number to which
  #   indexes belogs to.
  def read_relation_hash relation_hash, indexes, index
    identifier = indexes[index]
    value      = relation_hash[identifier]

    indexes[index+1].nil? ? value : read_relation_hash(value,indexes,index+1)
  end

  # Create the relation hash from supplied tuples.
  def create_relation_hash source, values
    source.each_with_index do |tuple, idx|
      populate @relation_hash, tuple, 0, values[idx]
    end   
  end

  def populate relation_hash, tuple, index, number
    identifier = tuple[index]

    if identifier
      if tuple[index+1] 
        relation_hash[identifier] ||= {}
      else
        relation_hash[identifier] = number
        return
      end
      populate relation_hash[identifier], tuple, index+1, number
    end
  end
end

class MultiIndex
  include Enumerable

  def each(&block)
    to_a.each(&block)  
  end

  def map(&block)
    to_a.map(&block)
  end

  attr_reader :labels

  def levels
    @levels.map { |e| e.keys }
  end

  def initialize opts={}
    labels = opts[:labels]
    levels = opts[:levels]

    raise ArgumentError, 
      "Must specify both labels and levels" unless labels and levels
    raise ArgumentError,
      "Labels and levels should be same size" if labels.size != levels.size
    raise ArgumentError,
      "Incorrect labels and levels" if incorrect_fields?(labels, levels)

    @labels = labels
    @levels = levels.map { |e| Hash[e.map.with_index.to_a]}
  end

  def incorrect_fields? labels, levels
    max_level = levels[0].size

    correct = labels.all? { |e| e.size == max_level }
    correct = levels.all? { |e| e.uniq.size == e.size }

    !correct
  end

  private :incorrect_fields?

  def self.from_arrays arrays
    levels = arrays.map { |e| e.uniq.sort_by { |a| a.to_s  } }
    labels = []

    arrays.each_with_index do |arry, level_index|
      label = []
      level = levels[level_index]
      arry.each do |lvl|
        label << level.index(lvl)
      end

      labels << label
    end

    MultiIndex.new labels: labels, levels: levels
  end

  def self.from_tuples tuples
    from_arrays tuples.transpose
  end

  def [] *key
    key.flatten!
    case
    when key[0].is_a?(Range) then retrieve_from_range(key[0])
    when key[0].is_a?(Integer) then try_retrieve_from_integer(key[0])
    else retrieve_from_tuples(key)
    end
  end

  def try_retrieve_from_integer int
    return retrieve_from_tuples(int) if @levels[0].has_key?(int)
    int
  end

  def retrieve_from_range range
    MultiIndex.from_tuples(range.map { |index| key(index) })
  end

  def retrieve_from_tuples key
    chosen = []

    key.each_with_index do |k, depth|
      level_index = @levels[depth][k]
      label = @labels[depth]
      chosen = find_all_indexes label, level_index, chosen
    end

    return chosen[0] if chosen.size == 1
    return multi_index_from_multiple_selections(chosen)              
  end

  def multi_index_from_multiple_selections chosen
    MultiIndex.from_tuples(chosen.map { |e| key(e) })
  end

  def find_all_indexes label, level_index, chosen
    if chosen.empty?
      label.each_with_index do |lbl, i|
        if lbl == level_index then chosen << i end
      end
    else
      chosen.keep_if { |c| label[c] == level_index }
    end

    chosen
  end

  private :find_all_indexes, :multi_index_from_multiple_selections,
    :retrieve_from_range, :retrieve_from_tuples

  def key index
    raise ArgumentError,
      "Key #{index} is too large" if index >= @labels[0].size

    level_indexes = 
    @labels.inject([]) do |memo, label|
      memo << label[index]
      memo
    end

    tuple = []
    level_indexes.each_with_index do |level_index, i|
      tuple << @levels[i].keys[level_index]
    end

    tuple
  end

  def dup
    MultiIndex.new levels: levels.dup, labels: labels
  end

  def drop_left_level by=1
    MultiIndex.from_arrays to_a.transpose[by..-1]
  end

  def | other
    MultiIndex.from_tuples(to_a | other.to_a)
  end

  def & other
    MultiIndex.from_tuples(to_a & other.to_a)
  end

  def empty?
    @labels.flatten.empty? and @levels.all? { |l| l.empty? }
  end

  def include? tuple
    tuple.flatten!
    tuple.each_with_index do |tup, i|
      return false unless @levels[i][tup]
    end
    true
  end

  def size
    @labels[0].size
  end

  def width
    @levels.size
  end

  def == other
    self.class == other.class  and 
    labels     == other.labels and 
    levels     == other.levels 
  end

  def to_a
    (0...size).map { |e| key(e) }
  end

  def values
    Array.new(size) { |i| i }
  end

  def inspect
    "Daru::MultiIndex:#{self.object_id} (levels: #{levels}\nlabels: #{labels})"
  end
end

require 'benchmark'
arr = [[:a,:b,:c,:d,:a,:b,:c,:d,:a,:b,:c,:d], 
[:one,:two,:one,:two,:one,:two,:one,:two,:one,:two,:one,:two],
[:bar, :baz,:foo,:bar, :baz,:foo,:bar, :baz,:foo,:bar, :baz,:foo,]]
tuples = arr.transpose

Benchmark.bm do |x|
  old = OldMultiIndex.new(tuples)
  ne  = MultiIndex.from_arrays arr

  x.report("Old index creation") do
    1000.times do
      OldMultiIndex.new(tuples)
    end
  end

  x.report("New index creation") do
    1000.times do
      MultiIndex.from_arrays arr
    end
  end

  x.report("Old access") do
    1000.times do 
      old[:a,:one,:bar]
    end
  end

  x.report("New access") do
    1000.times do 
      ne[:a,:one,:bar]
    end
  end
end

#        user     system      total        real
# Old index creation  0.020000   0.000000   0.020000 (  0.021659)
# New index creation  0.050000   0.000000   0.050000 (  0.045119)
# Old access  0.000000   0.000000   0.000000 (  0.003012)
# New access  0.010000   0.000000   0.010000 (  0.006865)