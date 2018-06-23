# Given a smaller string s and a bigger string b, design an algorithm to find
#   all permutations of the shorter string within the longer one. Print the location
#   of each permutation.
def shift(l, n):
    return l[n:] + l[:n]

d = "abcdsameerdcbabcdbadeshmukhabmdchkh"
s = "abcd"

# create each perm of s
perms = []
temp = list(s)
for i in range(len(s)):
    perms.append(''.join(temp))
    temp = shift(temp, 1)

# search for each perm
for perm in perms:
    print("searching perm " + perm)
    # search each perm p in d
    for dci, dc in enumerate(d):
        if perm[0] == dc:
            match = True
            for i in range(len(perm)):
                if perm[i] != d[dci+i]:
                    match = False
                    break

            if match:
                print(perm + " has matched at location " + str(dci))
            
