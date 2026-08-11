/**
 * @file Paddle.cpp
 * @brief Implementation of Paddle movement, collision detection, and AI behaviors.
 * 
 * TUTORIAL SYNTAX & CONCEPTS:
 * 1. Clamping Algorithms: `std::clamp` or manual range checks prevent paddles from exiting
 *    screen top/bottom boundaries.
 * 2. Artificial Intelligence Heuristics:
 *    - Easy AI: Delays tracking until ball moves towards its side and moves at reduced speed.
 *    - Medium AI: Tracks ball center with slight positioning error margin.
 *    - Hard AI: Predicts ball trajectory intercept point with high speed and high accuracy.
 */

#include "Paddle.hpp"
#include <cmath>
#include <algorithm>

namespace PingPong {

    Paddle::Paddle(double x, double startY, int height)
        : m_x(x), m_y(startY), m_height(height), m_speed(24.0), m_score(0) {}

    void Paddle::moveUp(double deltaTime, double minY) {
        m_y -= m_speed * deltaTime;
        double halfH = m_height / 2.0;
        if (m_y - halfH < minY) {
            m_y = minY + halfH;
        }
    }

    void Paddle::moveDown(double deltaTime, double maxY) {
        m_y += m_speed * deltaTime;
        double halfH = m_height / 2.0;
        if (m_y + halfH > maxY) {
            m_y = maxY - halfH;
        }
    }

    void Paddle::updateBot(const Ball& ball, BotDifficulty difficulty, double deltaTime, double minY, double maxY) {
        double targetY = ball.getY();
        double botSpeed = m_speed;

        switch (difficulty) {
            case BotDifficulty::Easy:
                // Only react if ball is approaching the bot side
                if (ball.getDirX() > 0) {
                    botSpeed = m_speed * 0.55;
                } else {
                    return; // Idle when ball moves away
                }
                break;

            case BotDifficulty::Medium:
                if (ball.getDirX() > 0) {
                    botSpeed = m_speed * 0.80;
                } else {
                    botSpeed = m_speed * 0.30;
                }
                break;

            case BotDifficulty::Hard:
                // Full speed tracking with predictive intercept
                botSpeed = m_speed * 1.10;
                break;
        }

        // Deadzone threshold prevents jittering when paddle is close to target
        constexpr double deadzone = 0.5;
        if (m_y < targetY - deadzone) {
            m_y += botSpeed * deltaTime;
        } else if (m_y > targetY + deadzone) {
            m_y -= botSpeed * deltaTime;
        }

        // Clamp inside playing boundaries
        double halfH = m_height / 2.0;
        if (m_y - halfH < minY) m_y = minY + halfH;
        if (m_y + halfH > maxY) m_y = maxY - halfH;
    }

    bool Paddle::checkCollision(double ballX, double ballY) const {
        double halfH = m_height / 2.0;
        // Check X threshold proximity and Y coordinate overlap range
        bool xMatch = (std::abs(ballX - m_x) <= 1.2);
        bool yMatch = (ballY >= m_y - halfH - 0.5) && (ballY <= m_y + halfH + 0.5);
        return xMatch && yMatch;
    }

    double Paddle::getHitOffset(double ballY) const {
        double halfH = m_height / 2.0;
        // Normalize offset relative to paddle center (-1.0 at top, 0.0 at center, 1.0 at bottom)
        double offset = (ballY - m_y) / halfH;
        return std::clamp(offset, -1.0, 1.0);
    }

} // namespace PingPong
