/**
 * @file test_physics.cpp
 * @brief Unit tests for Ball physics and Paddle collision detection in C++.
 * 
 * TUTORIAL SYNTAX & CONCEPTS:
 * 1. Unit Testing: Automated verification of individual code components.
 * 2. Assertions: `assert()` evaluates expressions; terminates with line details if false.
 */

#include "Ball.hpp"
#include "Paddle.hpp"
#include "Config.hpp"
#include <cassert>
#include <iostream>
#include <cmath>

using namespace PingPong;

void testBallMovement() {
    Ball ball(40.0, 12.0);
    double initialX = ball.getX();
    ball.update(0.1); // 100ms delta time
    (void)initialX;
    assert(ball.getX() != initialX);
    std::cout << "[PASS] Ball Movement Test" << std::endl;
}

void testWallBounce() {
    Ball ball(40.0, 0.0);
    double initialDirY = ball.getDirY();
    ball.bounceY();
    (void)initialDirY;
    assert(ball.getDirY() == -initialDirY);
    std::cout << "[PASS] Wall Bounce Test" << std::endl;
}

void testPaddleCollision() {
    Paddle paddle(5.0, 10.0, 5);
    // Ball at same coordinate should collide
    assert(paddle.checkCollision(5.0, 10.0) == true);
    // Ball far away should not collide
    assert(paddle.checkCollision(25.0, 10.0) == false);
    std::cout << "[PASS] Paddle Collision Test" << std::endl;
}

void testScoring() {
    Paddle paddle(5.0, 10.0, 5);
    assert(paddle.getScore() == 0);
    paddle.incrementScore();
    assert(paddle.getScore() == 1);
    paddle.resetScore();
    assert(paddle.getScore() == 0);
    std::cout << "[PASS] Score Tracking Test" << std::endl;
}

int main() {
    std::cout << "Running C++ Physics & Collision Unit Tests...\n";
    testBallMovement();
    testWallBounce();
    testPaddleCollision();
    testScoring();
    std::cout << "All C++ Unit Tests Passed Successfully!\n";
    return 0;
}
