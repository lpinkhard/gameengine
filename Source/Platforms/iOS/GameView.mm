//
//  GameView.m
//  GameEngine iOS
//
//  Created by Lionel Pinkhard on 2019/01/29.
//

#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/EAGLDrawable.h>
#import "GameView.h"

#include "Game.hpp"

// A class extension to declare private methods
@interface GameView ()
{
    Game game;
}

@property (nonatomic, retain) EAGLContext *context;

@end


@implementation GameView

@synthesize context;
@synthesize slider;
@synthesize tabBar;


// You must implement this method
+ (Class)layerClass
{
    return [CAEAGLLayer class];
}


// The GL view is stored in the nib file. When it's unarchived it's sent -initWithCoder:
- (id)initWithCoder:(NSCoder*)coder
{
    if ((self = [super initWithCoder:coder]))
    {
        // Get the layer
        CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
        eaglLayer.opaque = YES;
        eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                        [NSNumber numberWithBool:NO], kEAGLDrawablePropertyRetainedBacking,
                                        kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat,
                                        nil];
        
        context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
        if (!context || ![EAGLContext setCurrentContext:context])
        {
            return nil;
        }
        
        // Create system framebuffer object. The backing will be allocated in -reshapeFramebuffer
        glGenFramebuffersOES(1, &viewFramebuffer);
        glGenRenderbuffersOES(1, &viewRenderbuffer);
        glBindFramebufferOES(GL_FRAMEBUFFER_OES, viewFramebuffer);
        glBindRenderbufferOES(GL_RENDERBUFFER_OES, viewRenderbuffer);
        glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, viewRenderbuffer);
        
        // Perform additional one-time GL initialization
        game.init();
    }
    return self;
}


- (void)drawView
{
    int mode = (int)self.tabBar.selectedItem.tag;
    float value = self.slider.value;
    
    // This application only creates a single GL context, so it is already current here.
    // If there are multiple contexts, ensure the correct one is current before drawing.
    game.redraw();
    
    // This application only creates a single (color) renderbuffer, so it is already bound here.
    // If there are multiple renderbuffers (for example color and depth), ensure the correct one is bound here.
    [context presentRenderbuffer:GL_RENDERBUFFER_OES];
}


- (void)reshapeFramebuffer
{
    // Allocate GL color buffer backing, matching the current layer size
    [context renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:(CAEAGLLayer*)self.layer];
    
    glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_WIDTH_OES, &backingWidth);
    glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_HEIGHT_OES, &backingHeight);
    
    game.getHAL()->reshape(backingWidth, backingHeight, self.contentScaleFactor);
}


- (void)layoutSubviews
{
    [self reshapeFramebuffer];
    [self drawView];
}


- (void)dealloc
{
    if ([EAGLContext currentContext] == context)
        [EAGLContext setCurrentContext:nil];
    
    self.context = nil;
    self.slider = nil;
    self.tabBar = nil;
}

- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    UITouch *touch = [[event allTouches] anyObject];
    CGPoint touchLocation = [touch locationInView:self];
    game.getHAL()->eventPointerClick(InputEvent::BUTTON_PRIMARY, touchLocation.x, touchLocation.y);
}

- (void)touchesMoved:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    UITouch *touch = [[event allTouches] anyObject];
    CGPoint touchLocation = [touch locationInView:self];
    game.getHAL()->eventPointerMove(touchLocation.x, touchLocation.y);
}

- (void)touchesEnded:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    UITouch *touch = [[event allTouches] anyObject];
    CGPoint touchLocation = [touch locationInView:self];
    game.getHAL()->eventPointerRelease(InputEvent::BUTTON_PRIMARY, touchLocation.x, touchLocation.y);
}

- (void)touchesCancelled:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    UITouch *touch = [[event allTouches] anyObject];
    CGPoint touchLocation = [touch locationInView:self];
    game.getHAL()->eventPointerRelease(InputEvent::BUTTON_PRIMARY, touchLocation.x, touchLocation.y);
}

@end
