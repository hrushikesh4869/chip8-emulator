#include "cpu.h"

cpu::cpu()
{
    cout<<"this is constructor \n";
    sp = 0;
    I = 0;
    pc = 0x200;
    delay_timer = 0;
    sound_timer = 0;

    keys = (uint8_t *)malloc(sizeof(uint8_t) * 16);

    for(int i = 0; i < 16; i++)
    {
        registers[i] = 0;
        keys[i] = 0;
        stack[i] = 0;
    }
    initilize_lookup_table();
}

uint16_t cpu::fetch_instruction()
{
    uint16_t opcode = (Ram.read(pc) << 8) | (Ram.read(pc + 1));
    pc += 2;
    return opcode;
}

void cpu::decode_execute(uint16_t opcode)
{
    uint8_t first, third, fourth;
    int index = -1;
    first = (opcode & 0xf000) >> 12;
    third = (opcode & 0x00f0) >> 4;
    fourth = (opcode & 0x000f);

    switch(first)
    {
        case 0x0:
        {
            if(fourth == 0x0)
                index = 0;
            else
                index = 1;

        } break;

        case 0x1:
        {
            index = 2;
        } break;

        case 0x2:
        {
            index = 3;
        } break;

        case 0x3:
        {
            index = 4;
        } break;
        
        case 0x4:
        {
            index = 5;
        } break;

        case 0x5:
        {
            index = 6;
        } break;

        case 0x6:
        {
            index = 7;
        } break;

        case 0x7:
        {
            index = 8;
        } break;
        
        case 0x8:
        {
            switch(fourth)
            {
                case 0x0:
                {
                    index = 9;
                } break;
            
                case 0x1:
                {
                    index = 10;
                } break;

                case 0x2:
                {
                    index = 11;
                } break;

                case 0x3:
                {
                    index = 12;
                } break;

                case 0x4:
                {
                    index = 13;
                } break;

                case 0x5:
                {
                    index = 14;
                } break;

                case 0x6:
                {
                    index = 15;
                } break;

                case 0x7:
                {
                    index = 16;
                } break;

                case 0xe:
                {
                    index = 17;
                } break;
            }
        } break;

        case 0x9:
        {
            index = 18;
        } break;

        case 0xa:
        {
            index = 19;
        } break;

        case 0xb:
        {
            index = 20;
        } break;
        
        case 0xc:
        {
            index = 21;
        } break;

        case 0xd:
        {
            index = 22;
        } break;

        case 0xe:
        {
            if(third == 0x9)
                index = 23;
            else
                index = 24;
        } break;

        case 0xf:
        {
            uint8_t temp = (third << 4) | fourth;
            switch(temp)
            {
                case 0x07:
                {
                    index = 25;
                } break;
            
                case 0x0A:
                {
                    index = 26;
                } break;

                case 0x15:
                {
                    index = 27;
                } break;

                case 0x18:
                {
                    index = 28;
                } break;

                case 0x1E:
                {
                    index = 29;
                } break;

                case 0x29:
                {
                    index = 30;
                } break;

                case 0x33:
                {
                    index = 31;
                } break;

                case 0x55:
                {
                    index = 32;
                } break;

                case 0x65:
                {
                    index = 33;
                } break;
            }
        } break;
    }

    if(index >= 0)
    {
        lookup_table[index](this, opcode);
    }
    else
    {
        cout<<"Invalid instruction "<<opcode<<"\n";
    }
}

// 00E0
void cls(cpu *context, uint16_t opcode)
{
    context->Display.clear_display();
}

// 00EE
void ret(cpu *context, uint16_t opcode)
{
    context->pc = context->stack[context->sp];
    context->sp -= 1;
}

// 1nnn
void jump_addr(cpu *context, uint16_t opcode)
{
    context->pc = opcode & 0x0fff;
}

// 2nnn
void call_addr(cpu *context, uint16_t opcode)
{
    context->sp += 1;
    if(context->sp < 16)
    {
        context->stack[context->sp] = context->pc;
        context->pc = opcode & 0x0fff;
    }
    else
    {
        cout<<"stack overflow \n";
    }
}

// 3xkk
void skip_instr_eq(cpu *context, uint16_t opcode)
{
    uint8_t kk = opcode & 0x00ff;
    uint8_t x = (opcode & 0x0f00) >> 8;

    if(context->registers[x] == kk)
        context->pc += 2;
}

// 4xkk
void skip_instr_neq(cpu *context, uint16_t opcode)
{
    uint8_t kk = opcode & 0x00ff;
    uint8_t x = (opcode & 0x0f00) >> 8;

    if(context->registers[x] != kk)
        context->pc += 2;
}

// 5xy0
void skip_instr_reg_eq(cpu *context, uint16_t opcode)
{
    uint8_t x = (opcode & 0x0f00) >> 8;
    uint8_t y = (opcode & 0x00f0) >> 4;
    if(context->registers[x] == context->registers[y])
        context->pc += 2;
}

