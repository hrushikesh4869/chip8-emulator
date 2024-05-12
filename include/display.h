#include<iostream>
#include<cstdlib>
#include<cstdint>
using namespace std;

class display
{
private :
    uint16_t screen_height;
    uint16_t screen_width;
    uint8_t **screen;
public :
    display();
    void initialize_display();
    void clear_display();
    uint8_t draw_pixel(uint8_t vx, uint8_t vy);
    uint8_t **get_screen();
    void print_screen();
};