/*
* Caio Vinicius Pereira Silveira
* caiovpsilveira@gmail.com
* Maze path finder algorithm - Lab. AI
* CEFET-MG
* August 2023
*/

#ifndef A_STAR
#define A_STAR

#include <list>
#include <queue>
#include <set>
#include <string>
#include <unordered_map>

class Cell;
class Maze;
class Renderer;

class Node
{
public:
    Node(Node* parent, Cell* cell, unsigned currentCost, unsigned heuristicCost);

    Node* addChild(Cell* childCell, unsigned currentCost, unsigned heuristicCost);
    Cell* cell() const { return m_cell; }
    Node* parent() const  { return m_parent; }
    void setParent(Node* p) { m_parent = p; }
    unsigned currentCost() const { return m_currentCost; }
    void setCurrentCost(unsigned c) { m_currentCost = c; }
    unsigned heuristicCost() const { return m_heuristicCost; }
    unsigned activeChilds() const { return m_activeChilds; }
    void incActiveChilds() { ++m_activeChilds; }
    void decActiveChilds() { --m_activeChilds; }
private:
    Node* m_parent;
    Cell* m_cell;
    unsigned m_currentCost;
    unsigned m_heuristicCost; 
    unsigned m_activeChilds;
};

class NodeCostComparison { // custom comparator for priority queue
public:
    bool operator()(Node* a, Node* b){
        // lowest f() + g() has priority to expand

        // When true is returned, it means the order is correct and NO swapping of elements takes place.
        // When false is returned, it means the order is NOT correct and swapping of elements takes place.

        // prioritize leaves with greater currentCost when total cost is equal
        // this makes it looks nicer on the renderer imo :>
        // the only important thing is to preserve the A* expanding leaves with lowest total cost

        if(a->currentCost() + a->heuristicCost() == b->currentCost() + b->heuristicCost())
        {
            return a->currentCost() <= b->currentCost();
        }
        else
        {
            return a->currentCost() + a->heuristicCost() > b->currentCost() + b->heuristicCost();
        }
    }
};

template<typename T, typename T2, typename T3>
class CustomPriorityQueue : public std::priority_queue<T, T2, T3>
{
public:
    bool remove(const T& value);
};

class AStar
{
public:
    enum class Result
    {
        UNREACHABLE,
        END_REACHED
    };

    explicit AStar(const std::string& filepath);
    ~AStar();

    void advance();
    bool finished() const { return m_finished; }
    Result result() const { return m_result; }

    std::list<const Cell*> getPath() const;
private:
    void expandLeaves();
    void removeDeadEnds(Node* node);
    void swapParents(Node* childNode, Node* newParent);

private:
    Maze* m_maze;
    bool m_finished = false;
    Result m_result;
    Node* m_root;
    std::set<const Node*> m_innerNodes; // nodes that were already searched, and have childs. Used to more easily delete all nodes
    std::unordered_map<Cell*, Node*> m_cellToNode;

    // using a priority queue with custom comparator
    // to store the lowest f() + g() (currentCost + heuristicCost)
    // of the leaves
    // leaves are nodes yet to be expanded (searched for available moves)  
    CustomPriorityQueue<Node*, std::vector<Node*>, NodeCostComparison> m_leaves;

    Renderer* m_renderer;

    #ifdef DEBUG
        unsigned m_allocatedNodes = 1; // skipping root as is always allocated
        unsigned m_deletedNodes = 0;
    #endif
};

#endif // A_STAR