// 6xkk
void load_reg_byte(cpu *context, uint16_t opcode)
{
    uint8_t kk = opcode & 0x00ff;
    uint8_t x = (opcode & 0x0f00) >> 8;

    context->registers[x] = kk;
}

// 7xkk
void load_reg_add_byte(cpu *context, uint16_t opcode)
{
    uint8_t kk = opcode & 0x00ff;
    uint8_t x = (opcode & 0x0f00) >> 8;

    context->registers[x] += kk;
}

// 8xy0
void load_reg1_reg2(cpu *context, uint16_t opcode)
{
    uint8_t x = (opcode & 0x0f00) >> 8;
    uint8_t y = (opcode & 0x00f0) >> 4;

    context->registers[x] = context->registers[y];
}

// 8xy1
void or_reg1_reg2(cpu *context, uint16_t opcode)
{
    uint8_t x = (opcode & 0x0f00) >> 8;
    uint8_t y = (opcode & 0x00f0) >> 4;

    context->registers[x] = context->registers[x] | context->registers[y];
    context->registers[15] = 0;
}

// 8xy2
void and_reg1_reg2(cpu *context, uint16_t opcode)
{
    uint8_t x = (opcode & 0x0f00) >> 8;
    uint8_t y = (opcode & 0x00f0) >> 4;

    context->registers[x] = context->registers[x] & context->registers[y];
    context->registers[15] = 0;
}

// 8xy3
void xor_reg1_reg2(cpu *context, uint16_t opcode)
{
    uint8_t x = (opcode & 0x0f00) >> 8;
    uint8_t y = (opcode & 0x00f0) >> 4;

    context->registers[x] = context->registers[x] ^ context->registers[y];
    context->registers[15] = 0;
}

// 8xy4
void add_reg1_reg2(cpu *context, uint16_t opcode)
{
    uint8_t x = (opcode & 0x0f00) >> 8;
    uint8_t y = (opcode & 0x00f0) >> 4;
    uint16_t result;

    result = context->registers[x] + context->registers[y];
    context->registers[x] = result % 256;
    context->registers[15] = result / 256;
}

// 8xy5
void sub_reg1_reg2(cpu *context, uint16_t opcode)
{
    uint8_t x = (opcode & 0x0f00) >> 8;
    uint8_t y = (opcode & 0x00f0) >> 4;
    uint8_t vx = context->registers[x];
    uint8_t vy = context->registers[y];

    context->registers[x] = context->registers[x] - context->registers[y];

    if(vx >= vy)
        context->registers[15] = 1;
    else
        context->registers[15] = 0;
}

// 8xy6
void shift_right(cpu *context, uint16_t opcode)
{
    uint8_t x = (opcode & 0x0f00) >> 8;
    uint8_t y = (opcode & 0x00f0) >> 4;
    uint8_t vx = context->registers[x];
    uint8_t vy = context->registers[y];

    context->registers[x] = context->registers[y] >> 1;
    context->registers[15] = vy & 0x01;
}

// 8xy7
void sub_reg2_reg1(cpu *context, uint16_t opcode)
{
    uint8_t x = (opcode & 0x0f00) >> 8;
    uint8_t y = (opcode & 0x00f0) >> 4;
    uint8_t vx = context->registers[x];
    uint8_t vy = context->registers[y];

    context->registers[x] = context->registers[y] - context->registers[x];

    if(vx <= vy)
        context->registers[15] = 1;
    else
        context->registers[15] = 0;
}

// 8xyE
void shift_left(cpu *context, uint16_t opcode)
{
    uint8_t x = (opcode & 0x0f00) >> 8;
    uint8_t y = (opcode & 0x00f0) >> 4;
    uint8_t vx = context->registers[x];
    uint8_t vy = context->registers[y];

     context->registers[x] = context->registers[y] << 1;
    context->registers[15] = (vy & 0x80) >> 7;
   
}

// 9xy0
void skip_instr_reg_neq(cpu *context, uint16_t opcode)
{
    uint8_t x = (opcode & 0x0f00) >> 8;
    uint8_t y = (opcode & 0x00f0) >> 4;
    if(context->registers[x] != context->registers[y])
        context->pc += 2;
}

// Annn
void load_I(cpu *context, uint16_t opcode)
{
    uint16_t nnn = opcode & 0x0fff;
    context->I = nnn;
}

// Bnnn
void jump_nnn(cpu *context, uint16_t opcode)
{
    uint16_t nnn = opcode & 0x0fff;
    context->pc = context->registers[0] + nnn;
}

// Cxkk
void and_reg_random(cpu *context, uint16_t opcode)
{
    uint8_t random = rand() % 256;
    uint8_t kk = opcode & 0x00ff;
    uint8_t x = (opcode & 0x0f00) >> 8;

    context->registers[x] = random & kk;
}

