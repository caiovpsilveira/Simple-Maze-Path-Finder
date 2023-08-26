/*
* Caio Vinicius Pereira Silveira
* caiovpsilveira@gmail.com
* Maze path finder algorithm - Lab. AI
* CEFET-MG
* August 2023
*/

#include "AStar.h"

#include "Debug.h"
#include "Maze.h"
#include "Renderer.h"

#include <algorithm>
#include <cassert>
#include <chrono>
#include <thread>

Node::Node(Node* parent, Cell* cell, unsigned currentCost, unsigned heuristicCost)
    : m_parent(parent)
    , m_cell(cell)
    , m_currentCost(currentCost)
    , m_heuristicCost(heuristicCost)
    , m_activeChilds(0)
{}

Node*
Node::addChild(Cell* childCell, unsigned currentCost, unsigned heuristicCost)
{
    Node* n = new Node(this, childCell, currentCost, heuristicCost);
    incActiveChilds();
    DEBUG_PRINTLN("Added child [" << n->cell()->linePos() << "," << n->cell()->columnPos() << "] : cost "\
        << n->currentCost() + n->heuristicCost());
    return n;
}

namespace
{
unsigned manhattanDistance(const Cell& currentCell, const Cell& endPoint)
{
    unsigned xDistance = std::abs((int) currentCell.linePos() - (int) endPoint.linePos());
    unsigned yDistance = std::abs((int) currentCell.columnPos() - (int) endPoint.columnPos());
    return xDistance + yDistance;
}
}; // end of anonymous namespace

template<typename T, typename T2, typename T3>
bool CustomPriorityQueue<T, T2, T3>::remove(const T& value)
{
    auto it = std::find(this->c.begin(), this->c.end(), value);

    if (it == this->c.end()) {
        return false;
    }
    if (it == this->c.begin()) {
        // deque the top element
        this->pop();
    }    
    else {
        // remove element and re-heap
        // TODO: implement heap removal function
        // There is no need to re-heap when removing from
        // an already built heap.
        this->c.erase(it);
        std::make_heap(this->c.begin(), this->c.end(), this->comp);
    }
    return true;
}

AStar::AStar(const std::string& filepath)
    : m_maze(filepath)
    , m_renderer(m_maze, 600, 600)
{
    Cell& start = m_maze.startCell();
    const Cell& end = m_maze.endCell();
    unsigned currentCost = 0;
    unsigned heuristicCost = manhattanDistance(start, end);
    m_root = new Node(nullptr, &start, currentCost, heuristicCost);
    m_root->cell()->setDiscovered();
    m_root->cell()->setVisited();
    m_leaves.push(m_root);
    DEBUG_PRINTLN("Finished building starting leaf. Root cell is [" << m_root->cell()->linePos() << "," << m_root->cell()->columnPos() << "].");
};

AStar::~AStar()
{
    for(const Node* n : m_innerNodes)
    {
        assert(n);
        delete n;
        #ifdef DEBUG
            ++m_deletedNodes;
        #endif
    }
    while(!m_leaves.empty())
    {
        Node* n = m_leaves.top();
        delete n;
        #ifdef DEBUG
            ++m_deletedNodes;
        #endif
        m_leaves.pop();
    }
    DEBUG_PRINTLN("Allocated " << m_allocatedNodes << " nodes and deleted " << m_deletedNodes << " nodes.");
}

void
AStar::expandLeaves()
{
    Node* node = m_leaves.top();
    m_leaves.pop(); // remove searched node before adding childs on top
    assert(node);

    std::vector<Cell*> childs;

    {
        std::vector<Cell*> availableCells = m_maze.availableMoves(*node->cell());
        for(Cell* c : availableCells)
        {
            if(!c->visited()) childs.push_back(c);
        }
    }

    node->cell()->setVisited();
    m_renderer.update();

    for(Cell* c : childs)
    {
        assert(c);
        if(c->discovered())
        {
            Node* childNode = m_cellToNode[c];
            // If another path with lower cost to the previous one is found
            // swap parents
            if(childNode->currentCost() > node->currentCost() + 1)
            {
                swapParents(childNode, node);
            }
        }
        else{
            c->setDiscovered();
            m_renderer.update();

            Node* newNode = node->addChild(c, node->currentCost() + 1, manhattanDistance(*c, m_maze.endCell()));
            m_cellToNode[c] = newNode;
            #ifdef DEBUG
                ++m_allocatedNodes;
            #endif
            m_leaves.push(newNode);
        }
    }

    if(node->activeChilds() == 0)
    {
        removeDeadEnds(node);
        return;
    }

    m_innerNodes.insert(node);
}

void
AStar::swapParents(Node* childNode, Node* newParent)
{
    DEBUG_PRINTLN("New parent with cost " << newParent->currentCost() << " found. Current parent has cost "\
        << childNode->parent()->currentCost() << ". Swapping parents.");

    Node* prevParent = childNode->parent();
    prevParent->decActiveChilds();
    removeDeadEnds(prevParent);

    childNode->setParent(newParent);
    newParent->incActiveChilds();

    // update cost, there is no need to recompute manhattan distance
    childNode->setCurrentCost(newParent->currentCost() + 1);
    // heapify with updated cost
    m_leaves.remove(childNode);
    m_leaves.push(childNode);
}

void
AStar::removeDeadEnds(Node* node)
{
    // If dead end (no way to reach the end from that path)
    // there is no need to keep cached the nodes that expanded
    // ONLY into that particular path

    while(node && node->activeChilds() == 0)
    {
        DEBUG_PRINTLN("Dead end found at cell [" << node->cell()->linePos() << "," << node->cell()->columnPos() << "]. Freeing node.");
        Node* parent = node->parent();
        node->cell()->setDeleted(); // show de-allocation on renderer

        // Remove from inner nodes
        // This is used when swapping parents.
        // If the dead end is found because there is no child,
        // the node has not been inserted as a inner node
        auto it = m_innerNodes.find(node);
        if(it != m_innerNodes.end())
        {
            m_innerNodes.erase(it);
        }

        delete node;
        #ifdef DEBUG
            ++m_deletedNodes;
        #endif

        if(parent)
        {
            parent->decActiveChilds();
        }
        node = parent;
        std::this_thread::sleep_for(std::chrono::milliseconds(400));
        m_renderer.update();
    }
}

void
AStar::advance()
{
    if(m_leaves.empty())
    {
        m_finished = true;
        m_result = Result::UNREACHABLE;
        return;
    }

    expandLeaves();

    if(!m_leaves.empty() && m_leaves.top()->cell()->value() == Cell::Value::END)
    {
        m_finished = true;
        m_result = Result::END_REACHED;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(400));
    m_renderer.update();
};

std::list<const Cell*>
AStar::getPath()
{
    assert(m_finished);
    assert(m_leaves.top()->cell()->value() == Cell::Value::END);

    std::list<const Cell*> result;

    const Node* curNode = m_leaves.top();
    assert(curNode);

    // get reversed order of nodes that
    // expanded into this path
    // starting from root (start cell), until end cell
    while (curNode != nullptr) {
        Cell* c = curNode->cell();
        result.push_front(c);
        curNode = curNode->parent();

        c->setPath();
        std::this_thread::sleep_for(std::chrono::milliseconds(40));
        m_renderer.update();
    }

    return result;
}