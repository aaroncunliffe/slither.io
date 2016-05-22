#include "Food.h"

FoodMap::FoodMap(SDL_Renderer* rend) : renderer(rend)
{
    for (int i = 0; i < numberOfPieces; i++)
    {
        food* tempFood = new food;
        tempFood->tex = new Texture;
        tempFood->tex->loadFromFile(FOOD_FILE_PATHS[i % 2], renderer);
        tempFood->fileName = FOOD_FILE_PATHS[i % 2];
        tempFood->pos[0] = Random(0, LEVEL_SIZE.w);
        tempFood->pos[1] = Random(0, LEVEL_SIZE.h);
        tempFood->eaten = false;
        tempFood->radius = 10.0f;

        AllFood.push_back(tempFood);
    }
}

FoodMap::~FoodMap()
{
    for (int i = 0; i < numberOfPieces; i++)
    {
        AllFood.at(i)->tex->free();
    }
}

void FoodMap::DropFood(float x, float y)
{
    for (int i = 0; i < numberOfPieces; i++)
    {
        if (AllFood.at(i)->eaten)
        {
            AllFood.at(i)->pos[0] = x;
            AllFood.at(i)->pos[1] = y;
            AllFood.at(i)->eaten = false;
            break;
        }
    }
    
}

void FoodMap::GenerateFood(float frameTime)
{
    sinceLastFoodTime += frameTime;

    if (sinceLastFoodTime >= 3.0f)
    {
        for (int i = 0; i < numberOfPieces; i++)
        {
            if (AllFood.at(i)->eaten)
            {
                food* tempFood = new food;
                tempFood->tex = new Texture;
                tempFood->tex->loadFromFile(FOOD_FILE_PATHS[i % 2], renderer);
                tempFood->fileName = FOOD_FILE_PATHS[i % 2];
                tempFood->pos[0] = Random(0, LEVEL_SIZE.w);
                tempFood->pos[1] = Random(0, LEVEL_SIZE.h);
                tempFood->eaten = false;
                tempFood->radius = 10.0f;

                AllFood.push_back(tempFood);
                numberOfPieces++;
                break;
            }
        }
        sinceLastFoodTime = 0.0f;
    }

}

void FoodMap::HideFood(int index)
{
    AllFood.at(index)->eaten = true;
}

void FoodMap::Render(Texture* texture, SDL_Rect& camera)
{

    for (int i = 0; i < numberOfPieces; i++)
    {
        if (!AllFood.at(i)->eaten)
        {
            AllFood.at(i)->tex->renderMedia(AllFood.at(i)->pos[0] - camera.x, AllFood.at(i)->pos[1] - camera.y, renderer);
        }
        
    }
}


float FoodMap::Random(int rangeMin, int rangeMax)
{
    float result = (float)rand() / (float)(RAND_MAX + 1);
    result *= (float)(rangeMax - rangeMin);
    result += rangeMin;

    return result;
}