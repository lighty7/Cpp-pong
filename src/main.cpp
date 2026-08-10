/**
 * @file main.cpp
 * @brief Entry point for the C++ Google Ping-Pong Game.
 * 
 * TUTORIAL SYNTAX & CONCEPTS:
 * 1. `main()` Function: Program execution starting point. Returns integer status code
 *    (0 for success, non-zero for runtime failures).
 * 2. `try / catch`: Standard C++ Exception Handling syntax to catch runtime errors safely
 *    and ensure terminal raw mode is restored before exiting.
 */

#include "Game.hpp"
#include <iostream>
#include <exception>

int main() {
    try {
        PingPong::Game game;
        game.run();
    } catch (const std::exception& ex) {
        PingPong::Renderer::disableRawMode();
        std::cerr << "\n[Fatal Error]: " << ex.what() << std::endl;
        return 1;
    } catch (...) {
        PingPong::Renderer::disableRawMode();
        std::cerr << "\n[Fatal Error]: Unknown exception occurred." << std::endl;
        return 1;
    }

    return 0;
}
