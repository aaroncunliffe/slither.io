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
    int numberOfPieces = MAX_FOOD_PIECES;

    SDL_Renderer* renderer;

    float sinceLastFoodTime = 0.0f;
    
    int foodEaten = 0;

public:
    //Members
    std::vector<food*> AllFood;

    FoodMap(SDL_Renderer* rend);
    ~FoodMap();

    void Render(Texture* texture, SDL_Rect& camera);

    void DropFood(float x, float y);

    void HideFood(int index);
    void GenerateFood(float frameTime);

    float Random(int rangeMin, int rangeMax);


    //Getter
    int GetNumOfPieces() { return numberOfPieces; }

    std::vector<food*>GetAllFood() { return AllFood; }

};