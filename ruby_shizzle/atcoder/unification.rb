# https://atcoder.jp/contests/abc120/tasks/abc120_c

def jump_to_next initial, string
  if initial > string.size - 1
    return initial
  end
  
  if string[initial] != '2'
    return initial
  else
    i = initial
    while string[i] == '2'
      i += 1
    end
    return i
  end
end
  
def fast string
  rem = 0
  while true
    brick_removed = false
    i = jump_to_next 0, string
    j = jump_to_next i+1, string

    if i == j
      break
    end

    while true
      if (string[i] == '0' && string[j] == '1') || (string[i] == '1' && string[j] == '0')
        string[i] = '2'
        string[j] = '2'
        rem += 2
        brick_removed = true
        break
      end
      i = jump_to_next i+1, string
      j = jump_to_next i+1, string

      if j >= string.size - 1
        break
      end
    end

    if !brick_removed
      break
    end
    
    if j >= string.size - 1
      break
    end
  end

  rem
end

input = gets.strip

puts fast(input)
