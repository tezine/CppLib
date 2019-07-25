#ifdef Q_OS_IOS
#include <UIKit/UIKit.h>
#include "pios.h"

@interface PIOSInternal :NSObject{
}
-(int)shareTextInternal:(NSString *) textToShare;
@end

@implementation PIOSInternal
//implementacao da interface abaixo


void PIOS::shareText(const QString &txt){
    NSString*  textStr = [[NSString alloc] initWithUTF8String:txt.toUtf8().data()];
    void *self = [[PIOSInternal alloc] init];
    [(id) self shareTextInternal:textStr];
}

-(int)shareTextInternal:(NSString *) textToShare{
        /*UIActivityViewController *activityVC = [[UIActivityViewController alloc] initWithActivityItems:textToShare applicationActivities:nil];
        activityVC.excludedActivityTypes = @[UIActivityTypePrint, UIActivityTypeCopyToPasteboard, UIActivityTypeAssignToContact, UIActivityTypeSaveToCameraRoll]; //Exclude whichever aren't relevant
        [self presentViewController:activityVC animated:YES completion:nil];*/


     //NSString *shareText = @"This is my text I want to share.";
    /*NSString *shareURL = @"...";
    NSArray *itemsToShare = @[textToShare, shareURL];
    UIActivityViewController *activityVC = [[UIActivityViewController alloc] initWithActivityItems:itemsToShare applicationActivities:nil];
    [self presentViewController:activityVC animated:YES completion:nil];*/

    NSString *shareText = @"This is my text I want to share.";
    NSString *shareURL = @"...";
    NSArray *itemsToShare = @[textToShare, shareURL];
    UIViewController *qtController = [[UIApplication sharedApplication].keyWindow rootViewController];
    UIActivityViewController *activityController = [[UIActivityViewController alloc] initWithActivityItems:itemsToShare applicationActivities:nil];
    if ( [activityController respondsToSelector:@selector(popoverPresentationController)] ) { // iOS8
     activityController.popoverPresentationController.sourceView = qtController.view;
     }
    [qtController presentViewController:activityController animated:YES completion:nil];
    return 0;
}
@end
#endif
