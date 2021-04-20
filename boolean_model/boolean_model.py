import numpy as np
import glob
import re
import functools
import operator

# Step 0: Scan all text files
paths = glob.glob('data/*.txt')

def getIndex(vocab, word):
    index = vocab.get(word, len(vocab))
    if index == len(vocab):
        vocab[word] = index
    return index

def toRPN(query):
    '''
        Convert to Reverse Polish Notation
    '''
    output = []
    oper = []
    cnt = 0
    for token in query:
        if token == 'X':
            output.append(cnt)
            cnt += 1
        elif token == '(':
            oper.append(token)
        elif token == ')':
            while len(oper) > 0 and oper[-1] != '(':
                output.append(oper.pop())
            oper.pop()
        else:
            while len(oper) > 0 and oper[-1] != '(':
                output.append(oper.pop())
            oper.append(token)
    while len(oper) > 0:
        output.append(oper.pop())
    return output

def calculate(query):
    if len(query) == 1:
        return op[query[0]]
    tmp = np.ones(len(paths))
    st = []
    for x in query:
        if isinstance(x, int):
            st.append(op[x])
        elif x == '-':
            opp = st.pop()
            new_op = np.logical_not(opp)
            st.append(new_op)
        else:
            op1 = st.pop()
            op2 = st.pop()
            if x == '&':
                new_op = np.logical_and(op1, op2)
            elif x == '|':
                new_op = np.logical_or(op1, op2)
            else:
                new_op = np.logical_xor(op1, op2)
            st.append(new_op)
    return st.pop()

#Step 1-2: Read the file, build dict

contents = []
dictionary = dict()
for path in paths:
    # Read file
    f = open(path, encoding="utf8")
    str = f.read()
    words = re.findall(r"[\w']+", str)
    words = list(set(words))
    words = [getIndex(dictionary, word.lower()) for word in words]
    contents.append(words)

#Step 3: Create term_document matrix
term_doc = np.zeros((1, len(dictionary) * len(paths)))
lf = lambda i, x: list(map(lambda num: num * len(paths) + i, x))
index = list(map(lambda x: lf(x[0], x[1]), list(enumerate(contents))))
index = functools.reduce(operator.iconcat, index, [])
np.put(term_doc, index, 1)
term_doc = np.reshape(term_doc, (len(dictionary), len(paths)))

#Step 4: 
query = input()

#Normalize the query
x = re.findall(r"\'[\w\s\d]+\'", query)
query = re.sub(r"\'[\w\s\d]+\'", " X ", query)
query = re.sub(r"AND", " & ", query)
query = re.sub(r"OR", " | ", query)
query = re.sub(r"XOR", " ^ ", query)
query = re.sub(r"NOT", " - ", query)

query = query.split()

#Normalize the list of words
x = list(map(lambda x : x[1:-1], x))

#Get the word vector
op = []
for words in x:
    word = words.split()
    tmp = np.ones(len(paths))
    for w in word:
        if w.lower() in dictionary:
            tmp = np.logical_and(tmp, term_doc[dictionary[w.lower()], :])
        else:
            tmp = np.zeros(len(paths))
    op.append(tmp)

#Get the result
bool_query = toRPN(query)
res = calculate(bool_query)

#Get the argument
art = np.argwhere(res)
#Print the path
for index in art:
    print(paths[index[0]])
