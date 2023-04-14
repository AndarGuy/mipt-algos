from itertools import combinations
import random
import os

alphabet = 'abcdef'


def get_number_of_substrings(string):
    return len(set([string[x:y]
                    for x, y in combinations(range(len(string) + 1), r=2)]))


while True:
    N = 300
    string = ''.join(random.choices(alphabet, k=N))
    # string = 'cbabcbc'
    print(string)
    result = []
    for i in range(1, N + 1):
        result.append(get_number_of_substrings(string[:i]))
    stream = os.popen(
        f'echo {string} | /Users/mikhail/Documents/Projects/mipt-algos/build/O')
    output = list(map(int, stream.read().split('\n')[:-1]))
    if not output == result:
        print(result)
        print(output)
        break
