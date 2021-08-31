# Crappy N^2 solution.
def whatFlavors(cost, money):
    # Write your code here
    choice_1 = None
    choice_2 = None
    found = False
    for first_index, first_cost in enumerate(cost):
        if found:
           break
        for second_index, second_cost in enumerate(cost):
            if first_index != second_index:
                if first_cost + second_cost == money:
                    choice_1 = first_index + 1
                    choice_2 = second_index + 1
                    found = True
                    break
    if choice_1 < choice_2:
        print(f"{choice_1} {choice_2}")
    else:
        print(f"{choice_2} {choice_1}")



def whatFlavors(cost, money):
    saved = dict()

    for i, c in enumerate(cost):
        if (money-c) in saved:
            print(f"{saved[money-c]} {i+1}")
            break
        saved[c] = i + 1
