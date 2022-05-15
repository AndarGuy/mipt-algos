import random
from tqdm import tqdm

input = open("input.txt", "w")
output = open("output.txt", "w")

N, M = 1000, 100000
sets = [set() for _ in range(N)]

print(N, M, file=input)
for _ in tqdm(range(M)):
    command = random.choice(['+', '+', '+', '-', '=', '?', '??'])
    s = random.randint(0, N - 1)
    x = random.randint(0, 100)
    if command == '+':
        print(command, x, s, file=input)
        sets[s].add(x)
    if command == '-':
        print(command, x, s, file=input)
        if (x in sets[s]):
            sets[s].remove(x)
    if command == '=':
        print(command, s, file=input)
        sets[s] = set()
    if command == '?':
        print(command, s, file=input)
        if sets[s]:
            print(' '.join(map(str, sorted(sets[s]))), end=' ', file=output)
        else:
            print(-1, end='', file=output)
        print(file=output)
    if command == '??':
        print(command, x, file=input)
        found = False
        for i in range(N):
            if x in sets[i]:
                print(i, end=' ', file=output)
                found = True
        if not found:
            print(-1, end='', file=output)
        print(file=output)
