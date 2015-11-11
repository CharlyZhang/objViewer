//
//  ViewController.m
//  objViewer
//
//  Created by CharlyZhang on 15/10/23.
//  Copyright (c) 2015年 Founder. All rights reserved.
//

#import "ViewController.h"
#import "EAGLView.h"
#include "MBProgressHUD/MBProgressHUD.h"

@interface ViewController ()<UIPickerViewDataSource,UIPickerViewDelegate,MBProgressHUDDelegate>
{
    EAGLView *glView;
    NSArray *modelName;
    NSUInteger selectedModel;
    
    MBProgressHUD *hud;
}

@property (weak, nonatomic) IBOutlet UILabel *lightDirectionX;
@property (weak, nonatomic) IBOutlet UILabel *lightDirectionY;
@property (weak, nonatomic) IBOutlet UILabel *lightDirectionZ;
@property (weak, nonatomic) IBOutlet UILabel *diffuseItensity;

@property (weak, nonatomic) IBOutlet UILabel *ambientItensity;
@property (weak, nonatomic) IBOutlet UIPickerView *pickView;
@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    glView = [[EAGLView alloc]initWithFrame:self.view.bounds];
    [self.view insertSubview:glView atIndex:0];
    modelName = [NSArray arrayWithObjects:@"大提琴",@"低音提琴",@"小提琴",@"中提琴",nil];
    selectedModel = 0;
    
    [self.view setUserInteractionEnabled:YES];
    
    UIPanGestureRecognizer *rot = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(rotate:)];
    [rot setMinimumNumberOfTouches:1];
    [rot setMaximumNumberOfTouches:1];
    [self.view addGestureRecognizer:rot];
    
    UIPanGestureRecognizer *mov = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(move:)];
    [mov setMinimumNumberOfTouches:2];
    [mov setMaximumNumberOfTouches:2];
    [self.view addGestureRecognizer:mov];
    
    UIPinchGestureRecognizer *pinch = [[UIPinchGestureRecognizer alloc] initWithTarget:self action:@selector(scale:)];
    [self.view addGestureRecognizer:pinch];
    
    hud = [[MBProgressHUD alloc] initWithView:self.view];
    hud.delegate = self;
    [self.view addSubview:hud];

    hud.labelText = @"正在载入模型...";
}

- (void)dealloc {
   // [glView stopRenderLoop];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

-(void)rotate:(id)sender {
    NSLog(@"rotate");
    
    static CGPoint lastPoint;
    
    CGPoint point = [(UIPanGestureRecognizer*)sender translationInView:self.view];
    if([(UIPanGestureRecognizer*)sender state] == UIGestureRecognizerStateChanged) {
        [glView rotateWithX:point.x - lastPoint.x Y:-point.y + lastPoint.y];
    }
    
    lastPoint = point;
}

-(void)move:(id)sender {
    NSLog(@"move");
    
    static CGPoint lastPoint;
    
    CGPoint point = [(UIPanGestureRecognizer*)sender translationInView:self.view];
    if([(UIPanGestureRecognizer*)sender state] == UIGestureRecognizerStateChanged) {
        [glView moveWithX:(-point.x + lastPoint.x)/5 Y:(point.y - lastPoint.y)/5];
    }
    
    lastPoint = point;
}

- (void)scale:(UIPinchGestureRecognizer*)pinch {
    NSLog(@"scale");
    if (pinch.state == UIGestureRecognizerStateChanged) {
        [glView scale:pinch.scale];
        pinch.scale = 1;
    }
}

# pragma mark - Actions

- (IBAction)changeLightDirection:(UISlider *)sender {
    float x = [self.lightDirectionX.text floatValue];
    float y = [self.lightDirectionY.text floatValue];
    float z = [self.lightDirectionZ.text floatValue];
    NSLog(@"light diretion is (%0.3f,%0.3f,%0.3f)",x,y,z);
    switch (sender.tag) {
        case 0:
            self.lightDirectionX.text = [NSString stringWithFormat:@"%0.3f",sender.value ];
            break;
        case 1:
            self.lightDirectionY.text = [NSString stringWithFormat:@"%0.3f",sender.value ];
            break;
        case 2:
            self.lightDirectionZ.text = [NSString stringWithFormat:@"%0.3f",sender.value ];
            break;
            
        default:
            break;
    }
    
    [glView setLigthDirectionWithX:x Y:y Z:z];
}
- (IBAction)changeLightColor:(UISlider *)sender {
    unsigned char v = (unsigned char)(sender.value*255);
    NSLog(@"lightColor intensity = %d",(int)v);
    switch (sender.tag) {
        case 0:
            self.diffuseItensity.text = [NSString stringWithFormat:@"%0.3f",sender.value];
            [glView setDiffuseColorWithX:v Y:v Z:v];
            break;
        case 1:
            self.ambientItensity.text = [NSString stringWithFormat:@"%0.3f",sender.value];
            [glView setAmbientColorWithX:v Y:v Z:v];
            break;
        default:
            break;
    }
}

# pragma mark - Picker View
// returns the number of 'columns' to display.
- (NSInteger)numberOfComponentsInPickerView:(UIPickerView *)pickerView
{
    return 1;
}

// returns the # of rows in each component..
- (NSInteger)pickerView:(UIPickerView *)pickerView numberOfRowsInComponent:(NSInteger)component
{
    return [modelName count];
}

-(NSString*) pickerView:(UIPickerView *)pickerView titleForRow:(NSInteger)row forComponent:(NSInteger)component{
    return [modelName objectAtIndex:row];
}

- (void)pickerView:(UIPickerView *)pickerView didSelectRow:(NSInteger)row inComponent:(NSInteger)component {
    NSLog(@"selected instruments %d",row);
    if (selectedModel == row) {
        NSLog(@"same model");
        return;
    }
    selectedModel = row;
    [self.view setUserInteractionEnabled:NO];
    __block EAGLView *blockGlView = glView;
    [hud showAnimated:YES whileExecutingBlock:^{
        [blockGlView loadModel:row];
    } completionBlock:^ {
        [self.view setUserInteractionEnabled:YES];
        [blockGlView drawFrame];
    }];
}
@end
