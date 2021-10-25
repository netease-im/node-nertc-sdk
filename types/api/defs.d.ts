export interface RendererOptions {
    append: boolean;
}
/** 日志级别。 */
export declare enum NERtcLogLevel {
    kNERtcLogLevelFatal = 0,
    kNERtcLogLevelError = 1,
    kNERtcLogLevelWarning = 2,
    kNERtcLogLevelInfo = 3,
    kNERtcLogLevelDetailInfo = 4,
    kNERtcLogLevelVerbos = 5,
    kNERtcLogLevelDebug = 6,
    kNERtcLogLevelOff = 7
}
export interface NERtcEngineContext {
    app_key: string; /**< 用户注册云信的APP Key。如果你的开发包里面缺少 APP Key，请申请注册一个新的 APP Key。*/
    log_dir_path: string; /**< 日志目录的完整路径，采用UTF-8 编码。*/
    log_level: NERtcLogLevel; /**< 日志级别，默认级别为 kNERtcLogLevelInfo。*/
    log_file_max_size_KBytes: number; /**< 指定 SDK 输出日志文件的大小上限，单位为 KB。如果设置为 0，则默认为 20 M。*/
}
/** 场景模式 */
export declare enum NERtcChannelProfileType {
    kNERtcChannelProfileCommunication = 0,
    kNERtcChannelProfileLiveBroadcasting = 1
}
/** 视频流类型。*/
export declare enum NERtcRemoteVideoStreamType {
    kNERtcRemoteVideoStreamTypeHigh = 0,
    kNERtcRemoteVideoStreamTypeLow = 1,
    kNERtcRemoteVideoStreamTypeNone = 2
}
/** 错误代码。

错误代码意味着 SDK 遇到不可恢复的错误，需要应用程序干预。
*/
export declare enum NERtcErrorCode {
    kNERtcNoError = 0,
    kNERtcErrChannelReservePermissionDenied = 403,
    kNERtcErrChannelReserveTimeOut = 408,
    kNERtcErrChannelReserveErrorParam = 414,
    kNERtcErrChannelReserveServerFail = 500,
    kNERtcErrChannelReserveMoreThanTwoUser = 600,
    kNERtcErrLsTaskRequestInvalid = 1301,
    kNERtcErrLsTaskIsInvaild = 1400,
    kNERtcErrLsTaskRoomExited = 1401,
    kNERtcErrLsTaskNumLimit = 1402,
    kNERtcErrLsTaskDuplicateId = 1403,
    kNERtcErrLsTaskNotFound = 1404,
    kNERtcErrLsTaskRequestErr = 1417,
    kNERtcErrLsTaskInternalServerErr = 1500,
    kNERtcErrLsTaskInvalidLayout = 1501,
    kNERtcErrLsTaskUserPicErr = 1512,
    kNERtcErrChannelStartFail = 11000,
    kNERtcErrChannelDisconnected = 11001,
    kNERtcErrVersionSelfLow = 11002,
    kNERtcErrVersionRemoteLow = 11003,
    kNERtcErrChannelClosed = 11004,
    kNERtcErrChannelKicked = 11005,
    kNERtcErrDataError = 11400,
    kNERtcErrInvalid = 11403,
    kNERtcErrChannelJoinTimeOut = 20101,
    kNERtcErrChannelJoinMeetingModeError = 20102,
    kNERtcErrChannelJoinRtmpModeError = 20103,
    kNERtcErrChannelJoinRtmpNodesError = 20104,
    kNERtcErrChannelJoinRtmpHostError = 20105,
    kNERtcErrChannelJoinRtmpCreateError = 20106,
    kNERtcErrChannelJoinLayoutError = 20208,
    kNERtcErrChannelJoinInvalidParam = 20400,
    kNERtcErrChannelJoinDesKey = 20401,
    kNERtcErrChannelJoinInvalidRequst = 20417,
    kNERtcErrChannelServerUnknown = 20500,
    kNERtcErrFatal = 30001,
    kNERtcErrOutOfMemory = 30002,
    kNERtcErrInvalidParam = 30003,
    kNERtcErrNotSupported = 30004,
    kNERtcErrInvalidState = 30005,
    kNERtcErrLackOfResource = 30006,
    kNERtcErrInvalidIndex = 30007,
    kNERtcErrDeviceNotFound = 30008,
    kNERtcErrInvalidDeviceSourceID = 30009,
    kNERtcErrInvalidVideoProfile = 30010,
    kNERtcErrCreateDeviceSourceFail = 30011,
    kNERtcErrInvalidRender = 30012,
    kNERtcErrDevicePreviewAlreadyStarted = 30013,
    kNERtcErrTransmitPendding = 30014,
    kNERtcErrConnectFail = 30015,
    kNERtcErrCreateDumpFileFail = 30016,
    kNERtcErrStartDumpFail = 30017,
    kNERtcErrDesktopCaptureInvalidState = 30020,
    kNERtcErrDesktopCaptureInvalidParam = 30021,
    kNERtcErrDesktopCaptureNotReady = 30022,
    kNERtcErrChannelAlreadyJoined = 30100,
    kNERtcErrChannelNotJoined = 30101,
    kNERtcErrChannelRepleatedlyLeave = 30102,
    kNERtcErrRequestJoinChannelFail = 30103,
    kNERtcErrSessionNotFound = 30104,
    kNERtcErrUserNotFound = 30105,
    kNERtcErrInvalidUserID = 30106,
    kNERtcErrMediaNotStarted = 30107,
    kNERtcErrSourceNotFound = 30108,
    kNERtcErrConnectionNotFound = 30200,
    kNERtcErrStreamNotFound = 30201,
    kNERtcErrAddTrackFail = 30202,
    kNERtcErrTrackNotFound = 30203,
    kNERtcErrMediaConnectionDisconnected = 30204,
    kNERtcErrSignalDisconnected = 30205,
    kNERtcErrServerKicked = 30206,
    kNERtcErrKickedForRoomClosed = 30207,
    kNERtcRuntimeErrADMNoAuthorize = 40000,
    kNERtcRuntimeErrVDMNoAuthorize = 50000,
    kNERtcRuntimeErrScreenCaptureNoAuthorize = 60000
}
/** 用户离开原因。*/
export declare enum NERtcSessionLeaveReason {
    kNERtcSessionLeaveNormal = 0,
    kNERtcSessionLeaveForFailOver = 1,
    kNERtcSessionLeaveUpdate = 2,
    kNERtcSessionLeaveForKick = 3,
    kNERtcSessionLeaveTimeOut = 4
}
/** 视频编码配置。用于衡量编码质量。*/
export declare enum NERtcVideoProfileType {
    kNERtcVideoProfileLowest = 0,
    kNERtcVideoProfileLow = 1,
    kNERtcVideoProfileStandard = 2,
    kNERtcVideoProfileHD720P = 3,
    kNERtcVideoProfileHD1080P = 4,
    kNERtcVideoProfileNone = 5,
    kNERtcVideoProfileMAX = 4,
    kNERtcVideoProfileFake = 6
}
/** 视频缩放类型。*/
export declare enum NERtcVideoScalingMode {
    kNERtcVideoScaleFit = 0,
    kNERtcVideoScaleFullFill = 1,
    kNERtcVideoScaleCropFill = 2
}
/** @enum NERtcVideoMirrorMode 设置镜像模式。*/
export declare enum NERtcVideoMirrorMode {
    kNERtcVideoMirrorModeAuto = 0,
    kNERtcVideoMirrorModeEnabled = 1,
    kNERtcVideoMirrorModeDisabled = 2
}
export interface NERtcVideoCanvas {
    mode: NERtcVideoScalingMode;
    view: Element;
}
export declare enum NERtcAudioProfileType {
    kNERtcAudioProfileDefault = 0,
    kNERtcAudioProfileStandard = 1,
    kNERtcAudioProfileStandardExtend = 2,
    kNERtcAudioProfileMiddleQuality = 3,
    kNERtcAudioProfileMiddleQualityStereo = 4,
    kNERtcAudioProfileHighQuality = 5,
    kNERtcAudioProfileHighQualityStereo = 6
}
export declare enum NERtcAudioScenarioType {
    kNERtcAudioScenarioDefault = 0,
    kNERtcAudioScenarioSpeech = 1,
    kNERtcAudioScenarioMusic = 2
}
/** @enum NERtcVideoCropMode 视频画面裁剪模式。*/
export declare enum NERtcVideoCropMode {
    kNERtcVideoCropModeDefault = 0,
    kNERtcVideoCropMode16x9 = 1,
    kNERtcVideoCropMode4x3 = 2,
    kNERtcVideoCropMode1x1 = 3
}
/** @enum NERtcVideoFramerateType 视频帧率。*/
export declare enum NERtcVideoFramerateType {
    kNERtcVideoFramerateFpsDefault = 0,
    kNERtcVideoFramerateFps_7 = 7,
    kNERtcVideoFramerateFps_10 = 10,
    kNERtcVideoFramerateFps_15 = 15,
    kNERtcVideoFramerateFps_24 = 24,
    kNERtcVideoFramerateFps_30 = 30,
    kNERtcVideoFramerateFps_60 = 60
}
/** @enum NERtcDegradationPreference 视频编码策略。*/
export declare enum NERtcDegradationPreference {
    kNERtcDegradationDefault = 0,
    kNERtcDegradationMaintainFramerate = 1,
    kNERtcDegradationMaintainQuality = 2,
    kNERtcDegradationBalanced = 3
}
/** 视频配置的属性。*/
export interface NERtcVideoConfig {
    max_profile: NERtcVideoProfileType; /**< 视频编码的分辨率，用于衡量编码质量。*/
    width: number; /**< 视频编码自定义分辨率之宽度。width为0表示使用max_profile*/
    height: number; /**< 视频编码自定义分辨率之高度。height为0表示使用max_profile*/
    crop_mode: NERtcVideoCropMode; /**< 视频画面裁剪模式，默认kNERtcVideoCropModeDefault。*/
    framerate: NERtcVideoFramerateType; /**< 视频帧率 */
    min_framerate: NERtcVideoFramerateType; /**< 视频最小帧率 */
    bitrate: number; /**< 视频编码码率kbps，取0时使用默认值 */
    min_bitrate: number; /**< 视频编码码率下限kbps，取0时使用默认值 */
    degradation_preference: NERtcDegradationPreference; /**< 编码策略 */
}
/** 音频帧请求格式。*/
export interface NERtcAudioFrameRequestFormat {
    channels: number; /**< 音频频道数量(如果是立体声，数据是交叉的)。单声道: 1；双声道 : 2。*/
    sample_rate: number; /**< 采样率。*/
}
/** 创建混音的配置项 */
export interface NERtcCreateAudioMixingOption {
    path: String; /**< 本地文件全路径或URL < 256 chars */
    loop_count: number; /**< 循环次数， <= 0, 表示无限循环，默认 1 */
    send_enabled: boolean; /**< 是否可发送，默认为 true */
    send_volume: number; /**< 发送音量。最大为 100（默认）含义（0%-100%）*/
    playback_enabled: boolean; /**< 是否可回放，默认为 true */
    playback_volume: number; /**< 回放音量。最大为 100（默认）*/
}
/** 创建音效的配置项 */
export interface NERtcCreateAudioEffectOption {
    path: String; /**< 本地文件全路径或URL < 256 chars */
    loop_count: number; /**< 循环次数， <= 0, 表示无限循环，默认 1 */
    send_enabled: boolean; /**< 是否可发送，默认为 true */
    send_volume: number; /**< 发送音量。最大为 100（默认）含义（0%-100%）*/
    playback_enabled: boolean; /**< 是否可回放，默认为 true */
    playback_volume: number; /**< 回放音量。最大为 100（默认）*/
}
/** 待共享区域相对于整个屏幕或窗口的位置，如不填，则表示共享整个屏幕或窗口。*/
export interface NERtcRectangle {
    x: number; /**< 左上角的横向偏移。*/
    y: number; /**< 左上角的纵向偏移。*/
    width: number; /**< 待共享区域的宽。*/
    height: number; /**< 待共享区域的高。*/
}
/** 屏幕共享编码参数配置。*/
export declare enum NERtcScreenProfileType {
    kNERtcScreenProfile480P = 0,
    kNERtcScreenProfileHD720P = 1,
    kNERtcScreenProfileHD1080P = 2,
    kNERtcScreenProfileCustom = 3,
    kNERtcScreenProfileNone = 4,
    kNERtcScreenProfileMAX = 2
}
/** 视频类型。*/
export declare enum NERtcVideoType {
    kNERtcVideoTypeI420 = 0,
    kNERtcVideoTypeNV12 = 1,
    kNERtcVideoTypeNV21 = 2,
    kNERtcVideoTypeBGRA = 3,
    kNERtcVideoTypeCVPixelBuffer = 4
}
/** 视频旋转角度。*/
export declare enum NERtcVideoRotation {
    kNERtcVideoRotation_0 = 0,
    kNERtcVideoRotation_90 = 90,
    kNERtcVideoRotation_180 = 180,
    kNERtcVideoRotation_270 = 270
}
/** 视频帧。*/
export interface NERtcVideoFrame {
    format: NERtcVideoType; /**< 视频帧格式，详细信息请参考 NERtcVideoType。*/
    timestamp: number; /**< 视频时间戳，单位为毫秒。 */
    width: number; /**< 视频桢宽度 */
    height: number; /**< 视频桢宽高 */
    rotation: NERtcVideoRotation; /**<  视频旋转角度 详见: #NERtcVideoRotation */
    buffer: ArrayBuffer; /**<  视频桢数据 */
}
/** 音频类型。*/
export declare enum NERtcAudioType {
    kNERtcAudioTypePCM16 = 0
}
/** 音频格式。*/
export interface NERtcAudioFormat {
    type: NERtcAudioType; /**< 音频类型。*/
    channels: number; /**< 音频声道数量。如果是立体声，数据是交叉的。单声道: 1；双声道 : 2。*/
    sample_rate: number; /**< 采样率。*/
    bytes_per_sample: number; /**< 每个采样点的字节数。对于 PCM 来说，一般使用 16 bit，即两个字节。*/
    samples_per_channel: number; /**< 每个房间的样本数量。*/
}
/** 音频帧。*/
export interface NERtcAudioFrame {
    format: NERtcAudioFormat; /**< 音频格式。*/
    data: ArrayBuffer; /**< 数据缓冲区。有效数据长度为：samples_per_channel * channels * bytes_per_sample。*/
}
/** 视频尺寸。*/
export interface NERtcVideoDimensions {
    width: number; /**< 宽度 */
    height: number; /**< 高度 */
}
/** 屏幕共享功能的编码策略倾向
- kNERtcSubStreamContentPreferMotion: 内容类型为动画;当共享的内容是视频、电影或游戏时，推荐选择该内容类型
当用户设置内容类型为动画时，按用户设置的帧率处理
- kNERtcSubStreamContentPreferDetails: 内容类型为细节;当共享的内容是图片或文字时，推荐选择该内容类型
当用户设置内容类型为细节时，最高允许用户设置到10帧，设置超过10帧时，不生效，按10帧处理
 */
