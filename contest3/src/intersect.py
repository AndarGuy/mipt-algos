import random
from tqdm import tqdm

input = open("input.txt", "w")

N, M = 1000, 10000

print(N, M, file=input)
for _ in tqdm(range(N)):
    for _ in range(M):
        print(random.randint(-10000000000, +10000000000), end=' ', file=input)
    print(file=input)

