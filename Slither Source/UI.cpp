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
    networkTexture->free();
    fpsTexture->free();
}

bool UI::LoadMedia()
{
    scoreTexture = new Texture;
    networkTexture = new Texture;
    fpsTexture = new Texture;

    return true;

}

void UI::UpdateUI(int score, bool networkConnected, float avgFps)
{
    scoreTexture->loadText("Score: " + std::to_string(score) , textColor, renderer, font);

    std::string connectedString = (networkConnected) ? "connected" : "disconnected" ;
    networkTexture->loadText(connectedString, textColor, renderer, font);

    fpsTexture->loadText("FPS: " + std::to_string((int)avgFps), textColor, renderer, font);

}

void UI::Render(SDL_Point& touchLocation, SDL_Rect &camera)
{
    scoreTexture->renderMedia(10, SCREEN_SIZE.h - 50, renderer);
    networkTexture->renderMedia(SCREEN_SIZE.w - 250, 10, renderer);
    fpsTexture->renderMedia(10, 10, renderer);
    
   
}

