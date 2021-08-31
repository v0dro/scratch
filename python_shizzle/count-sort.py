active_numbers = 0
arr = [5,6,7,1,3,2,11,9,10]
counts = [0] * (max(arr) + 1)
sorted_arr = []
d = 11

for num in arr:
    counts[num] += 1
    active_numbers += 1

i = 0
median = 0

for c in counts:
    if i >= active_numbers / 2:
        break

    if c != 0:
        i += c
median = i

if active_numbers % 2 == 0:
    pass
else:
    print(i)
