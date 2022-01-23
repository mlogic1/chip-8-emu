#include "EmuWindow.h"
#include <iostream>

EmuWindow::EmuWindow()
{
    m_window = SDL_CreateWindow("Chip-8 Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 64, 32, 0);
    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC);
    m_graphicsTexture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 64, 32);
}

EmuWindow::~EmuWindow()
{
    SDL_DestroyTexture(m_graphicsTexture);
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
}

void EmuWindow::Update()
{
    SDL_Event event;
    SDL_PollEvent(&event);
    HandleEvents(event);

    // handle SDL events here

    static int prevTime = 0;
    static float thisTime = SDL_GetTicks();
    const float dt = (thisTime - prevTime) / 1000.0f;
    prevTime = thisTime;

    // render
    SDL_RenderClear(m_renderer);
    SDL_RenderCopy(m_renderer, m_graphicsTexture, NULL, NULL);
    SDL_RenderPresent(m_renderer);
    SDL_Delay(16);
}

void EmuWindow::UpdateGraphics(unsigned char* graphics)
{
    SDL_SetRenderTarget(m_renderer, m_graphicsTexture); // render to texture
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0 , 255);
    SDL_RenderClear(m_renderer);

    // draw white pixels
    SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
    /*for (int i = 0 ; i < 64 ; ++i)
    {
        for (int j = 0 ; j < 32 ; ++j)
        {
            if (graphics[i * 64 + j] == '1')

                SDL_RenderDrawPoint(m_renderer, i, j);
        }
    }*/

    SDL_RenderDrawPoint(m_renderer, 10, 10);
    SDL_RenderDrawPoint(m_renderer, 20, 20);
    SDL_RenderDrawPoint(m_renderer, 30, 30);

    SDL_RenderCopy(m_renderer, m_graphicsTexture, NULL, NULL);
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0 , 255);

    SDL_SetRenderTarget(m_renderer, NULL);  // continue rendering to screen
}

bool EmuWindow::IsRunning() const
{
    return m_isRunning;
}

void EmuWindow::HandleEvents(const SDL_Event& event)
{
    switch (event.type)
	{
		case SDL_KEYDOWN:
		{
			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				// m_isRunning = false;
			}

			break;
		}

		case SDL_KEYUP:
		{
			// m_currentScene->OnKeyReleased(event.key.keysym.sym);
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
