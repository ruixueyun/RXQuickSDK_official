//
//  SMPCQuickSDK.h
//
//
//
//
#define SMPC_QUICK_SDK_VERSION @"2.6.1" //QuickSDK基础库版本

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "SMPCQuickSDKDefines.h"
#import "SMPCQuickSDKInitConfigure.h"
#import "SMPCQuickSDKPayOrderInfo.h"
#import "SMPCQuickSDKGameRoleInfo.h"
#import "SMPCQuickSDKShareInfo.h"

typedef enum {
    QuickSDKUserTYPE_UNKnow        = 0,//未知的未定义的
    QuickSDKUserTYPE_Account       = 1,//账号
    QuickSDKUserTYPE_Phone         = 2,//手机号
    QuickSDKUserTYPE_Guest         = 3,//游客登录
    QuickSDKUserTYPE_WX            = 4,//微信
    QuickSDKUserTYPE_QQ            = 5,//手Q
    QuickSDKUserTYPE_WeiBo         = 6,//微博
    QuickSDKUserTYPE_GameCenter    = 7,//GameCenter
    QuickSDKUserTYPE_Apple         = 16,//苹果
    QuickSDKUserTYPE_OneKey        = 18,//阿里云一键登录
} QuickSDKUserTYPE;
#pragma mark 基本信息

@interface SMPCQuickSDK : NSObject

#pragma mark 单例对象
+ (SMPCQuickSDK *)defaultInstance;
- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;

#pragma mark 获取渠道基本信息
//获取渠道唯一标识，与服务器端channelID一致，任何时刻都可调用
- (int)channelType;

/**
 * 获取QuickGame渠道cps包的渠道号
 */
- (NSString *)cpsChannelId;
@end

#pragma mark - 初始化 接入产品信息

@interface SMPCQuickSDK(Base)

#pragma mark 初始化
/**
 @brief 应用初始化 完成后会发送通知kSmpcQuickSDKNotiInitDidFinished
 @param configure 初始化配置类，SDK会优先使用打包工具上在线配置的productCode
 @result 错误码
 @note 必接
 */
- (int)initWithConfig:(SMPCQuickSDKInitConfigure *)configure application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions;
/**
 @brief 调用组件功能，参数请看对应组件接入文档
 */
- (int)callPlug:(NSString *)name params:(NSArray *)args;
@end

#pragma mark - 用户部分 登录 注册 登出

@interface SMPCQuickSDK(Account)

/**
 @brief 登录接口 ，登录后会发送kSmpcQuickSDKNotiLogin通知
 @result 错误码
 @note 必接
 */
- (int)login;
/**
 @brief 登出接口 代码调用注销
 @result 错误码
 @note 成功调用该接口后，SDK会发出kSmpcQuickSDKNotiLogout通知
 */
- (int)logout;
/** 设置切换账号回调 */
- (void)configSwitchAccount:(void(^)(NSDictionary * resultDict))resultBlock;
// 登录后获取用户uid,这个uid在渠道上唯一，多个渠道的用户uid可能重复
- (NSString *)userId;
// 登录后获取userToken可用于验证用户信息
- (NSString *)userToken;
// 登录后获取用户昵称
- (NSString *)userNick;
/** 查询渠道实名认证信息 */
- (NSDictionary *)realNameInfo;
/** 获取设备id */
- (NSString *)getDeviceID;
/** 清除设备id缓存数据 */
- (void)resetDeviceID;
/**
 @brief 调用渠道实名认证
 @result 错误码若渠道没有提供此方法会返回非0
 @note 结果会发送kSmpcQuickSDKNotiRealAuth通知
*/
- (int)realAuth;
/** 获取渠道用户登录方式目前仅QuickGame越狱渠道有用 */
- (QuickSDKUserTYPE)getChannelUserLoginType;
/** 获取用户额外信息，若没有额外信息则返回空字典 */
- (NSDictionary *)getChannelUserExtInfo;
#pragma mark - 更新角色信息
// 进入游戏角色，角色信息变更时调用（比如升级）,请使用updateRoleInfoWith:isCreate:
- (void)updateRoleInfoWith:(SMPCQuickSDKGameRoleInfo *)info;
//// 创建角色、进入游戏角色、角色信息变更时调用（比如升级，isCreate表示是否为创建角色
- (void)updateRoleInfoWith:(SMPCQuickSDKGameRoleInfo *)info isCreate:(BOOL)isCreate;

@end
//MARK:- 充值
@interface SMPCQuickSDK(Pay)
/**
 @brief 商品购买
 *  @param orderInfo
 *  @param roleInfo
 @result 错误码
 */
- (int)payOrderInfo:(SMPCQuickSDKPayOrderInfo *)orderInfo
           roleInfo:(SMPCQuickSDKGameRoleInfo *)roleInfo;
/**
 @brief 商品信息
 *  @param productId 商品id
 */
-(void)Getlocalized:(NSString *)productIds;
@end

#pragma mark - 界面控制
@interface SMPCQuickSDK(UISetting)

