#!/usr/bin/python3

import tqdm
import random

N = random.randint(1, 1234567)
k = random.randint(1, 26)
alphabet = 'abcdefghijklmnopqrstuvwxyz'
using = random.sample(alphabet, k)

archive = set()

with open('input.txt', 'w') as file:
    for _ in tqdm.tqdm(range(N)):
        length = random.randint(1, 10)
        string = ''.join(random.choices(using, k=length))
        if string in archive:
            continue
        else:
            archive.add(string)

    print(len(archive), file=file)
    for string in archive:
        print(string, sep='', file=file)
