
//Using SDL and standard IO
#include <SDL.h>
#include <stdio.h>
#include <iostream>



#include "Game.h"
#include "Globals.h"


int main(int argc, char* args[])
{
    GameStates state = GameStates::PLAYING;

    //Event handler
    SDL_Event e;

    // setup our frame time
    Timer frameTimer;
    float frameTime = 0;

    Game* game = new Game;

    // while application is running
    while (state != GameStates::QUIT)
    {
        frameTime = static_cast<float>(frameTimer.getTicks()) / 1000.0f;
        // restart frame timer
        frameTimer.start();

        // do game stuff
        game->run(e, frameTime, state);

    }

  

    return 0;
}