#pragma mark 浮动条
/**
 @brief showToolBar:
 @brief 浮动工具栏，建议显示在左上角
 @result 错误码
 @note
 */
- (int)showToolBar:(SMPC_QUICK_SDK_TOOLBAR_PLACE)place;
//隐藏浮动工具栏
- (int)hideToolBar;

@end

@interface SMPCQuickSDK(Extend)
// 获取QuicSDK后台为渠道添加的自定义键值，任何时刻都可调用
// 如果没有将返回nil
- (NSString *)getConfigValue:(NSString *)key;
/**
 @brief isFunctionSupported:
 @brief 判断当前渠道是否实现了QuickSDK提供的某个接口
 @brief 如果当前渠道没有实现相应接口，就没有相应接口的回调
 @brief 例如渠道SDK没有实现pausedGame这个接口，即使调用了pausedGame，没有暂停恢复通知，因为pausedGame调用无效
 @result 是否
 @note
 */
- (BOOL)isFunctionTypeSupported:(SMPC_QUICK_SDK_FUNC_TYPE)type;

/**
 @brief 进入用户中心 如用户注销登录会发送kSmpcQPLogoutNotification（可多次触发）
 @result 错误码
 */
- (int)enterUserCenter;
/**
 @brief 调用渠道特定方法
 parameterDict: 调用方法需要传入的参数，如果没有则传入@{}
 @result 错误码
 */
- (int)callChannelSpecifyFunction:(NSInteger)functionName parameters:(NSDictionary *)parameterDict;

/**
 进入YunKefu

 @param gameRoleInfo
 @result 错误码
 */
- (int)enterYunKeFuCenter:(SMPCQuickSDKGameRoleInfo *)gameRoleInfo;
/** 显示是否同意游戏用户协议和隐私政策弹窗-可点击查看在QK后台自定义的用户协议和隐私政策详情
 * @param must 是否必须同意 YES:弹窗仅有同意按钮 NO同时显示不同意和同意按钮
 * @param resultBlock 结果回调 YES玩家已经同意 NO玩家没有同意
 */
- (void)showGameCustomPrivacyViewMustAgree:(BOOL)must result:(void(^)(BOOL hasAgreed))resultBlock;
/** 用手机浏览器打开指定url
 * @param url url地址
*/
- (void)openUrlOutAppWithBrowser:(NSString *)url;
/** 在app内用WKWebView打开指定url
 * @param url url地址
*/
- (void)openUrlInAppFullScreen:(NSString *)url;
/**  在APP内用SFSafariViewController打开指定url
 * @param url url地址
 * @param isCanClose 是否可以关闭浏览器 ，YES 可关闭
 */
- (void)openBrowserPageWithUrlString:(NSString *)url isCanClose:(BOOL)isCanClose;

/** 调用显示渠道用户隐私弹窗， 用户同意返回YES，不同意返回NO*/
- (void)showPrivacyView:(void (^)(BOOL isAgreed))isAgreed;

#pragma mark 分享
/**
 分享

 @param shareInfo shareInfo description
 @result 错误码
 */
- (int)callSDKShare:(SMPCQuickSDKShareInfo *)shareInfo;


//***********************应用生命周期的回调*******************//
//在应用对应的生命周期回调中调用
/**
 @brief - (BOOL)application:(UIApplication *)application handleOpenURL:(NSURL *)url;
 @brief - (BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(nullable NSString *)sourceApplication annotation:(id)annotation 
 @brief - (BOOL)application:(UIApplication *)app openURL:(NSURL *)url options:(NSDictionary<NSString*, id> *)options
 @brief 渠道处理应用跳转
 @result 错误码
 @note 必接
 */

- (int)openURL:(NSURL *)url application:(UIApplication *)application;
- (int)openURL:(NSURL *)url sourceApplication:(NSString *)sourceApp application:(UIApplication *)application annotation:(id)annotation;
- (int)openURL:(NSURL *)url application:(UIApplication *)app options:(NSDictionary <NSString *, id>*)options;
/**
 @brief application:didRegisterForRemoteNotificationsWithDeviceToken:
 @brief 推送消息
 @result 错误码
 @note 必接
 */
- (int)application:(UIApplication*)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData*)deviceToken;

/**
 @brief application:didFailToRegisterForRemoteNotificationsWithError:
 @brief 推送消息
 @result 错误码
 @note 必接
 */
- (int)application:(UIApplication*)application didFailToRegisterForRemoteNotificationsWithError:(NSError*)error;
- (int)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo;
- (int)applicationWillResignActive:(UIApplication *)application;
- (int)applicationDidEnterBackground:(UIApplication *)application;
- (int)applicationWillEnterForeground:(UIApplication *)application;
- (int)applicationDidBecomeActive:(UIApplication *)application;
- (int)applicationWillTerminate:(UIApplication *)application;
- (NSUInteger)application:(UIApplication *)application supportedInterfaceOrientationsForWindow:(UIWindow *)window;
- (BOOL)application:(UIApplication *)application continueUserActivity:(NSUserActivity *)userActivity restorationHandler:(void(^)(NSArray *  restorableObjects))restorationHandler;

@end
