#include "UI.h"

UI::UI(SDL_Renderer* rend, SDL_Rect* viewportMain) :
    renderer(rend), viewportMain(viewportMain)
{
    
    // load up the textures
    if (!LoadMedia())
        throw;

    // load TTF font
    font = TTF_OpenFont(TTF_FILE.c_str(), 36);
    textColor = { 0, 0, 0 };

}

UI::~UI()
{
    scoreTexture->free();
}

bool UI::LoadMedia()
{
    scoreTexture = new Texture;


    return true;

}

void UI::UpdateUI(int score)
{
    scoreTexture->loadText("Score: " + std::to_string(score) , textColor, renderer, font);
}

void UI::Render(SDL_Point& touchLocation, SDL_Rect &camera)
{
    scoreTexture->renderMedia(10, SCREEN_SIZE.h - 40, renderer);
    
   
}

