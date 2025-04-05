#pragma once

#include <SDL2/SDL.h>
#include "IDisplay.h"
#include "chip8.h"

class EmuWindow : public IDisplay
{
    public:
        EmuWindow();
        ~EmuWindow();

        void Update();
		void DrawInstruction(unsigned char* graphicsPtr) override;
        bool IsRunning() const;

    private:
        void HandleEvents(const SDL_Event& event);

    private:
        SDL_Window* m_window;
        SDL_Renderer* m_renderer;
        SDL_Texture* m_graphicsTexture; // a 64x32 native chip-8 texture

        bool m_isRunning = true;
		Chip8* m_emu = nullptr;
};
