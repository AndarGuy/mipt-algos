#!/usr/bin/python3

import random
import os


ALPHABET = 'abcd'

while True:
    N = random.randint(1, 100000)
    K = random.randint(1, N)
    string = ''.join(random.choices(ALPHABET, k=N))
    print(N, K)
    with open('input.txt', 'w') as input:
        print(string, file=input)
        print(K, file=input)
    doubled_string = string + string
    dictionary = set()
    for i in range(0, N):
        dictionary.add(doubled_string[i: i + N])
    dictionary = sorted(list(dictionary))

    answer = 'IMPOSSIBLE' if K > len(dictionary) else dictionary[K - 1]

    with open('output.txt', 'w') as input:
        print(answer, file=input)

    break

    # stream = os.popen(
    #     f'echo "{string}\n{K}" | /Users/mikhail/Documents/Projects/mipt-algos/build/R')
    # output = stream.read().strip('\n')
    # if not output == answer:
    #     print(answer)
    #     print("----")
    #     print(output)
    #     break
