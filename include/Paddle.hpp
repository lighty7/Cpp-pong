/**
 * @file Paddle.hpp
 * @brief Representation of player & AI paddles and autonomous movement logic.
 * 
 * TUTORIAL SYNTAX & CONCEPTS:
 * 1. Default Arguments & Initializer Lists: Efficient C++ initialization syntax.
 * 2. Static Cast: `static_cast<int>(val)` performs explicit type conversions safely.
 * 3. Autonomous AI Tracking: Mathematical prediction of target Y position based on ball position.
 */

#ifndef PADDLE_HPP
#define PADDLE_HPP

#include "Config.hpp"
#include "Ball.hpp"

namespace PingPong {

    class Paddle {
    private:
        double m_x;          ///< Fixed horizontal X coordinate of paddle
        double m_y;          ///< Current Y coordinate (center of paddle)
        int m_height;        ///< Height of paddle in characters
        double m_speed;      ///< Maximum vertical movement speed per second
        int m_score;         ///< Current player score

    public:
        /**
         * @brief Constructor
         * @param x Initial fixed X column position
         * @param startY Initial Y row position
         * @param height Height of the paddle in grid units
         */
        Paddle(double x, double startY, int height = Config::PADDLE_HEIGHT);

        /**
         * @brief Moves paddle up (decreases Y index in terminal coordinates).
         * @param deltaTime Time elapsed since last frame.
         * @param minY Boundary limit top
         */
        void moveUp(double deltaTime, double minY);

        /**
         * @brief Moves paddle down (increases Y index in terminal coordinates).
         * @param deltaTime Time elapsed since last frame.
         * @param maxY Boundary limit bottom
         */
        void moveDown(double deltaTime, double maxY);

        /**
         * @brief AI Control Logic: Automatically tracks and moves towards ball position.
         * @param ball Reference to current ball instance
         * @param difficulty Level of AI performance (Easy, Medium, Hard)
         * @param deltaTime Frame time step
         * @param minY Boundary top
         * @param maxY Boundary bottom
         */
        void updateAI(const Ball& ball, AIDifficulty difficulty, double deltaTime, double minY, double maxY);

        /**
         * @brief Checks if a given point (ballX, ballY) hits this paddle.
         */
        bool checkCollision(double ballX, double ballY) const;

        /**
         * @brief Calculates relative hit offset from center of paddle (-1.0 to 1.0).
         */
        double getHitOffset(double ballY) const;

        // Getters
        double getX() const { return m_x; }
        double getY() const { return m_y; }
        int getHeight() const { return m_height; }
        int getScore() const { return m_score; }

        // Score management
        void incrementScore() { m_score++; }
        void resetScore() { m_score = 0; }
        void setY(double y) { m_y = y; }
    };

} // namespace PingPong

#endif // PADDLE_HPP
