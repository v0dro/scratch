class A
  def self.hello
    puts "hello from A"
  end
end


class B < A
  def self.hello
    puts "hello from B"
    super
  end
end

B.hello