#pragma once
#include <SDL.h>
#include <string>
#include <math.h>
#include <vector>
#include <algorithm>

#define _USE_MATH_DEFINES


// Filename consts
const std::string TTF_FILE = { "media\\font.ttf" };
const std::string BACKGROUND_PATH = "media\\Background.png";
const std::string BORDER_PATH = "media\\Border.png";
const std::string SNAKE_HEAD_PATH = "media\\dot.png";
const std::string FOOD_FILE_PATHS[] = { "media\\reddot.png" ,"media\\bluedot.png" };

enum GameStates
{
    PLAYING, 
    PAUSED,
    OVER,
    WAITING,
    QUIT
};

//GameStates gameState;

enum PacketTypes
{
    SNAKEPOS,
    SNAKEADD,
    FOOD,
    NEWCONNECTION,
    DISCONNECTED,
};


//Game Consts
const int SCREEN_FPS = 60;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

const int NUMOFAI = 20;
const int AIDISTANCE = 600;
const int GRIDSIZE = 200;
const int BORDER = 100;

const SDL_Rect SCREEN_SIZE = { 0, 0, 1600, 900  };
const SDL_Rect UI_MAIN =     { 0, 0, 1600, 900  };
const SDL_Rect LEVEL_SIZE =  { 0, 0, 3000, 3000 }; //The dimensions of the level



// Food Consts
const int MAX_FOOD_PIECES = SCREEN_SIZE.w / 3;
const float FOODREGENRATE = 0.1f;
const float GENERATE_FOOD_TIME = 3.0f; // Amount of time between new food generation

// Snake Consts
const float SNAKE_BOOST_TIMER = 1.5f; // Amount of time while boosting before dropping food / score