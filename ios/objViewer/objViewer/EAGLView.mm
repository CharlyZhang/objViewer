//
//  EAGLView.m
//  AppleCoder-OpenGLES-00
//
//  Created by Simon Maurice on 18/03/09.
//  Copyright Simon Maurice 2009. All rights reserved.
//


#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/EAGLDrawable.h>

#import "EAGLView.h"
#include "Application3D.h"

#define USE_DEPTH_BUFFER 1
#define SHOW_RENDER_TIME
//declare private methods, so they can be used everywhere in this file
@interface EAGLView (PrivateMethods)
- (void)createFramebuffer;
- (void)deleteFramebuffer;
@end

@implementation EAGLView
{
    Application3D app3d;
    NSArray *models;
}

@synthesize context;
@synthesize animationTimer;
@synthesize animationInterval;

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (!self) {
        return nil;
    }
    CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
    
    //we don't want a transparent surface
    eaglLayer.opaque = TRUE;
    
    //here we configure the properties of our canvas, most important is the color depth RGBA8 !
    eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                    [NSNumber numberWithBool:FALSE], kEAGLDrawablePropertyRetainedBacking,
                                    kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat,
                                    nil];
    
    //create an OpenGL ES 2 context
    context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    
    //if this failed or we cannot set the context for some reason, quit
    if (!context || ![EAGLContext setCurrentContext:context]) {
        NSLog(@"Could not create context!");
       // [self release];
        return nil;
    }

//    NSString *model0 = [[[NSBundle mainBundle]bundlePath] stringByAppendingPathComponent:@"obj/中提琴/zhongtiqin.obj"];
//    NSString *model1 = [[[NSBundle mainBundle]bundlePath] stringByAppendingPathComponent:@"obj/LL/xiaotiqing.obj"];
//    NSString *model2 = [[[NSBundle mainBundle]bundlePath] stringByAppendingPathComponent:@"obj/低音提琴/diyintiqing.obj"];
//    NSString *model3 = [[[NSBundle mainBundle]bundlePath] stringByAppendingPathComponent:@"obj/大提琴/datiqing.obj"];
    NSString *model0 = [[[NSBundle mainBundle]bundlePath] stringByAppendingPathComponent:@"obj/DaTiQin/DaTiQin.obj"];
    NSString *model1 = [[[NSBundle mainBundle]bundlePath] stringByAppendingPathComponent:@"obj/DiYinTiQin/DiYinTiqin.obj"];
    NSString *model2 = [[[NSBundle mainBundle]bundlePath] stringByAppendingPathComponent:@"obj/XiaoTiQin/XiaoTiQin.obj"];
    NSString *model3 = [[[NSBundle mainBundle]bundlePath] stringByAppendingPathComponent:@"obj/ZhongTiQin/ZhongTiQin.obj"];
    NSString *configPath = [[[NSBundle mainBundle]bundlePath]stringByAppendingPathComponent:@"scene_violin.cfg"];
    NSString *docPath = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0];
    
    models = [NSArray arrayWithObjects:model0, model1, model2, model3,nil];
    app3d.setGLSLDirectory([[[[NSBundle mainBundle]bundlePath] stringByAppendingString:@"/"] UTF8String]);
    app3d.init([configPath UTF8String]);
    app3d.setDocDirectory([docPath UTF8String]);
    app3d.loadObjModel([model0 UTF8String]);
    
    return self;
}// We have to implement this method


+ (Class)layerClass
{
    return [CAEAGLLayer class];
}
//our EAGLView is the view in our MainWindow which will be automatically loaded to be displayed.
//when the EAGLView gets loaded, it will be initialized by calling this method.


//on iOS, all rendering goes into a renderbuffer,
//which is then copied to the window by "presenting" it.
//here we create it!
- (void)createFramebuffer
{
    // Create the framebuffer and bind it so that future OpenGL ES framebuffer commands are directed to it.
    glGenFramebuffers(1, &viewFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, viewFramebuffer);
    
    // Create a color renderbuffer, allocate storage for it, and attach it to the framebuffer.
    glGenRenderbuffers(1, &viewRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, viewRenderbuffer);
    // Create the color renderbuffer and call the rendering context to allocate the storage on our Core Animation layer.
    // The width, height, and format of the renderbuffer storage are derived from the bounds and properties of the CAEAGLLayer object
    // at the moment the renderbufferStorage:fromDrawable: method is called.
    [context renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer*)self.layer];
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, viewRenderbuffer);
    
    // Retrieve the height and width of the color renderbuffer.
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &backingWidth);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &backingHeight);
    
    app3d.setRenderBufferSize(backingWidth, backingHeight);
    
    if (USE_DEPTH_BUFFER) {
        // Perform similar steps to create and attach a depth renderbuffer.
        glGenRenderbuffers(1, &depthRenderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, backingWidth, backingHeight);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);
    }
    
    glGenFramebuffers(1, &msaaFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, msaaFramebuffer);
    glGenRenderbuffers(1, &msaaRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, msaaRenderbuffer);
    glRenderbufferStorageMultisampleAPPLE(GL_RENDERBUFFER, 4, GL_RGBA8_OES, backingWidth, backingHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, msaaRenderbuffer);
    
    if (USE_DEPTH_BUFFER) {
        // Perform similar steps to create and attach a depth renderbuffer.
        glGenRenderbuffers(1, &msaaDepthbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, msaaDepthbuffer);
        glRenderbufferStorageMultisampleAPPLE(GL_RENDERBUFFER, 4, GL_DEPTH_COMPONENT16, backingWidth, backingHeight);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, msaaDepthbuffer);
    }
    
    // Test the framebuffer for completeness.
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        NSLog(@"failed to make complete framebuffer object %x", glCheckFramebufferStatus(GL_FRAMEBUFFER));
        return ;
    }
}

