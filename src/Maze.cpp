/*
* Caio Vinicius Pereira Silveira
* caiovpsilveira@gmail.com
* Maze path finder algorithm - Lab. AI
* CEFET-MG
* August 2023
*/

#include "Maze.h"

#include "Debug.h"

#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>

Cell::Cell(Value v, unsigned linePos, unsigned columnPos)
    : m_value(v)
    , m_discovered(false)
    , m_visited(false)
    , m_nodeDeleted(false)
    , m_isPath(false)
    , m_linePos(linePos)
    , m_columnPos(columnPos)
{}

std::string
Cell::toString() const
{
    switch(m_value)
    {
        case Value::START:
            //return "Start";
            return "S";
        case Value::FREE:
            //return "Free";
            return "F";
        case Value::BLOCKED:
            //return "Blocked";
            return "B";
        case Value::END:
            //return "End";
            return "E";
        case Value::INVALID:
        default:
            //return "Invalid";
            return "I";
    }
}

Cell::Value
Cell::fromString(const std::string& str)
{
    if(str == "Start" || str == "S")
        return Value::START;
    else if(str == "Free" || str == "F")
        return Value::FREE;
    else if(str == "Blocked" || str == "B")
        return Value::BLOCKED;
    else if(str == "End" || str == "E")
        return Value::END;

    return Value::INVALID;
}

bool
Cell::isValidMove(Value v)
{
    return v == Value::FREE
        || v == Value::END;
}

Maze::Maze(const std::string& filepath)
    : m_startCell(nullptr)
    , m_endCell(nullptr)
{
    DEBUG_PRINTLN("About to read input file.");
    readInputFile(filepath);
    DEBUG_PRINTLN("Finished building maze.");

    DEBUG_PRINTLN("Start position is " << m_startCell->linePos() << "," << m_startCell->columnPos() << "].");
    DEBUG_PRINTLN("End position is [" << m_endCell->linePos() << "," << m_endCell->columnPos() << "].");
    DEBUG_CODE(
        std::cout << "\n";
        for(std::vector<Cell> lineVec : *m_maze)
        {
            for(Cell c : lineVec)
            {
                std::cout << c.toString() << " ";
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    );
}

Maze::~Maze()
{
    delete m_maze;
}

void
Maze::readInputFile(const std::string& filepath)
{
    std::ifstream file(filepath);
    if(!file)
    {
        std::cout << "Error reading file \"" << filepath << "\"." << std::endl;
        exit(-1);
    }

    std::stringstream ss;
    ss << file.rdbuf();

    ss >> m_lines;
    ss >> m_columns;
    m_maze = new std::vector<std::vector<Cell>>(m_lines);

    for(unsigned line = 0; line < m_lines; ++line)
    {
        // NOTE: columns has to be previously reserved
        // because when inserting into vector
        // it may cause the allocator to reallocate it,
        // invalidating the reference of m_startCell and
        // m_endCell, if already found
        m_maze->at(line).reserve(m_columns);
        for(unsigned column = 0; column < m_columns; ++column)
        {
            std::string s;
            bool eof = ss.eof();
            if(!eof)
            {
                ss >> s;
            }
            if(eof || ss.fail()) // unexpected eof OR couldn't parse to std::string
            {
                std::cout << "Error: input file contain less than " << m_lines << " x " << m_columns << " entries.\n";
                exit(-1); 
            }

            Cell::Value val = Cell::fromString(s);
            if(val == Cell::Value::INVALID)
            {
                std::cout << "Error reading cell at [" << line << "," << column << "] : invalid cell value.\n";
                exit(-1);
            }


            // Cell c(val, line, column); ....push_back(c)
            // !!! The above will NOT WORK! c is a temporary object
            // and push_back creates a copy
            m_maze->at(line).push_back(Cell(val, line, column));
            Cell& c = m_maze->at(line).back();

            if(val == Cell::Value::START)
            {
                if(m_startCell)
                {
                    std::cout << "Error: input file contain more than one start position.\n";
                    exit(-1);
                }
                m_startCell = &c;
            }
            else if(val == Cell::Value::END)
            {
                if(m_endCell)
                {
                    std::cout << "Error: input file contain more than one end position.\n";
                    exit(-1);
                }
                m_endCell = &c;
            }
        }
        assert(m_maze->at(line).size() == m_columns);
    }
    assert(m_maze->size() == m_lines);

    if(!m_startCell)
    {
        std::cout << "Error: no start point defined." << std::endl;
        exit(-1);
    }
    if(!m_endCell)
    {
        std::cout << "Error: no end point defined." << std::endl;
        exit(-1);
    }
}

const Cell*
Maze::cellAt(unsigned line, unsigned column) const
{
    if(line > m_lines || column > m_columns)
        return nullptr;

    return &m_maze->at(line).at(column);
}

std::vector<Cell*>
Maze::availableMoves(const Cell& cell)
{
    unsigned line =  cell.linePos();
    unsigned col = cell.columnPos();
    DEBUG_PRINTLN("Searching for available moves at cell [" << line << "," << col << "].");

    std::vector<Cell*> result;

    if(line > 0)
    {
        Cell& c = m_maze->at(line-1).at(col);
        Cell::Value v = c.value();
        if(Cell::isValidMove(v))
        {
            result.push_back(&c);
        }
    }

    if(line < m_lines - 1)
    {
        Cell& c = m_maze->at(line+1).at(col);
        Cell::Value v = c.value();
        if(Cell::isValidMove(v))
        {
            result.push_back(&c);
        }     
    }

    if(col > 0)
    {
        Cell& c = m_maze->at(line).at(col-1);
        Cell::Value v = c.value();
        if(Cell::isValidMove(v))
        {
            result.push_back(&c);
        }
    }

    if(col < m_columns - 1)
    {
        Cell& c = m_maze->at(line).at(col+1);
        Cell::Value v = c.value();
        if(Cell::isValidMove(v))
        {
            result.push_back(&c);
        }
    }

    return result;
};