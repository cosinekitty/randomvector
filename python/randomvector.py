#!/usr/bin/env python3

import math
import numpy

def randomvector(n):
    components = [numpy.random.normal() for i in range(n)]
    r = math.sqrt(sum(x*x for x in components))
    v = [x/r for x in components]
    return v

