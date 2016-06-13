
#include "Game.h"


Game::Game()
{
    if (!init(SCREEN_SIZE))
        throw;

    if (!CreateGameObjects())
        throw;


    // set default touch location to the centre of the screen
    touchLocation.x = SCREEN_SIZE.w / 2;
    touchLocation.y = SCREEN_SIZE.h / 2;

    // create map viewport
    viewportMain = new SDL_Rect(UI_MAIN);	// simulation area
    //miniMapViewport = new SDL_Rect{ UI_MAIN.w - 200, 0, 200, 200 };
    cameraFull = LEVEL_SIZE;
    viewportFull = new SDL_Rect(LEVEL_SIZE);

    //create camera to show only part of the map within viewportMain
    cameraMain = UI_MAIN;

    background = new Texture;
    border = new Texture;

    background->loadFromFile(BACKGROUND_PATH, renderer);
    border->loadFromFile(BORDER_PATH, renderer);
}

bool Game::CreateGameObjects()
{
    capTimer = new Timer();
    fpsTimer = new Timer();
    fpsTimer->start();

    ui = new UI(renderer, viewportMain);
    snake = new Snake(renderer, LEVEL_SIZE.w /3, LEVEL_SIZE.h / 3);
    food = new FoodMap(renderer);
    client = new SlitherClient();

    for (int i = 0; i < NUMOFAI; ++i)
    {
        Snake* tempSnake = new Snake(renderer, food->Random(0, LEVEL_SIZE.w), food->Random(0, LEVEL_SIZE.h));
        tempSnake->AI = true;
        snakes.push_back(tempSnake);
    }
    
    

    return true;
}

Game::~Game()
{
    background->free();
    border->free();

    delete ui;
    delete snake;
    /*delete food;
    delete client;*/

}

bool Game::init(const SDL_Rect& screenSize)
{
    // initialise SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        return false;
    }

    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
    {
        printf("Warning: Linear texture filtering not enabled!");
    }


    // initialise SDL_image
    int initFlag = IMG_INIT_PNG | IMG_INIT_JPG;
    if ((IMG_Init(initFlag) & initFlag) != initFlag)
    {
        SDL_Quit();
        return false;
    }

    if (TTF_Init() == -1)
    {
        IMG_Quit();
        SDL_Quit();
        return false;
    }

    window = SDL_CreateWindow("Slither", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenSize.w, screenSize.h, SDL_WINDOW_SHOWN);	// width and height not used when set to fullscreen

    if (window == nullptr)
    {
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr)
    {
        SDL_DestroyWindow(window);
        window = nullptr;
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return false;
    }
    return true;

    //SDL_RenderSetLogicalSize(renderer, 1920, 1080);
}

void Game::run(SDL_Event& e, float& frameTime, GameStates& state)
{
    capTimer->start();

    //Calculate and correct fps
    float avgFPS = countedFrames / (fpsTimer->getTicks() / 1000.f);
    if (avgFPS > 2000000)
    {
        avgFPS = 0;
    }

    // set the screen colour
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

    //Clear screen
    SDL_RenderClear(renderer);

    //------------------
    // PROCESS INPUT
    //------------------
    if (gameState == GameStates::PLAYING)
    {
        if (networkConnected)
        {
            if (client->Poll(lastPacket))
            {
                ProcessPackets();
            }
        }
        else
        {
            client->AttemptConnection(networkConnected);
        }

        ProcessInputs(e, frameTime, SCREEN_SIZE, touchLocation, state);


        //------------------
        // UPDATE GAME
        //------------------
        snake->CenterCamera(cameraMain);
        snake->Move(frameTime, cameraMain);
        AIControl();
        for (int i = 0; i < snakes.size(); i++)
        {
            snakes.at(i)->Move(frameTime, cameraMain);
        }



        ui->UpdateUI(score, networkConnected, avgFPS);

        food->GenerateFood(frameTime);


        //------------------
        // COLLISION DETECTION
        //------------------
        Collision();
        AICollision();
        

        if (snake->BoostCheck(frameTime, score))
        {
            if (score >= 0)
            {
                food->DropFood(snake->Pieces.at(snake->NumberOfPieces - 1)->Position[0], snake->Pieces.at(snake->NumberOfPieces - 1)->Position[1]);
            }
        }

    } // end of if playing
    

    //------------------
    // Rendering
    //------------------

    background->renderMedia(0 - cameraMain.x, 0 - cameraMain.y, renderer);
    border->renderMedia(-1000 - cameraMain.x, -1000 - cameraMain.y, renderer);

    food->Render(background, cameraMain);
    for (int i = 0; i < snakes.size(); i++)
    {
        snakes.at(i)->Render(viewportMain, cameraMain);
    }
    snake->Render(viewportMain, cameraMain);
    ui->Render(touchLocation, cameraFull);
    

    client->SendPositionPacket(snake->getPosX(), snake->getPosY(), cameraMain); // Send packets


    //Update screen
    SDL_RenderPresent(renderer);


    // Cap Framerate
    ++countedFrames;

    //int frameTicks = capTimer->getTicks();
    //if (frameTicks < SCREEN_TICKS_PER_FRAME)
    //{
    //    //Wait remaining time
    //    SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);
    //}

}

