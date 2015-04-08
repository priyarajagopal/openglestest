//
//  GameViewController.mm
//  testRenderLib
//
//  Created by Christian Utama on 7/4/15.
//  Copyright (c) 2015 Christian Utama. All rights reserved.
//

#import "GameViewController.h"
#import <OpenGLES/ES2/glext.h>
#import "viewer.h"

using namespace renderlib;

@interface GameViewController () {
    Viewer* _viewer;
}

@property (strong, nonatomic) EAGLContext *context;

@end

@implementation GameViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];

    if (!self.context) {
        NSLog(@"Failed to create ES context");
    }
    
    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
#if !(TARGET_IPHONE_SIMULATOR)
    view.drawableMultisample = GLKViewDrawableMultisample4X;
#endif
    [self setupGL];
}


-(void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    [self setupGL];
}

-(void)viewDidAppear:(BOOL)animated {
    [super viewDidAppear:animated];
    [self loadModel];
}

- (void)viewDidDisappear:(BOOL)animated
{
    [super viewDidDisappear:animated];
    [self tearDownGL];
    
    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];

    if ([self isViewLoaded] && ([[self view] window] == nil)) {
        self.view = nil;
        
        [self tearDownGL];
        
        if ([EAGLContext currentContext] == self.context) {
            [EAGLContext setCurrentContext:nil];
        }
        self.context = nil;
    }

    // Dispose of any resources that can be recreated.
}

#pragma mark - Helpers
- (void)setupGL
{
    [EAGLContext setCurrentContext:self.context];
    
    _viewer = new Viewer();
    _viewer->init();
     
}
     
- (void)loadModel
{
    _viewer->load_model([self.urlToLoad cStringUsingEncoding:NSASCIIStringEncoding]);
        
}


- (void)tearDownGL
{
    [EAGLContext setCurrentContext:self.context];
    delete _viewer;
 }

#pragma mark - GLKView and GLKViewController delegate methods

- (void)update
{
    _viewer->set_viewport(0, 0, self.view.bounds.size.width, self.view.bounds.size.height);
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    _viewer->draw();
}


@end
