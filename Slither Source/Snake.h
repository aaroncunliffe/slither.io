#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include "Globals.h"
#include "Texture.h";

struct SnakePiece
{
    Texture* tex;
    float Position[2];
    float directionVector[2];
};

class Snake
{
private:

    SDL_Renderer* renderer;

    Texture* head;

    //The dimensions of the dot
    const float HEAD_WIDTH = 20.0f;
    const float HEAD_HEIGHT = 20.0f;
    const float radius = 10.0f;

    //Maximum axis velocity of the dot
    const float SPEED_MULTIPLIER = 100.0f;
    const float BOOST_MULTIPLIER = 2.5f;

    //The X and Y offsets of the dot
    float posX, posY;
    float directionVector[2];

    //The velocity of the dot
    float velX = 0.0f;
    float velY = 0.0f;

    float boostTimer = 0.0f;
    

    // Score / length / sections 
    

    bool LoadMedia();

public:
    //Members
    int NumberOfPieces = 0;
    std::vector<SnakePiece*> Pieces;

    bool boosting = false;

    //Initializes the variables
    Snake(SDL_Renderer* rend);

    ~Snake();

    void AddNewPiece();
    void RemovePiece();
    bool BoostCheck(float frameTime, int &score);

    void CenterCamera(SDL_Rect& camera);

    //Shows the dot on the screen relative to the camera
    void Render(SDL_Rect* viewport, SDL_Rect &camera);

    //Position accessors
    int getPosX() { return posX; };
    int getPosY() { return posY; };
    int getRadius() { return radius; }

    //Movement
    void MoveTo(float x, float y, SDL_Rect& camera);
    void Move(float timeStep, SDL_Rect & camera);
};