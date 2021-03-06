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

#define MODEL_FROM_BUNDLE 0
#define MULTI_MODELS_FROM_BUNDLE 1

@interface ViewController ()<UIPickerViewDataSource,UIPickerViewDelegate,MBProgressHUDDelegate>
{
    EAGLView *glView;
    NSArray *modelName;
    NSArray *modelPath;
    NSUInteger selectedModel;
    UIView *displayView;
    
    MBProgressHUD *hud;
}

@property (weak, nonatomic) IBOutlet UILabel *lightDirectionX;
@property (weak, nonatomic) IBOutlet UILabel *lightDirectionY;
@property (weak, nonatomic) IBOutlet UILabel *lightDirectionZ;
@property (weak, nonatomic) IBOutlet UILabel *diffuseItensity;

@property (weak, nonatomic) IBOutlet UILabel *ambientItensity;
@property (weak, nonatomic) IBOutlet UIPickerView *pickView;

- (void)cleanTemp;
- (void)loadModels;

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    
    NSLog(@"%@",NSHomeDirectory());
    [self.view setUserInteractionEnabled:YES];
    
    // navigation bar
    UIBarButtonItem *cleanButton = [[UIBarButtonItem alloc]initWithTitle:@"清除缓存" style:UIBarButtonItemStylePlain target:self action:@selector(cleanTemp)];
    UIBarButtonItem *loadButton = [[UIBarButtonItem alloc]initWithTitle:@"载入模型" style:UIBarButtonItemStylePlain target:self action:@selector(loadModels)];
    self.navigationItem.leftBarButtonItem = cleanButton;
    self.navigationItem.rightBarButtonItem = loadButton;
    self.title = @"3D模型展示";
    
    
    [self.pickView setHidden:YES];
}

- (void)dealloc {
    [glView stopRenderLoop];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)cleanTemp
{
    // search Documents directory
    NSString *docPath = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0];
    
    BOOL res = YES;
    NSFileManager *fm = [NSFileManager defaultManager];
    NSArray *files = [[fm contentsOfDirectoryAtPath:docPath error:nil] pathsMatchingExtensions:[NSArray arrayWithObjects:@"b",nil]];
    for (NSString *filename in files)
    {
        NSString *filePath = [docPath stringByAppendingPathComponent:filename];
        res = [fm removeItemAtPath:filePath error:nil];
        if (!res)
        {
            NSLog(@"remove items failed");
            break;
        }
    }
    
    if (res) NSLog(@"temporary files removed successfully!");
}

- (void)loadModels
{
    // create model names and paths
    modelName = [[NSArray alloc]init];
    modelPath = [[NSArray alloc]init];
   
#if MODEL_FROM_BUNDLE
    
//    NSString *model0 = [[[NSBundle mainBundle]bundlePath] stringByAppendingPathComponent:@"obj/中提琴/zhongtiqin.obj"];
//    NSString *model1 = [[[NSBundle mainBundle]bundlePath] stringByAppendingPathComponent:@"obj/LL/xiaotiqing.obj"];
//    NSString *model2 = [[[NSBundle mainBundle]bundlePath] stringByAppendingPathComponent:@"obj/低音提琴/diyintiqing.obj"];
//    NSString *model3 = [[[NSBundle mainBundle]bundlePath] stringByAppendingPathComponent:@"obj/大提琴/datiqing.obj"];
//    NSString *model4 = [[[NSBundle mainBundle]bundlePath] stringByAppendingPathComponent:@"obj/DaTiQin/DaTiQin.obj"];
//    NSString *model5 = [[[NSBundle mainBundle]bundlePath] stringByAppendingPathComponent:@"obj/DiYinTiQin/DiYinTiqin.obj"];
//    NSString *model6 = [[[NSBundle mainBundle]bundlePath] stringByAppendingPathComponent:@"obj/XiaoTiQin/XiaoTiQin.obj"];
//    NSString *model7 = [[[NSBundle mainBundle]bundlePath] stringByAppendingPathComponent:@"obj/ZhongTiQin/ZhongTiQin.obj"];
//    NSString *model8 = [[[NSBundle mainBundle]bundlePath] stringByAppendingPathComponent:@"obj/11.obj"];
//    NSString *model9 = [[[NSBundle mainBundle]bundlePath] stringByAppendingPathComponent:@"obj/白塔1/baita.obj"];
//    NSString *model10 = [[[NSBundle mainBundle]bundlePath] stringByAppendingPathComponent:@"obj/银壶1/yinhu.obj"];
//    NSString *model11 = [[[NSBundle mainBundle]bundlePath] stringByAppendingPathComponent:@"obj/花灯1/huadeng.obj"];
//    NSString *model12 = [[[NSBundle mainBundle]bundlePath] stringByAppendingPathComponent:@"obj/教堂1/jiaotang.obj"];
    
//    modelName = [modelName arrayByAddingObject:@"教堂"];
//    modelPath = [modelPath arrayByAddingObject:model12];
//    modelName = [modelName arrayByAddingObject:@"白塔"];
//    modelPath = [modelPath arrayByAddingObject:model9];
//    modelName = [modelName arrayByAddingObject:@"银壶"];
//    modelPath = [modelPath arrayByAddingObject:model10];
    
    NSString *model13 = [[[NSBundle mainBundle]bundlePath] stringByAppendingPathComponent:@"obj/金龛塔1/jingheta.obj"];
    NSString *model14 = [[[NSBundle mainBundle]bundlePath] stringByAppendingPathComponent:@"obj/塔精模1/jingmota.obj"];
    NSString *model15 = [[[NSBundle mainBundle]bundlePath] stringByAppendingPathComponent:@"obj/金刚宝座塔1/baota.obj"];
    NSString *model16 = [[[NSBundle mainBundle]bundlePath] stringByAppendingPathComponent:@"obj/南禅寺1/ww.obj"];
    modelName = [modelName arrayByAddingObject:@"金龛塔"];
    modelPath = [modelPath arrayByAddingObject:model13];
    modelName = [modelName arrayByAddingObject:@"塔精模"];
    modelPath = [modelPath arrayByAddingObject:model14];
    modelName = [modelName arrayByAddingObject:@"金刚宝座塔"];
    modelPath = [modelPath arrayByAddingObject:model15];
    modelName = [modelName arrayByAddingObject:@"南禅寺"];
    modelPath = [modelPath arrayByAddingObject:model16];
    
#elif MULTI_MODELS_FROM_BUNDLE
    modelName = [modelName arrayByAddingObject:@"MX"];
    NSString *models = [[[NSBundle mainBundle]bundlePath] stringByAppendingPathComponent:@"obj/MX/"];
    modelPath = [modelPath arrayByAddingObject:models];
#else
    
    // search Documents directory
    NSString *docPath = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0];
    
    NSFileManager *fm = [NSFileManager defaultManager];
    NSArray *files = [fm contentsOfDirectoryAtPath:docPath error:nil];
    for (NSString* filename in files)
    {
        NSString *filePath = [docPath stringByAppendingPathComponent:filename];
        BOOL isDirectory;
        BOOL isExist = [fm fileExistsAtPath:filePath isDirectory:&isDirectory];
        if (isExist && isDirectory)
        {
            NSArray *modelFiles = [[fm contentsOfDirectoryAtPath:filePath error:nil] pathsMatchingExtensions:[NSArray arrayWithObjects:@"obj",nil]];
            if (modelFiles.count)
            {
                modelName = [modelName arrayByAddingObject:[[modelFiles lastObject]stringByDeletingPathExtension]];
                modelPath = [modelPath arrayByAddingObject:[filePath stringByAppendingPathComponent:[modelFiles lastObject]]];
            }
        }
    }
