/**
 * @file Renderer.cpp
 * @brief Implementation of POSIX raw input reading and ANSI terminal graphics.
 * 
 * TUTORIAL SYNTAX & CONCEPTS:
 * 1. Terminal Raw Mode (`termios` struct):
 *    - `c_lflag &= ~(ICANON | ECHO)`: Disables canonical buffer (so input is available immediately
 *      without waiting for Enter key) and disables echoing pressed characters to screen.
 *    - `c_cc[VMIN] = 0`, `c_cc[VTIME] = 0`: Non-blocking `read()` calls return 0 immediately if
 *      no key is queued.
 * 2. ANSI Escape Codes:
 *    - `\033[H`: Cursor home (0,0).
 *    - `\033[2J`: Clear entire screen.
 *    - `\033[?25l` / `\033[?25h`: Hide/Show terminal cursor.
 */

#include "Renderer.hpp"
#include <iostream>
#include <sstream>
#include <vector>
#include <termios.h>
#include <unistd.h>
#include <sys/select.h>
#include <cmath>

namespace PingPong {

    static struct termios orig_termios;
    static bool raw_mode_enabled = false;

    Renderer::Renderer(int width, int height) : m_width(width), m_height(height) {}

    Renderer::~Renderer() {
        disableRawMode();
        // Restore cursor and color
        std::cout << "\033[?25h" << Config::COLOR_RESET << std::flush;
    }

    void Renderer::enableRawMode() {
        if (raw_mode_enabled) return;

        tcgetattr(STDIN_FILENO, &orig_termios);
        struct termios raw = orig_termios;

        // Disable ECHO (showing typed keys) and ICANON (buffered line input)
        raw.c_lflag &= ~(ECHO | ICANON);
        raw.c_cc[VMIN] = 0;
        raw.c_cc[VTIME] = 0;

        tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
        raw_mode_enabled = true;

        // Hide terminal cursor
        std::cout << "\033[?25l" << std::flush;
    }

    void Renderer::disableRawMode() {
        if (!raw_mode_enabled) return;
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
        raw_mode_enabled = false;
        std::cout << "\033[?25h" << std::flush;
    }

    char Renderer::readKey() {
        char c = 0;
        if (read(STDIN_FILENO, &c, 1) == 1) {
            return c;
        }
        return 0;
    }

    void Renderer::clearScreen() const {
        std::cout << "\033[2J\033[H" << std::flush;
    }

