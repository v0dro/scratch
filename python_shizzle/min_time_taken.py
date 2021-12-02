# https://www.hackerrank.com/challenges/minimum-time-required/forum?h_l=interview&playlist_slugs%5B%5D=interview-preparation-kit&playlist_slugs%5B%5D=search

# def minTime(machines, goal):
#     machines.sort()
#     low_rate = machines[0]
#     lower_bound = (goal // (len(machines) / low_rate))
#     high_rate = machines[-1]
#     upper_bound = (goal // (len(machines) / high_rate)) + 1

#     while lower_bound < upper_bound:
#         num_days = (lower_bound + upper_bound) // 2
#         total = getNumItems(machines, goal, num_days)
#         if total >= goal:
#             upper_bound = num_days
#         else:
#             lower_bound = num_days + 1
#     return int(lower_bound)


# def getNumItems(machines, goal, num_days):
#     total = 0
#     for machine in machines:
#         total += (num_days // machine)
#     return total

# https://www.geeksforgeeks.org/minimum-time-required-produce-m-items/

# Complete the minTime function below.
def minTime(machines, goal):
    min_time = min(machines)
    max_time = max(machines)

    lower = int(goal // (len(machines) / min_time))
    upper = int(goal // (len(machines) / max_time)) + 1

    while lower < upper:
        mid = (lower + upper) // 2
        # sum of number of days needed per machine.
        total_items = sum([mid // machine for machine in machines])

        print("mid: ", mid, "total: " , total_items)

        if total_items >= goal:
            upper = mid
        else:
            lower = mid + 1

    return lower

print(minTime([2, 3, 2], 10))