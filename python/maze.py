import sys
import random

def create_walls_for_cell(cell, width, height):
    row,column = cell
    walls = []
    if column > 0: # left wall
        walls.append((row,column-1,"right"))
    if column < width - 1: # right wall
        walls.append((row,column,"right"))
    if row > 0: # up wall
        walls.append((row-1,column,"down"))
    if row < height - 1: # down wall
        walls.append((row, column, "down"))
    return walls

def create_maze(width, height):
    print "creating maze of %ix%i" % (width,height)

    walls = set()
    path = set()

    startCell = (0,0)
    path.add(startCell)

    walls.add((0,0,"right"))
    walls.add((0,0,"down"))

    openWalls = []

    while len(walls) > 0:
        # this is SLOW, but the set should never be very big (I think)
        wall = random.sample(walls, 1)[0]
        walls.remove(wall)

        # get two rooms
        if wall[2] == "right":
            room1 = (wall[0], wall[1])
            room2 = (wall[0], wall[1]+1)
        else: # down
            room1 = (wall[0], wall[1])
            room2 = (wall[0]+1, wall[1])

        # only one of the rooms is in the path
        if (room1 in path and room2 not in path) or (room1 not in path and room2 in path):
            openWalls.append(wall)
            if room1 in path:
                path.add(room2)
                for wall in create_walls_for_cell(room2, width, height):
                    walls.add(wall)
            else: # room2 already in path
                path.add(room1)
                for wall in create_walls_for_cell(room1, width, height):
                    walls.add(wall)

    return openWalls

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("usage: ./maze.py width height")
        exit(1)
    print create_maze(int(sys.argv[1]), int(sys.argv[2]))
