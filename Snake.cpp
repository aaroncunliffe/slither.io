#include "Snake.h"
#include <iostream>>



Snake::Snake(SDL_Renderer* rend) :
    renderer(rend)
{
    // load up the textures
    if (!LoadMedia())
        throw;
    posX = LEVEL_SIZE.w / 3;
    posY = LEVEL_SIZE.h / 3;
    //Initialize the offsets
    //posX = 0;
    //posY = 0;

    //Initialize the velocity
    velX = 0;
    velY = 0;

}

bool Snake::LoadMedia()
{
    head = new Texture;

    if (!head->loadFromFile(SNAKE_HEAD_PATH, renderer))
        return false;

    return true;
}

Snake::~Snake()
{
    head->free();
}

void Snake::Move(float timeStep, SDL_Rect & camera)
{
  
    //Move the dot left or right
    posX += velX * SPEED_MULTIPLIER * timeStep;
    //If the dot went too far to the left or right
    if ((posX < 0) || (posX + HEAD_WIDTH > LEVEL_SIZE.w))
    {
        //Move back
        posX -= velX;
    }

    //Move the dot up or down
    posY += velY * SPEED_MULTIPLIER * timeStep;
    //If the dot went too far up or down
    if ((posY < 0) || (posY + HEAD_HEIGHT > LEVEL_SIZE.h))
    {
        //Move back
        posY -= velY;
    }
    
    // Update the direction of each segment
    for (int i = 0; i < NumberOfPieces; i++)
    {
        //Calculate movement to previous segments position
        float Vector2[2];
        if (i == 0)
        {
            Vector2[0] = (posX - camera.x) - (Pieces.at(i)->Position[0] - camera.x);
            Vector2[1] = (posY - camera.y) - (Pieces.at(i)->Position[1] - camera.y);
        }
        else if (i > 0)
        {
            Vector2[0] = (Pieces.at(i - 1)->Position[0] - camera.x) - (Pieces.at(i)->Position[0] - camera.x);
            Vector2[1] = (Pieces.at(i - 1)->Position[1] - camera.y) - (Pieces.at(i)->Position[1] - camera.y);

        }

        float vectorLength = sqrt(Vector2[0] * Vector2[0] + Vector2[1] * Vector2[1]);

        Pieces.at(i)->directionVector[0] = Vector2[0] / vectorLength;
        Pieces.at(i)->directionVector[1] = Vector2[1] / vectorLength;

        if (vectorLength > (HEAD_HEIGHT / 2))
        {
            Pieces.at(i)->Position[0] += Pieces.at(i)->directionVector[0] * SPEED_MULTIPLIER * timeStep;
            Pieces.at(i)->Position[1] += Pieces.at(i)->directionVector[1] * SPEED_MULTIPLIER * timeStep;
        }
    }


    // Update the position of each segment
    for (int i = 0; i < NumberOfPieces; i++)
    {
        //if(vectorLength)
        
    }


    
}

void Snake::CenterCamera(SDL_Rect& camera)
{
    camera.x = (static_cast<int>(posX + (HEAD_WIDTH / 2.0f))) - SCREEN_SIZE.w / 2;
    camera.y = (static_cast<int>(posY + (HEAD_HEIGHT / 2.0f))) - SCREEN_SIZE.h / 2;

    //Keep the camera in bounds
    if (camera.x < 0)
    {
        camera.x = 0;
    }
    if (camera.y < 0)
    {
        camera.y = 0;
    }
    if (camera.x > LEVEL_SIZE.w - camera.w)
    {
        camera.x = LEVEL_SIZE.w - camera.w;
    }
    if (camera.y > LEVEL_SIZE.h - camera.h)
    {
        camera.y = LEVEL_SIZE.h - camera.h;
    }
}

void Snake::MoveTo(float x, float y, SDL_Rect& camera)
{
    float Vector[2];

    Vector[0] = x - (posX - camera.x);
    Vector[1] = y - (posY - camera.y);

    float vectorLength = sqrt(Vector[0] * Vector[0] + Vector[1] * Vector[1]);

    directionVector[0] = Vector[0] / vectorLength;
    directionVector[1] = Vector[1] / vectorLength;

    velX = directionVector[0];
    velY = directionVector[1];

    

    //float angle = atan2(directionVector[1], directionVector[0]);
    //float print = angle * 180 / M_PI;
}

void Snake::AddNewPiece()
{
    SnakePiece* tempPiece = new SnakePiece;
    tempPiece->tex = new Texture;
    tempPiece->tex->loadFromFile(SNAKE_HEAD_PATH, renderer);
    tempPiece->directionVector[0] = directionVector[0];
    tempPiece->directionVector[1] = directionVector[1];
    
    if (NumberOfPieces == 0)
    {
        tempPiece->Position[0] = posX + (-directionVector[0] * HEAD_WIDTH / 2);
        tempPiece->Position[1] = posY + (-directionVector[1] * HEAD_WIDTH / 2);
    }
    else
    {
        tempPiece->Position[0] = Pieces.at(NumberOfPieces -1)->Position[0] + (-Pieces.at(NumberOfPieces - 1)->directionVector[0] * HEAD_WIDTH / 2);
        tempPiece->Position[1] = Pieces.at(NumberOfPieces -1)->Position[1] + (-Pieces.at(NumberOfPieces - 1)->directionVector[1] * HEAD_WIDTH / 2);
    }
    

    std::cout << posX << " | " << posX + (-directionVector[0] * HEAD_WIDTH / 2) << std::endl;

    Pieces.push_back(tempPiece);
    NumberOfPieces++;
}

void Snake::Render(SDL_Rect* viewport, SDL_Rect &camera)
{

    for (int i = NumberOfPieces - 1; i >= 0; --i)
    {
        Pieces.at(i)->tex->renderMedia(Pieces.at(i)->Position[0] - camera.x, Pieces.at(i)->Position[1] - camera.y, renderer);

    }

    head->renderMedia(posX - camera.x, posY - camera.y, renderer);
}