#pragma once
#include <SDL.h>
#include <string>
#include <math.h>
#include <vector>

#define _USE_MATH_DEFINES

enum class GameStates
{
    PLAYING, 
    PAUSED,
    OVER,
    QUIT
};

//const SDL_Rect SCREEN_SIZE = { 0, 0, 1920, 1080 };

const SDL_Rect SCREEN_SIZE = { 0, 0, 1600, 900 };
const SDL_Rect UI_MAIN = { 0, 0, 1600, 900 };

//The dimensions of the level
const SDL_Rect LEVEL_SIZE{ 0, 0, 3000, 3000 };


// Filenames
const std::string BACKGROUND_PATH = "media\\Background.png";
const std::string SNAKE_HEAD_PATH = "media\\dot.png";
const std::string FOOD_FILE_PATHS[] = { "media\\reddot.png" ,"media\\bluedot.png" };

const std::string TTF_FILE = { "media\\font.ttf" };