void Game::ProcessInputs(SDL_Event& event, float& frameTime, const SDL_Rect& screenSize, SDL_Point& touchLocation, GameStates& state)
{
    while (SDL_PollEvent(&event) != 0)
    {

        //User requests quit
        if (event.type == SDL_QUIT)
        {
            state = GameStates::QUIT;
        }

        if (event.type == SDL_MOUSEBUTTONDOWN)
        {
            if (event.button.button == SDL_BUTTON_RIGHT)
            {
                /*if (score > 0)
                {*/
                    snake->boosting = true;
                //}
            }

        }
        else if (event.type == SDL_MOUSEBUTTONUP)
        {
            if (event.button.button == SDL_BUTTON_RIGHT )
            {
                snake->boosting = false;
            }
        }


        if (event.type == SDL_MOUSEMOTION)
        {
            touchLocation.x = event.button.x;
            touchLocation.y = event.button.y;

            
            //snakes.at(0)->MoveTo(touchLocation.x, touchLocation.y, cameraMain);
            snake->MoveTo(touchLocation.x, touchLocation.y, cameraMain);
        }

       
        if (event.type == SDL_KEYDOWN)
        {
            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                state = GameStates::QUIT;
            }
        }

       
    }
}

void Game::Collision()
{
    if (snake->dead)
        return;

    // Collision with food
    for (int i = 0; i < food->AllFood.size(); ++i)
    {
        if (food->AllFood.at(i)->eaten == false)
        {
            if (food->AllFood.at(i)->gridReference[0] <= snake->headGridReference[0] + 1 && food->AllFood.at(i)->gridReference[1] <= snake->headGridReference[1] + 1 ||
                food->AllFood.at(i)->gridReference[0] >= snake->headGridReference[0] - 1 && food->AllFood.at(i)->gridReference[1] >= snake->headGridReference[1] - 1)
            {
                if (SphereToSphere(snake->getPosX(), snake->getPosY(), 10.0f, food->AllFood.at(i)->pos[0], food->AllFood.at(i)->pos[1], 10.0f))
                {
                    //Collision
                    food->HideFood(i);
                    score++;
                    snake->AddNewPiece();
                    break;
                }
            }

        }
    } // End of for loop for pieces

    //Collision with player snake head to each AI/networked snake pieces
    for (int i = 0; i < snakes.size(); ++i)
    {   
        if (!snakes.at(i)->dead)
        {
            for (int j = 0; j < snakes.at(i)->Pieces.size(); ++j)
            {
                float snakeHeadX = snake->getPosX();
                float snakeHeadY = snake->getPosY();

                if (snakes.at(i)->Pieces.at(j)->gridReference[0] <= snake->headGridReference[0] + 1 && snakes.at(i)->Pieces.at(j)->gridReference[1] <= snake->headGridReference[1] + 1 ||
                    snakes.at(i)->Pieces.at(j)->gridReference[0] >= snake->headGridReference[0] - 1 && snakes.at(i)->Pieces.at(j)->gridReference[1] >= snake->headGridReference[1] - 1)
                {
                    if (SphereToSphere(snakeHeadX, snakeHeadY, snake->getRadius(), snakes.at(i)->Pieces.at(j)->Position[0], snakes.at(i)->Pieces.at(j)->Position[1], snakes.at(i)->Pieces.at(j)->radius))
                    {
                        //Head of player snake collided with any of the pieces from the other snakes
                        snake->Die();

                        food->DropFood(snakeHeadX, snakeHeadY);

                        for (int k = 0; k < snake->Pieces.size(); k++)
                        {
                            food->DropFood(snake->Pieces.at(k)->Position[0], snake->Pieces.at(k)->Position[1]);
                        }

                        gameState = GameStates::OVER;
                        //break;

                    }
                }
            }
        }

    }

}

