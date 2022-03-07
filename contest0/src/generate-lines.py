import random

with open("./input.txt", "w") as file:
    for _ in range(10000):
        for _ in range(random.randint(1, 10000)):
            file.write(chr(random.randint(ord('a'), ord('z'))))
        file.write("\n")