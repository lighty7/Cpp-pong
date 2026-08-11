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
        // Playing field dimensions (Terminal characters)
        static constexpr int BOARD_WIDTH = 80;
        static constexpr int BOARD_HEIGHT = 24;

        // Target Frames Per Second (FPS) for smooth physics calculations
        static constexpr int TARGET_FPS = 60;
        static constexpr double FRAME_DURATION_MS = 1000.0 / TARGET_FPS;

        // Paddle Dimensions
        static constexpr int PADDLE_HEIGHT = 5;

        // Winning Score Threshold
        static constexpr int MAX_SCORE = 5;

        // ANSI Color Codes for retro graphics in terminal
        static const std::string COLOR_RESET;
        static const std::string COLOR_BORDER;
        static const std::string COLOR_PLAYER;
        static const std::string COLOR_BOT;
        static const std::string COLOR_BALL;
        static const std::string COLOR_SCORE;
        static const std::string COLOR_TITLE;
    };

} // namespace PingPong

#endif // CONFIG_HPP
