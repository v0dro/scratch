# author: @v0dro
# distributed binary search

from multiprocessing import Process, Queue
import multiprocessing
import random
import time

def binary_search(number_list, number, cpu, q):
  print "Process starting on cpu ", str(cpu)

  sort_list = sorted(number_list)
  index = None
  if sort_list[0] < number and sort_list[-1] > number:
    low = 0
    high = len(sort_list)
    while low < high:
      mid = (low + high)/2
      if number == sort_list[mid]:
        index = mid
        break
      elif number < sort_list[mid]:
        high = mid - 1
      elif number > sort_list[mid]:
        low = mid + 1

  q.put((cpu, index))
  q.close()

N = 1000
cpus = multiprocessing.cpu_count()
numbers = [random.randint(0,N) for i in xrange(0,1000)] 
queue = Queue()
divisions = N / cpus
x = input("Enter number to search: ")

for cpu in xrange(0,cpus):
  low = cpu * divisions
  high = (cpu + 1) * divisions
  p = Process(target=binary_search, args=(numbers[low:high], x, cpu, queue))
  p.start()

while queue.qsize() != cpus:
  time.sleep(0.1)

for _ in xrange(0, cpus):
  result = queue.get()
  if result[1] != None:
    print "Element found by process number: ", result[0]