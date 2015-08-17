class Array
  # Recode repeated values on an array, adding the number of repetition
  # at the end
  # Example:
  #   a=%w{a b c c d d d e}
  #   a.recode_repeated
  #   => ["a","b","c_1","c_2","d_1","d_2","d_3","e"]
  def recode_repeated
    if size != uniq.size
      # Find repeated
      repeated = inject({}) do |acc, v|
        if acc[v].nil?
          acc[v] = 1
        else
          acc[v] += 1
        end
        acc
      end.select { |_k, v| v > 1 }.keys

      ns = repeated.inject({}) do |acc, v|
        acc[v] = 0
        acc
      end

      collect do |f|
        if repeated.include? f
          ns[f] += 1
          sprintf('%s_%d', f, ns[f])
        else
          f
        end
      end
    else
      self
    end
  end
end

hash1 = {
  :id => [1,2,3,4],
  :name => ['sam', 'sag', 'lean', 'mean'],
  :addr => ['JM road', 'Kothrud', 'Pune', 'Mumbai']
}

hash2 = {
  :id => [1,2,3,4],
  :name => ['bruh', 'leonard', 'sam', 'sag'],
  :state => ['OS', 'BS', 'MH','UK']
}

hk = hash1.keys + hash2.keys
recoded = hk.recode_repeated.map(&:to_sym)
diff = (recoded - hk).sort

puts "#{diff} #{recoded}"
diff.each_slice(2) do |a|
  puts "#{a}"
  first = a[0]
  second = a[1]

  matched1 = nil
  hash1.keys.each { |d| 
    if first.match(Regexp.new(d.to_s))
      matched1 = d
      break
    end  
  }

  matched2 = nil
  hash2.keys.each { |d|
    if second.match(Regexp.new(d.to_s))
      matched2 = d
      break
    end 
  }

  if matched1
    hash1[first] = hash1[matched1] 
    hash1.delete matched1
  end

  if matched2
    hash2[second] = hash2[matched2]
    hash2.delete matched2
  end
end

puts hash1
puts hash2