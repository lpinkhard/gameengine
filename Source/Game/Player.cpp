//
//  Player.cpp
//  GameEngine
//
//  Created by Lionel Pinkhard on 2019/01/30.
//

#include "Player.hpp"

Player::Player() {
    x = 0;
    y = 0;
    stopMovement();
}

Player::Player(int x, int y) {
    this->x = x;
    this->y = y;
    stopMovement();
}

void Player::moveX(float intensity) {
    movementX += intensity;
    printf("Moving X %f ", intensity);
}

void Player::moveY(float intensity) {
    movementY += intensity;
    printf("Moving Y %f ", intensity);
}

void Player::stopMoveX() {
    movementX = 0.0f;
}

void Player::stopMoveY() {
    movementY = 0.0f;
}

void Player::stopMovement() {
    movementX = 0.0f;
    movementY = 0.0f;
}

void Player::doMovement() {
    y += movementY;
    x += movementX;
}

int Player::getX() {
    return x;
}

int Player::getY() {
    return y;
}
