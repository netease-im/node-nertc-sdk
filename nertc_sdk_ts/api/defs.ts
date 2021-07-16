import { type } from "os";

export interface RendererOptions
{
    append: boolean
}

/** 日志级别。 */
export enum NERtcLogLevel{
    kNERtcLogLevelFatal    = 0,        /**< Fatal 级别日志信息。 */
    kNERtcLogLevelError    = 1,        /**< Error 级别日志信息。 */
    kNERtcLogLevelWarning  = 2,        /**< Warning 级别日志信息。 */
    kNERtcLogLevelInfo     = 3,        /**< Info 级别日志信息。默认级别 */
    kNERtcLogLevelDetailInfo   = 4,    /**< Detail Info 级别日志信息。 */
    kNERtcLogLevelVerbos   = 5,        /**< Verbos 级别日志信息。 */
    kNERtcLogLevelDebug    = 6,        /**< Debug 级别日志信息。如果你想获取最完整的日志，可以将日志级别设为该等级。*/
    kNERtcLogLevelOff      = 7,        /**< 不输出日志信息。*/
}

export interface NERtcEngineContext {
    app_key: string;		/**< 用户注册云信的APP Key。如果你的开发包里面缺少 APP Key，请申请注册一个新的 APP Key。*/
    log_dir_path: string;	/**< 日志目录的完整路径，采用UTF-8 编码。*/
    log_level: NERtcLogLevel;	/**< 日志级别，默认级别为 kNERtcLogLevelInfo。*/
    log_file_max_size_KBytes: number;	/**< 指定 SDK 输出日志文件的大小上限，单位为 KB。如果设置为 0，则默认为 20 M。*/
}

/** 场景模式 */
export enum NERtcChannelProfileType
{
    kNERtcChannelProfileCommunication       = 0,    /**< 通话场景 */
    kNERtcChannelProfileLiveBroadcasting    = 1,    /**< 直播推流场景 */
}

/** 视频流类型。*/
export enum NERtcRemoteVideoStreamType{
    kNERtcRemoteVideoStreamTypeHigh     = 0, /**< 默认大流 */
    kNERtcRemoteVideoStreamTypeLow      = 1, /**< 小流 */
    kNERtcRemoteVideoStreamTypeNone     = 2, /**< 不订阅 */
}

/** 错误代码。

错误代码意味着 SDK 遇到不可恢复的错误，需要应用程序干预。
*/
export enum NERtcErrorCode
{
	kNERtcNoError = 0,		/**< 没有错误 */
	//资源分配错误 （正常请求时不会有此类错误返回）
	kNERtcErrChannelReservePermissionDenied = 403,	/**< 没有权限，包括没有开通音视频功能、没有开通非安全但是请求了非安全等 */
	kNERtcErrChannelReserveTimeOut = 408,			/**< 请求超时 */
	kNERtcErrChannelReserveErrorParam = 414,		/**< 服务器请求参数错误 */
	kNERtcErrChannelReserveServerFail = 500,		/**< 分配频道服务器未知错误 */
	kNERtcErrChannelReserveMoreThanTwoUser = 600,	/**< 只支持两个用户, 有第三个人试图使用相同的频道名分配频道 */

    //livestream task
    kNERtcErrLsTaskRequestInvalid = 1301,	        /**< task请求无效，被后续操作覆盖 */
    kNERtcErrLsTaskIsInvaild = 1400,	            /**< task参数格式错误 */
    kNERtcErrLsTaskRoomExited = 1401,	            /**< 房间已经退出 */
    kNERtcErrLsTaskNumLimit = 1402,	                /**< 推流任务超出上限 */
    kNERtcErrLsTaskDuplicateId = 1403,	            /**< 推流ID重复 */
    kNERtcErrLsTaskNotFound = 1404,	                /**< taskId任务不存在，或频道不存在 */
    kNERtcErrLsTaskRequestErr = 1417,	            /**< 请求失败 */
    kNERtcErrLsTaskInternalServerErr = 1500,	    /**< 服务器内部错误 */
    kNERtcErrLsTaskInvalidLayout = 1501,	        /**< 布局参数错误 */
    kNERtcErrLsTaskUserPicErr = 1512,	            /**< 用户图片错误 */


