/**
 * @file Ball.hpp
 * @brief Representation of the Ping-Pong ball physics and kinematics.
 * 
 * TUTORIAL SYNTAX & CONCEPTS:
 * 1. Encapsulation: Private member variables (`m_x`, `m_y`) protect state from invalid external mutations.
 * 2. `const` Member Functions: Functions marked `const` (e.g., `getX() const`) guarantee they
 *    will not modify any member variables of the instance.
 * 3. Float kinematics: Position and Velocity use double/float precision for smooth fractional motion updates.
 */

#ifndef BALL_HPP
#define BALL_HPP

namespace PingPong {

    class Ball {
    private:
        double m_x;       ///< Current X coordinate on playing field
        double m_y;       ///< Current Y coordinate on playing field
        double m_dirX;    ///< Horizontal velocity component (-1.0 to 1.0)
        double m_dirY;    ///< Vertical velocity component (-1.0 to 1.0)
        double m_speed;   ///< Scalar movement speed factor

    public:
        /**
         * @brief Constructor initializing ball at center position.
         * @param startX Initial X coordinate
         * @param startY Initial Y coordinate
         */
        Ball(double startX, double startY);

        /**
         * @brief Resets ball to specified position with randomized serve direction.
         */
        void reset(double startX, double startY);

        /**
         * @brief Updates ball position based on current velocity and frame delta time.
         * @param deltaTime Time slice passed since last frame (in seconds).
         */
        void update(double deltaTime);

        /**
         * @brief Inverts vertical direction when hitting top/bottom walls.
         */
        void bounceY();

        /**
         * @brief Inverts horizontal direction and applies angle spin on paddle hit.
         * @param hitOffset Relative offset where ball struck the paddle (-1.0 to 1.0).
         */
        void bounceX(double hitOffset);

        // Getters (Const Correctness)
        double getX() const { return m_x; }
        double getY() const { return m_y; }
        double getDirX() const { return m_dirX; }
        double getDirY() const { return m_dirY; }

        // Setters for collision adjustment
        void setX(double x) { m_x = x; }
        void setY(double y) { m_y = y; }
    };

} // namespace PingPong

#endif // BALL_HPP
