/*
* Caio Vinicius Pereira Silveira
* caiovpsilveira@gmail.com
* Maze path finder algorithm - Lab. AI
* CEFET-MG
* August 2023
*/

/*
This program outputs a minimal path for a maze.
The maze must have 1 "start" point and one "end" point

The maze must be inputed as a grid in a file,
containing values "Start" (or "S"), "Free" (or "F"),
"Blocked" (or "B") or "End" (or "E").

The first entry should indicate the number of lines,
and the second the number of columns. After that, the
maze cell entries, iterated by column

Example of input file:
3
2
Start   Blocked 
Free    Free
Blocked End

The output will be the sequence traversed in the grid, [line, column]
[0, 0] is the top left grid cell, if the end can be reached,
"Unreachable", otherwise.

This program assumes that frontier grid cells (ex [-1, 0] and [0, -1])
are the same as blocked (maze walls).
*/

#include "Search.h"

#include <iostream>

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        std::cout << "Usage: " <<argv[0] << " filepath" << std::endl;
        exit(-1);
    }

    std::string filepath(argv[1]);
    Search search(filepath);

    search.search();
    std::cout << search.resultPathToStr();

    std::cin.get(); // wait for input before closing window

    return 0;
}