	//其他错误
	kNERtcErrChannelStartFail = 11000,				/**< 通道发起失败 */
	kNERtcErrChannelDisconnected = 11001,			/**< 断开连接 */
	kNERtcErrVersionSelfLow = 11002,				/**< 本人SDK版本太低不兼容 */
	kNERtcErrVersionRemoteLow = 11003,				/**< 对方SDK版本太低不兼容 */
	kNERtcErrChannelClosed = 11004,					/**< 通道被关闭 */
	kNERtcErrChannelKicked = 11005,					/**< 账号被踢 */
	kNERtcErrDataError = 11400,						/**< 数据错误 */
	kNERtcErrInvalid = 11403,						/**< 无效的操作 */
	//连接服务器错误
	kNERtcErrChannelJoinTimeOut = 20101,			/**< 请求超时 */
	kNERtcErrChannelJoinMeetingModeError = 20102,	/**< 会议模式错误 */
	kNERtcErrChannelJoinRtmpModeError = 20103,		/**< rtmp用户加入非rtmp频道 */
	kNERtcErrChannelJoinRtmpNodesError = 20104,		/**< 超过频道最多rtmp人数限制 */
	kNERtcErrChannelJoinRtmpHostError = 20105,		/**< 已经存在一个主播 */
	kNERtcErrChannelJoinRtmpCreateError = 20106,	/**< 需要旁路直播, 但频道创建者非主播 */
	kNERtcErrChannelJoinLayoutError = 20208,		/**< 主播自定义布局错误 */
	kNERtcErrChannelJoinInvalidParam = 20400,		/**< 错误参数 */
	kNERtcErrChannelJoinDesKey = 20401,				/**< 密码加密错误 */
	kNERtcErrChannelJoinInvalidRequst = 20417,		/**< 错误请求 */
	kNERtcErrChannelServerUnknown = 20500,			/**< 服务器内部错误 */
	//Engine error code
	kNERtcErrFatal = 30001,               /**< 通用错误 */
	kNERtcErrOutOfMemory = 30002,         /**< 内存耗尽 */
	kNERtcErrInvalidParam = 30003,        /**< 错误的参数 */
	kNERtcErrNotSupported = 30004,        /**< 不支持的操作 */
	kNERtcErrInvalidState = 30005,        /**< 当前状态不支持的操作 */
	kNERtcErrLackOfResource = 30006,      /**< 资源耗尽 */
	kNERtcErrInvalidIndex = 30007,        /**< 非法 index */
	kNERtcErrDeviceNotFound = 30008,      /**< 设备未找到 */
	kNERtcErrInvalidDeviceSourceID = 30009,/**< 非法设备 ID */
	kNERtcErrInvalidVideoProfile = 30010, /**< 非法的视频 profile type */
	kNERtcErrCreateDeviceSourceFail = 30011,  /**< 设备创建错误 */
	kNERtcErrInvalidRender = 30012,       /**< 非法的渲染容器 */
	kNERtcErrDevicePreviewAlreadyStarted = 30013,/**< 设备已经打开 */
	kNERtcErrTransmitPendding = 30014,    /**< 传输错误 */
	kNERtcErrConnectFail = 30015,         /**< 连接服务器错误 */
    kNERtcErrCreateDumpFileFail = 30016,    /**< 创建Audio dump文件失败 */
    kNERtcErrStartDumpFail = 30017,         /**< 开启Audio dump失败 */
    kNERtcErrDesktopCaptureInvalidState = 30020,    /**< 启动桌面录屏失败，不能与camera同时启动 */
    kNERtcErrDesktopCaptureInvalidParam = 30021,    /**< 桌面录屏传入参数无效 */
    kNERtcErrDesktopCaptureNotReady     = 30022,    /**< 桌面录屏未就绪 */

	kNERtcErrChannelAlreadyJoined = 30100,    /**< 重复加入频道 */
	kNERtcErrChannelNotJoined = 30101,        /**< 尚未加入频道 */
	kNERtcErrChannelRepleatedlyLeave = 30102, /**< 重复离开频道 */
	kNERtcErrRequestJoinChannelFail = 30103,  /**< 加入频道操作失败 */
	kNERtcErrSessionNotFound = 30104,         /**< 会话未找到 */
	kNERtcErrUserNotFound = 30105,            /**< 用户未找到 */
	kNERtcErrInvalidUserID = 30106,           /**< 非法的用户 ID */
	kNERtcErrMediaNotStarted = 30107,         /**< 用户多媒体数据未连接 */
	kNERtcErrSourceNotFound = 30108,          /**< source 未找到 */

	kNERtcErrConnectionNotFound = 30200,      /**< 连接未找到 */
	kNERtcErrStreamNotFound = 30201,          /**< 媒体流未找到 */
	kNERtcErrAddTrackFail = 30202,            /**< 加入 track 失败 */
	kNERtcErrTrackNotFound = 30203,           /**< track 未找到 */
	kNERtcErrMediaConnectionDisconnected = 30204, /**< 媒体连接断开 */
	kNERtcErrSignalDisconnected = 30205,      /**< 信令连接断开 */
	kNERtcErrServerKicked = 30206,            /**< 被踢出频道 */
    kNERtcErrKickedForRoomClosed = 30207,     /**< 因频道已关闭而被踢出 */
    
    kNERtcRuntimeErrADMNoAuthorize = 40000,    /**< 没有音频设备权限 */
    
    kNERtcRuntimeErrVDMNoAuthorize = 50000,    /**< 没有视频设备权限 */
    
    kNERtcRuntimeErrScreenCaptureNoAuthorize = 60000,    /**< 没有录制视频权限 */
}

/** 用户离开原因。*/
export enum NERtcSessionLeaveReason
{
    kNERtcSessionLeaveNormal = 0,       /**< 正常离开。*/
    kNERtcSessionLeaveForFailOver = 1,  /**< 用户断线导致离开。*/
    kNERtcSessionLeaveUpdate = 2,       /**< 用户 Failover 过程中产生的 leave。*/
    kNERtcSessionLeaveForKick = 3,      /**< 用户被踢导致离开。*/
    kNERtcSessionLeaveTimeOut = 4,      /**< 用户超时导致离开。*/
}

/** 视频编码配置。用于衡量编码质量。*/
export enum NERtcVideoProfileType
{
    kNERtcVideoProfileLowest = 0,       /**< 160x90/120, 15fps */
    kNERtcVideoProfileLow = 1,          /**< 320x180/240, 15fps */
    kNERtcVideoProfileStandard = 2,     /**< 640x360/480, 30fps */
    kNERtcVideoProfileHD720P = 3,       /**< 1280x720, 30fps */
    kNERtcVideoProfileHD1080P = 4,      /**< 1920x1080, 30fps */
    kNERtcVideoProfileNone = 5,
    kNERtcVideoProfileMAX = kNERtcVideoProfileHD1080P,
}

