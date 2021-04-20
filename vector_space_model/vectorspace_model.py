import numpy as np
import glob
import re
import functools
import operator
from collections import Counter
# Step 0: Scan all text files
paths = glob.glob('data/*.txt')


def getIndex(vocab, word):
    index = vocab.get(word, len(vocab))
    if index == len(vocab):
        vocab[word] = index
    return index

# Step 1: Read all text files and build dictionary
contents = []
dictionary = dict()
df = dict()
for path in paths:
    # Read file
    f = open(path, encoding="utf8")
    str = f.read()
    words = re.findall(r"[\w']+", str)
    words = [getIndex(dictionary, word.lower()) for word in words]
    contents.append(words)
    

# Step 2: create term-document frequency (TF)
term_doc = np.zeros((1, len(dictionary) * len(paths)))
lf = lambda i, x: list(map(lambda num: num * len(paths) + i, x))
index = list(map(lambda x: lf(x[0], x[1]), list(enumerate(contents))))
index = functools.reduce(operator.iconcat, index, [])
frq = Counter(index)
np.put(term_doc, list(frq.keys()), list(frq.values()))
term_doc = np.reshape(term_doc, (len(dictionary), len(paths)))


# Step 3: compute Inverse Document Frequency (IDF) using term-doc frequency
idf = np.array([np.count_nonzero(term_doc, axis=1)]).transpose()

# Step 4: compute TF-IDF matrix
term_doc = (np.log(term_doc, where=term_doc > 0) + 1 - np.where(term_doc == 0, 1, 0)) * (1 + np.log(len(paths)) - np.log(idf))

norm = np.sqrt(np.sum(term_doc * term_doc, axis=0))
term_doc = term_doc / norm


# Step 5: Retrieve from text query
query = input()
query = query.lower()
query = re.findall(r"[\w]+", query)
query = Counter(query)

vec = np.zeros((len(dictionary), 1))
for word, freq in query.items():
    if word in dictionary:
        vec[dictionary[word], 0] = 1 + np.log(freq)
vec = vec * (1 + np.log(len(paths)) - np.log(idf))

res = np.sum(term_doc * vec, axis=0) / np.sqrt(np.sum(vec * vec))

l = list(range(len(paths)))
l.sort(key=lambda x : res[x], reverse=True)

#List top 10
for index in l[:10]:
    print(paths[index])