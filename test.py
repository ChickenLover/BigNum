#!/usr/bin/env python3

import subprocess
import sys
import time
import random

TESTS = 1000
SHOW_OUTPUT = False

def gen_hex(chunks_from, chunks_to):
    return hex(random.randint(int('1' + '0' * (8 * chunks_from), 16),
                              int('1' + '0' * (8 * chunks_to), 16)))[2:].upper()

start_time = time.perf_counter()
for i in range(1, TESTS + 1):
    if not i % 100: print(f'{i} tests passed!')
    rand_n = random.randint(500, 501)
    b = gen_hex(rand_n, rand_n + 1)
    a = gen_hex(rand_n + 1, rand_n + 500)
    r = hex(int(a, 16) // int(b, 16))[2:].upper()

    data = subprocess.getoutput(' '.join(['./big', a, b])).split('\n')
    #print(data)
    if len(data) < 1:
        print('Incorrect response on test {} {}'.format(a, b))
        continue
    *_, answer = data
    answer = answer.upper()
    if SHOW_OUTPUT: print(answer)
    if answer.strip() != r:
        print("failed with {} as a and {} as b.\nanswer is {}\ncorrect_answer is {}".format(a, b, answer, r))
        break
    if SHOW_OUTPUT: print(f"Test {a}/{b} passed!")
end_time = time.perf_counter() - start_time
print('{} tests passed in {} seconds'.format(i, end_time))
