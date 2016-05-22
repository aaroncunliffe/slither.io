
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
    background->loadFromFile(BACKGROUND_PATH, renderer);
}

bool Game::CreateGameObjects()
{
    ui = new UI(renderer, viewportMain);
    snake = new Snake(renderer);
    food = new FoodMap(renderer);
    
    return true;
}

Game::~Game()
{
    background->free();

    delete snake;
    delete food;
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
    // set the screen colour
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

    //Clear screen
    SDL_RenderClear(renderer);

    background->renderMedia(0, 0, renderer, &cameraMain);    

    //Collision detection
    Collision();

    //Inputs
    ProcessInputs(e, frameTime, SCREEN_SIZE, touchLocation, state);


    //Movement
    snake->CenterCamera(cameraMain);
    snake->Move(frameTime, cameraMain);
    ui->UpdateUI(score);

    //Rendering
    food->Render(background, cameraMain);
    snake->Render(viewportMain, cameraMain);
    ui->Render(touchLocation, cameraFull);
    
    
    //Update screen
    SDL_RenderPresent(renderer);

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
                snake->boosting = true;
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
    for (int i = 0; i < food->GetNumOfPieces(); i++)
    {
        if (food->AllFood.at(i)->eaten == false)
        {
            float foodXPos = food->AllFood.at(i)->pos[0];
            float foodYPos = food->AllFood.at(i)->pos[1];

            float snakeXPos = snake->getPosX();
            float snakeYPos = snake->getPosY();

            float xDist = foodXPos - snakeXPos;
            float yDist = foodYPos - snakeYPos;

            float distance = sqrt(xDist * xDist + yDist * yDist);

            if (distance <= snake->getRadius() + food->AllFood.at(i)->radius)
            {

                //Collision
                food->AllFood.at(i)->eaten = true;
                food->FoodEaten.push_back(i);
                score++;
                snake->AddNewPiece();
                break;
            }

            
        }
    }
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
