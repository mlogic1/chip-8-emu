#include "EmuWindow.h"
#include "Util.h"
#include <iostream>
#include <unordered_map>

EmuWindow::EmuWindow()
{
	m_window = SDL_CreateWindow("Chip-8 Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 300, SDL_WINDOW_RESIZABLE);
	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC);
	m_graphicsTexture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 64, 32);


	int dataSize = 0;
	datatype data = LoadRom("/Users/filip/Projects/chip8_roms/bin/6-keypad.ch8", &dataSize);
	if (!data)
	{
		throw std::runtime_error("Unable to load rom file");

	}

	m_emu = (Chip8*)malloc(sizeof(Chip8));
	EmuInit(m_emu);
	EmuSetDisplay(m_emu, this);

	EmuLoadRom(m_emu, data, dataSize);
	UnloadRom(data);
}

EmuWindow::~EmuWindow()
{
	EmuDeInit(m_emu);
	free(m_emu);
	SDL_DestroyTexture(m_graphicsTexture);
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
}

void EmuWindow::Update()
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		HandleEvents(event);
	}

	static int prevTime = SDL_GetTicks();
	int thisTime = SDL_GetTicks();
	const float dt = (thisTime - prevTime) / 1000.0f;
	prevTime = thisTime;

	int windowWidth, windowHeight;
	SDL_GetWindowSize(m_window, &windowWidth, &windowHeight);

	SDL_Rect destRect;
	destRect.x = 0;
	destRect.y = 0;
	destRect.w = windowWidth;
	destRect.h = windowHeight;

	// render
	SDL_RenderClear(m_renderer);
	SDL_RenderCopy(m_renderer, m_graphicsTexture, NULL, &destRect);
	SDL_RenderPresent(m_renderer);
	// SDL_Delay(16);

	// Timing setup for cycles
	static Uint32 lastCycleTime = SDL_GetTicks();
	const int cycleRate = 700;							// Target: 700 instructions per second
	const float cycleInterval = 1000.0f / cycleRate;	// Milliseconds per cycle

	// Execute cycles until we've caught up to the target timing
	Uint32 currentTime = SDL_GetTicks();
	while (currentTime - lastCycleTime >= cycleInterval) {
		EmuCycle(m_emu); // Execute one cycle of the emulator
		lastCycleTime += cycleInterval;
	}
}

void EmuWindow::DrawInstruction(unsigned char* graphicsPtr)
{
	SDL_SetRenderTarget(m_renderer, m_graphicsTexture); // render to texture
	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0 , 255);
	SDL_RenderClear(m_renderer);

	// draw white pixels
	SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
	for (int y = 0; y < 32; ++y)
	{
		for (int x = 0; x < 64; ++x)
		{
			if (graphicsPtr[y * 64 + x] == 1) // Check for "on" pixels
			{
				SDL_RenderDrawPoint(m_renderer, x, y);
			}
		}
	}

	SDL_SetRenderTarget(m_renderer, NULL);
}

bool EmuWindow::IsRunning() const
{
	return m_isRunning;
}

void EmuWindow::HandleEvents(const SDL_Event& event)
{
	static std::unordered_map<SDL_Keycode, uint8_t> SDLKEY_CHIPMAP =
	{
		{ SDLK_0, 0 },
		{ SDLK_1, 1 },
		{ SDLK_2, 2 },
		{ SDLK_3, 3 },
		{ SDLK_4, 4 },
		{ SDLK_5, 5 },
		{ SDLK_6, 6 },
		{ SDLK_7, 7 },
		{ SDLK_8, 8 },
		{ SDLK_9, 9 },
		{ SDLK_a, 10 },
		{ SDLK_b, 11 },
		{ SDLK_c, 12 },
		{ SDLK_d, 13 },
		{ SDLK_e, 14 },
		{ SDLK_f, 15 }
	};

	switch (event.type)
	{
		case SDL_KEYDOWN:
		{
			if (SDLKEY_CHIPMAP.count(event.key.keysym.sym) > 0)
			{
				EmuKeyPress(m_emu, SDLKEY_CHIPMAP.at(event.key.keysym.sym));
			}

			break;
		}

		case SDL_KEYUP:
		{
			if (SDLKEY_CHIPMAP.count(event.key.keysym.sym) > 0)
			{
				EmuKeyRelease(m_emu, SDLKEY_CHIPMAP.at(event.key.keysym.sym));
			}

			break;
		}

		case SDL_QUIT:
		{
			m_isRunning = false;
			SDL_Quit();
			break;
		}

		default:
		{
			break;
		}
	}
}