export enum NERtcVideoScalingMode {
    kNERtcVideoScaleFit      = 0,   /**< 0: 视频尺寸等比缩放。优先保证视频内容全部显示。因视频尺寸与显示视窗尺寸不一致造成的视窗未被填满的区域填充黑色。*/
    kNERtcVideoScaleFullFill = 1,   /**< 1: 视频尺寸非等比缩放。保证视频内容全部显示，且填满视窗。*/
    kNERtcVideoScaleCropFill = 2,   /**< 2: 视频尺寸等比缩放。优先保证视窗被填满。因视频尺寸与显示视窗尺寸不一致而多出的视频将被截掉。*/    
}

/** @enum NERtcVideoMirrorMode 设置镜像模式。*/
export enum NERtcVideoMirrorMode
{
    kNERtcVideoMirrorModeAuto       = 0,    /**< 0: Windows/macOS SDK 启用镜像模式。在 iOS/Android 平台中：如果你使用前置摄像头，SDK 默认启用镜像模式；如果你使用后置摄像头，SDK 默认关闭镜像模式。*/
    kNERtcVideoMirrorModeEnabled    = 1,    /**< 1: 启用镜像模式。*/
    kNERtcVideoMirrorModeDisabled   = 2,    /**< 2: （默认）关闭镜像模式。*/
}

export interface NERtcVideoCanvas {
    mode: NERtcVideoScalingMode;
    view: Element;
}

export enum NERtcAudioProfileType {
    kNERtcAudioProfileDefault = 0,			    /**< 0: 默认设置。Speech场景下为 kNERtcAudioProfileStandard，Music场景下为 kNERtcAudioProfileHighQuality */
    kNERtcAudioProfileStandard = 1,			    /**< 1: 普通质量的音频编码，16000Hz，20Kbps */
    kNERtcAudioProfileMiddleQuality = 2,		/**< 2: 中等质量的音频编码，48000Hz，32Kbps */
    kNERtcAudioProfileMiddleQualityStereo = 3,  /**< 3: 中等质量的立体声编码，48000Hz * 2，64Kbps  */
    kNERtcAudioProfileHighQuality = 4,          /**< 4: 高质量的音频编码，48000Hz，64Kbps  */
    kNERtcAudioProfileHighQualityStereo = 5,    /**< 5: 高质量的立体声编码，48000Hz * 2，128Kbps  */
}

export enum NERtcAudioScenarioType {
    kNERtcAudioScenarioDefault  = 0,    /** 0: 默认设置:kNERtcChannelProfileCommunication下为kNERtcAudioScenarioSpeech，kNERtcChannelProfileLiveBroadcasting下为kNERtcAudioScenarioMusic。 */
    kNERtcAudioScenarioSpeech   = 1,    /** 1: 语音场景. NERtcAudioProfileType 推荐使用 kNERtcAudioProfileMiddleQuality 及以下 */
    kNERtcAudioScenarioMusic    = 2,    /** 2: 音乐场景。NERtcAudioProfileType 推荐使用 kNERtcAudioProfileMiddleQualityStereo 及以上 */
}

/** @enum NERtcVideoCropMode 视频画面裁剪模式。*/
export enum NERtcVideoCropMode {
    kNERtcVideoCropModeDefault = 0,     /**< Device Defalut */
    kNERtcVideoCropMode16x9    = 1,     /**< 16:9 */
    kNERtcVideoCropMode4x3     = 2,     /**< 4:3 */
    kNERtcVideoCropMode1x1     = 3,     /**< 1:1 */
}

/** @enum NERtcVideoFramerateType 视频帧率。*/
export enum NERtcVideoFramerateType {
    kNERtcVideoFramerateFpsDefault  = 0,    /**< 默认帧率 */
    kNERtcVideoFramerateFps_7       = 7,    /**< 7帧每秒 */
    kNERtcVideoFramerateFps_10      = 10,   /**< 10帧每秒 */
    kNERtcVideoFramerateFps_15      = 15,   /**< 15帧每秒 */
    kNERtcVideoFramerateFps_24      = 24,   /**< 24帧每秒 */
    kNERtcVideoFramerateFps_30      = 30,   /**< 30帧每秒 */
}

/** @enum NERtcDegradationPreference 视频编码策略。*/
export enum NERtcDegradationPreference {
    kNERtcDegradationDefault            = 0,  /**< 使用引擎推荐值。通话场景使用平衡模式，直播推流场景使用清晰优先 */
    kNERtcDegradationMaintainFramerate  = 1,  /**< 帧率优先 */
    kNERtcDegradationMaintainQuality    = 2,  /**< 清晰度优先 */
    kNERtcDegradationBalanced           = 3,  /**< 平衡模式 */
}

/** 视频配置的属性。*/
export interface NERtcVideoConfig {
    max_profile: NERtcVideoProfileType;	/**< 视频编码的分辨率，用于衡量编码质量。*/
    width: number;                     /**< 视频编码自定义分辨率之宽度。width为0表示使用max_profile*/
    height: number;                    /**< 视频编码自定义分辨率之高度。height为0表示使用max_profile*/
    crop_mode: NERtcVideoCropMode;      /**< 视频画面裁剪模式，默认kNERtcVideoCropModeDefault。*/
    framerate: NERtcVideoFramerateType;  /**< 视频帧率 */
    min_framerate: NERtcVideoFramerateType;  /**< 视频最小帧率 */
    bitrate: number;                   /**< 视频编码码率kbps，取0时使用默认值 */
    min_bitrate: number;               /**< 视频编码码率下限kbps，取0时使用默认值 */
    degradation_preference: NERtcDegradationPreference;   /**< 编码策略 */
}

