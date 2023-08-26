/*
* Caio Vinicius Pereira Silveira
* caiovpsilveira@gmail.com
* Maze path finder algorithm - Lab. AI
* CEFET-MG
* August 2023
*/

#include "Renderer.h"

#include "Maze.h"

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>
#include <assert.h>
#include <optional>
#include <tuple>

#define WHITE {255, 255, 255, 255}
#define BLACK {0, 0, 0, 0}
#define RED {255, 0, 0, 255}
#define GREEN {0, 255, 0, 255}
#define BLUE {0, 0, 255, 255}
#define YELLOW {255, 255, 0, 255}
#define LIGHT_BLUE {197, 239, 247, 1}
#define PURPLE {128, 0, 128, 255}
#define PLUM_PURPLE {221, 160, 221, 255}
#define NO_OUTLINE {}

Renderer::Renderer(const Maze& maze, unsigned w, unsigned h)
    : m_maze(maze)
    , m_windowWidth(w)
    , m_windowHeight(h)
    , m_window(nullptr)
    , m_renderer(nullptr)
{
    initWindow("Maze", m_windowHeight, m_windowWidth);
    initRenderer();
    update();
}

Renderer::~Renderer()
{
    if(m_renderer) SDL_DestroyRenderer(m_renderer);
    if(m_window) SDL_DestroyWindow(m_window);
    SDL_Quit();
}

void
Renderer::initWindow(const std::string& title, int width, int height)
{
    m_window = SDL_CreateWindow(title.c_str(),
                                SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED,
                                width, height, SDL_WINDOW_SHOWN);
}

void
Renderer::initRenderer()
{
    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
}

void
Renderer::update()
{
    setRenderDrawColor({0, 0, 0, 0}); // Black background
    SDL_RenderClear(m_renderer);
    for(unsigned line = 0; line < m_maze.lines(); ++line)
    {
        for(unsigned column = 0; column < m_maze.columns(); ++column)
        {
            const Cell* cell = m_maze.cellAt(line, column);
            assert(cell);
            drawCell(cell);
        }
    }
    SDL_RenderPresent(m_renderer);
}

void
Renderer::setRenderDrawColor(const rgbaTuple& tp)
{
    Uint8 r = std::get<0>(tp);
    Uint8 g = std::get<1>(tp);
    Uint8 b = std::get<2>(tp);
    Uint8 a = std::get<3>(tp);
    SDL_SetRenderDrawColor(m_renderer, r, g, b, a);
}

namespace
{

Renderer::rgbaTuple
cellColor(const Cell* cell)
{
    Renderer::rgbaTuple ret;
    Cell::Value val = cell->value();
    switch (val) 
    {
        case Cell::Value::FREE:
            if(cell->isPath())
                ret = YELLOW;
            else if(cell->nodeDeleted())
                ret = PURPLE;
            else if(cell->visited())
                ret = PLUM_PURPLE;
            else if(cell->discovered())
                ret = LIGHT_BLUE;
            else
                ret = WHITE;
        break;
        case Cell::Value::BLOCKED:
            ret = BLACK;
        break;
        case Cell::Value::START:
            ret = GREEN;
        break;
        case Cell::Value::END:
            ret = BLUE;
        break;
        case Cell::Value::INVALID:
        default:
            ret = BLACK;
        break;
    }
    return ret;
}

std::optional<Renderer::rgbaTuple>
cellOutline(const Cell* cell)
{
    std::optional<Renderer::rgbaTuple> ret;
    Cell::Value val = cell->value();
    switch (val) 
    {
        case Cell::Value::FREE:
            if(cell->nodeDeleted())
                ret = RED;
            else
                ret = BLACK;
        break;
        case Cell::Value::BLOCKED:
            ret = NO_OUTLINE;
        break;
        case Cell::Value::START:
            ret = NO_OUTLINE;
        break;
        case Cell::Value::END:
            ret = NO_OUTLINE;
        break;
        case Cell::Value::INVALID:
        default:
            ret = NO_OUTLINE;
        break;
    }
    return ret;
}

} // end of anonymous namespace

void
Renderer::drawCell(const Cell* cell)
{
    unsigned line = cell->linePos();
    unsigned column = cell->columnPos();

    unsigned cellWidth = m_windowWidth / m_maze.columns();
    unsigned cellHeigth = m_windowHeight / m_maze.lines();

    {
        // overwrite to draw square cell
        unsigned cellSize = std::min(cellWidth, cellHeigth); // draw square cells
        cellWidth = cellSize;
        cellHeigth = cellSize;
    }

    SDL_Rect rect;
    rect.x = column * cellWidth;
    rect.y = line * cellHeigth;
    rect.w = cellWidth;
    rect.h = cellHeigth;

    rgbaTuple tpCell = cellColor(cell);
    setRenderDrawColor(tpCell);
    SDL_RenderFillRect(m_renderer, &rect);

    auto outline = cellOutline(cell);
    if(outline)
    {
        rgbaTuple tpOutline = outline.value();
        setRenderDrawColor(tpOutline);
        SDL_RenderDrawRect(m_renderer, &rect);
    }
}