/*
* Caio Vinicius Pereira Silveira
* caiovpsilveira@gmail.com
* Maze path finder algorithm - Lab. AI
* CEFET-MG
* August 2023
*/

#ifndef MAZE
#define MAZE

#include <vector>
#include <string>

class Cell
{
public:
    enum class Value
    {
        START,
        FREE,
        BLOCKED,
        END,
        INVALID
    };

    Cell(Value v, unsigned linePos, unsigned columnPos);
    Value value() const { return m_value; };
    unsigned linePos() const { return m_linePos; }
    unsigned columnPos() const { return m_columnPos; }
    std::string toString() const;
    static Value fromString(const std::string& str);
    static bool isValidMove(Value v);
    bool discovered() const { return m_discovered; }
    void setDiscovered() { m_discovered = true; }
    bool visited() const { return m_visited; }
    void setVisited() { m_visited = true; }
    bool nodeDeleted() const { return m_nodeDeleted; }
    void setDeleted() { m_nodeDeleted = true; }
    bool isPath() const { return m_isPath; }
    void setPath() { m_isPath = true; }

private:
    Value m_value;
    bool m_discovered;
    bool m_visited;
    bool m_nodeDeleted; // only for showing the de-allocation of nodes when finding dead ends in the renderer.
    bool m_isPath;  // only for showing the path found in renderer
    unsigned m_linePos;
    unsigned m_columnPos;
};

class Maze
{
public:
    explicit Maze(const std::string& filepath);
    ~Maze();
    unsigned lines() const { return m_lines; }
    unsigned columns() const { return m_columns; }
    Cell& startCell() const { return *m_startCell; }
    Cell& endCell() const { return *m_endCell; }
    const Cell* cellAt(unsigned line, unsigned column) const;
    std::vector<Cell*> availableMoves(const Cell& cell);
private:
    void readInputFile(const std::string& filepath);
    unsigned m_lines;
    unsigned m_columns;
    std::vector<std::vector<Cell>>* m_maze;
    Cell* m_startCell;
    Cell* m_endCell;
};

#endif // MAZE