/** 音频帧请求格式。*/
export interface NERtcAudioFrameRequestFormat {
    channels: number;      /**< 音频频道数量(如果是立体声，数据是交叉的)。单声道: 1；双声道 : 2。*/
    sample_rate: number;   /**< 采样率。*/
}

/** 创建混音的配置项 */
export interface NERtcCreateAudioMixingOption {
    path: String;  /**< 本地文件全路径或URL < 256 chars */
    loop_count: number;                    /**< 循环次数， <= 0, 表示无限循环，默认 1 */
    send_enabled: boolean;              /**< 是否可发送，默认为 true */
    send_volume: number;           /**< 发送音量。最大为 100（默认）含义（0%-100%）*/
    playback_enabled: boolean;          /**< 是否可回放，默认为 true */
    playback_volume: number;       /**< 回放音量。最大为 100（默认）*/
}

/** 创建音效的配置项 */
export interface NERtcCreateAudioEffectOption {
    path: String;  /**< 本地文件全路径或URL < 256 chars */
    loop_count: number;                    /**< 循环次数， <= 0, 表示无限循环，默认 1 */
    send_enabled: boolean;              /**< 是否可发送，默认为 true */
    send_volume: number;           /**< 发送音量。最大为 100（默认）含义（0%-100%）*/
    playback_enabled: boolean;          /**< 是否可回放，默认为 true */
    playback_volume: number;       /**< 回放音量。最大为 100（默认）*/
}

/** 待共享区域相对于整个屏幕或窗口的位置，如不填，则表示共享整个屏幕或窗口。*/
export interface NERtcRectangle {
    x: number;      /**< 左上角的横向偏移。*/
    y: number;      /**< 左上角的纵向偏移。*/
    width: number;  /**< 待共享区域的宽。*/
    height: number; /**< 待共享区域的高。*/
}

/** 屏幕共享编码参数配置。*/
export enum NERtcScreenProfileType
{
    kNERtcScreenProfile480P     = 0,    /**< 640x480, 5fps */
    kNERtcScreenProfileHD720P   = 1,    /**< 1280x720, 5fps */
    kNERtcScreenProfileHD1080P  = 2,    /**< 1920x1080, 5fps。默认 */
    kNERtcScreenProfileCustom   = 3,    /**< 自定义 */
    kNERtcScreenProfileNone     = 4,
    kNERtcScreenProfileMAX = kNERtcScreenProfileHD1080P,
}

/** 视频尺寸。*/
export interface NERtcVideoDimensions
{
    width: number;     /**< 宽度 */
    height: number;    /**< 高度 */
}

/** 屏幕共享功能的编码策略倾向
- kNERtcSubStreamContentPreferMotion: 内容类型为动画;当共享的内容是视频、电影或游戏时，推荐选择该内容类型
当用户设置内容类型为动画时，按用户设置的帧率处理   
- kNERtcSubStreamContentPreferDetails: 内容类型为细节;当共享的内容是图片或文字时，推荐选择该内容类型
当用户设置内容类型为细节时，最高允许用户设置到10帧，设置超过10帧时，不生效，按10帧处理
 */

export enum NERtcSubStreamContentPrefer
{
    kNERtcSubStreamContentPreferMotion  = 0,    /**< 动画模式。*/
    kNERtcSubStreamContentPreferDetails = 1,    /**< 细节模式。*/
}

/** 屏幕共享编码参数配置。用于衡量编码质量。一期只支持profile设置。*/
export interface NERtcScreenCaptureParameters {
    profile: NERtcScreenProfileType;     /**< 屏幕共享编码参数配置。*/
    dimensions: NERtcVideoDimensions;    /**< 屏幕共享视频发送的最大像素值，kNERtcScreenProfileCustom下生效。*/
    frame_rate: number;                     /**< 共享视频的帧率，kNERtcScreenProfileCustom下生效，单位为 fps；默认值为 5，建议不要超过 15。*/
    bitrate: number;                        /**< 共享视频的码率，单位为 bps；默认值为 0，表示 SDK 根据当前共享屏幕的分辨率计算出一个合理的值。*/
    capture_mouse_cursor: boolean;          /**< 是否采集鼠标用于屏幕共享。*/
    window_focus: boolean;                  /**< 调用 startScreenCaptureByWindowId 方法共享窗口时，是否将该窗口前置。*/
    excluded_window_list: Array<Number>;         /**< 待屏蔽窗口的 ID 列表。 */
    excluded_window_count: number;          /**< 待屏蔽窗口的数量。*/
    prefer: NERtcSubStreamContentPrefer; /**< 编码策略倾向。*/
}

/** 直播推流模式 */
export enum NERtcLiveStreamMode
{
    kNERtcLsModeVideo  = 0, /**< 推流带视频 */
    kNERtcLsModeAudio  = 1, /**< 推流纯音频 */
}

/** 直播推流视频裁剪模式 */
export enum NERtcLiveStreamVideoScaleMode
{
    kNERtcLsModeVideoScaleFit      = 0, /**< 0: 视频尺寸等比缩放。优先保证视频内容全部显示。因视频尺寸与显示视窗尺寸不一致造成的视窗未被填满的区域填充背景色。*/
    kNERtcLsModeVideoScaleCropFill = 1, /**< 1: 视频尺寸等比缩放。优先保证视窗被填满。因视频尺寸与显示视窗尺寸不一致而多出的视频将被截掉。*/
}

