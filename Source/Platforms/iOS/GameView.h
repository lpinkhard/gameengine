//
//  GameView.h
//  GameEngine
//
//  Created by Lionel Pinkhard on 2019/01/29.
//

#ifndef GameView_h
#define GameView_h

#import <UIKit/UIKit.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>

enum {
    APPLE_texture_2D_limited_npot,
    IMG_texture_format_BGRA8888,
    NUM_EXTENSIONS
};

@interface GameView : UIView {
    
@private
    // The pixel dimensions of the backbuffer
    GLint backingWidth;
    GLint backingHeight;
    
    EAGLContext *context;
    IBOutlet UISlider *slider;
    IBOutlet UITabBar *tabBar;
    
    // OpenGL names for the renderbuffer and framebuffer used to render to this view
    GLuint viewRenderbuffer, viewFramebuffer;
}

@property (nonatomic, retain) UISlider *slider;
@property (nonatomic, retain) UITabBar *tabBar;

- (void)drawView;
- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event;
- (void)touchesMoved:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event;
- (void)touchesEnded:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event;
- (void)touchesCancelled:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event;

@end

#endif /* GameView_h */
