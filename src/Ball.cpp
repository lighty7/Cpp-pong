/**
 * @file Ball.cpp
 * @brief Implementation of Ball kinematics and bounce physics.
 * 
 * TUTORIAL SYNTAX & CONCEPTS:
 * 1. `<random>` Library: Modern C++ pseudo-random number generator using `std::mt19937`
 *    (Mersenne Twister algorithm) and `std::uniform_real_distribution` for unbiased random floats.
 * 2. Vector Kinematics: Position update `m_x += m_dirX * m_speed * deltaTime`.
 *    Multiplying by `deltaTime` ensures movement is frame-rate independent.
 */

#include "Ball.hpp"
#include "Config.hpp"
#include <random>
#include <cmath>

namespace PingPong {

    Ball::Ball(double startX, double startY)
        : m_x(startX), m_y(startY), m_dirX(1.0), m_dirY(0.0), m_speed(450.0) {
        reset(startX, startY);
    }

    void Ball::reset(double startX, double startY) {
        m_x = startX;
        m_y = startY;
        m_speed = 450.0; // Pixels per second

        // Use random engine to seed initial serve angle
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_real_distribution<double> dirDist(-0.6, 0.6);
        std::uniform_int_distribution<int> sideDist(0, 1);

        m_dirX = (sideDist(gen) == 0) ? 1.0 : -1.0;
        m_dirY = dirDist(gen);
    }

    void Ball::update(double deltaTime) {
        // Delta time scaling ensures physics remain identical across varying FPS rates
        m_x += m_dirX * m_speed * deltaTime;
        m_y += m_dirY * m_speed * deltaTime;
    }

    void Ball::bounceY() {
        m_dirY = -m_dirY; // Invert vertical velocity
    }

    void Ball::bounceX(double hitOffset) {
        m_dirX = -m_dirX; // Invert horizontal velocity
        
        // Adjust vertical bounce angle based on where the ball stikes the paddle
        // hitOffset range: -1.0 (top edge) to 1.0 (bottom edge)
        m_dirY = hitOffset * 1.2;

        // Slight speed increase on every paddle volley to increase difficulty
        if (m_speed < 850.0) {
            m_speed += 25.0;
        }
    }

} // namespace PingPong
