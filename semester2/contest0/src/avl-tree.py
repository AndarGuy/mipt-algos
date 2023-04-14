import random
from tqdm import tqdm

output = open("output.txt", "w")
input = open("input.txt", "w")

tree = dict()

def generate_word():
    return ''.join([chr(ord('a') + random.randint(0, 25)) for i in range(random.randint(1, 20))])

N = 50000
for _ in tqdm(range(N)):
    action = random.randint(1, 12)
    if action <= 10:
        type = random.randint(1, 2)
        if type == 1 or not tree:
            key = generate_word()
        else:
            key = random.choice(list(tree.keys()))
        value = generate_word()
        tree[key] = value
        print("insert", key, value, file=input)
    elif action <= 11:
        type = random.randint(1, 2)
        if type == 1 or not tree:
            key = generate_word()
        else:
            key = random.choice(list(tree.keys()))
        
        if key in tree:
            print(tree[key], file=output)
        else:
            print("MISSING", file=output)

        print("find", key, file=input)
    elif action <= 12:
        type = random.randint(1, 2)
        if type == 1 or not tree:
            key = generate_word()
        else:
            key = random.choice(list(tree.keys()))
        if key in tree:
            del tree[key]
        print("erase", key, file=input)

input.close()
output.close()