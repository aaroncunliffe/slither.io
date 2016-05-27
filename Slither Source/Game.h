#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <iostream>>
#include "Network.h"
#include "Globals.h"
#include "UI.h"
#include "Snake.h"
#include "Food.h"
#include "Timer.h"


class Game
{
private:
    SDL_Window* window;
    SDL_Renderer* renderer;

    SDL_Point touchLocation;	// also mouse pointer location

    SDL_Rect* viewportMain;
    SDL_Rect* viewportFull;

    SDL_Rect cameraMain;
    SDL_Rect cameraFull;

    SDL_Surface* screenSurface = NULL;
    
    Texture* background;

    //Event handler
    SDL_Event e;

    int score = 0;

    //Classes
    UI* ui;
    Snake* snake;
    FoodMap* food;
    SlitherClient* client;
    

public:

    Game();

    bool CreateGameObjects();
    
    ~Game();

    bool init(const SDL_Rect& screenSize);

    void run(SDL_Event& e, float& frameTime, GameStates& state);

    void ProcessInputs(SDL_Event& event, float& frameTime, const SDL_Rect& screenSize, SDL_Point& touchLocation, GameStates& state);

    void Collision();

    void close();
};