/** 直播成员布局 */
export interface NERtcLiveStreamUserTranscoding {
    uid: number;                      /**< 用户id */
    video_push: boolean;                /**< 是否推送该用户视频流，kNERtcLsModeAudio时无效 */
    adaption: NERtcLiveStreamVideoScaleMode;   /**< 视频流裁剪模式 */
    x: number;                          /**< 画面离主画面左边距 */
    y: number;                          /**< 画面离主画面上边距 */
    width: number;                      /**< 画面在主画面的显示宽度，画面右边超出主画面会失败 */
    height: number;                     /**< 画面在主画面的显示高度，画面底边超出主画面会失败 */
    audio_push: boolean;                /**< 是否推送该用户音频流 */
}

/** 图片布局 */
export interface NERtcLiveStreamImageInfo {
    url: String;            /**< 图片地址 */
    x: number;              /**< 画面离主画面左边距 */
    y: number;                          /**< 画面离主画面上边距 */
    width: number;                      /**< 画面在主画面的显示宽度，画面右边超出主画面会失败 */
    height: number;                     /**< 画面在主画面的显示高度，画面底边超出主画面会失败 */
}

/** 直播布局 */
export interface NERtcLiveStreamLayout {
    width: number;                              /**< 视频推流宽度 */
    height: number;                             /**< 视频推流高度 */
    background_color: number;          /**< 视频推流背景色，(R & 0xff) << 16 | (G & 0xff) << 8 | (B & 0xff) */
    user_count: number;                /**< 成员布局个数 */
    users: Array<NERtcLiveStreamUserTranscoding>;  /**< 成员布局数组 */
    bg_image: NERtcLiveStreamImageInfo;     /**< 背景图信息 */
}

/** 直播推流任务的配置项。*/
export interface NERtcLiveStreamTaskInfo
{
    task_id: String;    /**< 推流任务ID，为推流任务的唯一标识，用于过程中增删任务操作 < 64 chars*/
    stream_url: String;    /**< 直播推流地址 <256 chars */
    server_record_enabled: Boolean;             /**< 服务器录制功能是否开启 */
    ls_mode: NERtcLiveStreamMode;            /**< 直播推流模式，NERtcLiveStreamMode */
    layout: NERtcLiveStreamLayout;           /**< 视频布局 */
}

/** 音频设备链接类型。*/
export enum NERtcAudioDeviceTransportType
{
    kNERtcAudioDeviceTransportTypeUnknown           = 0,    /**< 未知设备 */
    kNERtcAudioDeviceTransportTypeBluetooth         = 1,    /**< 蓝牙设备 */
    kNERtcAudioDeviceTransportTypeBluetoothA2DP     = 2,    /**< 蓝牙立体声设备 */
    kNERtcAudioDeviceTransportTypeBluetoothLE       = 3,    /**< 蓝牙低功耗设备 */
    kNERtcAudioDeviceTransportTypeUSB               = 4,    /**< USB设备 */
    kNERtcAudioDeviceTransportTypeHDMI              = 5,    /**< HDMI设备 */
    kNERtcAudioDeviceTransportTypeBuiltIn           = 6,    /**< 内置设备 */
    kNERtcAudioDeviceTransportTypeThunderbolt       = 7,    /**< 雷电接口设备 */
    kNERtcAudioDeviceTransportTypeAirPlay           = 8,    /**< AirPlay设备 */
    kNERtcAudioDeviceTransportTypeVirtual           = 9,    /**< 虚拟设备 */
    kNERtcAudioDeviceTransportTypeOther             = 10,   /**< 其他设备 */
}

/** 摄像头设备链接类型。*/
export enum NERtcVideoDeviceTransportType
{
    kNERtcVideoDeviceTransportTypeUnknown   = 0,    /**< 未知设备 */
    kNERtcVideoDeviceTransportTypeUSB       = 1,    /**< USB设备 */
    kNERtcVideoDeviceTransportTypeVirtual   = 2,    /**< 虚拟设备 */
    kNERtcVideoDeviceTransportTypeOther     = 3,    /**< 其他设备 */
}

export interface NERtcDevice
{
    device_id: String;
    device_name: String;
    transport_type: number; //NERtcAudioDeviceTransportType or NERtcVideoDeviceTransportType
    suspected_unavailable: boolean;
    system_default_device: boolean;
}

/** 参会者角色类型 */
export enum NERtcClientRole
{
    kNERtcClientRoleBroadcaster     = 0,            /**< 主播 */
    kNERtcClientRoleAudience        = 1,            /**< 观众 */
}

export interface NERtcEngineAPI {
    initialize(context: NERtcEngineContext): number;
    release(): void;
    setChannelProfile(profile: NERtcChannelProfileType): number;
    joinChannel(token: String, channelName: String, uid: number): number;
    leaveChannel(): number;
    enableLocalAudio(enabled: Boolean): number;
    enableLocalVideo(enabled: Boolean): number;
    subscribeRemoteVideoStream(uid: number, type: NERtcRemoteVideoStreamType, subscribe: Boolean): number;

    setupVideoCanvas(uid: number, enabled: Boolean): number;
    onVideoFrame(callback: Function): number;
    onEvent(eventName: String, callback: Function): void;

