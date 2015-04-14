//
//  INVViewController.m
//  testRenderLib
//
//  Created by Priya Rajagopal on 4/8/15.
//  Copyright (c) 2015 Christian Utama. All rights reserved.
//

#import "INVViewController.h"
#import "GameViewController.h"
@interface INVViewController ()

@property (weak, nonatomic) IBOutlet UITextField *urlTextField;
- (IBAction)onEditDone:(id)sender;
- (IBAction)onLoadRequested:(id)sender;


@end

@implementation INVViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
    if ([segue.identifier isEqualToString:@"ShowViewerSegue"]) {
        
        GameViewController* gvc = segue.destinationViewController;
        gvc.urlToLoad = self.urlTextField.text;
    }
}


#pragma mark - IBActions

- (IBAction)onLoadRequested:(id)sender {
    [self performSegueWithIdentifier:@"ShowViewerSegue" sender:self];
}
@end
