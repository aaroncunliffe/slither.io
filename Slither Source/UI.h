#pragma once


#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "Globals.h"
#include "Texture.h"

class UI
{
private:
    SDL_Renderer* renderer;

    TTF_Font* font;
    SDL_Color textColor;

    SDL_Rect* viewportFull;
    SDL_Rect* viewportMain;

    Texture* scoreTexture;    
    Texture* networkTexture;
    Texture* fpsTexture;

public:
    UI(SDL_Renderer* rend, SDL_Rect* viewportMain);
    
    ~UI();

    bool LoadMedia();
    void UpdateUI(int score, bool networkConnected, float avgFps);

    void Render(SDL_Point& touchLocation, SDL_Rect &camera);

};