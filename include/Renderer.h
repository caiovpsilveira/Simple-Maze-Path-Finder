/*
* Caio Vinicius Pereira Silveira
* caiovpsilveira@gmail.com
* Maze path finder algorithm - Lab. AI
* CEFET-MG
* August 2023
*/

#ifndef RENDERER
#define RENDERER

#include <SDL2/SDL.h>
#include <optional>
#include <string>
#include <tuple>

class Cell;
class Maze;

class Renderer
{
public:
    using rgbaTuple = std::tuple<Uint8 /*r*/, Uint8 /*g*/, Uint8 /*b*/, Uint8 /*a*/>;

public:
    Renderer(const Maze& maze, unsigned w, unsigned h);
    ~Renderer();
    void update();
private:
    void initWindow(const std::string& title, int width, int height);
    void initRenderer();

    void setRenderDrawColor(const rgbaTuple& tp);
    void drawCell(const Cell* cell);
private:
    const Maze& m_maze;
    unsigned m_windowWidth;
    unsigned m_windowHeight;
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
};

#endif // RENDERER