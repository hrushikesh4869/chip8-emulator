#include<cstdint>
#include<cstdlib>
#include <cstring>
#include <fstream>

const unsigned int FONT_ADDRESS = 0x00;
const unsigned int ROM_ADDRESS = 0x200;

class ram
{
private:
    uint8_t *memory;
public:
    ram();
    uint8_t read(uint16_t addr);
    void write(uint16_t addr, uint8_t data);
    void load_rom(char const* filename);
};