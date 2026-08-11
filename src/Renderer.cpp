/**
 * @file Renderer.cpp
 * @brief Implementation of SDL2 2D Graphical Renderer.
 */

#include "Renderer.hpp"
#include <iostream>
#include <cmath>

namespace PingPong {

    Renderer::Renderer(int width, int height)
        : m_width(width), m_height(height), m_window(nullptr), m_sdlRenderer(nullptr) {}

    Renderer::~Renderer() {
        cleanup();
    }

    bool Renderer::init() {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
            return false;
        }

        m_window = SDL_CreateWindow(
            "Ping-Pong (C++ GUI Edition)",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            m_width,
            m_height,
            SDL_WINDOW_SHOWN
        );

        if (!m_window) {
            std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
            return false;
        }

        m_sdlRenderer = SDL_CreateRenderer(
            m_window,
            -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
        );

        if (!m_sdlRenderer) {
            // Fallback to software renderer if hardware acceleration is unavailable (e.g. headless/VM)
            m_sdlRenderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_SOFTWARE);
        }

        if (!m_sdlRenderer) {
            std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
            return false;
        }

        // Enable blend mode for transparent overlays
        SDL_SetRenderDrawBlendMode(m_sdlRenderer, SDL_BLENDMODE_BLEND);

        return true;
    }

    void Renderer::cleanup() {
        if (m_sdlRenderer) {
            SDL_DestroyRenderer(m_sdlRenderer);
            m_sdlRenderer = nullptr;
        }
        if (m_window) {
            SDL_DestroyWindow(m_window);
            m_window = nullptr;
        }
        SDL_Quit();
    }

    void Renderer::clearScreen() const {
        if (!m_sdlRenderer) return;
        SDL_SetRenderDrawColor(
            m_sdlRenderer,
            Config::COLOR_BACKGROUND.r,
            Config::COLOR_BACKGROUND.g,
            Config::COLOR_BACKGROUND.b,
            Config::COLOR_BACKGROUND.a
        );
        SDL_RenderClear(m_sdlRenderer);
    }

    void Renderer::present() const {
        if (m_sdlRenderer) {
            SDL_RenderPresent(m_sdlRenderer);
        }
    }

    void Renderer::drawRect(int x, int y, int w, int h, SDL_Color color) const {
        if (!m_sdlRenderer) return;
        SDL_SetRenderDrawColor(m_sdlRenderer, color.r, color.g, color.b, color.a);
        SDL_Rect rect = { x, y, w, h };
        SDL_RenderFillRect(m_sdlRenderer, &rect);
    }

    void Renderer::drawRectOutline(int x, int y, int w, int h, SDL_Color color) const {
        if (!m_sdlRenderer) return;
        SDL_SetRenderDrawColor(m_sdlRenderer, color.r, color.g, color.b, color.a);
        SDL_Rect rect = { x, y, w, h };
        SDL_RenderDrawRect(m_sdlRenderer, &rect);
    }

    void Renderer::drawFilledCircle(int centerX, int centerY, int radius, SDL_Color color) const {
        if (!m_sdlRenderer) return;
        SDL_SetRenderDrawColor(m_sdlRenderer, color.r, color.g, color.b, color.a);

        for (int w = 0; w < radius * 2; w++) {
            for (int h = 0; h < radius * 2; h++) {
                int dx = radius - w;
                int dy = radius - h;
                if ((dx * dx + dy * dy) <= (radius * radius)) {
                    SDL_RenderDrawPoint(m_sdlRenderer, centerX + dx, centerY + dy);
                }
            }
        }
    }

    void Renderer::render(const Ball& ball, const Paddle& player, const Paddle& bot, BotDifficulty diff, bool paused) const {
        (void)diff;
        clearScreen();

        // 1. Top & Bottom Borders
        drawRect(0, 0, m_width, 10, Config::COLOR_BORDER);
        drawRect(0, m_height - 10, m_width, 10, Config::COLOR_BORDER);

        // 2. Dashed Net Line (Center)
        int centerX = m_width / 2;
        for (int dashY = 15; dashY < m_height - 15; dashY += 25) {
            drawRect(centerX - 2, dashY, 4, 15, Config::COLOR_NET);
        }

        // 3. Player Paddle (Left)
        drawRect(
            static_cast<int>(player.getX() - Config::PADDLE_WIDTH / 2.0f),
            static_cast<int>(player.getY() - Config::PADDLE_HEIGHT / 2.0f),
            static_cast<int>(Config::PADDLE_WIDTH),
            static_cast<int>(Config::PADDLE_HEIGHT),
            Config::COLOR_PLAYER
        );

        // 4. Bot Paddle (Right)
        drawRect(
            static_cast<int>(bot.getX() - Config::PADDLE_WIDTH / 2.0f),
            static_cast<int>(bot.getY() - Config::PADDLE_HEIGHT / 2.0f),
            static_cast<int>(Config::PADDLE_WIDTH),
            static_cast<int>(Config::PADDLE_HEIGHT),
            Config::COLOR_BOT
        );

        // 5. Ball
        drawFilledCircle(
            static_cast<int>(ball.getX()),
            static_cast<int>(ball.getY()),
            static_cast<int>(Config::BALL_RADIUS),
            Config::COLOR_BALL
        );

        // 6. Scoreboard Visual Blocks (Top Left & Top Right)
        for (int i = 0; i < player.getScore(); ++i) {
            drawRect(40 + i * 25, 20, 18, 18, Config::COLOR_PLAYER);
        }
        for (int i = 0; i < bot.getScore(); ++i) {
            drawRect(m_width - 40 - (i + 1) * 25, 20, 18, 18, Config::COLOR_BOT);
        }

        // 7. Pause Overlay Screen
        if (paused) {
            drawRect(0, 0, m_width, m_height, SDL_Color{ 0, 0, 0, 165 });
            drawRect(m_width / 2 - 120, m_height / 2 - 30, 240, 60, SDL_Color{ 255, 200, 0, 220 });
        }

        present();
    }

    bool Renderer::renderMenu(BotDifficulty& selectedDiff, int mouseX, int mouseY, bool mouseClick, char keyChar) const {
        clearScreen();

        // Check Keyboard shortcuts [1, 2, 3]
        if (keyChar == '1') { selectedDiff = BotDifficulty::Easy; return true; }
        if (keyChar == '2') { selectedDiff = BotDifficulty::Medium; return true; }
        if (keyChar == '3') { selectedDiff = BotDifficulty::Hard; return true; }

        int btnWidth = 360;
        int btnHeight = 55;
        int btnX = (m_width - btnWidth) / 2;

        struct Option {
            BotDifficulty diff;
            int yPos;
        };

        Option options[] = {
            { BotDifficulty::Easy, 240 },
            { BotDifficulty::Medium, 320 },
            { BotDifficulty::Hard, 400 }
        };

        for (const auto& opt : options) {
            bool isHover = (mouseX >= btnX && mouseX <= btnX + btnWidth &&
                            mouseY >= opt.yPos && mouseY <= opt.yPos + btnHeight);

            SDL_Color btnColor = isHover ? SDL_Color{ 50, 115, 215, 255 } : SDL_Color{ 35, 45, 65, 255 };

            drawRect(btnX, opt.yPos, btnWidth, btnHeight, btnColor);
            drawRectOutline(btnX, opt.yPos, btnWidth, btnHeight, SDL_Color{ 100, 200, 255, 255 });

            if (isHover && mouseClick) {
                selectedDiff = opt.diff;
                return true;
            }
        }

        present();
        return false;
    }

    bool Renderer::renderGameOver(bool playerWon, int keySym) const {
        SDL_Color bgOverlay = playerWon ? SDL_Color{ 20, 80, 40, 200 } : SDL_Color{ 90, 20, 20, 200 };
        drawRect(0, 0, m_width, m_height, bgOverlay);
        drawRect(m_width / 2 - 150, m_height / 2 - 40, 300, 80, SDL_Color{ 255, 255, 255, 240 });

        present();
        return (keySym == SDLK_SPACE || keySym == SDLK_RETURN);
    }

} // namespace PingPong
