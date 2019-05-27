//
//  ViewController.h
//  GameEngine iOS
//
//  Created by Lionel Pinkhard on 2019/01/29.
//

#import <UIKit/UIKit.h>

@interface ViewController : UIViewController
{
    IBOutlet UISlider *slider;
    IBOutlet UITabBar *tabBar;
}

@property (nonatomic, retain) UISlider *slider;
@property (nonatomic, retain) UITabBar *tabBar;

- (IBAction)sliderAction:(id)sender;

@end
