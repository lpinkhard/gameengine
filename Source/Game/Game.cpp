//
//  Game.cpp
//  GameEngine
//
//  Created by Lionel Pinkhard on 2019/01/27.
//  Copyright © 2019 Lionel Pinkhard. All rights reserved.
//

#include <iostream>

#include "Game.hpp"

Game::Game() {
	hal = new HAL();
}

void Game::init() {
    hal->init(this);
    player = new Player();
}

void Game::redraw() {
    player->doMovement();
    printf("X %d, Y %d ", player->getX(), player->getY());
    
    hal->rectFill(NULL, 10, 10, 30, 30, 255, 255, 255, 255);
    hal->rect(NULL, 40, 40, 60, 60, 255, 0, 255, 255);
    hal->circle(NULL, 110, 110, 10, 255, 255, 0, 255);
    hal->circleFill(NULL, 80, 80, 10, 0, 255, 255, 255);
    hal->line(NULL, 10, 110, 110, 10, 255, 0, 0, 255);
    hal->putPixel(NULL, 50, 50, 255, 255, 255, 255);
    hal->triangleFill(NULL, 140, 120, 145, 150, 170, 150, 0, 255, 0, 255);
    
    hal->flip(NULL);
}

void Game::step() {

    redraw();
}

void Game::suspend() {

}

void Game::resume() {

}

void Game::shutdown() {
    hal->shutdown();
}

HAL *Game::getHAL() {
    return hal;
}

void Game::pointerMove(int x, int y, float rx, float ry) {
    
}

void Game::pointerScroll(float rz) {
    
}

void Game::pointerClick(int button, int x, int y) {
    switch (button) {
        case BUTTON_PRIMARY:
            int screenWidth = hal->getWidth();
            int screenHeight = hal->getHeight();
            
            int dx = x - hal->getWidth() / 2;
            int dy = y - hal->getHeight() / 2;
            
            player->moveX(float(dx) / float(screenWidth / 2));
            player->moveY(float(dy) / float(screenHeight / 2));
            break;
    }
    redraw();
}

void Game::pointerRelease(int button, int x, int y) {
    player->stopMovement();
    redraw();
}

void Game::motionMove(int x, int y, float rx, float ry) {
    
}

void Game::analogMove(int stick, float rx, float ry) {
    switch (stick) {
        case STICK_LEFT:
            player->moveX(rx);
            player->moveY(ry);
            break;
    }
    redraw();
}

void Game::triggerPress(int button, float intensity) {
    
}

void Game::triggerRelease(int button, float intensity) {
    
}

void Game::dpadPress(int direction) {
    
}

void Game::dpadRelease(int direction) {
    
}

void Game::buttonPress(int button) {
    
}

void Game::buttonRelease(int button) {
    
}

void Game::keyPress(int key) {
    switch (key) {
        case InputEvent::KEY_LEFT:
            player->moveX(-1.0f);
            break;
        case InputEvent::KEY_RIGHT:
            player->moveX(1.0f);
            break;
        case InputEvent::KEY_UP:
            player->moveY(-1.0f);
            break;
        case InputEvent::KEY_DOWN:
            player->moveY(1.0f);
            break;
    }
    redraw();
}

void Game::keyRelease(int key) {
    switch (key) {
        case InputEvent::KEY_LEFT:
        case InputEvent::KEY_RIGHT:
            player->stopMoveX();
            break;
        case InputEvent::KEY_UP:
        case InputEvent::KEY_DOWN:
            player->stopMoveY();
            break;
    }
    redraw();
}
