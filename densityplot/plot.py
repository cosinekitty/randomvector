#!/usr/bin/env python3
#
#   uniform.py  -  Don Cross  -  2020-07-18
#
#   Plot a uniform probability distribution.
#
import sys
import math
import matplotlib.pyplot as plt

def UniformDensity():
    xlist = []
    ylist = []
    for i in range(-2000, +2000):
        x = i / 1000
        y = 0.5 if (-1 <= x <= +1) else 0
        xlist.append(x)
        ylist.append(y)
    return xlist, ylist

def NormalDensity():
    xlist = []
    ylist = []
    denom = math.sqrt(math.pi)
    for i in range(-2000, +2000):
        x = i / 1000
        y = math.exp(-x**2) / denom
        xlist.append(x)
        ylist.append(y)
    return xlist, ylist

def Plot(filename, title, xlist, ylist):
    plt.figure(figsize=(8, 6), dpi=100)
    plt.plot(xlist, ylist, 'b-')
    plt.ylim(0.0, 0.6)
    plt.yticks([i/10 for i in range(7)])
    plt.xlabel('Random Value')
    plt.ylabel('Probability Density')
    plt.title(title)
    plt.savefig(filename, dpi=100, bbox_inches='tight')
    #plt.show()
    plt.close('all')

if __name__ == '__main__':
    if len(sys.argv) == 2 and sys.argv[1] == 'uniform':
        xlist, ylist = UniformDensity()
        Plot('uniform.png', 'Uniform Distribution', xlist, ylist)
        sys.exit(0)

    if len(sys.argv) == 2 and sys.argv[1] == 'normal':
        xlist, ylist = NormalDensity()
        Plot('normal.png', 'Normal Distribution', xlist, ylist)
        sys.exit(0)

    print('USAGE: plot.py [uniform | normal]')
    sys.exit(1)
