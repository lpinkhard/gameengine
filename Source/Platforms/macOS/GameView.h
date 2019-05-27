//
//  GameView.h
//  GameEngine
//
//  Created by Lionel Pinkhard on 2019/01/29.
//

#ifndef GameView_h
#define GameView_h

#import <Cocoa/Cocoa.h>

@interface GameView : NSOpenGLView
{
}
- (instancetype) initWithFrame:(NSRect) frameRect pixelFormat:(NSOpenGLPixelFormat *) format;
- (void) prepareOpenGL;
- (void) drawRect: (NSRect) bounds;
- (void) mouseMoved:(NSEvent *) event;
- (void) mouseDown:(NSEvent *) event;
- (void) mouseUp:(NSEvent *) event;
- (void) rightMouseDown:(NSEvent *) event;
- (void) rightMouseUp:(NSEvent *) event;
- (void) keyDown:(NSEvent *) event;
- (void) keyUp:(NSEvent *) event;
- (BOOL) acceptsFirstResponder;

@end

#endif /* GameView_h */
