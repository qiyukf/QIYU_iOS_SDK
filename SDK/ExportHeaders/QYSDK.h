//
//  QYSDK.h
//  QYSDK
//
//  version 5.7.3
//
//  Created by towik on 12/21/15.
//  Copyright (c) 2017 Netease. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "QYHeaders.h"

/**
 *  完成回调
 */
typedef void(^QYCompletionBlock)(BOOL success);

/**
 *  完成回调，带错误参数
 */
typedef void(^QYResultCompletionBlock)(BOOL success, NSError *error);

/**
 *  完成结果回调
 */
typedef void(^QYCompletionWithResultBlock)(BOOL isSuccess);

/**
 *  推送消息回调
 */
typedef void(^QYPushMessageBlock)(QYPushMessage *pushMessage);

/**
 *  清理缓存回调
 */
typedef void(^QYCleanCacheCompletion)(NSError *error);

/**
 *  错误码
 */
typedef NS_ENUM(NSInteger, QYLocalErrorCode) {
    QYLocalErrorCodeUnknown         = 0,    //未知错误
    QYLocalErrorCodeInvalidParam    = 1,    //错误参数
    QYLocalErrorCodeFusionNeeded    = 2,    //必须为融合SDK
    QYLocalErrorCodeAccountNeeded   = 3,    //帐号错误-底层通信IM帐号未登录
    QYLocalErrorCodeInvalidUserId   = 4,    //userId错误，应与帐号相同
    QYLocalErrorCodeNeedLogout      = 5,    //userId变化，应走帐号切换逻辑，先调用logout
};


/**
 *  QYSDK：单例模式，所有SDK接口都从此类获得
 */
@interface QYSDK : NSObject

/**
 *  返回 SDK 单例
 *
 *  @return SDK 单例
 */
+ (instancetype)sharedSDK;

/**
 *  注册SDK
 *
 *  @param appKey  对应管理后台分配的appkey
 *  @param appName  对应管理后台添加一个App时填写的App名称 (就是SDK1.0.0版本的cerName,参数名变动)
 */
- (void)registerAppId:(NSString *)appKey appName:(NSString *)appName;

/**
 *  返回AppKey
 *
 *  @return appKey
 */
- (NSString *)appKey;

/**
 *  返回客服聊天ViewController，必须嵌入至UINavigationController中
 *  须保证全局只有一个sessionViewController实例，退出后能够正确释放实例
 *
 *  @return 会话ViewController
 */
- (QYSessionViewController *)sessionViewController;

/**
 *  返回自定义UI类,通过此类可以设置sessionViewController内的界面效果
 *
 *  @return 自定义UI类
 */
- (QYCustomUIConfig *)customUIConfig;

/**
 *  返回自定义事件处理类,通过此类可以设置sessionViewController内的事件处理
 *
 *  @return 自定义事件处理类
 */
- (QYCustomActionConfig *)customActionConfig;

/**
 *  返回会话管理类
 *
 *  @return 会话管理类
 */
- (QYConversationManager *)conversationManager;

/**
 *  获取当前设置的用户信息ID
 *
 *  @return 返回当前已设置的用户信息ID
 */
- (NSString *)currentUserID;

/**
 *  获取当前登录的底层IM通信帐号ID，可据此判断当前是否已登录IM帐号
 *
 *  @return 返回当前登录的accountID
 *  @discussion 此帐号与外部设置的userId不同，是底层长连接通信帐号，七鱼内部会自动创建并登录；与外部传入的userId形成映射关系
 */
- (NSString *)currentIMAccountID;

/**
 *  设置用户信息，App帐号登录成功后上传
 *
 *  @param userInfo 用户信息
 *  @discussion 此方法尽量在App帐号登录成功后调用，不应仅在进入客服界面时调用；否则可能会造成客服连接状态不稳定
 *  @discussion 若设置的userId与上次设置不同，即需要实现帐号切换，应先调用logout还原为匿名帐号再进行设置
 */
- (void)setUserInfo:(QYUserInfo *)userInfo;