    getConnectionState(): number;
    muteLocalAudioStream(enabled: Boolean): number;
    setAudioProfile(profile: NERtcAudioProfileType, scenario: NERtcAudioScenarioType): number;
    subscribeRemoteAudioStream(uid: number, enabled: Boolean): number;
    setVideoConfig(config: NERtcVideoConfig): number;
    enableDualStreamMode(enabled: Boolean): number;
    startVideoPreview(): number;
    stopVideoPreview(): number;
    muteLocalVideoStream(enabled: Boolean): number;
    setParameters(parameters: String): number;
    setRecordingAudioFrameParameters(format: NERtcAudioFrameRequestFormat): number;
    setPlaybackAudioFrameParameters(format: NERtcAudioFrameRequestFormat): number;
    startAudioDump(): number;
    stopAudioDump(): number;
    startAudioMixing(opt: NERtcCreateAudioMixingOption): number;
    stopAudioMixing(): number;
    pauseAudioMixing(): number;
    resumeAudioMixing(): number;
    setAudioMixingSendVolume(volume: number): number;
    getAudioMixingSendVolume(): number;
    setAudioMixingPlaybackVolume(volume: number): number;
    getAudioMixingPlaybackVolume(): number;
    getAudioMixingDuration(): number;
    getAudioMixingCurrentPosition(): number;
    setAudioMixingPosition(pos: number): number;
    playEffect(effectId: number, opt: Array<NERtcCreateAudioEffectOption>): number;
    stopEffect(effectId: number): number;
    stopAllEffects(): number;
    pauseEffect(effectId: number): number;
    resumeEffect(effectId: number): number;
    pauseAllEffects(): number;
    resumeAllEffects(): number;
    setEffectSendVolume(effectId: number, volume: number): number;
    getEffectSendVolume(effectId: number): number;
    setEffectPlaybackVolume(effectId: number, volume: number): number;
    getEffectPlaybackVolume(effectId: number): number;
    enableEarback(enabled: boolean, volume: number): number;
    setEarbackVolume(volume: number): number;
    onStatsObserver(eventName: String, enabled: boolean, callback: Function): void;
    enableAudioVolumeIndication(enabled: boolean, interval: number): number;
    startScreenCaptureByScreenRect(screenRect: NERtcRectangle, regionRect: NERtcRectangle, param: NERtcScreenCaptureParameters): number;
    startScreenCaptureByDisplayId(displayId: number, regionRect: NERtcRectangle, param: NERtcScreenCaptureParameters): number;
    startScreenCaptureByWindowId(windowid: number, regionRect: NERtcRectangle, param: NERtcScreenCaptureParameters): number;
    updateScreenCaptureRegion(regionRect: NERtcRectangle): number;
    stopScreenCapture(): number;
    pauseScreenCapture(): number;
    resumeScreenCapture(): number;
    getVersion(): String;
    getErrorDescription(errorCode: number): String;
    uploadSdkInfo(): void;
    addLiveStreamTask(info: NERtcLiveStreamTaskInfo): number;
    updateLiveStreamTask(info: NERtcLiveStreamTaskInfo): number;
    removeLiveStreamTask(taskId: String): number;
    enumerateRecordDevices(): Array<NERtcDevice>;
    setRecordDevice(id: String): number; //<256chars
    getRecordDevice(): String;
    enumeratePlayoutDevices(): Array<NERtcDevice>;
    setPlayoutDevice(id: String): number; //<256chars
    getPlayoutDevice(): String;
    setRecordDeviceVolume(volume: number): number;
    getRecordDeviceVolume(): number;
    setPlayoutDeviceVolume(volume: number): number;
    getPlayoutDeviceVolume(): number;
    setPlayoutDeviceMute(mute: boolean): number;
    getPlayoutDeviceMute(): boolean;
    setRecordDeviceMute(mute: boolean): number;
    getRecordDeviceMute(): boolean;
    adjustRecordingSignalVolume(volume: number): number;
    adjustPlaybackSignalVolume(volume: number): number;
    startRecordDeviceTest(interval: number): number;
    stopRecordDeviceTest(): number;
    startPlayoutDeviceTest(path: String): number;
    stopPlayoutDeviceTest(): number;
    startAudioDeviceLoopbackTest(interval: number): number;
    stopAudioDeviceLoopbackTest(): number;
    enumerateCaptureDevices(): Array<NERtcDevice>;
    setDevice(id: String): number; //<256chars
    getDevice(): String;
    setLocalVideoMirrorMode(mode: NERtcVideoMirrorMode): number;
    //3.9
    setClientRole(role: NERtcClientRole): number;
    setupSubStreamVideoCanvas(uid: number, enabled: Boolean): number;
    subscribeRemoteVideoSubStream(uid: number, sub: boolean): number;

    //sc
    enumerateScreenCaptureSourceInfo(thumbWidth: number, thumbHeight: number, iconWidth: number, iconHeight: number): Array<Object>;

    //TODO
    // setMixedAudioFrameParameters(samplerate: number): number;
    // setExternalVideoSource(enabled: boolean): number;
    //pushExternalVideoFrame
    // setExternalAudioSource(enabled: boolean, samplerate: number, channel: number): number;
    //pushExternalAudioFrame
}

