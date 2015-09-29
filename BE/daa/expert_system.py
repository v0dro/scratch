# Name: Sameer Deshmukh
# Class: BE I
# Roll: 405119
# Title: Book recommender expert system based on user ratings

from math import pow

data = {
  'gone with the wind': {
    'chris broderick': 1.4, 
    'isaac asimov': 1, 
    'huzefa harawala': 1, 
    'faizan khan': 5, 
    'saul goodman': 2.5, 
    'walter white': 2.5, 
    'jaideep khekde': 3, 
    'sameer deshmukh': 4
  }, 

  'when i left home': {
    'chris broderick': 3.2, 
    'isaac asimov': 4, 
    'huzefa harawala': 4, 
    'faizan khan': 2, 
    'saul goodman': 3.5, 
    'walter white': 3.6, 
    'jaideep khekde': 4, 
    'sameer deshmukh': 3
  }, 

  'foundation and earth': {
    'chris broderick': 5, 
    'isaac asimov': 3.5, 
    'huzefa harawala': 1, 
    'faizan khan': 2, 
    'saul goodman': 1.5, 
    'walter white': 3.5, 
    'jaideep khekde': 1.4, 
    'sameer deshmukh': 4.1
  }, 

  'foundation and empire': {
    'chris broderick': 5, 
    'isaac asimov': 2, 
    'huzefa harawala': 3, 
    'faizan khan': 2, 
    'saul goodman': 4, 
    'walter white': 4, 
    'jaideep khekde': 2, 
    'sameer deshmukh': 2.4
  }, 

  'atlas shrugged': {
    'chris broderick': 3.5, 
    'isaac asimov': 4, 
    'huzefa harawala': 5, 
    'faizan khan': 3, 
    'saul goodman': 4.5, 
    'walter white': 5, 
    'jaideep khekde': 2.8, 
    'sameer deshmukh': 5
  }, 

  'forward the foundation': {
    'chris broderick': 2, 
    'isaac asimov': 2.8, 
    'huzefa harawala': 5, 
    'faizan khan': 2.5, 
    'saul goodman': 3, 
    'walter white': 4.5, 
    'jaideep khekde': 3.5, 
    'sameer deshmukh': 1.5
  },

  'war and peace': {
    'chris broderick': 5, 
    'isaac asimov': 3, 
    'huzefa harawala': 4, 
    'faizan khan': 1, 
    'saul goodman': 3, 
    'walter white': 3.2, 
    'jaideep khekde': 1, 
    'sameer deshmukh': 0.5
  }, 
}

def top_matches(prefs, book, n):
  scores=[(similarity(prefs,book,other),other) for other in prefs if other != book]
  # Sort the list so the highest scores appear at the top
  scores.sort()
  scores.reverse()
  return scores[0:n]

def similarity(prefs, book1, book2):
  # Get the list of shared_items
  si = {}
  for reviewer in prefs[book1]:
    if reviewer in prefs[book2]:
      si[reviewer] = 1

  # if they have no ratings in common, return 0
  if len(si) == 0: return 0
  # Add up the squares of all the differences
  sum_of_squares = sum([pow(prefs[book1][reviewer] - prefs[book2][reviewer], 2)
                        for reviewer in prefs[book1] if reviewer in prefs[book2]])

  return 1 / (1 + sum_of_squares)

print "Given that you like war and peace..."

matches = top_matches(data, "war and peace", 5)

print "You might want to check out the following books:"
print ""
for tup in matches:
  print tup[1]

################ OUTPUT ################
# 
# Given that you like war and peace...
# You might want to check out the following books:

# foundation and empire
# when i left home
# forward the foundation
# foundation and earth
# atlas shrugged