#endif
    
    [self.pickView setHidden:NO];
    [self.pickView reloadAllComponents];
    [self.navigationController setNavigationBarHidden:YES animated:YES];

    
    glView = [[EAGLView alloc]initWithFrame:self.view.bounds];
    [self.view insertSubview:glView atIndex:0];
    
    UIPanGestureRecognizer *rot = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(rotate:)];
    [rot setMinimumNumberOfTouches:1];
    [rot setMaximumNumberOfTouches:1];
    [glView addGestureRecognizer:rot];
    
    UIPanGestureRecognizer *mov = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(move:)];
    [mov setMinimumNumberOfTouches:2];
    [mov setMaximumNumberOfTouches:2];
    [glView addGestureRecognizer:mov];
    
    UIPinchGestureRecognizer *pinch = [[UIPinchGestureRecognizer alloc] initWithTarget:self action:@selector(scale:)];
    [glView addGestureRecognizer:pinch];
    
    UITapGestureRecognizer *tap = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(tap:)];
    tap.numberOfTouchesRequired = 2;
    tap.numberOfTapsRequired = 2;
    [glView addGestureRecognizer:tap];
    
    hud = [[MBProgressHUD alloc] initWithView:self.view];
    hud.delegate = self;
    [self.view addSubview:hud];
    
    hud.labelText = @"正在载入模型...";
    
    [glView startRenderLoop];
    
    [self loadModel:0];

}

#pragma mark -

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

- (void)tap:(UITapGestureRecognizer*) tap{
    if (tap.state == UIGestureRecognizerStateEnded) {
        [glView reset];
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

- (void)loadModel:(NSInteger) modelIdx
{
    if (modelIdx < 0 || modelIdx >= modelName.count) {
        NSLog(@"modelIdx is out of range");
        return;
    }
    
    selectedModel = modelIdx;
    
    [self.view setUserInteractionEnabled:NO];
    [glView stopRenderLoop];
    __block EAGLView *blockGlView = glView;
    [hud showAnimated:YES whileExecutingBlock:^{
#if MULTI_MODELS_FROM_BUNDLE
        NSString *path = [modelPath objectAtIndex:modelIdx];
        for (int i = 1; i <= 10; i ++) {
            [blockGlView loadModel:[NSString stringWithFormat:@"%@/%i/%i.obj",path,i,i]];
        }
#else
        [blockGlView loadModel:[modelPath objectAtIndex:modelIdx]];
#endif
    } completionBlock:^ {
        [self.view setUserInteractionEnabled:YES];
        [blockGlView drawFrame];
        [blockGlView startRenderLoop];
    }];
    
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
    NSLog(@"selected instruments %ld",(long)row);
    if (selectedModel == row) {
        NSLog(@"same model");
        return;
    }
    [self loadModel:row];
}
@end
