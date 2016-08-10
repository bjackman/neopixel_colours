#!/bin/python
from math import sin, pi

NUM_INCREMENTS = 100

angles = [i * (2 * pi / NUM_INCREMENTS) for i in range(NUM_INCREMENTS)]

# sinusoid ranging between 0 and 1
sin_0_1 = [(sin(angle) + 1) / 2 for angle in angles]

sin_u8 = [int(s * 255) for s in sin_0_1]

assert(all([0 <= s <= 255 for s in sin_u8]))

vals = ", ".join([str(s) for s in sin_u8])

c_array_def = "char sin_table[] = { " + vals + " };"

print(c_array_def)
