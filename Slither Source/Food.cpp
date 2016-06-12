#include "Food.h"

FoodMap::FoodMap(SDL_Renderer* rend) : renderer(rend)
{
    for (int i = 0; i < numberOfPieces; i++)
    {
        food* tempFood = new food;
        tempFood->tex = new Texture;
        tempFood->tex->loadFromFile(FOOD_FILE_PATHS[0], renderer);
        tempFood->fileName = FOOD_FILE_PATHS[0];
        tempFood->pos[0] = Random(0 + BORDER, LEVEL_SIZE.w - BORDER);
        tempFood->pos[1] = Random(0 + BORDER, LEVEL_SIZE.h - BORDER);
        tempFood->eaten = false;
        tempFood->radius = 10.0f;
        tempFood->gridReference[0] = tempFood->pos[0] / GRIDSIZE;
        tempFood->gridReference[1] = tempFood->pos[1] / GRIDSIZE;
        AllFood.push_back(tempFood);
    }

    srand(time(NULL));
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
    /*
    
    If piecesHidden > 0:
        Move hidden piece

    else 
        Generate new piece
        
    
    
    */

    if (piecesHidden > 0)
    {
        for (int i = 0; i < numberOfPieces; i++)
        {
            if (AllFood.at(i)->eaten)
            {
                AllFood.at(i)->pos[0] = x;
                AllFood.at(i)->pos[1] = y;
                AllFood.at(i)->eaten = false;
                AllFood.at(i)->gridReference[0] = AllFood.at(i)->pos[0] / GRIDSIZE;
                AllFood.at(i)->gridReference[1] = AllFood.at(i)->pos[1] / GRIDSIZE;
                piecesHidden--;
                break;

            }
        }
    }
    else
    {
        food* tempFood = new food;
        tempFood->tex = new Texture;
        tempFood->tex->loadFromFile(FOOD_FILE_PATHS[1], renderer);
        tempFood->fileName = FOOD_FILE_PATHS[1];
        tempFood->pos[0] = x;
        tempFood->pos[1] = y;
        tempFood->eaten = false;
        tempFood->radius = 10.0f;
        tempFood->gridReference[0] = tempFood->pos[0] / GRIDSIZE;
        tempFood->gridReference[1] = tempFood->pos[1] / GRIDSIZE;

        AllFood.push_back(tempFood);
        numberOfPieces++;
    }
    
    
}

void FoodMap::GenerateFood(float frameTime)
{
    //printf("Number of pieces hidden: (%i / %i)\n", piecesHidden, numberOfPieces);
    sinceLastFoodTime += frameTime;

    if (sinceLastFoodTime >= GENERATE_FOOD_TIME) // 3.0f
    {
        for (int j = 0; j < (int)(piecesHidden * FOODREGENRATE); ++j)
        {
            for (int i = 0; i < numberOfPieces; i++)
            {
                if (AllFood.at(i)->eaten)
                {
                   
                    food* tempFood = new food;
                    tempFood->tex = new Texture;
                    tempFood->tex->loadFromFile(FOOD_FILE_PATHS[1], renderer);
                    tempFood->fileName = FOOD_FILE_PATHS[1];
                    tempFood->pos[0] = Random(0 + BORDER, LEVEL_SIZE.w - BORDER);
                    tempFood->pos[1] = Random(0 + BORDER, LEVEL_SIZE.h - BORDER);
                    tempFood->eaten = false;
                    tempFood->radius = 10.0f;
                    tempFood->gridReference[0] = tempFood->pos[0] / GRIDSIZE;
                    tempFood->gridReference[1] = tempFood->pos[1] / GRIDSIZE;

                    AllFood.push_back(tempFood);
                    numberOfPieces++;
                    break;
                }
            }
            sinceLastFoodTime = 0.0f;
            
        }
       
    }

}

void FoodMap::HideFood(int index)
{
    AllFood.at(index)->eaten = true;
    piecesHidden++;
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