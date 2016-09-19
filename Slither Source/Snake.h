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
    float gridReference[2];
    float radius;
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
    float speed = 0;

    //The velocity of the dot
    float velX = 0.0f;
    float velY = 0.0f;

    float boostTimer = 0.0f;
    
    float lastScreenCoords[2] = { 0.0f, 0.0f };
    float lastScreenLength = 0.0f;
    float angle = 0.0f;     

    bool LoadMedia();

public:
    //Members
    int NumberOfPieces = 0;
    std::vector<SnakePiece*> Pieces;

    int ID;
    bool boosting = false;
    bool AI;
    bool dead = false;


    float Destination[2];
    float headGridReference[2];
    bool atDestination = true;

    //Initializes the variables
    Snake(SDL_Renderer* rend, float x, float y);

    ~Snake();

    void AddNewPiece();
    void RemovePiece();
    bool BoostCheck(float frameTime, int &score);

    void Die();

    void CenterCamera(SDL_Rect& camera);

    //Shows the dot on the screen relative to the camera
    void Render(SDL_Rect* viewport, SDL_Rect &camera);

    //Getters
    int getPosX() { return posX; };
    int getPosY() { return posY; };
    int getRadius() { return radius; }

    //Setters
    void setPosX(float x) { posX = x; }
    void setPosY(float y) { posY = y; }
    void setDirectionVector(int x, int y) { directionVector[0] = x; directionVector[1] = y; }

    //Movement
    void MoveTo(float x, float y, SDL_Rect& camera);
    void Move(float timeStep, SDL_Rect & camera);


    int getID() const{ return ID; }

    // For find
    /*bool operator== (const unsigned int &ID2) const
    {
        if (this->getID() == ID2)
            return true;
        else
            return false;
    }*/
};