export declare enum NERtcSubStreamContentPrefer {
    kNERtcSubStreamContentPreferMotion = 0,
    kNERtcSubStreamContentPreferDetails = 1
}
/** 屏幕共享编码参数配置。用于衡量编码质量。一期只支持profile设置。*/
export interface NERtcScreenCaptureParameters {
    profile: NERtcScreenProfileType; /**< 屏幕共享编码参数配置。*/
    dimensions: NERtcVideoDimensions; /**< 屏幕共享视频发送的最大像素值，kNERtcScreenProfileCustom下生效。*/
    frame_rate: number; /**< 共享视频的帧率，kNERtcScreenProfileCustom下生效，单位为 fps；默认值为 5，建议不要超过 15。*/
    bitrate: number; /**< 共享视频的码率，单位为 bps；默认值为 0，表示 SDK 根据当前共享屏幕的分辨率计算出一个合理的值。*/
    capture_mouse_cursor: boolean; /**< 是否采集鼠标用于屏幕共享。*/
    window_focus: boolean; /**< 调用 startScreenCaptureByWindowId 方法共享窗口时，是否将该窗口前置。*/
    excluded_window_list: Array<Number>; /**< 待屏蔽窗口的 ID 列表。 */
    excluded_window_count: number; /**< 待屏蔽窗口的数量。*/
    prefer: NERtcSubStreamContentPrefer; /**< 编码策略倾向。*/
}
export interface NERtcScreenCaptureWindowParam {
    window_list: Array<Number>; /**< 待屏蔽窗口的 ID 列表。 */
}
/** 图片水印设置参数。最多可以添加 4 个图片水印。 */
export interface NERtcImageWatermarkConfig {
    image_paths: Array<String>; /**< 水印图片路径。空时无效。*/
    offset_x: number; /**< 水印图片左上角与视频画布左上角的水平距离。单位为像素（pixel），默认值为 0。 */
    offset_y: number; /**< 水印图片左上角与视频画布左上角的垂直距离。单位为像素（pixel），默认值为 0。 */
    image_width: number; /**< 水印图片的宽度。单位为像素（pixel），默认值为 0 表示按原始图宽。*/
    image_height: number; /**< 水印图片的高度。单位为像素（pixel），默认值为 0 表示按原始图高。*/
    fps: number; /**< 播放帧率。默认 0 帧/秒，即不自动切换图片，图片单帧静态显示。注意：Windows端帧率不超过 20 fps。*/
    loop: boolean; /**< 是否设置循环。默认循环，设置为 false 后水印数组播放完毕后消失。*/
}
/** 文字水印设置参数。最多可添加 10 个文字水印。*/
export interface NERtcTextWatermarkConfig {
    content: String;
    font_path: String; /**< 字体路径，设置为空时，表示使用程序默认字体。*/
    font_size: number; /**< 字体大小。默认值为 10，相当于 144 dpi 设备上的 10 x 15 磅。*/
    font_color: number; /**< 字体颜色。ARGB 格式。 */
    offset_x: number; /**< 水印左上角与视频画布左上角的水平距离。单位为像素（pixel）。*/
    offset_y: number; /**< 水印左上角与视频画布左上角的垂直距离。单位为像素（pixel）。*/
    wm_color: number; /**< 水印框内背景颜色。ARGB格式，支持透明度设置。*/
    wm_width: number; /**< 水印框的宽度。单位为像素（pixel），默认值为 0，表示没有水印框。*/
    wm_height: number; /**< 水印框的高度。单位为像素（pixel），默认值为 0，表示没有水印框。*/
}
/** 时间戳水印设置。只能添加 1 个时间戳水印。 时间戳水印的时间和当前时间相同，且实时变化。*/
export interface NERtcTimestampWatermarkConfig {
    font_path: String; /**< 字体路径。若未设置，使用程序默认字体。*/
    font_size: number; /**< 字体大小。默认值为 10，相当于 144 dpi 设备上的 10 x 15 磅。*/
    font_color: number; /**< 字体颜色。ARGB 格式。 */
    offset_x: number; /**< 水印左上角与视频画布左上角的水平距离。单位为像素（pixel）。 */
    offset_y: number; /**< 水印左上角与视频画布左上角的垂直距离。单位为像素（pixel）。 */
    wm_color: number; /**< 水印框内背景颜色。ARGB格式，支持透明度设置。 */
    wm_width: number; /**< 水印框的宽度。单位为像素（pixel），默认值为 0，表示没有水印框。*/
    wm_height: number; /**< 水印框的高度。单位为像素（pixel），默认值为 0，表示没有水印框。*/
    ts_type: number; /**< 时间戳类型，支持设置为：
                                    - 1：yyyy-MM-dd HH:mm:ss。
                                    - 2：yyyy-MM-dd HH:mm:ss.SSS。精确到毫秒。*/
}
/** 画布水印设置。同时设置文字、时间戳或图片水印时，如果不同类型的水印位置有重叠，会按照图片、文本、时间戳的顺序进行图层覆盖。*/
export interface NERtcCanvasWatermarkConfig {
    image_watermarks: Array<NERtcImageWatermarkConfig>; /**< 图片水印数组指针。 */
    image_count_: number; /**< 图片水印个数，最多支持 4 个。 */
    text_watermarks: Array<NERtcTextWatermarkConfig>; /**< 文字水印数组指针。 */
    text_count_: number; /**< 文字水印个数，最多支持 10 个。 */
    timestamp_watermark: NERtcTimestampWatermarkConfig; /**< 时间戳水印指针，仅一个。 */
}
/** 录音音质 */
export declare enum NERtcAudioRecordingQuality {
    kNERtcAudioRecordingQualityLow = 0,
    kNERtcAudioRecordingQualityMedium = 1,
    kNERtcAudioRecordingQualityHigh = 2
}
/** 媒体优先级类型。*/
export declare enum NERtcMediaPriorityType {
    kNERtcMediaPriorityHigh = 50,
    kNERtcMediaPriorityNormal = 100
}
/** 直播推流模式 */
export declare enum NERtcLiveStreamMode {
    kNERtcLsModeVideo = 0,
    kNERtcLsModeAudio = 1
}
/** 直播推流视频裁剪模式 */
export declare enum NERtcLiveStreamVideoScaleMode {
    kNERtcLsModeVideoScaleFit = 0,
    kNERtcLsModeVideoScaleCropFill = 1
}
/** 直播推流音频采样率 */
export declare enum NERtcLiveStreamAudioSampleRate {
    kNERtcLiveStreamAudioSampleRate32000 = 32000,
    kNERtcLiveStreamAudioSampleRate44100 = 44100,
    kNERtcLiveStreamAudioSampleRate48000 = 48000 /**< （默认）采样率为 48 kHz。*/
}
/** 直播推流音频编码规格 */
export declare enum NERtcLiveStreamAudioCodecProfile {
    kNERtcLiveStreamAudioCodecProfileLCAAC = 0,
    kNERtcLiveStreamAudioCodecProfileHEAAC = 1 /**< HE-AAC 规格，表示高效音频编码规格。*/
}
/** 直播成员布局 */
export interface NERtcLiveStreamUserTranscoding {
    uid: number; /**< 用户id */
    video_push: boolean; /**< 是否推送该用户视频流，kNERtcLsModeAudio时无效 */
    adaption: NERtcLiveStreamVideoScaleMode; /**< 视频流裁剪模式 */
    x: number; /**< 画面离主画面左边距 */
    y: number; /**< 画面离主画面上边距 */
    width: number; /**< 画面在主画面的显示宽度，画面右边超出主画面会失败 */
    height: number; /**< 画面在主画面的显示高度，画面底边超出主画面会失败 */
    audio_push: boolean; /**< 是否推送该用户音频流 */
}
/** 图片布局 */
export interface NERtcLiveStreamImageInfo {
    url: String; /**< 图片地址 */
    x: number; /**< 画面离主画面左边距 */
    y: number; /**< 画面离主画面上边距 */
    width: number; /**< 画面在主画面的显示宽度，画面右边超出主画面会失败 */
    height: number; /**< 画面在主画面的显示高度，画面底边超出主画面会失败 */
}
/** 直播布局 */
export interface NERtcLiveStreamLayout {
    width: number; /**< 视频推流宽度 */
    height: number; /**< 视频推流高度 */
    background_color: number; /**< 视频推流背景色，(R & 0xff) << 16 | (G & 0xff) << 8 | (B & 0xff) */
    user_count: number; /**< 成员布局个数 */
    users: Array<NERtcLiveStreamUserTranscoding>; /**< 成员布局数组 */
    bg_image: NERtcLiveStreamImageInfo; /**< 背景图信息 */
}
export interface NERtcLiveConfig {
    single_video_passthrough: boolean; /**< 单路视频透传开关，默认为关闭状态，开启后，如果房间中只有一路视频流输入， 则不对输入视频流进行转码，不遵循转码布局，直接推流 CDN，如果有多个房间成员视频流混合为一路流，则该设置失效，并在恢复为一个成员画面（单路流）时也不会恢复。*/
    audio_bitrate: number; /**< 音频推流码率，单位为 kbps，取值范围为 10~192，语音场景建议设置为 64 及以上码率，音乐场景建议设置为 128 及以上码率。 */
    sample_rate: NERtcLiveStreamAudioSampleRate; /**< 音频推流采样率。单位为Hz。默认为 kNERtcLiveStreamAudioSampleRate48000，即采样率为 48 kHz。 */
    channels: number; /**< 音频推流声道数，默认值为 2 双声道。 */
    audio_codec_profile: NERtcLiveStreamAudioCodecProfile; /**< 音频编码规格。默认值 NERtcLiveStreamAudioCodecProfileLCAAC，普通编码规格，0: LC-AAC 规格，表示基本音频编码规格，1: HE-AAC 规格，表示高效音频编码规格 */
}
/** 直播推流任务的配置项。*/
export interface NERtcLiveStreamTaskInfo {
    task_id: String; /**< 推流任务ID，为推流任务的唯一标识，用于过程中增删任务操作 < 64 chars*/
    stream_url: String; /**< 直播推流地址 <256 chars */
    server_record_enabled: Boolean; /**< 服务器录制功能是否开启 */
    ls_mode: NERtcLiveStreamMode; /**< 直播推流模式，NERtcLiveStreamMode */
    layout: NERtcLiveStreamLayout; /**< 视频布局 */
    config: NERtcLiveConfig; /**< 音视频流编码参数等设置。详细信息请参考 NERtcLiveConfig */
}
/** 音频设备链接类型。*/
export declare enum NERtcAudioDeviceTransportType {
    kNERtcAudioDeviceTransportTypeUnknown = 0,
    kNERtcAudioDeviceTransportTypeBluetooth = 1,
    kNERtcAudioDeviceTransportTypeBluetoothA2DP = 2,
    kNERtcAudioDeviceTransportTypeBluetoothLE = 3,
    kNERtcAudioDeviceTransportTypeUSB = 4,
    kNERtcAudioDeviceTransportTypeHDMI = 5,
    kNERtcAudioDeviceTransportTypeBuiltIn = 6,
    kNERtcAudioDeviceTransportTypeThunderbolt = 7,
    kNERtcAudioDeviceTransportTypeAirPlay = 8,
    kNERtcAudioDeviceTransportTypeVirtual = 9,
    kNERtcAudioDeviceTransportTypeOther = 10
}
/** 摄像头设备链接类型。*/
export declare enum NERtcVideoDeviceTransportType {
    kNERtcVideoDeviceTransportTypeUnknown = 0,
    kNERtcVideoDeviceTransportTypeUSB = 1,
    kNERtcVideoDeviceTransportTypeVirtual = 2,
    kNERtcVideoDeviceTransportTypeOther = 3
}
export interface NERtcDevice {
    device_id: String;
    device_name: String;
    transport_type: number;
    suspected_unavailable: boolean;
    system_default_device: boolean;
}
/** 参会者角色类型 */
export declare enum NERtcClientRole {
    kNERtcClientRoleBroadcaster = 0,
    kNERtcClientRoleAudience = 1
}
/** SEI发送的流通道类型 */
export declare enum NERtcStreamChannelType {
    kNERtcStreamChannelTypeMainStream = 0,
    kNERtcStreamChannelTypeSubStream = 1
}
export interface NERtcPullExternalAudioFrameCb {
    (data: ArrayBuffer): void;
}
/** 音频流类型，目前同时支持音频两路流：主流和辅流 */
export declare enum NERtcAudioStreamType {
    kNERtcAudioStreamMain = 0,
    kNERtcAudioStreamSub = 1
}
export declare enum NERtcVideoStreamType {
    kNERtcVideoStreamMain = 0,
    kNERtcVideoStreamSub = 1 /** 视频流副流 */
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
    setRecordDevice(id: String): number;
    getRecordDevice(): String;
    enumeratePlayoutDevices(): Array<NERtcDevice>;
    setPlayoutDevice(id: String): number;
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
    setDevice(id: String): number;
    getDevice(): String;
    setLocalVideoMirrorMode(mode: NERtcVideoMirrorMode): number;
    setClientRole(role: NERtcClientRole): number;
    setupSubStreamVideoCanvas(uid: number, enabled: Boolean): number;
    subscribeRemoteVideoSubStream(uid: number, sub: boolean): number;
    enumerateScreenCaptureSourceInfo(thumbWidth: number, thumbHeight: number, iconWidth: number, iconHeight: number): Array<Object>;
    startSystemAudioLoopbackCapture(): number;
    stopSystemAudioLoopbackCapture(): number;
    setSystemAudioLoopbackCaptureVolume(volume: number): number;
    sendSEIMsg(data: ArrayBuffer): number;
    sendSEIMsgEx(data: ArrayBuffer, type: NERtcStreamChannelType): number;
    setExternalAudioRender(enable: boolean, sampleRate: number, channels: number): number;
    pullExternalAudioFrame(pullLength: number, cb: NERtcPullExternalAudioFrameCb): number;
    setAudioEffectPreset(type: NERtcVoiceChangerType): number;
    setVoiceBeautifierPreset(type: NERtcVoiceBeautifierType): number;
    setLocalVoicePitch(pitch: number): number;
    setLocalVoiceEqualization(bandFrequency: NERtcVoiceEqualizationBand, bandGain: number): number;
    adjustUserPlaybackSignalVolume(uid: number, volume: number): number;
    switchChannel(token: String, channelName: String): number;
    setLocalRenderMode(scalingMode: NERtcVideoScalingMode): number;
    setLocalSubStreamRenderMode(scalingMode: NERtcVideoScalingMode): number;
    setRemoteRenderMode(uid: number, scalingMode: NERtcVideoScalingMode): number;
    setLocalMediaPriority(priority: NERtcMediaPriorityType, preemptive: boolean): number;
    setExcludeWindowList(param: NERtcScreenCaptureWindowParam): number;
    startAudioRecording(filePath: String, sampleRate: number, quality: NERtcAudioRecordingQuality): number;
    stopAudioRecording(): number;
    setRemoteSubSteamRenderMode(uid: number, scalingMode: NERtcVideoScalingMode): number;
    setMixedAudioFrameParameters(samplerate: number): number;
    setExternalAudioSource(enabled: boolean, samplerate: number, channel: number): number;
    setExternalVideoSource(enabled: boolean): number;
    pushExternalVideoFrame(opt: NERtcVideoFrame): number;
    pushExternalAudioFrame(opt: NERtcAudioFrame): number;
}
/** 通话相关的统计信息。*/
export interface NERtcStats {
    cpu_app_usage: number; /**< 当前 App 的 CPU 使用率 (%)。*/
    cpu_idle_usage: number; /**< 当前系统的 CPU 空闲率 (%)。*/
    cpu_total_usage: number; /**< 当前系统的 CPU 使用率 (%)。*/
    memory_app_usage: number; /**< 当前App的内存使用率 (%)。*/
    memory_total_usage: number; /**< 当前系统的内存使用率 (%)。*/
    memory_app_kbytes: number; /**< 当前App的内存使用量 (KB)。*/
    total_duration: number; /**< 通话时长（秒）。*/
    tx_bytes: number; /**< 发送字节数，累计值。(bytes)*/
    rx_bytes: number; /**< 接收字节数，累计值。(bytes)*/
    tx_audio_bytes: number; /**< 音频发送字节数，累计值。(bytes)*/
    tx_video_bytes: number; /**< 视频发送字节数，累计值。(bytes)*/
    rx_audio_bytes: number; /**< 音频接收字节数，累计值。(bytes)*/
    rx_video_bytes: number; /**< 视频接收字节数，累计值。(bytes)*/
    tx_audio_kbitrate: number; /**< 音频发送码率。(kbps)*/
    rx_audio_kbitrate: number; /**< 音频接收码率。(kbps)*/
    tx_video_kbitrate: number; /**< 视频发送码率。(kbps)*/
    rx_video_kbitrate: number; /**< 视频接收码率。(kbps)*/
    up_rtt: number; /**< 上行平均往返时延rtt(ms) */
    tx_audio_packet_loss_rate: number; /**< 本地上行音频实际丢包率。(%) */
    tx_video_packet_loss_rate: number; /**< 本地上行视频实际丢包率。(%) */
    tx_audio_packet_loss_sum: number; /**< 本地上行音频实际丢包数。 */
    tx_video_packet_loss_sum: number; /**< 本地上行视频实际丢包数。 */
    tx_audio_jitter: number; /**< 本地上行音频抖动计算。(ms) */
    tx_video_jitter: number; /**< 本地上行视频抖动计算。(ms) */
    rx_audio_packet_loss_rate: number; /**< 本地下行音频实际丢包率。(%) */
    rx_video_packet_loss_rate: number; /**< 本地下行视频实际丢包率。(%) */
    rx_audio_packet_loss_sum: number; /**< 本地下行音频实际丢包数。 */
    rx_video_packet_loss_sum: number; /**< 本地下行视频实际丢包数。 */
    rx_audio_jitter: number; /**< 本地下行音频抖动计算。(ms) */
    rx_video_jitter: number; /**< 本地下行视频抖动计算。(ms) */
}
/** 本地视频流上传统计信息。*/
export interface NERtcVideoSendStats {
    width: number; /**< 视频流宽（像素）。*/
    height: number; /**< 视频流高（像素）。*/
    capture_frame_rate: number; /**< 视频采集帧率。*/
    encoder_frame_rate: number; /**< （上次统计后）编码帧率 (fps)。*/
    sent_bitrate: number; /**< （上次统计后）发送码率(Kbps)。*/
    sent_frame_rate: number; /**< （上次统计后）发送帧率 (fps)。*/
    target_bitrate: number; /**< （上次统计后）编码器目标码率(Kbps)。*/
}
/** 远端视频流的统计信息。*/
export interface NERtcVideoRecvStats {
    uid: number; /**< 用户 ID，指定是哪个用户的视频流。*/
    width: number; /**< 视频流宽（像素）。*/
    height: number; /**< 视频流高（像素）。*/
    received_bitrate: number; /**< （上次统计后）接收到的码率(Kbps)。*/
    packet_loss_rate: number; /**< 下行丢包率(%)。*/
    decoder_frame_rate: number; /**< （上次统计后）解码帧率 (fps)。*/
    render_frame_rate: number; /**< （上次统计后）渲染帧率 (fps)。*/
    received_frame_rate: number; /**< （上次统计后）接收到的帧率 (fps)。*/
    total_frozen_time: number; /**< 用户的下行视频卡顿累计时长(ms)。*/
    frozen_rate: number; /**< 用户的下行视频平均卡顿率(%)。*/
}
/** 本地音频流上传统计信息。*/
export interface NERtcAudioSendStats {
    num_channels: number; /**< 当前采集声道数。*/
    sent_sample_rate: number; /**< 本地上行音频采样率。*/
    sent_bitrate: number; /**< （上次统计后）发送码率(Kbps)。*/
    audio_loss_rate: number; /**< 特定时间内的音频丢包率 (%)。*/
    rtt: number; /**< RTT。*/
    volume: number; /**< 音量，范围为 0（最低）- 100（最高）。*/
}
/** 远端用户的音频统计。*/
export interface NERtcAudioRecvStats {
    uid: number; /**< 用户 ID，指定是哪个用户的音频流。*/
    received_bitrate: number; /**< （上次统计后）接收到的码率(Kbps)。*/
    total_frozen_time: number; /**< 用户的下行音频卡顿累计时长(ms)。*/
    frozen_rate: number; /**< 用户的下行音频平均卡顿率(%)。*/
    audio_loss_rate: number; /**< 特定时间内的音频丢包率 (%)。*/
    volume: number; /**< 音量，范围为 0（最低）- 100（最高）。*/
}
/** @enum NERtcNetworkQualityType 网络质量类型。*/
export declare enum NERtcNetworkQualityType {
    kNERtcNetworkQualityUnknown = 0,
    kNERtcNetworkQualityExcellent = 1,
    kNERtcNetworkQualityGood = 2,
    kNERtcNetworkQualityPoor = 3,
    kNERtcNetworkQualityBad = 4,
    kNERtcNetworkQualityVeryBad = 5,
    kNERtcNetworkQualityDown = 6
}
/** 网络质量统计信息。*/
export interface NERtcNetworkQualityInfo {
    uid: number; /**< 用户 ID，指定是哪个用户的网络质量统计。*/
    tx_quality: NERtcNetworkQualityType; /**< 该用户的上行网络质量，详见 #NERtcNetworkQualityType.*/
    rx_quality: NERtcNetworkQualityType; /**< 该用户的下行网络质量，详见 #NERtcNetworkQualityType.*/
}
/** 连接状态 */
export declare enum NERtcConnectionStateType {
    kNERtcConnectionStateDisconnected = 1,
    kNERtcConnectionStateConnecting = 2,
    kNERtcConnectionStateConnected = 3,
    kNERtcConnectionStateReconnecting = 4,
    kNERtcConnectionStateFailed = 5
}
/** 驱动安装状态 */
export declare enum NERtcInstallCastAudioDriverResult {
    kNERtcInstallCastAudioDriverSuccess = 0,
    kNERtcInstallCastAudioDriverNotAuthorized = 1,
    kNERtcInstallCastAudioDriverFailed = 2
}
/** 屏幕分享状态 */
export declare enum NERtcScreenCaptureStatus {
    kScreenCaptureStatusStart = 1,
    kScreenCaptureStatusPause = 2,
    kScreenCaptureStatusResume = 3,
    kScreenCaptureStatusStop = 4,
    kScreenCaptureStatusCovered = 5 /**< 屏幕分享的目标窗口被覆盖*/
}
/** 录音回调事件错误码 */
export declare enum NERtcAudioRecordingCode {
    kNERtcAudioRecordErrorSuffix = 1,
    kNERtcAudioRecordOpenFileFailed = 2,
    kNERtcAudioRecordStart = 3,
    kNERtcAudioRecordError = 4,
    kNERtcAudioRecordFinish = 5
}
/** 连接状态变更原因 */
export declare enum NERtcReasonConnectionChangedType {
    kNERtcReasonConnectionChangedLeaveChannel = 1,
    kNERtcReasonConnectionChangedChannelClosed = 2,
    kNERtcReasonConnectionChangedBeKicked = 3,
    kNERtcReasonConnectionChangedTimeOut = 4,
    kNERtcReasonConnectionChangedJoinChannel = 5,
    kNERtcReasonConnectionChangedJoinSucceed = 6,
    kNERtcReasonConnectionChangedReJoinSucceed = 7,
    kNERtcReasonConnectionChangedMediaConnectionDisconnected = 8,
    kNERtcReasonConnectionChangedSignalDisconnected = 9,
    kNERtcReasonConnectionChangedRequestChannelFailed = 10,
    kNERtcReasonConnectionChangedJoinChannelFailed = 11
}
/** 音频设备类型。*/
export declare enum NERtcAudioDeviceType {
    kNERtcAudioDeviceUnknown = 0,
    kNERtcAudioDeviceRecord = /**< 音频采集设备 */ 1,
    kNERtcAudioDevicePlayout = /**< 音频播放设备 */ 2
}
/** 音频设备类型状态。*/
export declare enum NERtcAudioDeviceState {
    kNERtcAudioDeviceActive = 0,
    kNERtcAudioDeviceUnactive = /**< 音频设备未激活 */ 1
}
/** 音乐文件播放状态。
*/
export declare enum NERtcAudioMixingState {
    kNERtcAudioMixingStateFinished = 0,
    kNERtcAudioMixingStateFailed = 1
}
/** @enum NERtcAudioMixingErrorCode 混音音乐文件错误码。
*/
export declare enum NERtcAudioMixingErrorCode {
    kNERtcAudioMixingErrorOK = 0,
    kNERtcAudioMixingErrorFatal = 1,
    kNERtcAudioMixingErrorCanNotOpen = /**< 伴音不能正常打开*/ 2,
    kNERtcAudioMixingErrorDecode = /**<音频解码错误*/ 3,
    kNERtcAudioMixingErrorInterrupt = /**<操作中断码*/ 4,
    kNERtcAudioMixingErrorHttpNotFound = /**<404 file not found，only for http / https*/ 5,
    kNERtcAudioMixingErrorOpen = /**<打开流 / 文件失败*/ 6,
    kNERtcAudioMixingErrorNInfo = /**<获取解码信息失败 / 超时*/ 7,
    kNERtcAudioMixingErrorNStream = /**<无音频流*/ 8,
    kNERtcAudioMixingErrorNCodec = /**<无解码器*/ 9,
    kNERtcAudioMixingErrorNMem = /**<无内存*/ 10,
    kNERtcAudioMixingErrorCodecOpen = /**<解码器打开失败 / 超时*/ 11,
    kNERtcAudioMixingErrorInvalidInfo = /**<无效音频参数（声道、采样率）*/ 12,
    kNERtcAudioMixingErrorOpenTimeout = /**<打开流 / 文件超时*/ 13,
    kNERtcAudioMixingErrorIoTimeout = /**<网络io超时*/ 14,
    kNERtcAudioMixingErrorIo = /**<网络io错误*/ 15
}
/** 声音音量信息。一个数组，包含每个说话者的用户 ID 和音量信息。*/
export interface NERtcAudioVolumeInfo {
    uid: number; /**< 说话者的用户 ID。如果返回的 uid 为 0，则默认为本地用户。*/
    volume: number; /**< 说话者的音量，范围为 0（最低）- 100（最高）。*/
}
/** 直播推流状态。*/
export declare enum NERtcLiveStreamStateCode {
    kNERtcLsStatePushing = 505,
    kNERtcLsStatePushFail = 506,
    kNERtcLsStatePushStopped = 511,
    kNERtcLsStateImageError = 512
}
/** @enum NERtcMediaStatsEventName  SDK 向 App 上报统计信息 */
export declare enum NERtcMediaStatsEventName {
    RtcStats = "onRtcStats",
    LocalAudioStats = "onLocalAudioStats",
    RemoteAudioStats = "onRemoteAudioStats",
    LocalVideoStats = "onLocalVideoStats",
    RemoteVideoStats = "onRemoteVideoStats",
    NetworkQuality = "onNetworkQuality"
}
/** 4.1.1 */
/** 变声 预设值 */
export declare enum NERtcVoiceChangerType {
    kNERtcVoiceChangerOff = 0,
    kNERtcVoiceChangerRobot = 1,
    kNERtcVoiceChangerGaint = 2,
    kNERtcVoiceChangerHorror = 3,
    kNERtcVoiceChangerMature = 4,
    kNERtcVoiceChangerManToWoman = 5,
    kNERtcVoiceChangerWomanToMan = 6,
    kNERtcVoiceChangerManToLoli = 7,
    kNERtcVoiceChangerWomanToLoli = 8 /**< 女变萝莉 */
}
/** 预设的美声效果 */
export declare enum NERtcVoiceBeautifierType {
    kNERtcVoiceBeautifierOff = 0,
    kNERtcVoiceBeautifierMuffled = 1,
    kNERtcVoiceBeautifierMellow = 2,
    kNERtcVoiceBeautifierClear = 3,
    kNERtcVoiceBeautifierMagnetic = 4,
    kNERtcVoiceBeautifierRecordingstudio = 5,
    kNERtcVoiceBeautifierNature = 6,
    kNERtcVoiceBeautifierKTV = 7,
    kNERtcVoiceBeautifierRemote = 8,
    kNERtcVoiceBeautifierChurch = 9,
    kNERtcVoiceBeautifierBedroom = 10,
    kNERtcVoiceBeautifierLive = 11
}
/** 音效均衡波段的中心频率 */
export declare enum NERtcVoiceEqualizationBand {
    kNERtcVoiceEqualizationBand_31 = 0,
    kNERtcVoiceEqualizationBand_62 = 1,
    kNERtcVoiceEqualizationBand_125 = 2,
    kNERtcVoiceEqualizationBand_250 = 3,
    kNERtcVoiceEqualizationBand_500 = 4,
    kNERtcVoiceEqualizationBand_1K = 5,
    kNERtcVoiceEqualizationBand_2K = 6,
    kNERtcVoiceEqualizationBand_4K = 7,
    kNERtcVoiceEqualizationBand_8K = 8,
    kNERtcVoiceEqualizationBand_16K = 9
}
