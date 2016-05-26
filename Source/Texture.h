#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>

class Texture
{
private:

    SDL_Texture* texture;

    int width;
    int height;
    std::string fileName;


public:

    // constructor
    Texture();

    // destructor
    ~Texture();

    bool loadFromFile(const std::string& fileName, SDL_Renderer* rend);
    void free();

    void setColourModulation(Uint8 R, Uint8 G, Uint8 B);
    void setBlendMode(SDL_BlendMode blendMode);
    int setAplha(Uint8 alpha);

    bool loadText(std::string text, SDL_Colour textColour, SDL_Renderer* rend, TTF_Font* font);
    void renderMedia(int x, int y, SDL_Renderer* rend, SDL_Rect* clip = NULL, double dAngle = 0.0, SDL_Point* centre = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
    bool createBlank(SDL_Renderer* rend, int width, int height, SDL_TextureAccess access);
    void setAsRenderTarget(SDL_Renderer* rend);

    SDL_Texture* getTexture() const { return texture; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    std::string getFileName() const { return fileName; }

};