# Simple-Maze-Path-Finder
Simple maze path finder written in C++ using A* search algorithm and SDL2 to render.

# About
This program outputs a minimum path for the inputted maze, if it exists, "Unreachable", otherwise. This was done as an assignment of my college AI course, in August of 2023.

## Inputting the maze
The maze can be inputted in a file, representing the maze as a grid of maze cells.
Each cell can be one of four values: "S" for start cell, "E" for end cell, "F" for maze paths (free), and "B" for maze walls (blocked).
The first value of the input must be L, the number of lines in the maze grid, followed by C, the number of columns in the maze grid.
After that, the cell values, by column. There must be exactly one start cell and one end cell. Examples of input can be found in the directory "tests".

## Output
If the path is found, it will be colored by yellow, and the sequence of cells travelled will be outputted, beginning from the start cell. The rendered maze will be updated after each step done by the algorithm. Note that this program outputs only the first minimum path it finds.
There is no randomness in the search, so it will always find the same path for a given maze.

The top left maze cell is at position [0, 0], and the botton right cell is at position [L-1, C-1], where L is the number of lines and C the number of columns in the maze.

The start cell is colored by green.

The end cell is colored by blue.

Discovered cells are colored by light blue.

Visited cells are colored by light purple.

Dead end cells are colored by dark purple.

Once the path is found, the cells that represent the path are colored by yellow.

## Building and running
This program depends on SDL2. To install it, run
`sudo apt-get install libsdl2-dev`

It can be built using the provided Makefile. To build it, run `make` on the root directory. You can also build a debug version with `make debug`.
Use `make clean` to remove all built files (*.o and *.out).

If you have any problems building/linking it with SDL2, check where was the library installed on your system and update the Makefile.

To run it, run `./program.out maze_filepath`.

# Implementation details
The search is done using the A* (A star) algorithm, with the manhattan distance between the current cell and the end cell as the heuristic.
Since this heuristic is admissible, it is guaranteed that the algorithm will find an optimal solution.

It uses a mimimum heap as a priority queue to store the leaves. This makes the algorithm always expand the leaf with the lowest total cost (current cost + heuristic cost) by each step.
This also makes inserting new leaves a cost of O(log n), where n is the number of current leaves.

As said, at each step the lowest cost leaf is expanded. New discovered leaves are inserted into the queue and the explored leaf is set as visited, removed from the queue and added as a inner node.

If it's found that there are no available childs from a certain leaf, it is set as a "Dead end", and the node is deleted from the memory. This is done iteratively for the leaf's parent,
until all the nodes that leaded into that dead end are removed.

Once the end cell is found, it's also added as a leaf. The stop condition is when the end is the priority queue's top, which mean the end was reached with the lowest cost possible. When this happens, the top (end cell node) leaf is set as a path node, and also the top node's parent, and so on, iteratively, until the start cell's node is found.

The graphical output is done by using the SDL2 to render. At the end of each stop done by the algorithm (visiting and discovering cells, removing dead ends and finding the path) the renderer is updated.

## Images
### Maze expanding
![image](https://github.com/caiovpsilveira/Simple-Maze-Path-Finder/assets/86082269/90ddf8f3-43fb-4885-ad14-04113789e69c)
### Maze path found
![image](https://github.com/caiovpsilveira/Simple-Maze-Path-Finder/assets/86082269/714dc744-c41f-48b3-8923-fa02294ef405)
### Minimum path
![image](https://github.com/caiovpsilveira/Simple-Maze-Path-Finder/assets/86082269/4535ad95-20dc-4591-9bc3-991005652c98)
### Unreachable end
![image](https://github.com/caiovpsilveira/Simple-Maze-Path-Finder/assets/86082269/f7b2a943-6620-45f3-9ca4-37769dd4d936)
### Dead end found
![image](https://github.com/caiovpsilveira/Simple-Maze-Path-Finder/assets/86082269/7979a2ac-0c0c-4813-b87c-4346cf377ff4)
### Big maze
![image](https://github.com/caiovpsilveira/Simple-Maze-Path-Finder/assets/86082269/b64ae94e-654e-4570-a5a5-a1f4b92f37cc)



