//
//  GameView.m
//  GameEngine macOS
//
//  Created by Lionel Pinkhard on 2019/01/29.
//

#import "GameView.h"
#include <OpenGL/gl.h>

#include "../../Game/Game.hpp"

@interface GameView()
{
    Game gameInstance;
}
@end

@implementation GameView

- (instancetype) initWithFrame:(NSRect) frameRect pixelFormat:(NSOpenGLPixelFormat *) format
{
    self = [super initWithFrame:frameRect pixelFormat: format];
    [self setWantsBestResolutionOpenGLSurface:NO];
    [[self window] setAcceptsMouseMovedEvents:YES];
    return self;
}

- (void)prepareOpenGL
{
    [super prepareOpenGL];

    gameInstance.init();
    
    gameInstance.getHAL()->reshape(self.bounds.size.width, self.bounds.size.height, 1);
}

- (void)reshape
{
    gameInstance.getHAL()->reshape(self.bounds.size.width, self.bounds.size.height, 1);
}

-(void) drawRect: (NSRect) bounds
{
    gameInstance.redraw();
}

- (void) mouseMoved:(NSEvent *) event
{
    NSPoint point = [self convertPoint:[event locationInWindow] fromView:nil];
    gameInstance.getHAL()->eventPointerMove(point.x, point.y);
}

- (void) mouseDown:(NSEvent *) event
{
    NSPoint point = [self convertPoint:[event locationInWindow] fromView:nil];
    gameInstance.getHAL()->eventPointerClick(InputEvent::BUTTON_PRIMARY, point.x, point.y);
}

- (void) mouseUp:(NSEvent *) event
{
    NSPoint point = [self convertPoint:[event locationInWindow] fromView:nil];
    gameInstance.getHAL()->eventPointerRelease(InputEvent::BUTTON_PRIMARY, point.x, point.y);
}

- (void) rightMouseDown:(NSEvent *) event
{
    NSPoint point = [self convertPoint:[event locationInWindow] fromView:nil];
    gameInstance.getHAL()->eventPointerClick(InputEvent::BUTTON_SECONDARY, point.x, point.y);
}

- (void) rightMouseUp:(NSEvent *) event
{
    NSPoint point = [self convertPoint:[event locationInWindow] fromView:nil];
    gameInstance.getHAL()->eventPointerRelease(InputEvent::BUTTON_SECONDARY, point.x, point.y);
}

- (void) keyDown:(NSEvent *) event
{
    if ([event isARepeat] == NO) {
        gameInstance.getHAL()->eventKeyDown([event keyCode]);
    }
}

- (void) keyUp:(NSEvent *) event
{
    gameInstance.getHAL()->eventKeyUp([event keyCode]);
}

- (BOOL)acceptsFirstResponder
{
    return YES;
}

@end
