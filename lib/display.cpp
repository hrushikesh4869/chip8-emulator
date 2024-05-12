#include "display.h"

display::display()
{
    screen_height = 32;
    screen_width = 64;

    screen = (uint8_t **)malloc(sizeof(uint8_t *) * screen_height);

    for(int i = 0; i < screen_height; i++)
        screen[i] = (uint8_t *)malloc(sizeof(uint8_t) * screen_width);

    initialize_display();
}

void display::initialize_display()
{
    for(int i = 0; i < screen_height; i++)
        for(int j = 0; j < screen_width; j++)
            screen[i][j] = 0;
}

void display::clear_display()
{
    for(int i = 0; i < screen_height; i++)
        for(int j = 0; j < screen_width; j++)
            screen[i][j] = 0;
}

uint8_t display::draw_pixel(uint8_t vx, uint8_t vy)
{
    vx = vx % screen_height;
    vy = vy % screen_width;
    uint8_t collision = screen[vx][vy];
    screen[vx][vy] = screen[vx][vy] ^ 1;
    return collision;
}

uint8_t** display::get_screen()
{
    return screen;
}

void display::print_screen()
{
    for(int i = 0; i<32; i++)
    {
        for(int j = 0; j<64; j++)
            cout<<(int)screen[i][j]<<" ";
        cout<<endl;
    }
}