// Dxyn
void draw_reg1_reg2_n(cpu *context, uint16_t opcode)
{
    uint8_t x = (opcode & 0x0f00) >> 8;
    uint8_t y = (opcode & 0x00f0) >> 4;
    uint8_t n = (opcode & 0x000f);
    uint8_t vx = context->registers[x];
    uint8_t vy = context->registers[y];
    bool flag = 0,c;

    for(int i = 0; i < n; i++)
    {
        uint8_t temp = context->Ram.read(context->I + i);
        uint8_t mask = 0x80;
        
        for(int j = 0; j < 8; j++)
        {
            if(temp & mask)
            {
                c = context->Display.draw_pixel(vy + i, vx + j);
                if(c)
                    flag = 1;
            }
            mask = mask >> 1;
        }
    }
    if(flag)
        context->registers[15] = 1;
}

// Ex9E
void skip_key_press(cpu *context, uint16_t opcode)
{
    uint8_t x = (opcode & 0x0f00) >> 8;
    uint8_t vx = context->registers[x];
    if(context->keys[vx])
    {
        context->pc += 2;
    }
}

// ExA1
void skip_key_not_press(cpu *context, uint16_t opcode)
{
    uint8_t x = (opcode & 0x0f00) >> 8;
    uint8_t vx = context->registers[x];
    if(context->keys[vx] == 0)
    {
        context->pc += 2;
    }
}

// Fx07
void set_delay_timer_reg(cpu *context, uint16_t opcode)
{
    uint8_t x = (opcode & 0x0f00) >> 8;
    context->registers[x] = context->delay_timer;
}

// Fx0A
void store_key_press(cpu *context, uint16_t opcode)
{
    uint8_t x = (opcode & 0x0f00) >> 8;
    for(int i = 0; i<16; i++)
    {
        if(context->keys[i])
        {
            context->registers[x] = i;
            return;
        }
    }
    context->pc -= 2;
}

// Fx15
void set_delay_timer(cpu *context, uint16_t opcode)
{
    uint8_t x = (opcode & 0x0f00) >> 8;
    context->delay_timer = context->registers[x];
}

// Fx18
void set_sound_timer(cpu *context, uint16_t opcode)
{
    uint8_t x = (opcode & 0x0f00) >> 8;
    context->sound_timer = context->registers[x];
}

// Fx1E
void add_I_reg(cpu *context, uint16_t opcode)
{
    uint8_t x = (opcode & 0x0f00) >> 8;
    context->I += context->registers[x];
}

// Fx29
void set_sprite_address(cpu *context, uint16_t opcode)
{
    uint8_t x = (opcode & 0x0f00) >> 8;
    context->I = context->registers[x] * 5;
}

// Fx33
void store_bcd(cpu *context, uint16_t opcode)
{
    uint8_t x = (opcode & 0x0f00) >> 8;
    uint8_t reg = context->registers[x];
    uint16_t I = context->I;

    context->Ram.write(I, reg / 100);
    context->Ram.write(I + 1, (reg / 10) % 10);
    context->Ram.write(I + 2, reg % 10);
}

// Fx55
void store_regs(cpu *context, uint16_t opcode)
{
    uint8_t x = (opcode & 0x0f00) >> 8;
    uint16_t I = context->I;

    for(int i = 0; i <= x; i++)
        context->Ram.write(I + i, context->registers[i]);

    context->I = context->I + x + 1;
}

// Fx65
void restore_regs(cpu *context, uint16_t opcode)
{
    uint8_t x = (opcode & 0x0f00) >> 8;
    uint16_t I = context->I;

    for(int i = 0; i <= x; i++)
        context->registers[i] = context->Ram.read(I + i);
    
    context->I = context->I + x + 1;
}


void cpu::initilize_lookup_table()
{
    lookup_table[0] = cls;
    lookup_table[1] = ret;
    lookup_table[2] = jump_addr;
    lookup_table[3] = call_addr;
    lookup_table[4] = skip_instr_eq;
    lookup_table[5] = skip_instr_neq;
    lookup_table[6] = skip_instr_reg_eq;
    lookup_table[7] = load_reg_byte;
    lookup_table[8] = load_reg_add_byte;
    lookup_table[9] = load_reg1_reg2;
    lookup_table[10] = or_reg1_reg2;
    lookup_table[11] = and_reg1_reg2;
    lookup_table[12] = xor_reg1_reg2;
    lookup_table[13] = add_reg1_reg2;
    lookup_table[14] = sub_reg1_reg2;
    lookup_table[15] = shift_right;
    lookup_table[16] = sub_reg2_reg1;
    lookup_table[17] = shift_left;
    lookup_table[18] = skip_instr_reg_neq;
    lookup_table[19] = load_I;
    lookup_table[20] = jump_nnn;
    lookup_table[21] = and_reg_random;
    lookup_table[22] = draw_reg1_reg2_n;
    lookup_table[23] = skip_key_press;
    lookup_table[24] = skip_key_not_press;
    lookup_table[25] = set_delay_timer_reg;
    lookup_table[26] = store_key_press;
    lookup_table[27] = set_delay_timer;
    lookup_table[28] = set_sound_timer;
    lookup_table[29] = add_I_reg;
    lookup_table[30] = set_sprite_address;
    lookup_table[31] = store_bcd;
    lookup_table[32] = store_regs;
    lookup_table[33] = restore_regs;
}