//deleting the framebuffer and all the buffers it contains
- (void)deleteFramebuffer
{
    //we need a valid and current context to access any OpenGL methods
    glDeleteFramebuffers(1, &viewFramebuffer);
    glDeleteFramebuffers(1, &msaaFramebuffer);
    viewFramebuffer = 0;
    msaaFramebuffer = 0;
    glDeleteRenderbuffers(1, &viewRenderbuffer);
    glDeleteRenderbuffers(1, &msaaRenderbuffer);
    viewRenderbuffer = 0;
    msaaRenderbuffer = 0;
    
    if(depthRenderbuffer) {
        glDeleteRenderbuffers(1, &depthRenderbuffer);
        glDeleteRenderbuffers(1, &msaaDepthbuffer);
        depthRenderbuffer = 0;
        msaaDepthbuffer = 0;
    }
}


//this is where all the magic happens!
- (void)drawFrame
{
    //we need a context for rendering
    if (context != nil)
    {
        //make it the current context for rendering
        [EAGLContext setCurrentContext:context];
        
        //if our framebuffers have not been created yet, do that now!
        glBindFramebuffer(GL_FRAMEBUFFER, msaaFramebuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, msaaRenderbuffer);
        app3d.frame();
        //we need a lesson to be able to render something
        
        //perform the actual drawing!
        
        //finally, get the color buffer we rendered to, and pass it to iOS
        //so it can display our awesome results!
        glBindFramebuffer(GL_READ_FRAMEBUFFER_APPLE, msaaFramebuffer);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER_APPLE, viewFramebuffer);
        glResolveMultisampleFramebufferAPPLE();
        
        glBindFramebuffer(GL_FRAMEBUFFER, viewFramebuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, viewRenderbuffer);
        
        // Assuming you allocated a color renderbuffer to point at a Core Animation layer, you present its contents by making it the current renderbuffer
        // and calling the presentRenderbuffer: method on your rendering context.
        [context presentRenderbuffer:GL_RENDERBUFFER];
        CZCheckGLError();
    }
    else
        NSLog(@"Context not set!");
}

//our render loop just tells the iOS device that we want to keep refreshing our view all the time
- (void)startRenderLoop
{
    /* [EAGLContext setCurrentContext:context];
     [self deleteFramebuffer];
     [self createFramebuffer];*/
    
#ifdef SHOW_RENDER_TIME
    animationTimer = [NSTimer scheduledTimerWithTimeInterval:animationInterval target:self selector:@selector(drawFrame) userInfo:nil repeats:YES];
#endif
}


//we have to be able to stop the render loop
- (void)stopRenderLoop
{
#ifdef SHOW_RENDER_TIME
    [animationTimer invalidate];
#endif
    
}

- (void)rotateWithX:(float)x Y:(float)y
{
    app3d.rotate(x, y);
    [self drawFrame];
}

- (void) moveWithX:(float)x Y:(float)y
{
    app3d.translate(x, y);
    [self drawFrame];
}

- (void) scale:(float)s
{
    app3d.scale(s);
    [self drawFrame];
}

// for debug
- (BOOL) loadModel:(NSUInteger) modelIdx
{
    if (modelIdx > 3) {
        NSLog(@"modelIdx is out of range");
        return NO;
    }
    
    BOOL result = NO;
    if (context != nil)
    {
        [EAGLContext setCurrentContext:context];
    
        result = app3d.loadObjModel([[models objectAtIndex:modelIdx] UTF8String]);
    }
    
    //[self drawFrame];
    return result;
}

- (void) setCameraPositionWithX:(float)x Y:(float)y Z:(float) z
{
    app3d.setCameraPosition(x, y, z);
    [self drawFrame];
}
- (void) setLigthDirectionWithX:(float)x Y:(float)y Z:(float) z
{
    app3d.setLigthDirection(x, y, z);
    [self drawFrame];
}
- (void) setAmbientColorWithX:(unsigned char)x Y:(unsigned char)y Z:(unsigned char) z
{
    app3d.setAmbientColor(x, y, z);
    [self drawFrame];
}
- (void) setDiffuseColorWithX:(unsigned char)x Y:(unsigned char)y Z:(unsigned char) z
{
    app3d.setDiffuseColor(x, y, z);
    [self drawFrame];
}

- (void)layoutSubviews
{
    [EAGLContext setCurrentContext:context];
    [self deleteFramebuffer];
    [self createFramebuffer];
    [self drawFrame];
}

//cleanup our view
- (void)dealloc
{
    [self deleteFramebuffer];
    //[context release];
    //[super dealloc];
}
/*
 // Only override drawRect: if you perform custom drawing.
 // An empty implementation adversely affects performance during animation.
 - (void)drawRect:(CGRect)rect
 {
 // Drawing code
 }
 */

@end

