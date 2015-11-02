//
//  ViewController.m
//  objViewer
//
//  Created by CharlyZhang on 15/10/23.
//  Copyright (c) 2015年 Founder. All rights reserved.
//

#import "ViewController.h"
#import "EAGLView.h"

@interface ViewController ()
{
    EAGLView *glView;
}
@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    glView = [[EAGLView alloc]initWithFrame:self.view.bounds];
    [self.view addSubview:glView];
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(willResignActive)
                                                 name:UIApplicationWillResignActiveNotification
                                               object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(didBecomeActive)
                                                 name:UIApplicationDidBecomeActiveNotification
                                               object:nil];
    
    UIPanGestureRecognizer *rot = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(rotate:)];
    [self.view setUserInteractionEnabled:YES];
    [rot setMinimumNumberOfTouches:1];
    [rot setMaximumNumberOfTouches:1];
    [self.view addGestureRecognizer:rot];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)willResignActive{
    [glView stopRenderLoop];
}

-(void)didBecomeActive{
    [glView startRenderLoop];
}

-(void)rotate:(id)sender {
    
    static CGPoint lastPoint;
    
    CGPoint point = [(UIPanGestureRecognizer*)sender translationInView:self.view];
    if([(UIPanGestureRecognizer*)sender state] == UIGestureRecognizerStateChanged) {
        [glView rotateWithX:point.x - lastPoint.x Y:point.y - lastPoint.y];
    }
    
    lastPoint = point;
}

@end
