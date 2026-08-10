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

    // Google-branded palette: Blue (#4285F4), Red (#EA4335), Yellow (#FBBC05), Green (#34A853)
    const std::string Config::COLOR_RESET  = "\033[0m";
    const std::string Config::COLOR_BORDER = "\033[38;2;66;133;244m";  // Google Blue
    const std::string Config::COLOR_PLAYER = "\033[38;2;52;168;83m";   // Google Green
    const std::string Config::COLOR_AI     = "\033[38;2;234;67;53m";   // Google Red
    const std::string Config::COLOR_BALL   = "\033[38;2;251;188;5m";  // Google Yellow
    const std::string Config::COLOR_SCORE  = "\033[1;37m";             // Bold White
    const std::string Config::COLOR_TITLE  = "\033[1;36m";             // Bold Cyan

} // namespace PingPong
