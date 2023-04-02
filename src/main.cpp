#include <SDL.h>
#include <SDL_ttf.h>
#include "ff_constants.h"
#include "ff2.h"
#include "i_graphics_middleware.h"
#include <cstdlib>
#include <ctime>
#include <cstring>

class GraphicsMiddleware : public IGraphicsMiddleware
{
private:
    SDL_Renderer *renderer_;
    TTF_Font *font_;
    SDL_Color color_;

    int palette_[256][3];

public:
    GraphicsMiddleware(SDL_Renderer *renderer)
        : renderer_(renderer)
    {
        for (int idx = 0; idx < 256; ++idx)
        {
            palette_[idx][0] = 0;
            palette_[idx][1] = 0;
            palette_[idx][2] = 0;
        }

        TTF_Init();
        this->font_ = TTF_OpenFont(FONT_FILE, FONT_SIZE);
    }
    ~GraphicsMiddleware()
    {
    }

    void cleardevice()
    {
    }
    void setrgbpalette(int color_code, int r, int g, int b)
    {
        SDL_assert(color_code >= 0 && color_code < 256);
        palette_[color_code][0] = r;
        palette_[color_code][1] = g;
        palette_[color_code][2] = b;
    }

    void putpixel(int x, int y, int color_code)
    {
        SDL_assert(color_code >= 0 && color_code < 256);
        Uint8 alpha = 255;
        SDL_SetRenderDrawColor(this->renderer_, this->palette_[color_code][0], this->palette_[color_code][1], this->palette_[color_code][2], alpha);
        SDL_RenderDrawPoint(this->renderer_, x, y);
    }

    int getpixel(int x, int y)
    {
        return 0;
    }

    void setcolor(int color_code)
    {
        Uint8 alpha = 255;
        this->color_.r = this->palette_[color_code][0];
        this->color_.g = this->palette_[color_code][1];
        this->color_.b = this->palette_[color_code][2];

        SDL_SetRenderDrawColor(this->renderer_, this->palette_[color_code][0], this->palette_[color_code][1], this->palette_[color_code][2], alpha);
    }

    void rectangle(int x1, int y1, int x2, int y2)
    {
        SDL_Rect rect;
        rect.x = x1;
        rect.y = y1;
        rect.w = x2 - x1;
        rect.h = y2 - y1;
        SDL_RenderDrawRect(this->renderer_, &rect);
    }

    void line(int x1, int y1, int x2, int y2)
    {
        SDL_RenderDrawLine(this->renderer_, x1, y1, x2, y2);
    }

    void outtextxy(int x, int y, const char *text)
    {
        SDL_Color color = this->color_;
        SDL_Surface *surface = TTF_RenderText_Blended(this->font_, text, color);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(this->renderer_, surface);
        int w = surface->w;
        int h = surface->h;
        SDL_Rect dstrect = {x, y, w, h};
        SDL_RenderCopy(this->renderer_, texture, NULL, &dstrect);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }

    void settextstyle(int font_style, int something, int size)
    {
    }
};

class LogicMiddleware : public ILogicMiddleware
{
public:
    LogicMiddleware()
    {
    }
    ~LogicMiddleware()
    {
    }

    unsigned random(unsigned upper_bound)
    {
        std::srand(std::time(nullptr));
        return std::rand() % upper_bound;
    }
    void randomize()
    {
        std::srand(std::time(nullptr));
    }
    void delay(unsigned milliseconds)
    {
        SDL_Delay(milliseconds);
    }

    void mouseState(int *x, int *y, int *button)
    {
        SDL_GetMouseState(x, y);
        *button = SDL_GetMouseState(NULL, NULL);
    }

    void mousemove(int x, int y)
    {
        SDL_WarpMouseInWindow(NULL, x, y);
    }
};

void putPixel(SDL_Renderer *renderer, int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderDrawPoint(renderer, x, y);
}

void renderText(SDL_Renderer *renderer, TTF_Font *font, const char *text, int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    SDL_Color color = {r, g, b, a};
    SDL_Surface *surface = TTF_RenderText_Blended(font, text, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    int w = surface->w;
    int h = surface->h;
    SDL_Rect dstrect = {x, y, w, h};
    SDL_RenderCopy(renderer, texture, NULL, &dstrect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void prepareScene(SDL_Renderer *renderer, SDL_Texture *texture)
{
    SDL_SetRenderDrawColor(renderer, 10, 12, 25, 255);
    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, texture, NULL, NULL);
}

void presentScene(SDL_Renderer *renderer)
{
    SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[])
{
    srand(static_cast<unsigned int>(time(NULL))); // seed the random number generator

    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window *window = SDL_CreateWindow("FF2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Texture *texture = SDL_CreateTexture(renderer, SCREEN_FORMAT, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);

    TTF_Init();
    TTF_Font *font = TTF_OpenFont(FONT_FILE, FONT_SIZE);

    Uint32 *pixels = new Uint32[SCREEN_WIDTH * SCREEN_HEIGHT];

    bool isRunning = true;
    SDL_Event event;

    LogicMiddleware lm;
    GraphicsMiddleware gm(renderer);
    FF2 game(&gm, &lm);
    game.switch_to_state(FF2_STATE_LOADING_SCREEN);

    while (isRunning)
    {
        prepareScene(renderer, texture);
        renderText(renderer, font, "Press ESC to exit", 10, 10, 255, 255, 255, 255);

        // handle events
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT) // window closed
            {
                isRunning = false;
            }
            else if (event.type == SDL_KEYDOWN) // key pressed
            {
                if (event.key.keysym.sym == SDLK_ESCAPE) // escape key
                {
                    isRunning = false;
                }
            }
        }

        // clear the screen
        memset(pixels, 0, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));

        game.main_loop(33);

        // generate a random pixel color and location
        // Uint8 r = 255; // rand() % 256;
        // Uint8 g = rand() % 256;
        // Uint8 b = rand() % 256;
        // int x = rand() % SCREEN_WIDTH;
        // int y = rand() % SCREEN_HEIGHT;


        // // put the random pixel on the screen
        // putPixel(renderer, x, y, r, g, b, 255);
        // putPixel(renderer, x + 1, y, r, g, b, 255);
        // putPixel(renderer, x, y + 1, r, g, b, 255);
        // putPixel(renderer, x + 1, y + 1, r, g, b, 255);

        // copy the pixels to the texture
        SDL_UpdateTexture(texture, NULL, pixels, SCREEN_WIDTH * sizeof(Uint32));

        // render the texture

        presentScene(renderer);
        SDL_Delay(33);
    }

    delete[] pixels;
    SDL_DestroyTexture(texture);

    // Font cleanup
    TTF_CloseFont(font);
    TTF_Quit();

    // App Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
