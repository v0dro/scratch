class A
  @default_values = {
    abs_x: 1,
    abs_y: 2
  }
  
  def reset!
    puts "#{self.class}"
    self.class.default_values.each do |k, v|
      self.instance_variable_set("@#{k}", v)
    end
  end
end

class B < A
  @default_values = {
    a: 1,
    b: nil,
    c: ""
  }
  
  def self.default_values
    @default_values
  end

  def get_a
    @a
  end
end

class C < A
  @default_values = {
    a: 3,
    b: "",
    c: nil
  }
end

q = B.new
q.reset!
puts q.get_a