/** 通话相关的统计信息。*/
export interface NERtcStats
{
    cpu_app_usage: number;     /**< 当前 App 的 CPU 使用率 (%)。*/
    cpu_idle_usage: number;    /**< 当前系统的 CPU 空闲率 (%)。*/
    cpu_total_usage: number;	/**< 当前系统的 CPU 使用率 (%)。*/
    memory_app_usage: number;	/**< 当前App的内存使用率 (%)。*/
    memory_total_usage: number;/**< 当前系统的内存使用率 (%)。*/
    memory_app_kbytes: number;	/**< 当前App的内存使用量 (KB)。*/
    total_duration: number;         /**< 通话时长（秒）。*/
    tx_bytes: number;	        /**< 发送字节数，累计值。(bytes)*/
    rx_bytes: number;	        /**< 接收字节数，累计值。(bytes)*/
    tx_audio_bytes: number;	/**< 音频发送字节数，累计值。(bytes)*/
    tx_video_bytes: number;	/**< 视频发送字节数，累计值。(bytes)*/
    rx_audio_bytes: number;	/**< 音频接收字节数，累计值。(bytes)*/
    rx_video_bytes: number;	/**< 视频接收字节数，累计值。(bytes)*/
    tx_audio_kbitrate: number;	    /**< 音频发送码率。(kbps)*/
    rx_audio_kbitrate: number;	    /**< 音频接收码率。(kbps)*/
    tx_video_kbitrate: number;	    /**< 视频发送码率。(kbps)*/
    rx_video_kbitrate: number;	    /**< 视频接收码率。(kbps)*/
    up_rtt: number;	                /**< 上行平均往返时延rtt(ms) */
    tx_audio_packet_loss_rate: number;  /**< 本地上行音频实际丢包率。(%) */
    tx_video_packet_loss_rate: number;  /**< 本地上行视频实际丢包率。(%) */
    tx_audio_packet_loss_sum: number;   /**< 本地上行音频实际丢包数。 */
    tx_video_packet_loss_sum: number;   /**< 本地上行视频实际丢包数。 */
    tx_audio_jitter: number;            /**< 本地上行音频抖动计算。(ms) */
    tx_video_jitter: number;            /**< 本地上行视频抖动计算。(ms) */
    rx_audio_packet_loss_rate: number;  /**< 本地下行音频实际丢包率。(%) */
    rx_video_packet_loss_rate: number;  /**< 本地下行视频实际丢包率。(%) */
    rx_audio_packet_loss_sum: number;   /**< 本地下行音频实际丢包数。 */
    rx_video_packet_loss_sum: number;   /**< 本地下行视频实际丢包数。 */
    rx_audio_jitter: number;            /**< 本地下行音频抖动计算。(ms) */
    rx_video_jitter: number;            /**< 本地下行视频抖动计算。(ms) */
}

/** 本地视频流上传统计信息。*/
export interface NERtcVideoSendStats
{
    width: number;      /**< 视频流宽（像素）。*/
    height: number;     /**< 视频流高（像素）。*/
    capture_frame_rate: number; /**< 视频采集帧率。*/
    encoder_frame_rate: number;	/**< （上次统计后）编码帧率 (fps)。*/
    sent_bitrate: number;		/**< （上次统计后）发送码率(Kbps)。*/
    sent_frame_rate: number;	/**< （上次统计后）发送帧率 (fps)。*/
    target_bitrate: number;		/**< （上次统计后）编码器目标码率(Kbps)。*/
}

/** 远端视频流的统计信息。*/
export interface NERtcVideoRecvStats
{
    uid: number;      /**< 用户 ID，指定是哪个用户的视频流。*/
    width: number;      /**< 视频流宽（像素）。*/
    height: number;     /**< 视频流高（像素）。*/
    received_bitrate: number;     /**< （上次统计后）接收到的码率(Kbps)。*/
    packet_loss_rate: number;     /**< 下行丢包率(%)。*/
    decoder_frame_rate: number;	/**< （上次统计后）解码帧率 (fps)。*/
    render_frame_rate: number;	/**< （上次统计后）渲染帧率 (fps)。*/
    received_frame_rate: number;  /**< （上次统计后）接收到的帧率 (fps)。*/
    total_frozen_time: number;  /**< 用户的下行视频卡顿累计时长(ms)。*/
    frozen_rate: number;        /**< 用户的下行视频平均卡顿率(%)。*/
}

/** 本地音频流上传统计信息。*/
export interface NERtcAudioSendStats
{
    num_channels: number;	    /**< 当前采集声道数。*/
    sent_sample_rate: number;	/**< 本地上行音频采样率。*/
    sent_bitrate: number;		/**< （上次统计后）发送码率(Kbps)。*/
    audio_loss_rate: number;    /**< 特定时间内的音频丢包率 (%)。*/
    rtt: number;            /**< RTT。*/
    volume: number;	/**< 音量，范围为 0（最低）- 100（最高）。*/
}

/** 远端用户的音频统计。*/
export interface NERtcAudioRecvStats
{
    uid: number;              /**< 用户 ID，指定是哪个用户的音频流。*/
    received_bitrate: number;   /**< （上次统计后）接收到的码率(Kbps)。*/
    total_frozen_time: number;  /**< 用户的下行音频卡顿累计时长(ms)。*/
    frozen_rate: number;        /**< 用户的下行音频平均卡顿率(%)。*/
    audio_loss_rate: number;  /**< 特定时间内的音频丢包率 (%)。*/
    volume: number;	/**< 音量，范围为 0（最低）- 100（最高）。*/
}

/** @enum NERtcNetworkQualityType 网络质量类型。*/
export enum NERtcNetworkQualityType
{
    kNERtcNetworkQualityUnknown = 0,    /**< 0: 网络质量未知。*/
    kNERtcNetworkQualityExcellent = 1,  /**< 1: 网络质量极好。*/
    kNERtcNetworkQualityGood = 2,       /**< 2: 用户主观感觉和 excellent 差不多，但码率可能略低于 excellent。*/
    kNERtcNetworkQualityPoor = 3,       /**< 3: 用户主观感受有瑕疵但不影响沟通。*/
    kNERtcNetworkQualityBad = 4,        /**< 4: 勉强能沟通但不顺畅。*/
    kNERtcNetworkQualityVeryBad = 5,    /**< 5: 网络质量非常差，基本不能沟通。*/
    kNERtcNetworkQualityDown = 6,       /**< 6: 完全无法沟通。*/
}

/** 网络质量统计信息。*/
export interface NERtcNetworkQualityInfo
{
    uid: number;                          /**< 用户 ID，指定是哪个用户的网络质量统计。*/
    tx_quality: NERtcNetworkQualityType; /**< 该用户的上行网络质量，详见 #NERtcNetworkQualityType.*/
    rx_quality: NERtcNetworkQualityType; /**< 该用户的下行网络质量，详见 #NERtcNetworkQualityType.*/
}