void Game::AIControl()
{
    
    for (int i = 0; i < snakes.size(); i++)
    {
        if (!snakes.at(i)->dead)
        {

            if (snakes.at(i)->AI)
            {
                // If at destination, calculate new destination
                if (snakes.at(i)->atDestination)
                {
                    float minX = (snakes.at(i)->getPosX() - AIDISTANCE < 0 + BORDER) ? 0 + BORDER : snakes.at(i)->getPosX() - AIDISTANCE;
                    float maxX = (snakes.at(i)->getPosX() + AIDISTANCE > LEVEL_SIZE.w - BORDER) ? LEVEL_SIZE.w - BORDER : snakes.at(i)->getPosX() + AIDISTANCE;

                    float minY = (snakes.at(i)->getPosY() - AIDISTANCE < 0 + BORDER) ? 0 + BORDER : snakes.at(i)->getPosY() - AIDISTANCE;
                    float maxY = (snakes.at(i)->getPosY() + AIDISTANCE > LEVEL_SIZE.h - BORDER) ? LEVEL_SIZE.h - BORDER : snakes.at(i)->getPosY() + AIDISTANCE;

                    float x = food->Random(minX, maxX);
                    float y = food->Random(minY, maxY);

                    snakes.at(i)->Destination[0] = x;
                    snakes.at(i)->Destination[1] = y;
                    snakes.at(i)->atDestination = false;
                }
                else
                {
                    for (int j = 0; j < food->AllFood.size(); ++j)
                    {
                        if (food->AllFood.at(j)->eaten == false)
                        {
                            if (food->AllFood.at(i)->gridReference[0] <= snakes.at(i)->headGridReference[0] + 1 && food->AllFood.at(i)->gridReference[1] <= snakes.at(i)->headGridReference[1] + 1 ||
                                food->AllFood.at(i)->gridReference[0] >= snakes.at(i)->headGridReference[0] - 1 && food->AllFood.at(i)->gridReference[1] >= snakes.at(i)->headGridReference[1] - 1)
                            {
                                if (SphereToSphere(snakes.at(i)->getPosX(), snakes.at(i)->getPosY(), 10, food->AllFood.at(j)->pos[0], food->AllFood.at(j)->pos[1], 10))
                                {
                                    //Collision
                                    food->HideFood(j);
                                    snakes.at(i)->AddNewPiece();
                                    break;
                                }
                            }
                        }

                    }

                    if (SphereToSphere(snakes.at(i)->getPosX(), snakes.at(i)->getPosY(), snakes.at(i)->getRadius(), snakes.at(i)->Destination[0], snakes.at(i)->Destination[1], 10))
                    {
                        snakes.at(i)->atDestination = true;
                    }
                    else
                    {
                        snakes.at(i)->MoveTo(snakes.at(i)->Destination[0], snakes.at(i)->Destination[1], cameraFull); // Majority of the loop here
                    }
                }
            }


        } // end of if AI

    } // end of for loop for all AI
}

