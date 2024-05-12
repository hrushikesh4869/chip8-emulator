#include "cpu.h"
#include "io.h"

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        cout<<"Usage "<<argv[0]<<" rom_file \n";
        return 0;
    }

    cpu cpu;
    io_devices peripherals;
    bool quit = false;
    char const* filename = argv[1];

    cpu.Ram.load_rom(filename);

    while(!quit)
    {
        quit = peripherals.process_inputs(cpu.keys);
        
        uint16_t opcode = cpu.fetch_instruction();
        cpu.decode_execute(opcode);

        if(cpu.delay_timer > 0)
            cpu.delay_timer--;

        if(cpu.sound_timer > 0)
            cpu.sound_timer--;

        peripherals.update_screen(cpu.Display.get_screen());
    }
    return 0;
}