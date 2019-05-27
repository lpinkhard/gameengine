//
//  Player.hpp
//  GameEngine
//
//  Created by Lionel Pinkhard on 2019/01/30.
//

#ifndef Player_hpp
#define Player_hpp

#include <stdio.h>

class Player {
public:
    Player();
    Player(int x, int y);
    void moveX(float intensity);
    void moveY(float intensity);
    void stopMoveX();
    void stopMoveY();
    void stopMovement();
    void doMovement();
    int getX();
    int getY();
    
private:
    float x;
    float y;
    float movementX;
    float movementY;
};

#endif /* Player_hpp */
