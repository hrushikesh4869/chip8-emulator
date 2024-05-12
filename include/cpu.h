#include<iostream>
#include<cstdlib>
#include<cstdint>
#include<map>
#include "ram.h"
#include "display.h"
using namespace std;

class cpu
{
public:
    cpu();
    uint8_t registers[16];
    uint16_t I;
    uint8_t sp;
    uint16_t pc;
    uint16_t stack[16];
    ram Ram;
    display Display;
    uint8_t *keys;
    uint8_t delay_timer;
    uint8_t sound_timer;
    void (*lookup_table[36])(cpu *context, uint16_t opcode);
    void initilize_lookup_table();
    uint16_t fetch_instruction();
    void decode_execute(uint16_t opcode);
};


