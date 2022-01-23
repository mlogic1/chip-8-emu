#pragma once

#include <SDL2/SDL.h>

class EmuWindow
{
    public:
        EmuWindow();
        ~EmuWindow();

        void Update();
        void UpdateGraphics(unsigned char* graphics);
        bool IsRunning() const;

    private:
        void HandleEvents(const SDL_Event& event);

    private:
        SDL_Window* m_window;
        SDL_Renderer* m_renderer;
        SDL_Texture* m_graphicsTexture; // a 64x32 native chip-8 texture

        bool m_isRunning = true;

};
