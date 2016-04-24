import os, sys
from random import randint as ri
from math import sqrt


maxW = 1000
maxL = 1000

def main():
    if len(sys.argv) < 2:
        print 'Please provide input parameter n'
        sys.exit()
    else:
        try:
            n = int(sys.argv[1])
            if n < 1:
                raise
        except:
            print '1st Argument must be valid positive integer'
            sys.exit()

    matrix = gen(n)
    for i in range(n):
        for j in range(n):
            #print matrix[i][j], ' '
            sys.stdout.write(str(matrix[i][j]) + ' ')
        #print '\n'
        sys.stdout.write('\n')

def gen(n):
    global maxW, maxL
    pos = [ (ri(0, maxW), ri(0, maxL)) for _ in range(n) ]
    M = [ [] for _ in range(n) ] * n
    for i in range(n):
        for j in range(n):
            M[i].append(dist(pos[i], pos[j]))
    return M

def dist(x, y):
    return int(sqrt((x[0] - y[0])**2 + ((x[1] - y[1]) ** 2)))


if __name__ == '__main__':
    main()
