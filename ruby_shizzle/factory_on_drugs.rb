class A

  def initialize a, b
    puts "Class A created #{a + b}"
  end

  def self.new(*args, &block)
    puts "THis is a factory"
    o = self.allocate
    o.send(:initialize, *args, &block)
    o
  end
end

a = A.new 3,4

puts "#{a.class}"

class B
  def self.new
    "This is B"
  end
end

puts "#{B.new.class}"