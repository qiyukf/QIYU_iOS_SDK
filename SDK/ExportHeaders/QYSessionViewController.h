//
//  QYSessionViewController.h
//  QYSDK
//
//  Created by towik on 12/21/15.
//  Copyright (c) 2015 Netease. All rights reserved.
//
#import <UIKit/UIKit.h>

@class QYSource;

/**
 *  客服会话ViewController,必须嵌入到UINavigationcontroller中
 */
@interface QYSessionViewController : UIViewController

/**
 *  会话窗口标题
 */
@property (nonatomic,copy)      NSString    *sessionTitle;

/**
 *  会话窗口来源
 */
@property (nonatomic,strong)    QYSource   *source;
@end
