#include "Texture.h"


// constructor
Texture::Texture() : texture(nullptr), width(0), height(0)
{
}

// destructor
Texture::~Texture()
{
    free();
}

bool Texture::loadFromFile(const std::string& file, SDL_Renderer* rend)
{
    fileName = file;
    // deallocate any texture already stored and reset all member vars
    free();

    SDL_Texture* newTexture = nullptr;

    SDL_Surface* surface = IMG_Load(fileName.c_str());
    if (surface == nullptr)
    {
        return false;
    }
    else
    {
        // set the transparent background colour
        SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0, 255, 255));
    }

    // create the texture
    newTexture = SDL_CreateTextureFromSurface(rend, surface);
    if (newTexture == nullptr)
    {
        return false;
    }
    width = surface->w;
    height = surface->h;

    // free up memory
    SDL_FreeSurface(surface);

    texture = newTexture;

    return texture != nullptr;
}

void Texture::free()
{
    if (texture != nullptr)
    {
        SDL_DestroyTexture(texture);
    }
    texture = nullptr;
    width = 0;
    height = 0;
}

void Texture::setColourModulation(Uint8 R, Uint8 G, Uint8 B)
{
    SDL_SetTextureColorMod(texture, R, G, B);
}

void Texture::setBlendMode(SDL_BlendMode blendMode)
{
    SDL_SetTextureBlendMode(texture, blendMode);
}

int Texture::setAplha(Uint8 alpha)
{
    // return only for debugging
    int success = SDL_SetTextureAlphaMod(texture, alpha);
    return success;
}

bool Texture::loadText(std::string text, SDL_Colour textColour, SDL_Renderer* rend, TTF_Font* font)
{
    // clean up preexisting texture
    free();

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), textColour);
    if (textSurface == NULL)
    {
        printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
    }
    else
    {
        texture = SDL_CreateTextureFromSurface(rend, textSurface);
        if (texture == NULL)
        {
            printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
        }
        else
        {
            width = textSurface->w;
            height = textSurface->h;
        }

        SDL_FreeSurface(textSurface);
    }
    return true;
}

void Texture::renderMedia(int x, int y, SDL_Renderer* rend, SDL_Rect* clip, double dAngle, SDL_Point* centre, SDL_RendererFlip flip)
{
    SDL_Rect dst = { x, y, width, height };

    if (clip != NULL)
    {
        dst.w = clip->w;
        dst.h = clip->h;
    }

    SDL_RenderCopyEx(rend, texture, clip, &dst, dAngle, centre, flip);
}

bool Texture::createBlank(SDL_Renderer* rend, int width, int height, SDL_TextureAccess access)
{
    //Create uninitialized texture
    texture = SDL_CreateTexture(rend, SDL_PIXELFORMAT_RGBA8888, access, width, height);
    if (texture == NULL)
    {
        printf("Unable to create blank texture! SDL Error: %s\n", SDL_GetError());
    }
    else
    {
        width = width;
        height = height;
    }

    return texture != NULL;
}

void Texture::setAsRenderTarget(SDL_Renderer* rend)
{
    //Make self render target
    SDL_SetRenderTarget(rend, texture);
}