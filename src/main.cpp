/**
 * @file main.cpp
 * @brief Entry point for the C++ Ping-Pong GUI Game.
 */

#include "Game.hpp"
#include <iostream>
#include <exception>

int main() {
    try {
        PingPong::Game game;
        game.run();
    } catch (const std::exception& ex) {
        std::cerr << "\n[Fatal Error]: " << ex.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "\n[Fatal Error]: Unknown exception occurred." << std::endl;
        return 1;
    }

    return 0;
}
