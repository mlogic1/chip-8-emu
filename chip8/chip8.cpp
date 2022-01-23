#include <iostream>
#include "chip8.h"
#include "Util.h"
#include "EmuWindow.h"

int main()
{
    EmuWindow window;
	// load rom

	// datatype data = LoadRom("C:\\Users\\Filip\\Desktop\\pong.ch8");
	int dataSize = 0;
	// datatype data = LoadRom("/home/filip/Desktop/c8 data/pong.ch8", &dataSize);
	datatype data = LoadRom("/home/filip/Desktop/c8 data/ibm-logo.ch8", &dataSize);
	if (!data)
	{
		std::cout<<"Unable to load rom file"<<std::endl;
		return -1;
	}

	EmuInit();
	EmuLoadRom(data, dataSize);
	UnloadRom(data);

	// don't do this
	win = &window;

	while (true)
	{
		EmuCycle();
        window.Update();

        if (!window.IsRunning())
            break;
	}

	EmuDeInit();
	return 0;
}