    void Renderer::render(const Ball& ball, const Paddle& player, const Paddle& ai, AIDifficulty diff, bool paused) const {
        std::stringstream ss;

        // Move cursor to top-left corner (0,0) instead of clearing screen to prevent flicker
        ss << "\033[H";

        // Header Title & Scoreboard
        ss << Config::COLOR_TITLE << "  === GOOGLE PING-PONG (C++ EDITION) ===  " << Config::COLOR_RESET << "\n";
        
        std::string diffStr;
        switch (diff) {
            case AIDifficulty::Easy: diffStr = "EASY"; break;
            case AIDifficulty::Medium: diffStr = "MEDIUM"; break;
            case AIDifficulty::Hard: diffStr = "HARD"; break;
        }

        ss << Config::COLOR_SCORE << "  [ PLAYER (W/S) : " << player.getScore() 
           << " ]    AI (" << diffStr << ") : " << ai.getScore() << "    [ Q:Quit  P:Pause ]" 
           << Config::COLOR_RESET << "\n";

        // Top Border
        ss << Config::COLOR_BORDER << "+";
        for (int i = 0; i < m_width; ++i) ss << "-";
        ss << "+\n" << Config::COLOR_RESET;

        // Grid representation
        int ballX = static_cast<int>(std::round(ball.getX()));
        int ballY = static_cast<int>(std::round(ball.getY()));

        int playerX = static_cast<int>(std::round(player.getX()));
        int playerY = static_cast<int>(std::round(player.getY()));
        int playerHalfH = player.getHeight() / 2;

        int aiX = static_cast<int>(std::round(ai.getX()));
        int aiY = static_cast<int>(std::round(ai.getY()));
        int aiHalfH = ai.getHeight() / 2;

        for (int y = 0; y < m_height; ++y) {
            ss << Config::COLOR_BORDER << "|" << Config::COLOR_RESET;

            for (int x = 0; x < m_width; ++x) {
                // Ball
                if (x == ballX && y == ballY) {
                    ss << Config::COLOR_BALL << "O" << Config::COLOR_RESET;
                }
                // Player Paddle (Left side)
                else if (x == playerX && (y >= playerY - playerHalfH && y <= playerY + playerHalfH)) {
                    ss << Config::COLOR_PLAYER << "#" << Config::COLOR_RESET;
                }
                // AI Paddle (Right side)
                else if (x == aiX && (y >= aiY - aiHalfH && y <= aiY + aiHalfH)) {
                    ss << Config::COLOR_AI << "#" << Config::COLOR_RESET;
                }
                // Center Net Line
                else if (x == m_width / 2) {
                    ss << Config::COLOR_BORDER << ":" << Config::COLOR_RESET;
                }
                else {
                    ss << " ";
                }
            }
            ss << Config::COLOR_BORDER << "|\n" << Config::COLOR_RESET;
        }

        // Bottom Border
        ss << Config::COLOR_BORDER << "+";
        for (int i = 0; i < m_width; ++i) ss << "-";
        ss << "+\n" << Config::COLOR_RESET;

        if (paused) {
            ss << Config::COLOR_TITLE << "              *** GAME PAUSED - Press P to Resume ***              " << Config::COLOR_RESET << "\n";
        } else {
            ss << "                                                                   \n";
        }

        std::cout << ss.str() << std::flush;
    }

    AIDifficulty Renderer::renderMenu() const {
        clearScreen();
        std::cout << Config::COLOR_TITLE << "\n"
                  << "   =========================================\n"
                  << "         GOOGLE PING-PONG (C++ EDITION)     \n"
                  << "   =========================================\n"
                  << Config::COLOR_RESET << "\n";

        std::cout << Config::COLOR_SCORE << "  Select AI Difficulty Level:\n\n" << Config::COLOR_RESET;
        std::cout << "  [1] Easy   (Relaxed pace)\n";
        std::cout << "  [2] Medium (Standard challenge)\n";
        std::cout << "  [3] Hard   (Expert precise AI)\n\n";
        std::cout << "  Press key [1, 2, or 3] to start: " << std::flush;

        enableRawMode();

        while (true) {
            char key = readKey();
            if (key == '1') return AIDifficulty::Easy;
            if (key == '2') return AIDifficulty::Medium;
            if (key == '3') return AIDifficulty::Hard;
            usleep(10000); // 10ms sleep to avoid 100% CPU lock in menu loop
        }
    }

    void Renderer::renderGameOver(bool playerWon) const {
        std::cout << "\n\n";
        if (playerWon) {
            std::cout << Config::COLOR_PLAYER
                      << "  =========================================\n"
                      << "          YOU WON! CONGRATULATIONS!        \n"
                      << "  =========================================\n"
                      << Config::COLOR_RESET << "\n";
            std::cout << Config::COLOR_SCORE << "  You defeated the Computer AI!\n" << Config::COLOR_RESET;
        } else {
            std::cout << Config::COLOR_AI
                      << "  =========================================\n"
                      << "          GAME OVER - COMPUTER AI WON      \n"
                      << "  =========================================\n"
                      << Config::COLOR_RESET << "\n";
            std::cout << Config::COLOR_SCORE << "  Better luck next time!\n" << Config::COLOR_RESET;
        }
        std::cout << "\n  Press any key to exit...\n" << std::flush;

        while (readKey() == 0) {
            usleep(20000);
        }
    }

} // namespace PingPong
