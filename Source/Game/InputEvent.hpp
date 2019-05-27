//
//  InputEvent.h
//  GameEngine
//
//  Created by Lionel Pinkhard on 2019/01/29.
//

#ifndef InputEvent_h
#define InputEvent_h

class InputEvent {
public:
    // D-pad directions
    const static int DPAD_UP = 1;
    const static int DPAD_DOWN = 2;
    const static int DPAD_LEFT = 4;
    const static int DPAD_RIGHT = 8;
    
    // Sticks
    const static int STICK_LEFT = 1;
    const static int STICK_RIGHT = 2;
    
    // Pointer buttons
    const static int BUTTON_PRIMARY = 1;
    const static int BUTTON_SECONDARY = 2;
    const static int BUTTON_MIDDLE = 3;
    
    // Joystick buttons (numbers overlap due to different platforms)
    const static int BUTTON_1 = 1;    // Desktop computer buttons are numbered
    const static int BUTTON_2 = 2;
    const static int BUTTON_3 = 4;
    const static int BUTTON_4 = 8;
    const static int BUTTON_5 = 16;
    const static int BUTTON_6 = 32;
    const static int BUTTON_X = 1;    // X is used on all popular consoles
    const static int BUTTON_Y = 2;    // Nintendo and Xbox use X, Y, A, B (in different positions)
    const static int BUTTON_A = 4;
    const static int BUTTON_B = 8;
    const static int BUTTON_LB = 16;    // Xbox uses LB, RB
    const static int BUTTON_RB = 32;
    const static int BUTTON_SQUARE = 2; // Playstation uses X, shapes, L1 and R1
    const static int BUTTON_CIRCLE = 4;
    const static int BUTTON_TRIANGLE = 8;
    const static int BUTTON_L1 = 16;
    const static int BUTTON_R1 = 32;
    const static int BUTTON_SELECT = 64;
    const static int BUTTON_START = 128;
    
    // Triggers (numbers overlap due to different platforms and similarity in function)
    const static int TRIGGER_LT = 1;    // Xbox triggers
    const static int TRIGGER_RT = 2;
    const static int TRIGGER_L2 = 1;    // Playstation triggers
    const static int TRIGGER_R2 = 2;
    
    // Keyboard keys
    const static int KEY_LEFT = 123;
    const static int KEY_RIGHT = 124;
    const static int KEY_DOWN = 125;
    const static int KEY_UP = 126;

    // Pointing devices (mouse, touch screen, etc.)
    virtual void pointerMove(int x, int y, float rx, float ry) = 0;
    virtual void pointerScroll(float rz) = 0;
    virtual void pointerClick(int button, int x, int y) = 0;
    virtual void pointerRelease(int button, int x, int y) = 0;

    // Motion tracking devices (accelerometer, gyroscope,
    virtual void motionMove(int x, int y, float rx, float ry) = 0;

    // Analog sticks
    virtual void analogMove(int stick, float rx, float ry) = 0;

    // Trigger buttons
    virtual void triggerPress(int button, float intensity) = 0;
    virtual void triggerRelease(int button, float intensity) = 0;

    // D-pads / HAT switches
    virtual void dpadPress(int direction) = 0;
    virtual void dpadRelease(int direction) = 0;

    // Joystick buttons
    virtual void buttonPress(int button) = 0;
    virtual void buttonRelease(int button) = 0;

    // Keyboard
    virtual void keyPress(int key) = 0;
    virtual void keyRelease(int key) = 0;
};

#endif /* InputEvent_h */
