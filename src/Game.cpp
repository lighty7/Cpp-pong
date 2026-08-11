/**
 * @file Game.cpp
 * @brief Implementation of the main game loop, frame timing, collisions, and state.
 * 
 * TUTORIAL SYNTAX & CONCEPTS:
 * 1. High-Resolution Game Loop:
 *    - `std::chrono::high_resolution_clock::now()` captures exact time points.
 *    - `std::chrono::duration<double>(now - lastTime).count()` converts time span into fractional seconds.
 *    - `std::this_thread::sleep_for(duration)` yields CPU time to keep frame rates steady at ~60 FPS.
 * 2. Axis-Aligned Bounding Box (AABB) Collisions & Angle Reflections.
 */

#include "Game.hpp"
#include <chrono>
#include <thread>
#include <iostream>

namespace PingPong {

    Game::Game()
        : m_ball(Config::BOARD_WIDTH / 2.0, Config::BOARD_HEIGHT / 2.0),
          m_player(2.0, Config::BOARD_HEIGHT / 2.0),
          m_bot(Config::BOARD_WIDTH - 3.0, Config::BOARD_HEIGHT / 2.0),
          m_renderer(Config::BOARD_WIDTH, Config::BOARD_HEIGHT),
          m_difficulty(BotDifficulty::Medium),
          m_state(GameState::Menu),
          m_running(true),
          m_playerWon(false) {}

    void Game::run() {
        // Show start menu to select bot difficulty
        m_difficulty = m_renderer.renderMenu();
        m_state = GameState::Playing;

        // Clear menu screen before starting game render
        m_renderer.clearScreen();

        auto lastTime = std::chrono::high_resolution_clock::now();

        while (m_running && m_state != GameState::GameOver) {
            auto currentTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsedTime = currentTime - lastTime;
            lastTime = currentTime;

            double deltaTime = elapsedTime.count();

            // Clamp delta time to avoid large physics jumps during system lag spikes
            if (deltaTime > 0.05) deltaTime = 0.05;

            processInput();

            if (m_state == GameState::Playing) {
                update(deltaTime);
                handleCollisions();
                checkScore();
            }

            m_renderer.render(m_ball, m_player, m_bot, m_difficulty, m_state == GameState::Paused);

            // Cap game loop at target FPS (~60 FPS -> 16.6ms per frame)
            auto frameEndTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> frameDuration = frameEndTime - currentTime;

            if (frameDuration.count() < Config::FRAME_DURATION_MS) {
                double sleepTime = Config::FRAME_DURATION_MS - frameDuration.count();
                std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(sleepTime));
            }
        }

        Renderer::disableRawMode();
        m_renderer.renderGameOver(m_playerWon);
    }

    void Game::processInput() {
        char key = Renderer::readKey();
        if (key == 0) return;

        // Key controls
        if (key == 'q' || key == 'Q') {
            m_running = false;
        } else if (key == 'p' || key == 'P') {
            if (m_state == GameState::Playing) {
                m_state = GameState::Paused;
            } else if (m_state == GameState::Paused) {
                m_state = GameState::Playing;
            }
        }

        if (m_state == GameState::Playing) {
            // W / S keys for up/down movement
            if (key == 'w' || key == 'W') {
                m_player.moveUp(0.05, 0.0);
            } else if (key == 's' || key == 'S') {
                m_player.moveDown(0.05, Config::BOARD_HEIGHT);
            }
        }
    }

    void Game::update(double deltaTime) {
        m_ball.update(deltaTime);
        m_bot.updateBot(m_ball, m_difficulty, deltaTime, 0.0, Config::BOARD_HEIGHT);
    }

    void Game::handleCollisions() {
        // Top and Bottom Wall collisions
        if (m_ball.getY() <= 0.0) {
            m_ball.setY(0.0);
            m_ball.bounceY();
        } else if (m_ball.getY() >= Config::BOARD_HEIGHT - 1.0) {
            m_ball.setY(Config::BOARD_HEIGHT - 1.0);
            m_ball.bounceY();
        }

        // Left Player Paddle Collision
        if (m_player.checkCollision(m_ball.getX(), m_ball.getY()) && m_ball.getDirX() < 0) {
            double offset = m_player.getHitOffset(m_ball.getY());
            m_ball.setX(m_player.getX() + 1.0);
            m_ball.bounceX(offset);
        }

        // Right Bot Paddle Collision
        if (m_bot.checkCollision(m_ball.getX(), m_ball.getY()) && m_ball.getDirX() > 0) {
            double offset = m_bot.getHitOffset(m_ball.getY());
            m_ball.setX(m_bot.getX() - 1.0);
            m_ball.bounceX(offset);
        }
    }

    void Game::checkScore() {
        // Ball went past Left Wall -> Point for Bot
        if (m_ball.getX() < 0) {
            m_bot.incrementScore();
            resetRound();
        }
        // Ball went past Right Wall -> Point for Player
        else if (m_ball.getX() > Config::BOARD_WIDTH) {
            m_player.incrementScore();
            resetRound();
        }

        // Check Match Winner
        if (m_player.getScore() >= Config::MAX_SCORE) {
            m_playerWon = true;
            m_state = GameState::GameOver;
        } else if (m_bot.getScore() >= Config::MAX_SCORE) {
            m_playerWon = false;
            m_state = GameState::GameOver;
        }
    }

    void Game::resetRound() {
        m_ball.reset(Config::BOARD_WIDTH / 2.0, Config::BOARD_HEIGHT / 2.0);
        m_player.setY(Config::BOARD_HEIGHT / 2.0);
        m_bot.setY(Config::BOARD_HEIGHT / 2.0);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

} // namespace PingPong
