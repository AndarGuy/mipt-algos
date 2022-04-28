import random

input = open("input.txt", "w")

N = 100000
print(N, file=input)
for _ in range(N):
    a, b, c = random.randint(-100, 100), random.randint(-100, 100), random.randint(-100, 100)
    print(a, b, c, file=input)