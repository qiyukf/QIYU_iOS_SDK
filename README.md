# 网易七鱼 iOS SDK 开发指南

## 简介

网易七鱼 iOS SDK 是一个 iOS 端客服系统访客解决方案，既包含了客服聊天逻辑管理，也提供了聊天界面，开发者可方便的将客服功能集成到自己的 APP 中。iOS SDK 支持 iOS 7 以上版本。

## 添加SDK
* 下载 QY SDK，得到一个 .a 文件、 QYResouce 文件夹和 ExportHeaders 文件夹，将他们导入工程
* 添加 QY SDK 依赖库

* MobileCoreService.framework
* SystemConfiguration.framework
* AVFoundation.framwork
* CoreTelephony.framework
* CoreMedia.framework
* AudioToolbox.framework
* libz.lib
* libstdc++.6.0.9.dylib
* libsqlite3.0.dylib

* 在 Other Linker Flags 中添加 -ObjC 
* 如果是用XCode7开发程序，需要在Info.plist中加入以下代码：

```
<key>NSAppTransportSecurity</key>
<dict>
<key>NSAllowsArbitraryLoads</key>
<true/>
</dict>
```

由于SDK与服务器之间有部分请求使用的是http，如果不加此代码，将无法进行http请求。

* 在需要使用 SDK 的地方 import "QYSDK.h"

**由于 SDK 是静态库，且为了方便开发者使用，我们将 armv7 i386 x86_64 arm64 平台的静态库合并成一个 Fat Library ，导致整个 SDK 比较大。但实际编译后大约只会增加 app 4-5M 大小**


### SDK类讲解

QY SDK 主要提供了如下类(协议)与方法

* QYSDK 整个SDK的主入口，单例；主要提供SDK初始化、追踪用户浏览信息、注销当前账号、获取客服聊天ViewController、获取会话管理类功能。

### SDK初始化

* 在需要使用的 SDK 的地方导入头文件 QYSDK.h
* 添加 SDK 初始化方法

```objc
-(BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {

[[QYSDK sharedSDK] registerAppId:AppKey
appName:App名称];
return YES;
}
```
appName(就是SDK 1.0.0版本的cerName,参数名变了) 对应管理后台添加一个app时填写的App名称

### 登出

调用

```objc
[[QYSDK sharedSDK] logout:^(){}];
```

应用层登出/注销自己的账号时需要调用 SDK 的登出操作，该操作会通知服务器进行 APNS 推送信息的解绑操作，避免用户已登出但推送依然发送到当前设备的情况发生。

### 使用客服聊天ViewController

获取

```objc
[[QYSDK sharedSDK] sessionViewController];
```

应用层获取此 ViewController 之后，必须嵌入到 UINavigationcontroller 中，就可以获得会话窗口的UI以及所有功能。 sessionViewController 只会使用到导航栏的 self.navigationItem.title 和 self.navigationItem.rightBarButtonItem 。 self.navigationItem.title 放置标题栏； self.navigationItem.rightBarButtonItem 放置"人工客服"、“评价”入口。

如果调用代码所在的viewController在UINavigationcontroller中，可以如下方式集成（第一种集成方式）：

```objc
QYSource *source = [[QYSource alloc] init];
source.title =  @"七鱼金融";
source.urlString = @"https://8.163.com/";

QYSessionViewController *session = 
[[QYSDK sharedSDK] sessionViewController];
session.sessionTitle = @"七鱼金融";
session.source = source;
session.hidesBottomBarWhenPushed = YES;
[self.navigationController pushViewController:session animated:YES];
```

如果调用代码所在的viewController不在UINavigationcontroller中，可如下方式集成（第二种集成方式）：

```objc
QYSource *source = [[QYSource alloc] init];
source.title =  @"七鱼金融";
source.urlString = @"https://8.163.com/";

QYSessionViewController *session = 
[[QYSDK sharedSDK] sessionViewController];
session.sessionTitle = @"七鱼金融";
session.source = source;
session.hidesBottomBarWhenPushed = YES;

UINavigationController *nav = 
[[UINavigationController alloc] initWithRootViewController:vc];
[self presentViewController:nav animated:YES completion:nil];
```

如果您的代码要求所有viewController继承某个公共基类，并且公共基类对UINavigationController统一做了某些处理；或者对UINavigationController做了自己的扩展，并且这会导致集成之后有某些问题；或者其他原因导致使用第一种方式集成会有问题；这些情况下，建议您使用第二种方式集成。

### 获取会话管理类

调用

```objc
[[QYSDK sharedSDK] conversationManager];
```

返回的是一个协议QYConversationManager；可通过此协议获得消息未读数以及设置会话委托。

### 追踪用户浏览信息

用户在使用你的 APP 时，SDK 提供了接口用于跟踪其浏览路径，用于后期用户行为分析以及主动营销。接口为：

```objc
[[QYSDK sharedSDK] trackHistory：urlString withAttributes ： attributes];
```

### 设置用户信息

```objc
[[QYSDK sharedSDK] setUserInfo：userInfo];
```
userInfo: 字段“id”表示用户id，字段“data”表示用户信息，具体请看官网轻量CRM相关文档:
<a>http://qiyukf.com/doc/sdk/_book/qiyu_crm_interface.html</a>

### 获取自定义UI类

```objc
[[QYSDK sharedSDK] customUIConfig];
```
QYCustomUIConfig是负责自定义UI的类；目前主要是定义会话相关的字体颜色、大小、头像图片等。

### 更换图片素材

QYCustomUIConfig只是负责替换部分皮肤相关内容，不包含所有的图片素材的替换。iOS SDK支持所有图片素材替换，只需要新建QYCustomResource.bundle，在其中放置跟QYResource.bundle中同名的图片素材，即可实现替换。为了效果好，应该放置同等尺寸的图片。

## APNS推送
* [制作推送证书并在管理后台配置](./iOS_apns.html "target=_blank")
* 客户端配置推送证书名，推送证书名就是App名称

```objc
-(BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
[[QYSDK sharedSDK] registerAppId:AppKey
appName:App名称];
return YES;
}
```

* 客户端注册 APNS，并在获取到 APNS Token 时将值传给 SDK

```objc
- (void)registerAPNs
{
if ([[UIApplication sharedApplication] respondsToSelector:@selector(registerForRemoteNotifications)])
{
UIUserNotificationType types = UIRemoteNotificationTypeBadge | UIRemoteNotificationTypeSound | 		UIRemoteNotificationTypeAlert;
UIUserNotificationSettings *settings = [UIUserNotificationSettings settingsForTypes:types
categories:nil];
[[UIApplication sharedApplication] registerUserNotificationSettings:settings];
[[UIApplication sharedApplication] registerForRemoteNotifications];
}
else
{
UIRemoteNotificationType types = UIRemoteNotificationTypeAlert | UIRemoteNotificationTypeSound | 		UIRemoteNotificationTypeBadge;
[[UIApplication sharedApplication] registerForRemoteNotificationTypes:types];
}
}
```

```objc
- (void)application:(UIApplication *)app didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken
{
[[QYSDK sharedSDK] updateApnsToken:deviceToken];
}
```

## 补充说明

如果您看完此文档后，还有任何集成方面的疑问，建议您可以参考下 iOS SDK Demo 源码，在官网即可下载得到。源码充分的展示了 iOS SDK 的能力，并且为集成 iOS SDK 提供了样例代码。




