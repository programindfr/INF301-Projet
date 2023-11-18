#!python3

import sys
from math import sqrt

class Node:
    def __init__(self, ctx, coords, parent = None):
        self.x = coords[0]
        self.y = coords[1]
        self.h = ctx.hmap[self.y][self.x]
        self.g = parent.g + 1 if parent is not None else 0
        self.parent = parent

    def __repr__(self):
        return f"<Node ({self.x},{self.y}) c={self.h+self.g} \n \\--> {self.parent}>"

    def __eq__(self, other):
        return self.x == other.x and self.y == other.y

    def __lt__(self, other):
        return self.h + self.g < other.h + other.g

class Map:
    def __init__(self, mapstr: str):
        self.hmap = []

        for y, line in enumerate(mapstr.split("\n")):
            newline = []
            for x, c in enumerate(line):
                if c in ('.', 'C', '@'):
                    newline.append(0)
                    if c == 'C':
                        self.dep = (x, y)
                    elif c == '@':
                        self.arr = (x, y)
                elif c in ('~', '#'):
                    newline.append(1000)
            self.hmap.append(newline)

        for i, line in enumerate(self.hmap):
            for j in range(len(line)):
                self.hmap[i][j] += int(sqrt((self.arr[0] - j)**2 + (self.arr[1] - i)**2))

    def to_cpath(self):
        node_list = []
        while self.final is not None:
            node_list.insert(0, self.final)
            self.final = self.final.parent

        cpath = ""
        orientation = 'E'

        for idx, current in enumerate(node_list):
            if idx+1 < len(node_list):
                next_node = node_list[idx+1] 

                if orientation == 'E':
                    if next_node.x - current.x == 1 \
                       and next_node.y == current.y:
                        cpath += "A"
                    elif next_node.x - current.x == -1 \
                         and next_node.y == current.y:
                        cpath += "GGA"
                        orientation = 'O'
                    elif next_node.x == current.x \
                         and next_node.y - current.y == 1:
                        cpath += "DA"
                        orientation = 'S'
                    elif next_node.x == current.x \
                         and next_node.y - current.y == -1:
                        cpath += "GA"
                        orientation = 'N'
                elif orientation == 'O':
                    if next_node.x - current.x == 1 \
                       and next_node.y == current.y:
                        cpath += "DDA"
                        orientation = 'E'
                    elif next_node.x - current.x == -1 \
                         and next_node.y == current.y:
                        cpath += "A"
                    elif next_node.x == current.x \
                         and next_node.y - current.y == 1:
                        cpath += "GA"
                        orientation = 'S'
                    elif next_node.x == current.x \
                         and next_node.y - current.y == -1:
                        cpath += "DA"
                        orientation = 'N'
                elif orientation == 'N':
                    if next_node.x - current.x == 1 \
                       and next_node.y == current.y:
                        cpath += "DA"
                        orientation = 'E'
                    elif next_node.x - current.x == -1 \
                         and next_node.y == current.y:
                        cpath += "GA"
                        orientation = 'O'
                    elif next_node.x == current.x \
                         and next_node.y - current.y == 1:
                        cpath += "DDA"
                        orientation = 'S'
                    elif next_node.x == current.x \
                         and next_node.y - current.y == -1:
                        cpath += "A"
                elif orientation == 'S':
                    if next_node.x - current.x == 1 \
                       and next_node.y == current.y:
                        cpath += "GA"
                        orientation = 'E'
                    elif next_node.x - current.x == -1 \
                         and next_node.y == current.y:
                        cpath += "DA"
                        orientation = 'O'
                    elif next_node.x == current.x \
                         and next_node.y - current.y == 1:
                        cpath += "A"
                    elif next_node.x == current.x \
                         and next_node.y - current.y == -1:
                        cpath += "DDA"
                        orientation = 'N'

        print(cpath)

    def findpath(self):
        open_list = [Node(self, self.dep)]
        closed_list = []

        while Node(self, self.arr) not in closed_list:
            open_list.sort()
            current = open_list.pop(0)
            closed_list.append(current)

            if current is not None:
                for c in ((current.x, current.y-1), 
                          (current.x, current.y+1), 
                          (current.x-1, current.y), 
                          (current.x+1, current.y)):
                    if c[0] >= 0 and c[1] >= 0 \
                       and c[0] < len(self.hmap[0]) \
                       and c[1] < len(self.hmap):
                        child = Node(self, c, current)
                        if child not in closed_list:
                            if child in open_list:
                                old_idx = open_list.index(child)
                                if child < open_list[old_idx]:
                                    open_list.pop(old_idx)
                                    open_list.append(child)
                            else:
                                open_list.append(child)
            else:
                raise Exception("WTF")

        self.final = closed_list.pop(closed_list.index(Node(self, self.arr)))

        return self

if __name__ == "__main__":
    Map(sys.argv[1]).findpath().to_cpath()
