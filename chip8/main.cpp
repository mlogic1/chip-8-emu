#include <iostream>
#include "chip8.h"
#include "Util.h"
#include "EmuWindow.h"

// bad pointer
extern EmuWindow* win;

int main()
{
	EmuWindow window;

	// datatype data = LoadRom("C:\\Users\\Filip\\Desktop\\pong.ch8");
	int dataSize = 0;
	datatype data = LoadRom("/Users/filip/Projects/chip8_roms/bin/ibm_logo.ch8", &dataSize);
	if (!data)
	{
		std::cerr << "Unable to load rom file" << std::endl;
		return -1;
	}

	Chip8* chip8 = (Chip8*)malloc(sizeof(Chip8));
	

	EmuInit(chip8);
	EmuLoadRom(chip8, data, dataSize);
	UnloadRom(data);

	// don't do this
	win = &window;

	while (true)
	{
		EmuCycle(chip8);
		window.Update();

		if (!window.IsRunning())
			break;
	}

	EmuDeInit(chip8);
	return 0;
}