/**
 *  设置用户信息，App帐号登录成功后上传，带结果回调
 *
 *  @param userInfo 用户信息
 *  @param userInfoBlock userInfo上报结果回调
 *  @discussion 此方法尽量在App帐号登录成功后调用，不应仅在进入客服界面时调用；否则可能会造成客服连接状态不稳定
 *  @discussion 若设置的userId与上次设置不同，即需要实现帐号切换，应先调用logout还原为匿名帐号再进行设置
 */
- (void)setUserInfo:(QYUserInfo *)userInfo userInfoResultBlock:(QYResultCompletionBlock)userInfoBlock;

/**
 *  注销当前帐号，App帐号注销时调用
 *
 *  @param completion 完成回调
 *  @discussion 切换帐号包含注销和登录过程，在注销阶段也要调用此函数
 */
- (void)logout:(QYCompletionBlock)completion;

/**
 *  设置待校验的authToken
 */
- (void)setAuthToken:(NSString *)authToken;

/**
 *  设置用户信息，App帐号登录成功后上传，带authToken校验
 *
 *  @param userInfo 用户信息
 *  @param block authToken校验结果回调
 */
- (void)setUserInfo:(QYUserInfo *)userInfo authTokenVerificationResultBlock:(QYCompletionWithResultBlock)block;

/**
 *  设置用户信息，App帐号登录成功后上传，带authToken校验，带结果回调
 *
 *  @param userInfo 用户信息
 *  @param userInfoBlock userInfo上报结果回调
 *  @param authTokenBlock authToken校验结果回调
 *  @discussion 此方法尽量在App帐号登录成功后调用，不应仅在进入客服界面时调用；否则可能会造成客服连接状态不稳定
 *  @discussion 若设置的userId与上次设置不同，即需要实现帐号切换，应先调用logout还原为匿名帐号再进行设置
 */
- (void)setUserInfo:(QYUserInfo *)userInfo userInfoResultBlock:(QYResultCompletionBlock)userInfoBlock authTokenResultBlock:(QYCompletionBlock)authTokenBlock;

/**
 *  更新推送token
 *
 *  @param token 推送token
 */
- (void)updateApnsToken:(NSData *)token;

/**
 *  获取推送消息
 *
 *  @param messageId 消息id
 */
- (void)getPushMessage:(NSString *)messageId;

/**
 *  注册推送消息通知回调
 *
 *  @param block 收到消息的回调
 */
- (void)registerPushMessageNotification:(QYPushMessageBlock)block;

/**
 *  访问轨迹
 *  @param title 标题
 *  @param enterOrOut 进入还是退出
 */
- (void)trackHistory:(NSString *)title enterOrOut:(BOOL)enterOrOut key:(NSString *)key;

/**
 *  行为轨迹
 *  @param title 标题
 *  @param description 具体信息，以key-value表示信息对，例如key为“商品价格”，value为“999”
 */
- (void)trackHistory:(NSString *)title description:(NSDictionary *)description key:(NSString *)key;

/**
 *  获取七鱼日志文件路径
 *
 *  @return 日志文件路径
 */
- (NSString *)qiyuLogPath;

/**
 *  清理接收文件缓存
 *  @param completion 清理缓存完成回调
 */
- (void)cleanResourceCacheWithBlock:(QYCleanCacheCompletion)completion;

/**
 *  清理账号信息
 *  @discussion 清理全部账号信息会登出当前账号，并新建匿名账号，请在调用完成后使用setUserInfo:接口恢复为有名账号；请在合理时机调用本接口
 *  @param cleanAll 是否清理当前所有账号信息，NO表示清理历史无用账号，YES表示清理全部
 *  @param completion 清理缓存完成回调
 */
- (void)cleanAccountInfoForAll:(BOOL)cleanAll completion:(QYCleanCacheCompletion)completion;

#pragma mark - Deprecated
///**
// *  已废弃
// *  追踪用户浏览信息;暂时客服端还没有入口可以查看这部分信息
// *  @param urlString  浏览url
// *  @param attributes 附加信息
// */
//- (void)trackHistory:(NSString *)urlString withAttributes:(NSDictionary *)attributes;

///**
// *  已废弃，使用setUserInfo替代
// *  设置userInfo.userId即可，userInfo.data忽略
// *  添加个人信息
// *
// *  @param infos 个人信息；目前有两个key，“foreignid”表示用户id，“name”表示用户名
// */
//- (void)addUserInfo:(NSDictionary *)infos;

@end
