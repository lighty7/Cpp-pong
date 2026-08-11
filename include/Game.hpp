/**
 * @file Game.hpp
 * @brief High-level Game Engine managing state updates, physics loop, & AI logic.
 * 
 * TUTORIAL SYNTAX & CONCEPTS:
 * 1. High Resolution Timing: Uses `<chrono>` library (`std::chrono::high_resolution_clock`)
 *    for frame rate regulation and frame delta time (`dt`) calculation.
 * 2. Game Engine Loop: `Input -> Update (Physics & AI) -> Collision Detection -> Render -> Sleep`.
 */

#ifndef GAME_HPP
#define GAME_HPP

#include "Config.hpp"
#include "Ball.hpp"
#include "Paddle.hpp"
#include "Renderer.hpp"

namespace PingPong {

    enum class GameState {
        Menu,
        Playing,
        Paused,
        GameOver
    };

    class Game {
    private:
        Ball m_ball;
        Paddle m_player;
        Paddle m_bot;
        Renderer m_renderer;
        BotDifficulty m_difficulty;
        GameState m_state;
        bool m_running;
        bool m_playerWon;

    public:
        Game();

        /**
         * @brief Main Game Loop entrypoint.
         */
        void run();

        /**
         * @brief Updates a single frame for Raylib event loop.
         * @return True if game continues running, false to exit.
         */
        bool updateFrame();

    private:
        void processInput(double deltaTime);
        void update(double deltaTime);
        void handleCollisions();
        void checkScore();
        void checkMatchOver();
        void resetRound();
    };

} // namespace PingPong

#endif // GAME_HPP
