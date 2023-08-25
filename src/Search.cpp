/*
* Caio Vinicius Pereira Silveira
* caiovpsilveira@gmail.com
* Maze path finder algorithm - Lab. AI
* CEFET-MG
* August 2023
*/

#include "Search.h"

#include "Debug.h"
#include "Maze.h" // Cell

#include <cassert>
#include <string>

Search::Search(const std::string& filepath)
    : m_aStar(filepath)
{}

void
Search::search()
{
    DEBUG_PRINTLN("Starting search.");
    while(!m_aStar.finished())
    {
        m_aStar.advance();
    }
    DEBUG_PRINTLN("Finished search.");
}

std::string
Search::resultPathToStr()
{
    assert(m_aStar.finished());

    std::string str;
    AStar::Result res = m_aStar.result();

    if(res == AStar::Result::UNREACHABLE)
        str = "Unreachable.\n";
    else
    {
        assert(res == AStar::Result::END_REACHED);
        std::list<const Cell*> path = m_aStar.getPath(); // this is non const because updates the renderer
        for(const Cell* c : path)
        {
            assert(c);
            str += "[" + std::to_string(c->linePos()) + "," + std::to_string(c->columnPos()) + "]\n";
        }
        str += "Total cost: " + std::to_string(path.size() - 1 /*start position is put in path*/) + "\n";
    }
    return str;
}