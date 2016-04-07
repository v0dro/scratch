"""
author: @v0dro
Dining philosophers problem
"""

import threading
import time

N = 5
forks = []
threads = []
sem = threading.Semaphore()
for i in xrange(0,N):
  forks.append(0)

# Note: When fork = 0, it is not in use. When fork = 1, it is in use.
# Semaphore has been used below because the STDOUT is a single shared resource
#   between all the philosophers, unlike forks, each of which needs to be shared
#   between only two philosophers at max.

def init_philosopher(left, right, number):
  def eat(num):
    sem.acquire()
    print "Philosopher ", num, " has started eating..."
    sem.release()

  def think(num):
    sem.acquire()
    print "Philosopher ", num, " has started thinking..."
    sem.release()

  while forks[left] == 1 or forks[right] == 1:
    think(number)
    time.sleep(1)

  if forks[left] == 0 and forks[right] == 0:
    forks[left] = 1
    forks[right] = 1
    eat(number)
    forks[left] = 0
    forks[right] = 0

for i in xrange(0,N):
  t = threading.Thread(
    target=init_philosopher, args=(i, (i+1) % N, str(i)))
  t.start()
  threads.append(t)

for t in threads:
  t.join()