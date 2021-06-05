import timeit

n = 1000000
a = set(list(range(n)))

print(timeit.timeit(stmt='999999 in a', globals=globals()))
print(timeit.timeit(stmt='1 in a', globals=globals()))
