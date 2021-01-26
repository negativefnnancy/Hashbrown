#!/usr/bin/env python3

# generate a sine wave table as a static array

import math

n_samples = 1024
max_value = 256
radius = max_value / 2

samples = [round(min(max_value - 1, radius * math.sin(2 * math.pi * i / n_samples) + radius)) for i in range(n_samples)]
samples_string = ',\n    '.join([str(sample) for sample in samples])
print(f'{{\n    {samples_string}\n}}')