/** 连接状态 */
export enum NERtcConnectionStateType
{
    kNERtcConnectionStateDisconnected   = 1, /**< 没加入频道。*/
    kNERtcConnectionStateConnecting     = 2, /**< 正在加入频道。*/
    kNERtcConnectionStateConnected      = 3, /**< 加入频道成功。*/
    kNERtcConnectionStateReconnecting   = 4, /**< 正在尝试重新加入频道。*/
    kNERtcConnectionStateFailed         = 5, /**< 加入频道失败。*/
}


/** 连接状态变更原因 */
export enum NERtcReasonConnectionChangedType
{
    kNERtcReasonConnectionChangedLeaveChannel           = 1, //kNERtcConnectionStateDisconnected 离开房间
    kNERtcReasonConnectionChangedChannelClosed          = 2, //kNERtcConnectionStateDisconnected 	房间被关闭
    kNERtcReasonConnectionChangedBeKicked               = 3, //kNERtcConnectionStateDisconnected 用户被踢
    kNERtcReasonConnectionChangedTimeOut                = 4, //kNERtcConnectionStateDisconnected	服务超时
    kNERtcReasonConnectionChangedJoinChannel            = 5, //kNERtcConnectionStateConnecting 加入房间
    kNERtcReasonConnectionChangedJoinSucceed            = 6, //kNERtcConnectionStateConnected 加入房间成功
    kNERtcReasonConnectionChangedReJoinSucceed          = 7, //kNERtcConnectionStateConnected 重新加入房间成功（重连）
    kNERtcReasonConnectionChangedMediaConnectionDisconnected = 8, //kNERtcConnectionStateReconnecting 媒体连接断开
    kNERtcReasonConnectionChangedSignalDisconnected     = 9, //kNERtcConnectionStateReconnecting 信令连接断开
    kNERtcReasonConnectionChangedRequestChannelFailed   = 10, //kNERtcConnectionStateFailed 请求频道失败
    kNERtcReasonConnectionChangedJoinChannelFailed      = 11, //kNERtcConnectionStateFailed 加入频道失败
}

/** 音频设备类型。*/
export enum NERtcAudioDeviceType
{
    kNERtcAudioDeviceUnknown = 0,       /**< 未知音频设备 */
    kNERtcAudioDeviceRecord,            /**< 音频采集设备 */
    kNERtcAudioDevicePlayout,           /**< 音频播放设备 */
}

/** 音频设备类型状态。*/
export enum NERtcAudioDeviceState
{
    kNERtcAudioDeviceActive = 0,    /**< 音频设备已激活 */
    kNERtcAudioDeviceUnactive,      /**< 音频设备未激活 */
}

/** 音乐文件播放状态。
*/
export enum NERtcAudioMixingState
{
    kNERtcAudioMixingStateFinished  = 0,       /**< 音乐文件播放结束。*/
    kNERtcAudioMixingStateFailed    = 1,       /**< 音乐文件报错。详见: #NERtcAudioMixingErrorCode*/
}

/** @enum NERtcAudioMixingErrorCode 混音音乐文件错误码。
*/
export enum NERtcAudioMixingErrorCode
{
    kNERtcAudioMixingErrorOK            = 0,      			/**< 没有错误。*/
    kNERtcAudioMixingErrorFatal         = 1,      			/**< 通用错误。*/
    kNERtcAudioMixingErrorCanNotOpen    = 100,      		/**< 音乐文件打开出错。*/
    //kNERtcAudioMixingErrorTooFrequentCall = 101,      	/**< 音乐文件打开太频繁。*/
    //kNERtcAudioMixingErrorInterruptedEOF= 102,      		/**< 音乐文件播放中断。*/
}

/** 声音音量信息。一个数组，包含每个说话者的用户 ID 和音量信息。*/
export interface NERtcAudioVolumeInfo
{
    uid: number;				/**< 说话者的用户 ID。如果返回的 uid 为 0，则默认为本地用户。*/
    volume: number;	/**< 说话者的音量，范围为 0（最低）- 100（最高）。*/
}

/** 直播推流状态。*/
export enum NERtcLiveStreamStateCode
{
    kNERtcLsStatePushing        = 505,  /**< 推流中 */
    kNERtcLsStatePushFail       = 506,  /**< 互动直播推流失败 */
    kNERtcLsStatePushStopped    = 511,  /**< 推流结束 */
    kNERtcLsStateImageError     = 512,  /**< 背景图片设置出错 */
}

/** @enum NERtcMediaStatsEventName  SDK 向 App 上报统计信息 */
export enum NERtcMediaStatsEventName {
	RtcStats = "onRtcStats",		/**< 当前通话统计回调, 每 2 秒触发一次 */
	LocalAudioStats = "onLocalAudioStats", 	/**< 本地音频流统计信息回调,每 2 秒触发一次 */
	RemoteAudioStats = "onRemoteAudioStats",	/**< 通话中远端音频流的统计信息回调, 每 2 秒触发一次 */
	LocalVideoStats = "onLocalVideoStats",	/**< 本地视频流统计信息回调, 每 2 秒触发一次 */
	RemoteVideoStats = "onRemoteVideoStats",	/**< 通话中远端视频流的统计信息回调, 每 2 秒触发一次 */
	NetworkQuality = "onNetworkQuality",	/**< 通话中每个用户的网络上下行质量报告回调, 每 2 秒触发一次, 只上报状态有变更的成员 */
}