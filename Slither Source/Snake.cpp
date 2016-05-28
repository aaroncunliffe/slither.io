#include "Snake.h"
#include <iostream>>


// MOVEMENT CODE NEEDS TIDYING UP


Snake::Snake(SDL_Renderer* rend, float x, float y) :
    renderer(rend)
{
    // load up the textures
    if (!LoadMedia())
        throw;

    posX = x;
    posY = y;
    //Initialize the offsets
    //posX = 0;
    //posY = 0;

    //Initialize the velocity
    velX = 0;
    velY = 0;

    //Positions all of the new pieces correctly
    directionVector[0] = 0.0f;
    directionVector[1] = -0.5f;

    AddNewPiece();
    AddNewPiece();
    AddNewPiece();
    AddNewPiece();

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

void Snake::Move(float frameTime, SDL_Rect & camera)
{
    if (boosting)
    {
        posX += velX * (SPEED_MULTIPLIER * BOOST_MULTIPLIER) * frameTime;  //Move the dot left or right
        posY += velY * (SPEED_MULTIPLIER * BOOST_MULTIPLIER) * frameTime;  //Move the dot up or down
    }
    else
    {
        posX += velX * SPEED_MULTIPLIER * frameTime;  //Move the dot left or right
        posY += velY * SPEED_MULTIPLIER * frameTime;  //Move the dot up or down
    }
    
    //If the dot went too far to the left or right
    if ((posX < 0) || (posX + HEAD_WIDTH > LEVEL_SIZE.w))
    {
        if (boosting)
        {
            posX -= velX * (SPEED_MULTIPLIER * BOOST_MULTIPLIER) * frameTime;  //Move the dot left or right
        }
        else
        {
            posX -= velX * SPEED_MULTIPLIER * frameTime;  //Move the dot left or right
        }
    }

    //If the dot went too far up or down
    if ((posY < 0) || (posY + HEAD_HEIGHT > LEVEL_SIZE.h))
    {
        if (boosting)
        {
            posY -= velY * (SPEED_MULTIPLIER * BOOST_MULTIPLIER) * frameTime;  //Move the dot up or down
        }
        else
        {
            posY -= velY * SPEED_MULTIPLIER * frameTime;  //Move the dot up or down
        }
    }
    
    // Update the direction of each segment
    for (int i = 0; i < NumberOfPieces; i++)
    {
        //Calculate movement to previous segments position
        float Vector[2];
        if (i == 0) // if first piece, follow head
        {
            Vector[0] = (posX - camera.x) - (Pieces.at(i)->Position[0] - camera.x);
            Vector[1] = (posY - camera.y) - (Pieces.at(i)->Position[1] - camera.y);
        }
        else if (i > 0)
        {
            Vector[0] = (Pieces.at(i - 1)->Position[0] - camera.x) - (Pieces.at(i)->Position[0] - camera.x);
            Vector[1] = (Pieces.at(i - 1)->Position[1] - camera.y) - (Pieces.at(i)->Position[1] - camera.y);

        }

        float vectorLength = sqrt(Vector[0] * Vector[0] + Vector[1] * Vector[1]);

        Pieces.at(i)->directionVector[0] = Vector[0] / vectorLength;
        Pieces.at(i)->directionVector[1] = Vector[1] / vectorLength;

        // Moves each piece keeping a fixed distance between them
        if (vectorLength > (HEAD_HEIGHT / 2))
        {
            if (i == 0) // if first piece, follow head
            {
                Pieces.at(i)->Position[0] = posX - ((HEAD_HEIGHT / 2) * Pieces.at(i)->directionVector[0]);
                Pieces.at(i)->Position[1] = posY - ((HEAD_HEIGHT / 2) * Pieces.at(i)->directionVector[1]);
            }
            else if (i > 0)
            {
                Pieces.at(i)->Position[0] = Pieces.at(i - 1)->Position[0] - ((HEAD_HEIGHT / 2) * Pieces.at(i)->directionVector[0]);
                Pieces.at(i)->Position[1] = Pieces.at(i - 1)->Position[1] - ((HEAD_HEIGHT / 2) * Pieces.at(i)->directionVector[1]);

            }
           
            
        }
    }
}

bool Snake::BoostCheck(float frameTime, int &score)
{
    if (boosting && score > 0)
    {
        boostTimer += frameTime;
        if (boostTimer > SNAKE_BOOST_TIMER)
        {

            boostTimer = 0.0f;
            RemovePiece();
            score--;
            return true;
        }

    }
    else
    {
        boostTimer = 0.0f;
        boosting = false;
    }
    return false;
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
    
    Pieces.push_back(tempPiece);
    NumberOfPieces++;
}

void Snake::RemovePiece()
{
    Pieces.pop_back();
    NumberOfPieces--;
}

void Snake::Render(SDL_Rect* viewport, SDL_Rect &camera)
{

    for (int i = NumberOfPieces - 1; i >= 0; --i)
    {
        Pieces.at(i)->tex->renderMedia(Pieces.at(i)->Position[0] - camera.x, Pieces.at(i)->Position[1] - camera.y, renderer);
    }

    head->renderMedia(posX - camera.x, posY - camera.y, renderer);
}


// Old code for fixed distance between segments
/*if (boosting)
{
Pieces.at(i)->Position[0] += Pieces.at(i)->directionVector[0] * (SPEED_MULTIPLIER * BOOST_MULTIPLIER) * frameTime;
Pieces.at(i)->Position[1] += Pieces.at(i)->directionVector[1] * (SPEED_MULTIPLIER * BOOST_MULTIPLIER) * frameTime;
}
else
{
Pieces.at(i)->Position[0] += Pieces.at(i)->directionVector[0] * SPEED_MULTIPLIER * frameTime;
Pieces.at(i)->Position[1] += Pieces.at(i)->directionVector[1] * SPEED_MULTIPLIER * frameTime;
}*/