void Game::AICollision()
{
    //Check AI snake collision with other AI snakes
    for (int i = 0; i < snakes.size(); i++)
    {
        if (!snakes.at(i)->dead)
        {

            for (int j = 0; j < snakes.size(); j++)
            {
                if (!snakes.at(j)->dead)
                {
                    if (i != j)
                    {
                        for (int k = 0; k < snakes.at(j)->Pieces.size(); k++)
                        {
                            if (snakes.at(j)->Pieces.at(k)->gridReference[0] <= snakes.at(i)->headGridReference[0] + 1 && snakes.at(j)->Pieces.at(k)->gridReference[1] <= snakes.at(i)->headGridReference[1] + 1 ||
                                snakes.at(j)->Pieces.at(k)->gridReference[0] >= snakes.at(i)->headGridReference[0] - 1 && snakes.at(j)->Pieces.at(k)->gridReference[1] >= snakes.at(i)->headGridReference[1] - 1)
                            {
                                if (SphereToSphere(snakes.at(i)->getPosX(), snakes.at(i)->getPosY(), snakes.at(i)->getRadius(), snakes.at(j)->Pieces.at(k)->Position[0], snakes.at(j)->Pieces.at(k)->Position[1], snakes.at(j)->Pieces.at(k)->radius))
                                {
                                    snakes.at(i)->Die();

                                    food->DropFood(snakes.at(i)->getPosX(), snakes.at(i)->getPosY());

                                    for (int l = 0; l < snakes.at(i)->Pieces.size(); l++)
                                    {
                                        food->DropFood(snakes.at(i)->Pieces.at(l)->Position[0], snakes.at(i)->Pieces.at(l)->Position[1]);
                                    }
                                    //break;

                                }

                            }
                        } // End of pieces

                    }
                }
                
            }

            //Check for AaI snake collision with player snake
            for (int j = 0; j < snake->Pieces.size(); ++j)
            {
                if (snake->Pieces.at(j)->gridReference[0] <= snakes.at(i)->headGridReference[0] + 1 && snake->Pieces.at(j)->gridReference[1] <= snakes.at(i)->headGridReference[1] + 1 ||
                    snake->Pieces.at(j)->gridReference[0] >= snakes.at(i)->headGridReference[0] - 1 && snake->Pieces.at(j)->gridReference[1] >= snakes.at(i)->headGridReference[1] - 1)
                {
                    if (SphereToSphere(snakes.at(i)->getPosX(), snakes.at(i)->getPosY(), snakes.at(i)->getRadius(), snake->Pieces.at(j)->Position[0], snake->Pieces.at(j)->Position[1], snake->Pieces.at(j)->radius))
                    {
                        snakes.at(i)->Die();

                        food->DropFood(snakes.at(i)->getPosX(), snakes.at(i)->getPosY());

                        for (int l = 0; l < snakes.at(i)->Pieces.size(); l++)
                        {
                            food->DropFood(snakes.at(i)->Pieces.at(l)->Position[0], snakes.at(i)->Pieces.at(l)->Position[1]);
                        }


                        //break;
                    }
                }

            }
        } // end of !=dead

    } // end for i
}

void Game::ProcessPackets()
{

    if (std::find(knownIDs.begin(), knownIDs.end(), client->ID) != knownIDs.end())
    {
        //printf("ID known %i\n", client->ID);
    }
    else
    {
        printf("ID NOT KNOWN \n");
        lastPacket = PacketTypes::NEWCONNECTION;
    }

    switch (lastPacket)
    {
        case PacketTypes::NEWCONNECTION:
        {
            if (NumberOfConnectedSnakes < 2)
            {
                snakes.at(NumberOfConnectedSnakes)->ID = client->ID;
                snakes.at(NumberOfConnectedSnakes)->AI = false;
            }
            else
            {
                Snake* tempSnake = new Snake(renderer, LEVEL_SIZE.w / 3 + (10.0f * NumberOfConnectedSnakes), LEVEL_SIZE.h / 3);
                tempSnake->AI = false;
                snakes.push_back(tempSnake);
            }

            knownIDs.push_back(client->ID);
            NumberOfConnectedSnakes++;
            break;
        }

        case PacketTypes::SNAKEPOS:
        {   int x = client->X;
            int y = client->Y;

            if (x >= 0 && y >= 0)
            {

               /* std::vector<Snake*>::iterator it;
                it = std::find(snakes.begin(), snakes.end(), client->ID);
*/
                for (int i = 0; i < NumberOfConnectedSnakes; ++i)
                {
                    if (snakes.at(i)->getID() == client->ID)
                    {
                        //printf("ID FOUND");
                        snakes.at(i)->setPosX(client->X);
                        snakes.at(i)->setPosY(client->Y);
                    }
                    if (i == NumberOfConnectedSnakes - 1)
                    {
                        // Not found
                        //printf("ID NOT FOUND");
                    
                    }
                }

                //if (it != snakes.end())
                //{
                //    //Found
                //    printf("ID FOUND");
                //    (*it)->setPosX(client->X);
                //    (*it)->setPosY(client->Y);
                //}
                //else
                //{
                //    // Not found
                //    printf("ID NOT FOUND");
                //}

                
            }
            break;
        }

        case PacketTypes::SNAKEADD:
        {

            break;
        }

        
    }
        
    
    


}



float Game::Distance(float x1, float y1, float x2, float y2)
{
    float xDist = x2 - x1;
    float yDist = y2 - y1;

    return sqrt(xDist * xDist + yDist * yDist);

}

bool Game::SphereToSphere(float x1, float y1, float r1, float x2, float y2, float r2)
{
    float distance = Distance(x1, y1, x2, y2);

    return (distance <= (10.0f + 10.0f));
}


void Game::close()
{
    // clean up renderer and window
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    renderer = nullptr;
    window = nullptr;

    // unload SDL and subsystems
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    
}
