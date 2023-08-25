/*
* Caio Vinicius Pereira Silveira
* caiovpsilveira@gmail.com
* Maze path finder algorithm - Lab. AI
* CEFET-MG
* August 2023
*/

#ifndef SEARCH
#define SEARCH

#include "AStar.h"

#include <string>

class Maze;

class Search
{
public:
    explicit Search(const std::string& filepath);
    void search();
    std::string resultPathToStr() const;
private:
    AStar m_aStar;
};

#endif // SEARCH