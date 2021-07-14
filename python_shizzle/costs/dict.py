import timeit

n = 1000000
a = dict.fromkeys(list(range(n)), 1)

print(timeit.repeat(stmt='1 in a', globals=globals()))
print(timeit.repeat(stmt='999999 in a', globals=globals()))
print(timeit.repeat(stmt='1 in a', globals=globals()))
