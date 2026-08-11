#include "Game.hpp"
#include <SDL.h>
#include <algorithm>

namespace PingPong {

    Game::Game()
        : m_ball(Config::SCREEN_WIDTH / 2.0, Config::SCREEN_HEIGHT / 2.0),
          m_player(30.0, Config::SCREEN_HEIGHT / 2.0),
          m_bot(Config::SCREEN_WIDTH - 30.0, Config::SCREEN_HEIGHT / 2.0),
          m_renderer(Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT),
          m_difficulty(BotDifficulty::Medium),
          m_state(GameState::Menu),
          m_running(true),
          m_playerWon(false) {}

    void Game::run() {
        if (!m_renderer.init()) {
            return;
        }

        Uint32 lastTime = SDL_GetTicks();

        while (m_running) {
            Uint32 currentTime = SDL_GetTicks();
            double deltaTime = (currentTime - lastTime) / 1000.0;
            lastTime = currentTime;

            if (deltaTime > 0.05) deltaTime = 0.05;

            int mouseX = 0, mouseY = 0;
            SDL_GetMouseState(&mouseX, &mouseY);
            bool mouseClick = false;

            char keyChar = 0;
            int keySym = 0;

            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    m_running = false;
                } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        mouseClick = true;
                    }
                } else if (event.type == SDL_KEYDOWN) {
                    keySym = event.key.keysym.sym;
                    if (keySym == SDLK_ESCAPE) {
                        m_running = false;
                    } else if (keySym >= SDLK_1 && keySym <= SDLK_3) {
                        keyChar = '1' + (keySym - SDLK_1);
                    }
                }
            }

            const Uint8* keyboardState = SDL_GetKeyboardState(NULL);

            switch (m_state) {
                case GameState::Menu:
                    if (m_renderer.renderMenu(m_difficulty, mouseX, mouseY, mouseClick, keyChar)) {
                        resetRound();
                        m_state = GameState::Playing;
                    }
                    break;

                case GameState::Playing:
                    if (keySym == SDLK_p) {
                        m_state = GameState::Paused;
                    } else {
                        if (keyboardState[SDL_SCANCODE_W] || keyboardState[SDL_SCANCODE_UP]) {
                            m_player.moveUp(deltaTime, 10.0);
                        }
                        if (keyboardState[SDL_SCANCODE_S] || keyboardState[SDL_SCANCODE_DOWN]) {
                            m_player.moveDown(deltaTime, Config::SCREEN_HEIGHT - 10.0);
                        }
                        update(deltaTime);
                        handleCollisions();
                        checkScore();
                    }
                    m_renderer.render(m_ball, m_player, m_bot, m_difficulty, false);
                    break;

                case GameState::Paused:
                    if (keySym == SDLK_p) {
                        m_state = GameState::Playing;
                    }
                    m_renderer.render(m_ball, m_player, m_bot, m_difficulty, true);
                    break;

                case GameState::GameOver:
                    if (m_renderer.renderGameOver(m_playerWon, keySym)) {
                        m_player.resetScore();
                        m_bot.resetScore();
                        m_state = GameState::Menu;
                    }
                    break;
            }

            SDL_Delay(16);
        }

        m_renderer.cleanup();
    }

    bool Game::updateFrame() {
        return m_running;
    }

    void Game::processInput(double deltaTime) {
        (void)deltaTime;
    }

    void Game::update(double deltaTime) {
        m_ball.update(deltaTime);
        m_bot.updateBot(m_ball, m_difficulty, deltaTime, 10.0, Config::SCREEN_HEIGHT - 10.0);
    }

    void Game::handleCollisions() {
        // Top and Bottom Wall collisions
        if (m_ball.getY() <= 10.0 + Config::BALL_RADIUS) {
            m_ball.setY(10.0 + Config::BALL_RADIUS);
            m_ball.bounceY();
        } else if (m_ball.getY() >= Config::SCREEN_HEIGHT - 10.0 - Config::BALL_RADIUS) {
            m_ball.setY(Config::SCREEN_HEIGHT - 10.0 - Config::BALL_RADIUS);
            m_ball.bounceY();
        }

        // Left Player Paddle Collision
        if (m_player.checkCollision(m_ball.getX(), m_ball.getY()) && m_ball.getDirX() < 0) {
            double offset = m_player.getHitOffset(m_ball.getY());
            m_ball.setX(m_player.getX() + Config::PADDLE_WIDTH / 2.0 + Config::BALL_RADIUS);
            m_ball.bounceX(offset);
        }

        // Right Bot Paddle Collision
        if (m_bot.checkCollision(m_ball.getX(), m_ball.getY()) && m_ball.getDirX() > 0) {
            double offset = m_bot.getHitOffset(m_ball.getY());
            m_ball.setX(m_bot.getX() - Config::PADDLE_WIDTH / 2.0 - Config::BALL_RADIUS);
            m_ball.bounceX(offset);
        }
    }

    void Game::checkScore() {
        // Ball went past Left Wall -> Point for Bot
        if (m_ball.getX() < 0) {
            m_bot.incrementScore();
            checkMatchOver();
        }
        // Ball went past Right Wall -> Point for Player
        else if (m_ball.getX() > Config::SCREEN_WIDTH) {
            m_player.incrementScore();
            checkMatchOver();
        }
    }

    void Game::checkMatchOver() {
        if (m_player.getScore() >= Config::MAX_SCORE) {
            m_playerWon = true;
            m_state = GameState::GameOver;
        } else if (m_bot.getScore() >= Config::MAX_SCORE) {
            m_playerWon = false;
            m_state = GameState::GameOver;
        } else {
            resetRound();
        }
    }

    void Game::resetRound() {
        m_ball.reset(Config::SCREEN_WIDTH / 2.0, Config::SCREEN_HEIGHT / 2.0);
        m_player.setY(Config::SCREEN_HEIGHT / 2.0);
        m_bot.setY(Config::SCREEN_HEIGHT / 2.0);
    }

} // namespace PingPong
