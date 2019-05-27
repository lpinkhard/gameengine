//
//  Game.hpp
//  GameEngine
//
//  Created by Lionel Pinkhard on 2019/01/29.
//

#ifndef Game_hpp
#define Game_hpp

#include <cmath>

#include "HAL.hpp"
#include "Player.hpp"

class Game : public InputEvent {
public:
	Game();
    void init();
    void redraw();
    void step();
	void suspend();
	void resume();
    void shutdown();
    
    // Input events
    void pointerMove(int x, int y, float rx, float ry);
    void pointerScroll(float rz);
    void pointerClick(int button, int x, int y);
    void pointerRelease(int button, int x, int y);
    void motionMove(int x, int y, float rx, float ry);
    void analogMove(int stick, float rx, float ry);
    void triggerPress(int button, float intensity);
    void triggerRelease(int button, float intensity);
    void dpadPress(int direction);
    void dpadRelease(int direction);
    void buttonPress(int button);
    void buttonRelease(int button);
    void keyPress(int key);
    void keyRelease(int key);

    // HAL access
    HAL *getHAL();
    
private:
    HAL *hal;
    Player *player;
};

#endif /* Game_hpp */
