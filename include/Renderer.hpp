/**
 * @file Renderer.hpp
 * @brief Terminal ANSI Rendering Engine and POSIX Raw Terminal Input Handler.
 * 
 * TUTORIAL SYNTAX & CONCEPTS:
 * 1. Low-Level Input Handling: Disables canonical buffer & echo mode using `<termios.h>`
 *    to read single keypresses non-blockingly without needing Enter key press.
 * 2. Double-Buffering / ANSI Escape Sequences: Uses ANSI codes like `\033[H` (move cursor home)
 *    and `\033[2J` (clear screen) to prevent terminal flickering during 60 FPS updates.
 */

#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "Config.hpp"
#include "Ball.hpp"
#include "Paddle.hpp"
#include <SDL.h>

namespace PingPong {

    class Renderer {
    private:
        int m_width;
        int m_height;
        SDL_Window* m_window;
        SDL_Renderer* m_sdlRenderer;

    public:
        Renderer(int width = Config::SCREEN_WIDTH, int height = Config::SCREEN_HEIGHT);
        ~Renderer();

        bool init();
        void cleanup();

        void clearScreen() const;
        void present() const;

        void render(const Ball& ball, const Paddle& player, const Paddle& bot, BotDifficulty diff, bool paused) const;
        bool renderMenu(BotDifficulty& selectedDiff, int mouseX, int mouseY, bool mouseClick, char keyChar) const;
        bool renderGameOver(bool playerWon, int keySym) const;

        SDL_Window* getWindow() const { return m_window; }
        SDL_Renderer* getSDLRenderer() const { return m_sdlRenderer; }

    private:
        void drawFilledCircle(int centerX, int centerY, int radius, SDL_Color color) const;
        void drawRect(int x, int y, int w, int h, SDL_Color color) const;
        void drawRectOutline(int x, int y, int w, int h, SDL_Color color) const;
    };

} // namespace PingPong

#endif // RENDERER_HPP
