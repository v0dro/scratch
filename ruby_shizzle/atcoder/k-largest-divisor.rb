# https://atcoder.jp/contests/abc120/tasks/abc120_b

def find_gcd a, b
  while a != b
    if a > b
      a = a - b
    else
      b = b - a
    end
  end

  return a
end

def find_kth gcd, a, b, k
  num = 1
  ks = []
  1.upto(gcd) do |n|
    if a % n == 0 && b % n == 0
      ks << n
      num += 1
    end
  end

  ks[-k]
end

input = gets
matcher = input.match(/(\d+)\s*(\d+)\s*(\d+)/)
a, b, k = matcher[1].to_i, matcher[2].to_i, matcher[3].to_i

gcd = find_gcd a, b
num = find_kth gcd, a, b, k

puts num
