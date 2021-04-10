#https://www.hackerrank.com/challenges/abbr/problem?h_l=interview&playlist_slugs%5B%5D=interview-preparation-kit&playlist_slugs%5B%5D=dynamic-programming

# https://www.youtube.com/watch?v=DW92IHf8KK8

# Simple solution (no dyn prog)
# Failing case:

# KXzQ
# K

# Reason: problem states that you can delete only lowercase
# letters from a.
def abbrev(a, b):
    i = 0
    j = 0
    while i < len(a) and j < len(b):
        print("a(i)=", a[i],  " b(j)=", b[j])
        if a[i] == b[j] or a[i].upper() == b[j]:
            i += 1
            j += 1
            continue
        
        if a[i] != b[j] or a[i].upper() != b[j]:
            i += 1
            continue
            
    if j == len(b):
        return "YES"
    else:
        return "NO"

print(abbrev("KXzQ", "K"))
