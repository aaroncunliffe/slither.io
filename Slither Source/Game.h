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
//#include "Collision.h"



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

    Timer* capTimer;
    Timer* fpsTimer;
    int countedFrames = 0;

    std::vector<int> knownIDs;
    std::vector<Snake*> snakes;
    int NumberOfConnectedSnakes = 0;

    GameStates gameState = GameStates::PLAYING;
    
    bool networkConnected = false;
    PacketTypes lastPacket = PacketTypes::DISCONNECTED;


public:

    Game();

    bool CreateGameObjects();
    
    ~Game();

    bool init(const SDL_Rect& screenSize);

    void run(SDL_Event& e, float& frameTime, GameStates& state);

    void ProcessInputs(SDL_Event& event, float& frameTime, const SDL_Rect& screenSize, SDL_Point& touchLocation, GameStates& state);
    void ProcessPackets();

    void AIControl();
    void AICollision();

    void Collision();

    //Maths Functions
    float Distance(float x1, float y1, float x2, float y2);
    bool SphereToSphere(float x1, float y1, float r1, float x2, float y2, float r2);
    //


    void close();
};