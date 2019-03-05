from random import *
import sys

"""
Code taken from http://rosettacode.org/wiki/Maze_generation#Python

Generates a maze of size w x h
w is a positive integer
h is a positive integer
if w and/or h are too large (50 +), the code will crash

Returns a string that represents a maze.
"""
def make_maze(w, h):
    vis = [[0] * w + [1] for _ in range(h)] + [[1] * (w + 1)]
    ver = [["|   "] * w + ['|'] for _ in range(h)] + [[]]
    hor = [["+---"] * w + ['+'] for _ in range(h + 1)]
 
    def walk(x, y):
        vis[y][x] = 1
 
        d = [(x - 1, y), (x, y + 1), (x + 1, y), (x, y - 1)]
        shuffle(d)
        for (xx, yy) in d:
            if vis[yy][xx]: continue
            if xx == x: hor[max(y, yy)][x] = "+   "
            if yy == y: ver[y][max(x, xx)] = "    "
            walk(xx, yy)
 
    walk(randrange(w), randrange(h))
 
    s = ""
    for (a, b) in zip(hor, ver):
        s += ''.join(a + ['\n'] + b + ['\n'])
    return s
 
if __name__ == '__main__':
    if len(sys.argv)<2 or len(sys.argv)>2:
        print "Script takes one argument -- the length of the side of the maze"
        sys.exit(0)
    mazesize = 0    
    try:
        mazesize = int(sys.argv[1])
    except Exception as e:
        print "Script takes one argument -- the length of the side of the maze"
        sys.exit(0)
    if mazesize >40:
        print "Script cannot make mazes with a length greater than 40"
        sys.exit(0)
    print(make_maze(mazesize, mazesize))


