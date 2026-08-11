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

namespace PingPong {

    class Renderer {
    private:
        int m_width;
        int m_height;

    public:
        Renderer(int width = Config::BOARD_WIDTH, int height = Config::BOARD_HEIGHT);
        ~Renderer();

        /**
         * @brief Enables POSIX raw terminal mode for non-blocking key presses.
         */
        static void enableRawMode();

        /**
         * @brief Restores original terminal settings on exit.
         */
        static void disableRawMode();

        /**
         * @brief Checks if a key has been pressed on stdin without blocking execution.
         * @return Character code pressed, or 0 if no input available.
         */
        static char readKey();

        /**
         * @brief Clears screen buffer.
         */
        void clearScreen() const;

        /**
         * @brief Renders the entire game state to stdout.
         */
        void render(const Ball& ball, const Paddle& player, const Paddle& bot, BotDifficulty diff, bool paused) const;

        /**
         * @brief Displays start menu to pick bot difficulty level.
         */
        BotDifficulty renderMenu() const;

        /**
         * @brief Displays game over summary screen.
         */
        void renderGameOver(bool playerWon) const;
    };

} // namespace PingPong

#endif // RENDERER_HPP
