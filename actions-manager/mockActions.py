import random

# # # # # # # # # # # # # # # # # # # # # # #

MIN_KEY_SIZE = 3
MAX_KEY_SIZE = 31
MIN_PATH_SIZE = 2
MAX_PATH_SIZE = 4

# # # # # # # # # # # # # # # # # # # # # # #

caps = [chr(i) for i in range(65, 91)]
allowed = [chr(i) for i in range(48, 58)] + [chr(i) for i in range(65, 91)] + [chr(i) for i in range(97, 123)] + ['-', '_']


def generateKey():
    size = random.randint(MIN_KEY_SIZE, MAX_KEY_SIZE)
    return caps[random.randint(0, len(caps)-1)] + "".join([allowed[random.randint(0, len(allowed)-1)] for i in range(0, size)])


def makeState():
    size = random.randint(MIN_PATH_SIZE, MAX_PATH_SIZE)
    path = [generateKey() for i in range(0, size)]
    action = generateKey()
    stringified = f"{'.'.join(path)}:{action}"
    return {'path': path, 'action': action, 'string': stringified}