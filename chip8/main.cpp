#include <iostream>
#include "chip8.h"
#include "Util.h"
#include "EmuWindow.h"

int main()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
		return -1;
	}
	EmuWindow window;

	while (true)
	{
		window.Update();

		if (!window.IsRunning())
			break;
	}

	return 0;
}
