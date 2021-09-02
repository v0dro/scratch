import bisect

# Tried to sort the lists and bisect the needed values. But some cases dont
# work, and others give time limit issues.
def triplets_old(a, b, c):
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
print(triplets_old([1, 3, 5, 7], [5, 7, 9], [7, 9, 11, 13]))

# Following is a working solution

def triplets(a, b, c):
    a = sorted(set(a))
    b = sorted(set(b))
    c = sorted(set(c))

    ai = 0
    bi = 0
    ci = 0

    count = 0

    while bi < len(b):
        # number of elements used
        while ai < len(a) and a[ai] <= b[bi]:
            # Keep incrementing ai values since for every new value
            # of bi, there will be  more elements that can be incorporated
            # into the tuple that eventually is generated.
            ai += 1

        while ci < len(c) and b[bi] >= c[ci]:
            # Same explanation goes for ci as ai above.
            ci += 1

        count += ai * ci
        bi += 1

    return count
