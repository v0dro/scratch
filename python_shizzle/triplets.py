import bisect

def triplets(a, b, c):
    a = set(a)
    b = sorted(list(set(b)))
    c = sorted(list(set(c)))

    count = 0
    for ai in a:
        b_index = bisect.bisect_left(b, ai)
        if b_index != len(b):
            for bi in range(b_index, len(b)):
                c_index = bisect.bisect_left(c, b[bi])

                if c_index != len(c):
                    for ci in range(0, c_index+1):
                        count += 1

    return count



# print(triplets([1, 3, 5], [2, 3], [1, 2, 3]))
# print(triplets([1, 4, 5], [2, 3, 3], [1, 2, 3]))
print(triplets([1, 3, 5, 7], [5, 7, 9], [7, 9, 11, 13]))
