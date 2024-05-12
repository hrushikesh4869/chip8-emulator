#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <math.h>
#include <iostream>
#include <cstdint>

class io_devices
{
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* pixels;
    int width;
    int height;

public:
    io_devices();
    void update_screen(uint8_t **display);
    bool process_inputs(uint8_t *keys);
};