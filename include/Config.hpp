/**
 * @file Config.hpp
 * @brief Global configuration constants, difficulty levels, and ANSI visual tokens.
 * 
 * TUTORIAL SYNTAX & CONCEPTS:
 * 1. `#ifndef / #define / #endif`: Header guards prevent duplicate inclusions of headers
 *    during compilation units, avoiding redefinition errors.
 * 2. `namespace`: Namespaces group related identifiers (classes, variables, functions)
 *    to prevent name collisions in larger codebases.
 * 3. `constexpr`: Evaluated at compile-time instead of runtime, maximizing execution speed
 *    and eliminating memory lookup overhead.
 * 4. `enum class`: Strongly-typed enumeration in C++11+. Unlike traditional C-style enums,
 *    values are scoped (e.g. `AIDifficulty::Hard`) and do not implicitly convert to `int`.
 */

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <SDL.h>
#include <string>

namespace PingPong {

    /**
     * @brief Defines Bot Difficulty Levels affecting paddle reaction speed & prediction accuracy.
     */
    enum class BotDifficulty {
        Easy,     // Bot moves with intentional delay and reduced speed
        Medium,   // Bot moves at normal speed with occasional miscalculations
        Hard      // Bot calculates trajectory mathematically with high precision
    };

    /**
     * @brief Global Game Configuration Parameters
     */
    struct Config {
        // GUI Window dimensions
        static constexpr int SCREEN_WIDTH = 800;
        static constexpr int SCREEN_HEIGHT = 600;

        // Legacy board dimensions
        static constexpr int BOARD_WIDTH = 80;
        static constexpr int BOARD_HEIGHT = 24;

        // Target Frames Per Second (FPS)
        static constexpr int TARGET_FPS = 60;

        // Paddle & Ball Dimensions (Pixels)
        static constexpr float PADDLE_WIDTH = 16.0f;
        static constexpr float PADDLE_HEIGHT = 100.0f;
        static constexpr float BALL_RADIUS = 12.0f;

        // Winning Score Threshold
        static constexpr int MAX_SCORE = 5;

        // SDL_Color instances
        static const SDL_Color COLOR_BACKGROUND;
        static const SDL_Color COLOR_BORDER;
        static const SDL_Color COLOR_PLAYER;
        static const SDL_Color COLOR_BOT;
        static const SDL_Color COLOR_BALL;
        static const SDL_Color COLOR_NET;
    };

} // namespace PingPong

#endif // CONFIG_HPP
