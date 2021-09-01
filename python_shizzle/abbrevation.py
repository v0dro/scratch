#https://www.hackerrank.com/challenges/abbr/problem?h_l=interview&playlist_slugs%5B%5D=interview-preparation-kit&playlist_slugs%5B%5D=dynamic-programming

# https://www.youtube.com/watch?v=DW92IHf8KK8

# https://iawale.medium.com/abbreviation-hackerrank-medium-4601219c07d1

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

def abbrev_rec(a, b):
    if a[0].islower():
        abbrev_rec(a[0].upper() + a[1:], b)
        abbrev_rec(a[1:], b)
    else:
        if a == b:
            return True
        else:
            return False

#print(abbrev_rec("daBcd", "ABC"))
#print(abbrev_rec("KXzQ", "K"))

import copy

def abbreviation(a, b):
    memo = list()
    for _ in range(len(a)+1):
        memo.append([False] * (len(b) + 1))

    memo[0][0] = True

    for i in range(1, len(a) + 1):
        if a[i - 1].isupper():
            memo[i][0] = False
        else:
            memo[i][0] = True


    for i in range(1, len(a) + 1):
        for j in range(1, len(b) + 1):
            if a[i-1] == b[j-1]:
                memo[i][j] = memo[i-1][j-1]
            elif a[i-1].upper() == b[j-1]:
                memo[i][j] = memo[i-1][j-1] or memo[i-1][j]
            elif a[i-1].isupper():
                memo[i][j] = False
            else:
                memo[i][j] = memo[i-1][j]


    if memo[len(a)][len(b)]:
        return "YES"
    else:
        return "NO"




print(abbreviation("daBcd", "ABC"))
