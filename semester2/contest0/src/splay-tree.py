import random
from tqdm import tqdm

output = open("output.txt", "w")
input = open("input.txt", "w")

tree = set()
last_sum = 0

N = 50000
MAX = 1000000000
print(N, file=input)
for _ in tqdm(range(N)):
    action = random.randint(1, 2)
    if (action == 1):
        insert = random.randint(1, MAX)
        print("+", insert, file=input)
        tree.add((insert + last_sum) % 1000000000)
        last_sum = 0
    else:
        min = random.randint(1, MAX)
        max = random.randint(min, MAX)
        print("?", min, max, file=input)
        last_sum = 0
        for element in tree:
            if min <= element <= max:
                last_sum += element
        print(last_sum, file=output)

input.close()
output.close()