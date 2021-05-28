import numpy as np
import math

# https://mathonweb.com/help_ebook/html/expressions_8.htm


b = [5, 5, 5, 5, 5, 5]
cnt = 0
arr = []

for i in range(0, 11):
    for j in range(0, 11):
        for k in range(0, 11):
            for l in range(0, 11):
                for m in range(0, 11):
                    for n in range(0, 11):
                        if i + j + k + l + m + n == 10:
                            i_ = math.factorial(i)
                            j_ = math.factorial(j)
                            k_ = math.factorial(k)
                            l_ = math.factorial(l)
                            m_ = math.factorial(m)
                            n_ = math.factorial(n)

                            den = float(i_ * j_ * k_ * l_ * m_ * n_)

                            coeff = math.factorial(10) / den
                            power = i + j + k + l + m + n
                            print("power: ", power, " coeff: ", coeff, " i: ", i,
                                  " j: ", j, " k: ", k, " l: ", l, " m: ", m, " n: ", n)

                            num = 1.0
                            if i != 0:
                                num *= b[0] ** i
                            if j != 0:
                                num *= b[1] ** j
                            if k != 0:
                                num *= b[2] ** k
                            if l != 0:
                                num *= b[3] ** l
                            if m != 0:
                                num *= b[4] ** m
                            if n != 0:
                                num *= b[5] ** n

                            print("pre num: ", num)

                            num = num * coeff
                            arr.append(num)
                            print("num: ", num)

print(arr)

k = 10
def compute(b, curr_k):
    if curr_k < k:
        for i in range(0, b[curr_k]+1):
            pass

compute(b, 0)
