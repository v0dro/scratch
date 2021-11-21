swaps = 0

def partition(arr, low, high):
    global swaps
    i = (low-1)         # index of smaller element
    pivot = arr[high]     # pivot

    for j in range(low, high):

        # If current element is smaller than or
        # equal to pivot
        if arr[j] <= pivot:

            # increment index of smaller element
            i = i+1
            swaps += 1
            arr[i], arr[j] = arr[j], arr[i]

    # swaps += 1
    arr[i+1], arr[high] = arr[high], arr[i+1]
    return (i+1)

def quick_sort(arr, left, right):
    if left < right:
        pi = partition(arr, left, right)
        quick_sort(arr, left, pi-1)
        quick_sort(arr, pi+1, right)

# Funda: Do not treat this like a sorting problem.
# The problem states that all the elements are within the range
# of the index, so take advantage of that.
def count_swaps(arr):
    a = dict(enumerate(arr, 1)) # dict of index : value pairs
    b = {v:k for k, v in a.items()} # make key : value as value : key
    count = 0

    print(a)
    print(b)

    for i in a:
        x = a[i]                # x is element at the i'th index
        if x != i:              # if the element is not in the same place as the index.
            y = b[i]            # Get the index of the same element since indices are same numbers as elements.
            a[y] = x            # Set the y'indexed number to x.
            b[x] = y            # Set t
            count += 1          # Increase swap count
    return count

arr = [7, 1, 3, 2, 4, 5, 6]

c = count_swaps(arr)
print(c)
