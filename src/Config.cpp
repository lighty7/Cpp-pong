/**
 * @file Config.cpp
 * @brief Initialization of static ANSI color formatting strings.
 * 
 * TUTORIAL SYNTAX & CONCEPTS:
 * 1. ANSI Escape Codes: Terminal control sequences starting with `\033[` (ESC code 27).
 *    - `\033[0m`: Resets text color and formatting to default.
 *    - `\033[38;2;R;G;Bm`: 24-bit TrueColor RGB color setting syntax.
 * 2. Static Member Definitions: In C++, non-integral `static const` members declared in
 *    a header file must be explicitly defined in a single translation unit (.cpp file).
 */

#include "Config.hpp"

namespace PingPong {

    // Modern color palette instances for SDL2 hardware renderer
    const SDL_Color Config::COLOR_BACKGROUND = { 20, 24, 32, 255 };   // Dark sleek background
    const SDL_Color Config::COLOR_BORDER     = { 66, 133, 244, 255 };  // Electric Blue
    const SDL_Color Config::COLOR_PLAYER     = { 52, 168, 83, 255 };   // Vibrant Green
    const SDL_Color Config::COLOR_BOT        = { 234, 67, 53, 255 };   // Crimson Red
    const SDL_Color Config::COLOR_BALL       = { 251, 188, 5, 255 };   // Neon Yellow
    const SDL_Color Config::COLOR_NET        = { 100, 115, 140, 150 }; // Translucent Net

} // namespace PingPong
