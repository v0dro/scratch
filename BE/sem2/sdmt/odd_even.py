import random
from multiprocessing import Process, Queue

def even_sorter(size, N, array, q):
  even_list = array[::2]
  q.put(('even', sorted(even_list)))

def odd_sorter(size, N, array, q):
  odd_list = array[1::2]
  q.put(('odd', sorted(odd_list)))

def merge_lists(odd_sorted_list, even_sorted_list, odd_size, even_size):
  merge_list = []
  i = 0
  j = 0

  while i < odd_size and j < even_size:
    if odd_sorted_list[i] < even_sorted_list[j]:
      merge_list.append(odd_sorted_list[i])
      i += 1
    else:
      merge_list.append(even_sorted_list[j])
      j += 1

  if j < even_size:
    merge_list.extend(even_sorted_list[j:])

  if i < odd_size:
    merge_list.extend(odd_sorted_list[i:])

  return merge_list

N = 1000
array = random.sample(xrange(N), N)

odd_size = N/2
even_size = N/2

shared_queue = Queue()

odd = Process(target=odd_sorter, args=(odd_size, N, array, shared_queue))
even = Process(target=even_sorter, args=(even_size, N, array, shared_queue))

odd.start()
even.start()
odd.join()
even.join()

objects = []
objects.append(shared_queue.get())
objects.append(shared_queue.get())

for tup in objects:
  if tup[0] == 'even':
    even_sorted_list = tup[1]
  elif tup[0] == 'odd':
    odd_sorted_list = tup[1]

merged_sorted_list = merge_lists(odd_sorted_list, even_sorted_list, 
  odd_size, even_size)


print merged_sorted_list
