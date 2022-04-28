import random

def word():
    return ''.join([chr(ord('a') + random.randint(1, 26)) for _ in range(random.randint(1, 20))])

tables = [{}, {}, {}, {}, {}, {}, {}, {}, {}, {}]
for i in range(1):
    print("create", i)
for _ in range(10):
    table = random.randint(0, 9)
    action = random.randint(1, 100)
    if action <= 50:
        type = random.randint(1, 2)
        value = word()
        if type == 1:
            key = word()
        else:
            key = random.choice(tables[table].keys())
        tables[table][key] = value
        print("insert", table, key, value)
    elif action <= 90:
        type = random.randint(1, 2)
        if type == 1:
            key = word()
        else:
            key = random.choice(tables[table].keys())
        print("find", table, key)
    elif action <= 95:
        key = random.choice(tables[table].keys())
        print("erase", table, key)
    elif action <= 100:
        print("destroy", table)