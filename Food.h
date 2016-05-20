#pragma once

#include "Globals.h"
#include "Texture.h"

struct food
{
    Texture* tex;
    std::string fileName;
    int pos[2];
    bool eaten;
    float radius;
};

class FoodMap
{

private:
    int NUM_PIECES = LEVEL_SIZE.w / 4;

    SDL_Renderer* renderer;

public:
    //Members
    std::vector<food*> AllFood;

    FoodMap(SDL_Renderer* rend);
    ~FoodMap();

    void Render(Texture* texture, SDL_Rect& camera);
    
    float Random(int rangeMin, int rangeMax);

    //Getter
    int GetNumOfPieces() { return NUM_PIECES; }

    std::vector<food*>GetAllFood() { return AllFood; }

};