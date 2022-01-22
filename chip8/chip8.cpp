#include <iostream>
#include "chip8.h"
#include "Util.h"

int main()
{

	// load rom

	// datatype data = LoadRom("C:\\Users\\Filip\\Desktop\\pong.ch8");
	int dataSize = 0;
	datatype data = LoadRom("C:\\Users\\Filip\\Desktop\\ibm-logo.ch8", &dataSize);
	if (!data)
	{
		std::cout<<"Unable to load rom file"<<std::endl;
		return -1;
	}

	EmuInit();
	EmuLoadRom(data, dataSize);
	UnloadRom(data);

	while (true)
	{
		EmuCycle();
		EmuCycle();
		EmuCycle();
		EmuCycle();
		EmuCycle();
		EmuCycle();
		EmuCycle();
		EmuCycle();

		EmuCycle();
		EmuCycle();
		EmuCycle();
		EmuCycle();
		EmuCycle();
		EmuCycle();
		EmuCycle();
		EmuCycle();
		EmuCycle();
		EmuCycle();

		EmuCycle();
		EmuCycle();
		EmuCycle();
		EmuCycle();
		EmuCycle();
		EmuCycle();


		EmuCycle();
		EmuCycle();
		EmuCycle();
		EmuCycle();
		EmuCycle();

		// break;
	}

	EmuDeInit();
	return 0;
}
