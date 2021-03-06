/// <reference types="node" />
import { IRenderer } from '../renderer';
import { NERtcEngineAPI, NERtcEngineContext, NERtcChannelProfileType, NERtcRemoteVideoStreamType, NERtcVideoCanvas, NERtcErrorCode, NERtcSessionLeaveReason, NERtcVideoProfileType, NERtcAudioProfileType, NERtcAudioScenarioType, NERtcVideoConfig, NERtcCreateAudioMixingOption, NERtcCreateAudioEffectOption, NERtcRectangle, NERtcScreenCaptureParameters, NERtcDevice, NERtcStats, NERtcAudioSendStats, NERtcAudioRecvStats, NERtcVideoSendStats, NERtcVideoRecvStats, NERtcNetworkQualityInfo, NERtcClientRole, NERtcConnectionStateType, NERtcReasonConnectionChangedType, NERtcAudioDeviceType, NERtcAudioDeviceState, NERtcAudioMixingState, NERtcAudioMixingErrorCode, NERtcAudioVolumeInfo, NERtcLiveStreamStateCode, NERtcLiveStreamTaskInfo, NERtcVideoMirrorMode, NERtcVideoScalingMode, NERtcVoiceChangerType, NERtcVoiceBeautifierType, NERtcVoiceEqualizationBand, NERtcStreamChannelType, NERtcPullExternalAudioFrameCb, NERtcAudioStreamType, NERtcVideoStreamType, NERtcInstallCastAudioDriverResult } from './defs';
import { EventEmitter } from 'events';
/**
 * @class NERtcEngine
 */
declare class NERtcEngine extends EventEmitter {
    nertcEngine: NERtcEngineAPI;
    renderers: Map<string, IRenderer>;
    substreamRenderers: Map<string, IRenderer>;
    renderMode: 1 | 2 | 3;
    customRenderer: any;
    /**
     * NERtcEngine类构造函数
     * @returns {NERtcEngine}
     */
    constructor();
    /**
     * 初始化 NERTC SDK 服务。
     * <pre>
     * 该方法用来进行初始化 NERTC 服务。传入云信为开发者签发的厂商秘钥进行初始化。在使用其他方法前，必须先调用该方法进行初始化。
     * 初始化成功后，默认处于音视频通话模式。
     * <b>NOTE:</b>
     * - 必须使用同一个 App Key 才能进行通话。
     * - 一个 NERtcEngine 实例对象只能使用一个 App Key。如需更换 App Key，必须先调用 {@link NERtcEngine#release} 方法释放当前实例资源再调用本方法初始化实例。
     * </pre>
     * @param {Object} context 传入的RTC engine context对象: NERtcEngineContext.
     * @param {String} context.app_key 用户注册云信的APP Key。如果你的开发包里面缺少 APP Key，请申请注册一个新的 APP Key。
     * @param {String} context.log_dir_path 日志目录的完整路径，采用UTF-8 编码。
     * @param {number} [context.log_level=3] 日志级别，默认级别为 kNERtcLogLevelInfo。
     * <pre>
     * - 0 Fatal级别日志信息
     * - 1 Error级别日志信息
     * - 2 Warning级别日志信息
     * - 3 Info级别日志信息。默认级别
     * - 4 DetailInfo级别日志信息
     * - 5 Verbos级别日志信息
     * - 6 Debug级别日志信息。如果你想获取最完整的日志，可以将日志级别设为该等级
     * - 7 不输出日志信息
     * </pre>
     * @param {number} [context.log_file_max_size_KBytes=20480] 指定 SDK 输出日志文件的大小上限，单位为 KB。如果设置为 0，则默认为 20 M。
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    initialize(context: NERtcEngineContext): number;
    /**
     * 释放资源。
     */
    release(): void;
    /**
     * 设置参会者角色
     * <pre>
     * {@link NERtcEngine#setClientRole} 接口用于在直播场景中设置用户角色。默认情况下用户以主播角色加入房间。
     * 在加入房间前，用户需要调用setClientRole 接口设置本端模式为观众或主播模式。在加入房间后，用户可以通过本接口切换用户模式。
     * 用户角色支持设置为主播或观众，主播和观众的权限不同。默认情况下用户以主播角色加入房间。
     * 可以在通话前后设置，通话前设置会在加入频道之后生效。如果设置观众模式。则会停止音视频设备。
     * </pre>
     * @param {number} role 参会者模式
     * <pre>
     * - 主播(0)：可以开关摄像头等设备、可以发布流、可以操作互动直播推流相关接口、上下线对其他房间内用户可见
     * - 观众(1)：不可以开关摄像头等设备、不可以发布流、不可以操作互动直播推流相关接口、上下线对其他房间内用户不可见
     * </pre>
     * @fires NERtcEngine#onClientRoleChanged
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    setClientRole(role: NERtcClientRole): number;
    /**
     * 设置通话模式
     * <pre>
     * 必须在加入通话前设置，开始通话后设置无效，结束通话后保留之前的设置。
     * </pre>
     * @param {number} profile 场景模式
     * <pre>
     * - 通话场景(0)
     * - 直播推流场景(1)
     * </pre>
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    setChannelProfile(profile: NERtcChannelProfileType): number;
    /**
     * 加入频道。如果频道还未创建，会自动尝试创建频道。
     * <pre>
     * 该方法让用户加入通话频道，在同一个频道内的用户可以互相通话，多个用户加入同一个频道，可以群聊。 使用不同 App Key 的 App 是不能互通的。如果已在通话中，用户必须调用 {@link NERtcEngine#leaveChannel} 退出当前通话，才能进入下一个频道。
     * 频道内每个用户的用户 ID 必须是唯一的。
     * </pre>
     * @param {String} token 动态秘钥。安全要求不高: 将值设为 空字符串。安全要求高: 将值设置为 Token。如果你已经启用了 App Certificate, 请务必使用 Token。
     * @param {String} channelName 标识通话的频道名称，长度在 64 字节以内的字符串。以下为支持的字符集范围（共 89 个字符）: a-z, A-Z, 0-9, space, !#$%&()+-:;&le;.,>? @[]^_{|}~”
     * @param {number} uid 用户ID。
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    joinChannel(token: String, channelName: String, uid: number): number;
    /**
     * 离开频道。
     * <pre>
     * 离开频道，即挂断或退出通话。
     * 当调用 {@link NERtcEngine#joinChannel} 方法后，必须调用 {@link NERtcEngine#leaveChannel} 结束通话，否则无法开始下一次通话。 不管当前是否在通话中，都可以调用 leaveChannel，没有副作用。该方法会把会话相关的所有资源释放掉。
     * 该方法是异步操作，调用返回时并没有真正退出频道。在真正退出频道后，SDK 会触发 onLeaveChannel 事件。
     * 如果你调用了 {@link NERtcEngine#leaveChannel} 后立即调用 {@link NERtcEngine#release} , SDK 将无法触发 onLeaveChannel 事件。
     * </pre>
     * @fires NERtcEngine#onLeaveChannel
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    leaveChannel(): number;
    /**
     * 开启或关闭本地语音采集或处理
     * <pre>
     * 该方法可以重新开启本地语音功能，重新开始本地音频采集及处理。
     * 该方法不影响接收或播放远端音频流。
     * <b>NOTE:</b>
     * - 该方法与 {@link NERtcEngine#muteLocalAudioStream} 的区别在于:
     * - {@link NERtcEngine#enableLocalAudio}: 开启本地语音采集及处理
     * - {@link NERtcEngine#muteLocalAudioStream}: 停止或继续发送本地音频流
     * 该方法设置内部引擎为启用状态，在 {@link NERtcEngine#leaveChannel} 后仍然有效。
     * </pre>
     * @param {Boolean} enabled
     * <pre>
     * - true: 重新开启本地语音功能，即开启本地语音采集或处理（默认）
     * - false: 关闭本地语音功能，即停止本地语音采集或处理
     * </pre>
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    enableLocalAudio(enabled: Boolean): number;
    /**
     * 设置本地视图。
     * <pre>
     * 该方法设置本地视频显示信息。App 通过调用此接口绑定本地视频流的显示视窗(view)。 在 App 开发中，通常在初始化后调用该方法进行本地视频设置，然后再加入频道。
     * </pre>
     * @param {Object} canvas 视频画布信息
     * @param {number} canvas.mode 视频画布缩放模式
     * <pre>
     * - 0 视频尺寸等比缩放。优先保证视频内容全部显示。因视频尺寸与显示视窗尺寸不一致造成的视窗未被填满的区域填充黑色。
     * - 1 视频尺寸非等比缩放。保证视频内容全部显示，且填满视窗。
     * - 2 视频尺寸等比缩放。优先保证视窗被填满。因视频尺寸与显示视窗尺寸不一致而多出的视频将被截掉。
     * </pre>
     * @param {Element} canvas.view 视频画布对象
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    setupLocalVideoCanvas(canvas: NERtcVideoCanvas): number;
    /**
     * 设置远端用户视图。
     * <pre>
     * 该方法绑定远端用户和显示视图，即设定 uid 指定的用户用哪个视图显示。调用该接口时需要指定远端视频的 uid，一般可以在用户加入后设置好。
     * 如果 App 不能事先知道对方的 uid，可以在 APP 收到 onUserJoined 事件时设置。
     * 退出频道后，SDK 会把远端用户的绑定关系清除掉。
     * </pre>
     * @param {number} uid 远端用户 ID。
     * @param {Object} canvas 视频画布信息
     * @param {number} canvas.mode 视频画布缩放模式
     * <pre>
     * - 0 视频尺寸等比缩放。优先保证视频内容全部显示。因视频尺寸与显示视窗尺寸不一致造成的视窗未被填满的区域填充黑色。
     * - 1 视频尺寸非等比缩放。保证视频内容全部显示，且填满视窗。
     * - 2 视频尺寸等比缩放。优先保证视窗被填满。因视频尺寸与显示视窗尺寸不一致而多出的视频将被截掉。
     * </pre>
     * @param {Element} canvas.view 视频画布对象
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    setupRemoteVideoCanvas(uid: number, canvas: NERtcVideoCanvas): number;
    /**
     * 在指定画布上截图
     * @param {'local'|number} uid 要截图的 uid，本地视频为 local，远端用户为远端 uid
     * @param {NERtcVideoStreamType} streamType 流类型：
     * <pre>
     * 0 - 视频流主流
     * 1 - 视频流副流
     * </pre>
     * @returns string 返回 base64 截图数据
     */
    captureImageByUid(uid: 'local' | number, streamType?: NERtcVideoStreamType): string;
    /**
     * 开启或关闭本地视频采集和渲染
     * <pre>
     * 该方法启用本地视频采集功能。
     * 该方法设置内部引擎为启用状态，在 {@link NERtcEngine#leaveChannel} 后仍然有效。
     * </pre>
     * @param {boolean} enabled 是否启用本地视频:
     * <pre>
     * - true: 开启本地视频采集和渲染 (默认)；
     * - false: 关闭使用本地摄像头设备。关闭后，远端用户会接收不到本地用户的视频流；但本地用户依然可以接收远端用户的视频流。设置为 false 时，该方法不需要本地有摄像头。
     * </pre>
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    enableLocalVideo(enabled: Boolean): number;
    /**
     * 订阅 / 取消订阅指定远端用户的视频流。对方打开视频后需要主动订阅
     * @param {number} uid 指定用户的用户 ID。
     * @param {number} type 流类型:
     * <pre>
     * - 0 默认大流
     * - 1 小流
     * - 2 不订阅
     * </pre>
     * @param {boolean} subscribe
     * <pre>
     * - true: 订阅指定远端用户的视频流；
     * - false: 取消订阅指定远端用户的视频流。
     * </pre>
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
      */
    subscribeRemoteVideoStream(uid: number, type: NERtcRemoteVideoStreamType, subscribe: Boolean): number;
    /**
     * 获取当前网络状态。
     * @returns {number}
     * <pre>
     * - 1 没加入频道。
     * - 2 正在加入频道。
     * - 3 加入频道成功。
     * - 4 正在尝试重新加入频道。
     * - 5 加入频道失败。
     * </pre>
     */
    getConnectionState(): number;
    /**
     * 开关本地音频发送。该方法用于允许/禁止往网络发送本地音频流。
     * <pre>
     * 该方法不影响录音状态，因为并没有禁用录音设备。
     * </pre>
     * @param {boolean} mute 静音/取消静音:
     * <pre>
     * - true: 静音本地音频
     * - false: 取消静音本地音频（默认）
     * </pre>
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    muteLocalAudioStream(enabled: Boolean): number;
    /**
     * 设置音频编码属性。
     * <pre>
     * <b>NOTE:</b>
     * - 该方法需要在 {@link NERtcEngine#joinChannel} 之前设置好， {@link NERtcEngine#joinChannel} 之后设置不生效。
     * - 音乐场景下，建议将 profile 设置为 4.
     * @param {number}  profile 设置采样率，码率，编码模式和声道数:
     * <pre>
     * - 0 默认设置。Speech场景下为 1，Music场景下为 2
     * - 1 普通质量的音频编码，16000Hz，20Kbps
     * - 2 普通质量的音频编码，16000Hz，32Kbps
     * - 3 中等质量的音频编码，48000Hz，32Kbps
     * - 4 中等质量的立体声编码，48000Hz * 2，64Kbps
     * - 5 高质量的音频编码，48000Hz，64Kbps
     * - 6 高质量的立体声编码，48000Hz * 2，128Kbps
     * </pre>
     * @param {number}  scenario 设置音频应用场景:
     * <pre>
     * - 0: 默认设置: 通过{@link NERtcEngine#setChannelProfile}设置通话模式时为语音场景(1)，直播推流模式时为音乐场景(2)。
     * - 1: 语音场景. profile 推荐使用 2 及以下
     * - 2: 音乐场景。profile 推荐使用 3 及以上
     * </pre>
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    setAudioProfile(profile: NERtcAudioProfileType, scenario: NERtcAudioScenarioType): number;
    /**
     * 订阅／取消订阅指定音频流。
     * @param {number} uid 指定用户的 ID
     * @param {boolean} subscribe
     * <pre>
     * - true: 订阅指定音频流（默认）；
     * - false: 取消订阅指定音频流。
     * </pre>
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    subscribeRemoteAudioStream(uid: number, enabled: Boolean): number;
    /**
     * 设置视频配置。
     * <pre>
     * 该方法设置视频配置。每个属性对应一套视频参数，如分辨率等，会在摄像头重启后生效。 所有设置的参数均为理想情况下的最大值。当视频引擎因网络环境等原因无法达到设置的分辨率的最大值时，会取最接近最大值的那个值。
     * </pre>
     * @param {object} config 视频配置:
     * @param {number} config.max_profile 视频编码的分辨率，用于衡量编码质量:
     * <pre>
     * - 0 160x90/120, 15fps
     * - 1 320x180/240, 15fps
     * - 2 640x360/480, 30fps
     * - 3 1280x720, 30fps
     * - 4 1920x1080, 30fps
     * </pre>
     * @param {number} config.width 视频编码自定义分辨率之宽度。width为0表示使用max_profile
     * @param {number} config.height 视频编码自定义分辨率之高度。height为0表示使用max_profile
     * @param {number} config.crop_mode 视频画面裁剪模式:
     * <pre>
     * - 0 Device Defalut
     * - 1 16:9
     * - 2 4:3
     * - 3 1:1
     * </pre>
     * @param {number} config.framerate 视频帧率:
     * <pre>
     * - 0 默认帧率
     * - 7 7帧每秒
     * - 10 10帧每秒
     * - 15 15帧每秒
     * - 24 24帧每秒
     * - 30 30帧每秒
     * - 60 60帧每秒
     * </pre>
     * @param {number} config.min_framerate 视频最小帧率:
     * <pre>
     * - 0 默认帧率
     * - 7 7帧每秒
     * - 10 10帧每秒
     * - 15 15帧每秒
     * - 24 24帧每秒
     * - 30 30帧每秒
     * </pre>
     * @param {number} [config.bitrate=0] 视频编码码率kbps，取0时使用默认值
     * @param {number} [config.min_bitrate=0] 视频编码码率下限kbps，取0时使用默认值
     * @param {number} config.degradation_preference 编码策略:
     * <pre>
     * - 0 使用引擎推荐值。通话场景使用平衡模式，直播推流场景使用清晰优先
     * - 1 帧率优先
     * - 2 清晰度优先
     * - 3 平衡模式
     * </pre>
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    setVideoConfig(config: NERtcVideoConfig): number;
    /**
     * 设置视频双流发送。
     * <pre>
     * 调用该方法控制是否支持发送双流，只对摄像头数据生效，自定义输入、取屏等无效。设置后，会在摄像头重启后生效。
     * </pre>
     * @param {boolean} enable
     * <pre>
     * - true: 双流发送（默认）
     * - false: 只发送一个流
     * </pre>
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    enableDualStreamMode(enabled: Boolean): number;
    /**
     * 设置本地辅流视图。
     * <pre>
     * 该方法设置本地辅流视频显示信息。App 通过调用此接口绑定本地辅流的显示视窗(view)。 在 App 开发中，通常在初始化后调用该方法进行本地视频设置，然后再加入频道。
     * </pre>
     * @param {Object} canvas 视频画布信息
     * @param {number} canvas.mode 视频显示模式
     * <pre>
     * - 0 视频尺寸等比缩放。优先保证视频内容全部显示。因视频尺寸与显示视窗尺寸不一致造成的视窗未被填满的区域填充黑色。
     * - 1 视频尺寸非等比缩放。保证视频内容全部显示，且填满视窗。
     * - 2 视频尺寸等比缩放。优先保证视窗被填满。因视频尺寸与显示视窗尺寸不一致而多出的视频将被截掉。
     * </pre>
     * @param {Element} canvas.view 视频画布对象
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    setupLocalSubStreamVideoCanvas(canvas: NERtcVideoCanvas): number;
    /**
     * 设置本地视图/远端视图显示模式。
     * <pre>
     * 该方法设置视图显示模式。App 可以多次调用此方法更改显示模式。
     * </pre>
     * @param {(string|number)} uid
     * <pre>
     * 'local':本地视图
     * {number}: 远端用户 ID。
     * </pre>
     * @param {number} mode  视频显示模式:
     * <pre>
     * - 0 视频尺寸等比缩放。优先保证视频内容全部显示。因视频尺寸与显示视窗尺寸不一致造成的视窗未被填满的区域填充黑色。
     * - 1 视频尺寸非等比缩放。保证视频内容全部显示，且填满视窗。
     * - 2 视频尺寸等比缩放。优先保证视窗被填满。因视频尺寸与显示视窗尺寸不一致而多出的视频将被截掉。
     * </pre>
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    setRenderMode(uid: 'local' | number, mode: NERtcVideoScalingMode): number;
    /**
     * 设置辅流视图/远端辅流视图显示模式。
     * <pre>
     * 该方法设置视图显示模式。App 可以多次调用此方法更改显示模式。
     * </pre>
     * @param {(string|number)} uid
     * <pre>
     * 'local':本地辅流视图
     * {number}: 远端辅流用户 ID。
     * </pre>
     * @param {number} mode  视频显示模式:
     * <pre>
     * - 0 视频尺寸等比缩放。优先保证视频内容全部显示。因视频尺寸与显示视窗尺寸不一致造成的视窗未被填满的区域填充黑色。
     * - 1 视频尺寸非等比缩放。保证视频内容全部显示，且填满视窗。
     * - 2 视频尺寸等比缩放。优先保证视窗被填满。因视频尺寸与显示视窗尺寸不一致而多出的视频将被截掉。
     * </pre>
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    setSubStreamRenderMode(uid: 'local' | number, mode: NERtcVideoScalingMode): number;
    /**
     * 设置本地视频镜像模式。
     * <pre>
     * 该方法设置本地视频镜像模式。 App 可以多次调用此方法更改镜像模式。
     * </pre>
     * @param {number} mode  视频镜像模式:
     * <pre>
     * - 0 Windows/macOS SDK 启用镜像模式。
     * - 1 启用镜像模式。
     * - 2 （默认）关闭镜像模式。
     * </pre>
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    setLocalVideoMirrorMode(mode: NERtcVideoMirrorMode): number;
    /**
     * 设置远端用户辅流视图。
     * <pre>
     * 该方法绑定远端用户和辅流显示视图，即设定 uid 指定的用户用哪个视图显示。调用该接口时需要指定远端视频的 uid，一般可以在用户加入后设置好。
     * <b>NODE:</b>
     * - 如果 App 不能事先知道对方的 uid，可以通过{@link NERtcEngine#on}方法监听 onUserJoined 事件时设置。
     * - 退出频道后，SDK 会把远端用户的绑定关系清除掉。
     * </pre>
     * @param {number} uid 远端用户 ID。
     * @param {Object} canvas 视频画布信息
     * @param {number} canvas.mode 视频显示模式
     * <pre>
     * - 0 视频尺寸等比缩放。优先保证视频内容全部显示。因视频尺寸与显示视窗尺寸不一致造成的视窗未被填满的区域填充黑色。
     * - 1 视频尺寸非等比缩放。保证视频内容全部显示，且填满视窗。
     * - 2 视频尺寸等比缩放。优先保证视窗被填满。因视频尺寸与显示视窗尺寸不一致而多出的视频将被截掉。
     * </pre>
     * @param {Element} canvas.view 视频画布对象
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    setupRemoteSubStreamVideoCanvas(uid: number, canvas: NERtcVideoCanvas): number;
    /**
     * 订阅 / 取消订阅指定远端用户的视频辅流。
     * <pre>
     * 对方打开视频后需要主动订阅
     * <b>NOTE:</b>
     * - 必须在远端加入房间后调用。
     * - 必须先设置远端辅流画布。
     * </pre>
     * @param {number} uid 指定用户的用户 ID。
     * @param {number} subscribe
     * <pre>
     * - true: 订阅指定远端用户的视频流；
     * - false: 取消订阅指定远端用户的视频流。
     * </pre>
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    subscribeRemoteVideoSubStream(uid: number, sub: boolean): number;
    /**
     * 开启视频预览。
     * <pre>
     * 该方法用于在进入频道前启动本地视频预览。调用该 API 前，必须:
     * - 调用 {@link NERtcEngine#setupLocalVideoCanvas} 设置预览窗口；
     * 启用了本地视频预览后，在进入频道前，本地预览必须先关闭，需要调用 {@link NERtcEngine#stopVideoPreview} 。
     * </pre>
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    startVideoPreview(): number;
    /**
     * 停止视频预览。
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    stopVideoPreview(): number;
    /**
     * 开关本地视频发送。
     * <pre>
     * 调用该方法禁视频流时，SDK 不再发送本地视频流，但摄像头仍然处于工作状态。相比于 {@link NERtcEngine#enableLocalVideo} (false) 用于控制本地视频流发送的方法，该方法响应速度更快。该方法不影响本地视频流获取，没有禁用摄像头。
     * </pre>
     * @param {boolean} mute
     * <pre>
     * - true: 不发送本地视频流
     * - false: 发送本地视频流（默认）
     * </pre>
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    muteLocalVideoStream(enabled: Boolean): number;
    /**
     * 通过 JSON 配置 SDK 提供技术预览或特别定制功能。以标准化方式公开 JSON 选项。
     * @param {object} parameters JSON 字符串形式的参数
     * @param {boolean} parameters.record_host_enabled
     * <pre>
     * - true 录制主讲人
     * - false 不是录制主讲人
     * </pre>
     * @param {boolean} [parameters.record_audio_enabled=false] 启用服务器音频录制。默认值 false
     * @param {boolean} [parameters.record_video_enabled=false] 启用服务器视频录制。默认值 false
     * @param {number} parameters.record_type 录制类型：
     * <pre>
     * - 0 参与混合录制且录制单人文件
     * - 1 参与混合录制
     * - 2 只录单人文件
     * </pre>
     * @param {boolean} [parameters.auto_subscribe_audio=true] 其他用户打开音频时，自动订阅。 默认值 true
     * @param {boolean} [parameters.publish_self_stream_enabled=false]   开启旁路直播。默认值 false
     * @param {number} [parameters.log_level=3] 日志级别，默认级别为 kNERtcLogLevelInfo。
     * <pre>
     * - 0 Fatal级别日志信息
     * - 1 Error级别日志信息
     * - 2 Warning级别日志信息
     * - 3 Info级别日志信息。默认级别
     * - 4 DetailInfo级别日志信息
     * - 5 Verbos级别日志信息
     * - 6 Debug级别日志信息。如果你想获取最完整的日志，可以将日志级别设为该等级
     * - 7 不输出日志信息
     * </pre>
     * @param {boolean} [parameters.audio_processing_aec_enable=true] AEC开关，默认值 true
     * @param {boolean} [parameters.audio_processing_agc_enable=true] AGC开关，默认值 true
     * @param {boolean} [parameters.audio_processing_ns_enable=true] NS开关，默认值 true
     * @param {boolean} [parameters.audio_processing_ai_ns_enable=false] AI NS开关，建议通话前修改，默认值 false
     * @param {boolean} [parameters.audio_processing_external_audiomix_enable=false] 输入混音开关，默认值 false
     * @param {boolean} [parameters.audio_processing_earphone=false] 通知SDK是否使用耳机:
     * <pre>
     * - true: 使用耳机
     * - false: 不使用耳机
     * </pre>
     * @param {number} [parameters.video_sendonpub_type=1] 设置视频发送策略，默认发送大流(1)；通话前设置有效
     * <pre>
     * - 0 不主动发送数据流，被订阅后发送。
     * - 1 主动发送大流。
     * - 2 主动发送小流。
     * - 3 主动发送大小流。
     * </pre>
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    setParameters(parameters: String): number;
    /**
     * 开启音频dump。
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    startAudioDump(): number;
    /**
     * 结束音频dump。
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    stopAudioDump(): number;
    /**
     * 开始播放伴奏。
     * <pre>
     * 指定本地或在线音频文件来和录音设备采集的音频流进行混音。成功调用该方法后，可以通过{@link NERtcEngine#on}方法监听 onAudioMixingStateChanged 播放状态改变事件。
     * 请在频道内调用该方法，如果在频道外调用该方法可能会出现问题。
     * </pre>
     * @param {object} option 创建混音任务配置的选项，包括混音任务类型、混音文件全路径或URL等。支持的音频格式包括: AAC、MP3 和 WAVE。
     * @param {string} option.path 本地文件全路径或URL <256 chars
     * @param {number} [option.loop_count=1] 循环次数， <= 0, 表示无限循环，默认 1
     * @param {boolean} [option.send_enabled=true] 是否可发送，默认为 true
     * @param {number} [option.send_volume=100] 发送音量。最大为 100（默认）含义（0%-100%）
     * @param {boolean} [option.playback_enabled=true] 是否可回放，默认为 true
     * @param {number} [option.playback_volume=100] 回放音量。最大为 100（默认）
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    startAudioMixing(opt: NERtcCreateAudioMixingOption): number;
    /**
     * 停止播放伴奏。
     * <pre>
     * 该方法停止播放伴奏。请在频道内调用该方法。
     * </pre>
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    stopAudioMixing(): number;
    /**
     * 暂停播放伴奏。
     * <pre>
     * 该方法暂停播放伴奏。请在频道内调用该方法。
     * </pre>
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    pauseAudioMixing(): number;
    /**
     * 恢复播放伴奏。
     * <pre>
     * 该方法恢复混音，继续播放伴奏。请在频道内调用该方法。
     * </pre>
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    resumeAudioMixing(): number;
    /**
     * 调节伴奏发送音量。
     * <pre>
     * 该方法调节混音里伴奏的发送音量大小。请在频道内调用该方法。
     * </pre>
     * @param {number} volume 伴奏音量范围为 0~100。默认 100 为原始文件音量。
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    setAudioMixingSendVolume(volume: number): number;
    /**
     * 获取伴奏发送音量。
     * <pre>
     * 该方法获取混音里伴奏的发送音量大小。请在频道内调用该方法。
     * </pre>
     * @returns {number}
     * <pre>
     * - >=0: 伴奏发送音量；
     * - 其他: 方法调用失败。
     * </pre>
     */
    getAudioMixingSendVolume(): number;
    /**
     * 调节伴奏播放音量。
     * <pre>
     * 该方法调节混音里伴奏的播放音量大小。请在频道内调用该方法。
     * </pre>
     * @param {number} volume 伴奏音量范围为 0~100。默认 100 为原始文件音量。
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    setAudioMixingPlaybackVolume(volume: number): number;
    /**
     * 获取伴奏播放音量。
     * <pre>
     * 该方法获取混音里伴奏的播放音量大小。请在频道内调用该方法。
     * </pre>
     * @returns {number}
     * <pre>
     * - >= 0: 伴奏播放音量；
     * - 其他: 方法调用失败。
     * </pre>
     */
    getAudioMixingPlaybackVolume(): number;
    /**
     * 获取伴奏时长。
     * <pre>
     * 该方法获取伴奏时长，单位为毫秒。请在频道内调用该方法。
     * </pre>
     * @returns {number}
     * <pre>
     * - >=0: 伴奏时长，单位为毫秒；
     * - 其他: 方法调用失败。
     * </pre>
     */
    getAudioMixingDuration(): number;
    /**
     * 获取音乐文件的播放进度。
     * <pre>
     * 该方法获取当前伴奏播放进度，单位为毫秒。请在频道内调用该方法。
     * </pre>
     * @returns {number}
     * <pre>
     * - >= 0: 伴奏播放进度，单位为毫秒；
     * - 其他: 方法调用失败。
     * </pre>
     */
    getAudioMixingCurrentPosition(): number;
    /**
     * 设置音乐文件的播放位置。
     * <pre>
     * 该方法可以设置音频文件的播放位置，这样你可以根据实际情况播放文件，而非从头到尾播放整个文件。
     * </pre>
     * @param {number} pos 进度条位置，单位为毫秒。
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    setAudioMixingPosition(pos: number): number;
    /**
     * 播放指定音效文件。
     * <pre>
     * 你可以多次调用该方法，通过传入不同的音效文件的 effect_id 和 option ，同时播放多个音效文件，实现音效叠加。为获得最佳用户体验，我们建议同时播放的音效文件不要超过 3 个。成功调用该方法后，可以通过{@link NERtcEngine#on}方法监听 onAudioEffectFinished 播放结束事件。
     * 请在频道内调用该方法，如果在频道外调用该方法可能会出现问题。
     * </pre>
     * @param {number} effectId 指定音效的 ID。每个音效均有唯一的 ID。
     * @param {object[]} opt 创建音效任务配置的选项，包括混音任务类型、混音文件全路径或URL等。支持的音频格式包括: AAC、MP3 和 WAVE。
     * @param {String} opt[].path 本地文件全路径或URL <256 chars
     * @param {number} [opt[].loop_count=1] 循环次数， <= 0, 表示无限循环，默认 1
     * @param {boolean} [opt[].send_enabled=true] 是否可发送，默认为 true
     * @param {number} opt[].send_volume 发送音量。最大为 100（默认）含义（0%-100%）
     * @param {boolean} [opt[].playback_enabled=true] 是否可回放，默认为 true
     * @param {number} opt[].playback_volume  回放音量。最大为 100（默认）
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    playEffect(effectId: number, opt: Array<NERtcCreateAudioEffectOption>): number;
    /**
     * 停止播放指定音效文件。
     * <pre>
     * 请在频道内调用该方法。
     * </pre>
     * @param {number} effectId 指定音效的 ID。每个音效均有唯一的 ID。
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    stopEffect(effectId: number): number;
    /**
     * 停止播放所有音效文件。
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    stopAllEffects(): number;
    /**
     * 暂停音效文件播放。
     * <pre>
     * 请在频道内调用该方法。
     * </pre>
     * @param {number} effectId 指定音效的 ID。每个音效均有唯一的 ID。
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    pauseEffect(effectId: number): number;
    /**
     * 恢复播放指定音效文件。
     * <pre>
     * 请在频道内调用该方法。
     * </pre>
     * @param {number} effectId 指定音效的 ID。每个音效均有唯一的 ID。
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    resumeEffect(effectId: number): number;
    /**
     * 暂停所有音效文件播放。
     * <pre>
     * 请在频道内调用该方法。
     * </pre>
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    pauseAllEffects(): number;
    /**
     * 恢复播放所有音效文件。
     * <pre>
     * 请在频道内调用该方法。
     * </pre>
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    resumeAllEffects(): number;
    /**
     * 调节音效发送音量。
     * <pre>
     * 该方法调节音效的发送音量大小。请在频道内调用该方法。
     * </pre>
     * @param {number} effectId 指定音效的 ID。每个音效均有唯一的 ID。
     * @param {number} volume 音效音量范围为 0~100。默认 100 为原始文件音量。
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    setEffectSendVolume(effectId: number, volume: number): number;
    /**
     * 获取音效发送音量。
     * <pre>
     * 该方法获取音效的发送音量大小。请在频道内调用该方法。
     * </pre>
     * @param {number} effectId 指定音效的 ID。每个音效均有唯一的 ID。
     * @returns {number}
     * - >=0: 音效发送音量；
     * - 其他: 方法调用失败。
     */
    getEffectSendVolume(effectId: number): number;
    /**
     * 调节音效播放音量。
     * <pre>
     * 该方法调节音效的播放音量大小。请在频道内调用该方法。
     * </pre>
     * @param {number} effectId 指定音效的 ID。每个音效均有唯一的 ID。
     * @param {number} volume 音效音量范围为 0~100。默认 100 为原始文件音量。
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    setEffectPlaybackVolume(effectId: number, volume: number): number;
    /**
     * 获取音效播放音量。
     * <pre>
     * 该方法获取音效的播放音量大小。请在频道内调用该方法。
     * </pre>
     * @param {number} effectId 指定音效的 ID。每个音效均有唯一的 ID。
     * @returns {number}
     * - >= 0: 音效播放音量；
     * - 其他: 方法调用失败。
     */
    getEffectPlaybackVolume(effectId: number): number;
    /**
     * 开启或关闭耳返。
     * <pre>
     * 请在频道内调用该方法。
     * </pre>
     * @param {boolean} enabled 开启或关闭。
     * @param {number} volume 耳返音量。
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    enableEarback(enabled: boolean, volume: number): number;
    /**
     * 设置耳返音量。
     * <pre>
     * 请在频道内调用该方法。
     * </pre>
     * @param {number} volume 耳返音量。
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    setEarbackVolume(volume: number): number;
    /**
     * 启用说话者音量提示。该方法允许 SDK 定期向 App 反馈当前谁在说话以及说话者的音量。
     * <pre>
     * 启用该方法后，无论频道内是否有人说话，可以通过{@link NERtcEngine#on}方法监听 onRemoteAudioVolumeIndication，根据设置的间隔时间返回音量提示事件。
     * </pre>
     * @param {boolean} enable 是否启用说话者音量提示。
     * @param {number} interval 指定音量提示的时间间隔，单位为毫秒。必须设置为 100 毫秒的整数倍值。
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    enableAudioVolumeIndication(enabled: boolean, interval: number): number;
    /**
     * 通过指定区域共享屏幕。共享一个屏幕或该屏幕的部分区域。用户需要在该方法中指定想要共享的屏幕区域。
     * <pre>
     * <b>NOTE:</b>
     * - 该方法仅适用于 Windows。
     * - 该方法打开视频辅流。
     * </pre>
     * @param {object} screenRect 指定待共享的屏幕相对于虚拟屏的位置。
     * @param {number} screenRect.x 左上角的横向偏移
     * @param {number} screenRect.y 左上角的纵向偏移
     * @param {number} screenRect.width 待共享区域的宽
     * @param {number} screenRect.height 待共享区域的高
     * @param {object} [regionRect=] (可选) 指定待共享区域相对于整个屏幕屏幕的位置。如果设置的共享区域超出了屏幕的边界，则只共享屏幕内的内容；如果将 width 或 height 设为 0, 则共享整个屏幕。
     * @param {number} regionRect.x 左上角的横向偏移
     * @param {number} regionRect.y 左上角的纵向偏移
     * @param {number} regionRect.width 待共享区域的宽
     * @param {number} regionRect.height 待共享区域的高
     * @param {object} param 屏幕共享的编码参数配置。
     * @param {object} [param.profile=2] 屏幕共享编码参数配置:
     * <pre>
     * - 0 640x480, 5fps
     * - 1 1280x720, 5fps
     * - 2 1920x1080, 5fps。默认
     * - 3 自定义
     * </pre>
     * @param {object} param.dimensions 屏幕共享视频发送的最大像素值，param.profile=3时生效:
     * @param {number} param.dimensions.width  宽度
     * @param {number} param.dimensions.height  高度
     * @param {number} [param.frame_rate=5] 共享视频的帧率，param.profile=3时生效，单位为 fps；默认值为 5，建议不要超过 15
     * @param {number} [param.bitrate=0] 共享视频的码率，单位为 bps；默认值为 0，表示 SDK 根据当前共享屏幕的分辨率计算出一个合理的值
     * @param {boolean} param.capture_mouse_cursor 是否采集鼠标用于屏幕共享
     * @param {boolean} param.window_focus 调用 {@link NERtcEngine#startScreenCaptureByWindowId} 方法共享窗口时，是否将该窗口前置
     * @param {number[]} param.excluded_window_list 待屏蔽窗口的 ID 列表
     * @param {number} param.excluded_window_count 待屏蔽窗口的数量
     * @param {number} param.prefer 编码策略倾向:
     * <pre>
     * - 0 动画模式
     * - 1 细节模式
     * </pre>
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    startScreenCaptureByScreenRect(screenRect: NERtcRectangle, regionRect: NERtcRectangle, param: NERtcScreenCaptureParameters): number;
    /**
     * 通过屏幕 ID 共享屏幕。共享一个屏幕或该屏幕的部分区域。用户需要在该方法中指定想要共享的屏幕 ID。
     * <pre>
     * <b>NOTE:</b>
     * - 该方法仅适用于 Windows 和 macOS。
     * - 该方法打开视频辅流。
     * </pre>
     * @param {number} displayId 指定待共享的屏幕 ID。开发者需要通过该参数指定你要共享的那个屏幕。
     * @param {object} [regionRect=] (可选) 指定待共享区域相对于整个屏幕屏幕的位置。如果设置的共享区域超出了屏幕的边界，则只共享屏幕内的内容；如果将 width 或 height 设为 0, 则共享整个屏幕。
     * @param {number} regionRect.x 左上角的横向偏移
     * @param {number} regionRect.y 左上角的纵向偏移
     * @param {number} regionRect.width 待共享区域的宽
     * @param {number} regionRect.height 待共享区域的高
     * @param {object} param 屏幕共享的编码参数配置。
     * @param {object} [param.profile=2] 屏幕共享编码参数配置:
     * <pre>
     * - 0 640x480, 5fps
     * - 1 1280x720, 5fps
     * - 2 1920x1080, 5fps。默认
     * - 3 自定义
     * </pre>
     * @param {object} param.dimensions 屏幕共享视频发送的最大像素值，param.profile=3时生效:
     * @param {number} param.dimensions.width  宽度
     * @param {number} param.dimensions.height  高度
     * @param {number} [param.frame_rate=5] 共享视频的帧率，param.profile=3时生效，单位为 fps；默认值为 5，建议不要超过 15
     * @param {number} [param.bitrate=0] 共享视频的码率，单位为 bps；默认值为 0，表示 SDK 根据当前共享屏幕的分辨率计算出一个合理的值
     * @param {boolean} param.capture_mouse_cursor 是否采集鼠标用于屏幕共享
     * @param {boolean} param.window_focus 调用 {@link NERtcEngine#startScreenCaptureByWindowId} 方法共享窗口时，是否将该窗口前置
     * @param {number[]} param.excluded_window_list 待屏蔽窗口的 ID 列表
     * @param {number} param.excluded_window_count 待屏蔽窗口的数量
     * @param {number} param.prefer 编码策略倾向:
     * <pre>
     * - 0 动画模式
     * - 1 细节模式
     * </pre>
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - -100: 需要先调用 {@link NERtcEngine#enumerateScreenCaptureSourceInfo} 缓存桌面信息
     * - 其他: 方法调用失败。
     * </pre>
     */
    startScreenCaptureByDisplayId(displayId: number, regionRect: NERtcRectangle, param: NERtcScreenCaptureParameters): number;
    /**
     * 通过窗口 ID 共享窗口。共享一个窗口或该窗口的部分区域。用户需要在该方法中指定想要共享的窗口 ID。
     * <pre>
     * <b>NOTE:</b>
     * - 该方法仅适用于 Windows 和 macOS。
     * - 该方法打开视频辅流。
     * </pre>
     * @param {number} windowid 指定待共享的窗口 ID。
     * @param {object} [regionRect=] (可选) 指定待共享区域相对于整个屏幕屏幕的位置。如果设置的共享区域超出了屏幕的边界，则只共享屏幕内的内容；如果将 width 或 height 设为 0, 则共享整个屏幕。
     * @param {number} regionRect.x 左上角的横向偏移
     * @param {number} regionRect.y 左上角的纵向偏移
     * @param {number} regionRect.width 待共享区域的宽
     * @param {number} regionRect.height 待共享区域的高
     * @param {object} param 屏幕共享的编码参数配置。
     * @param {object} [param.profile=2] 屏幕共享编码参数配置:
     * <pre>
     * - 0 640x480, 5fps
     * - 1 1280x720, 5fps
     * - 2 1920x1080, 5fps。默认
     * - 3 自定义
     * </pre>
     * @param {object} param.dimensions 屏幕共享视频发送的最大像素值，param.profile=3时生效:
     * @param {number} param.dimensions.width  宽度
     * @param {number} param.dimensions.height  高度
     * @param {number} [param.frame_rate=5] 共享视频的帧率，param.profile=3时生效，单位为 fps；默认值为 5，建议不要超过 15
     * @param {number} [param.bitrate=0] 共享视频的码率，单位为 bps；默认值为 0，表示 SDK 根据当前共享屏幕的分辨率计算出一个合理的值
     * @param {boolean} param.capture_mouse_cursor 是否采集鼠标用于屏幕共享
     * @param {boolean} param.window_focus 调用 {@link NERtcEngine#startScreenCaptureByWindowId} 方法共享窗口时，是否将该窗口前置
     * @param {number[]} param.excluded_window_list 待屏蔽窗口的 ID 列表
     * @param {number} param.excluded_window_count 待屏蔽窗口的数量
     * @param {number} param.prefer 编码策略倾向:
     * <pre>
     * - 0 动画模式
     * - 1 细节模式
     * </pre>
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    startScreenCaptureByWindowId(windowid: number, regionRect: NERtcRectangle, param: NERtcScreenCaptureParameters): number;
    /**
     * 在共享屏幕或窗口时，更新共享的区域。
     * @param {object} param 屏幕共享的编码参数配置。
     * @param {object} [param.profile=2] 屏幕共享编码参数配置:
     * <pre>
     * - 0 640x480, 5fps
     * - 1 1280x720, 5fps
     * - 2 1920x1080, 5fps。默认
     * - 3 自定义
     * </pre>
     * @param {object} param.dimensions 屏幕共享视频发送的最大像素值，param.profile=3时生效:
     * @param {number} param.dimensions.width  宽度
     * @param {number} param.dimensions.height  高度
     * @param {number} [param.frame_rate=5] 共享视频的帧率，param.profile=3时生效，单位为 fps；默认值为 5，建议不要超过 15
     * @param {number} [param.bitrate=0] 共享视频的码率，单位为 bps；默认值为 0，表示 SDK 根据当前共享屏幕的分辨率计算出一个合理的值
     * @param {boolean} param.capture_mouse_cursor 是否采集鼠标用于屏幕共享
     * @param {boolean} param.window_focus 调用 {@link NERtcEngine#startScreenCaptureByWindowId} 方法共享窗口时，是否将该窗口前置
     * @param {number[]} param.excluded_window_list 待屏蔽窗口的 ID 列表
     * @param {number} param.excluded_window_count 待屏蔽窗口的数量
     * @param {number} param.prefer 编码策略倾向:
     * <pre>
     * - 0 动画模式
     * - 1 细节模式
     * </pre>
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    updateScreenCaptureRegion(regionRect: NERtcRectangle): number;
    /**
     * 停止屏幕共享。
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    stopScreenCapture(): number;
    /**
     * 暂停屏幕共享。
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    pauseScreenCapture(): number;
    /**
     * 恢复屏幕共享。
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    resumeScreenCapture(): number;
    /**
     * 开启声音共享。
     * @deprecated 该接口已经在 4.1.x 版本后废弃
     * <pre>
     * <b>NOTE:</b>
     * - 该方法仅适用于 Windows。
     * - 请在频道内调用该方法，该方法会捕获系统声音发送，开启本地语音后工作。
     * </pre>
     * @return {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    startSystemAudioLoopbackCapture(): number;
    /**
     * 关闭声音共享。
     * @deprecated 该接口已经在 4.1.x 版本后废弃
     * <pre>
     * <b>NOTE:</b>
     * - 该方法仅适用于 Windows。
     * - 请在频道内调用该方法，通话结束后自动关闭。
     * </pre>
     * @return {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    stopSystemAudioLoopbackCapture(): number;
    /**
     * 设置声音共享音量。
     * @deprecated 该接口已经在 4.1.x 版本后废弃
     * <pre>
     * <b>NOTE:</b>
     * - 该方法仅适用于 Windows。
     * - 请在频道内调用该方法。
     * </pre>
     * @param {number} volume 音效音量范围为 0~100。默认 100 为原始音量。
     * @return {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    setSystemAudioLoopbackCaptureVolume(volume: number): number;
    /**
     * 发送媒体补充增强信息（SEI）。
     * @since 4.1.110
     * <pre>
     * 在本端推流传输视频流数据同时，发送流媒体补充增强信息来同步一些其他附加信息。当推流方发送 SEI 后，拉流方可通过监听 onReceSEIMsg 的回调获取 SEI 内容。
     * - 调用时机：视频流（主流）开启后，可调用此函数。
     * - 数据长度限制： SEI 最大数据长度为 4096 字节，超限会发送失败。如果频繁发送大量数据会导致视频码率增大，可能会导致视频画质下降甚至卡顿。
     * - 发送频率限制：最高为视频发送的帧率，建议不超过 10 次/秒。
     * - 生效时间：调用本接口之后，最快在下一帧视频数据帧之后发送 SEI 数据，最慢在接下来的 5 帧视频之后发送。
     * <b>NOTE:</b>
     * - SEI 数据跟随视频帧发送，由于在弱网环境下可能丢帧，SEI 数据也可能随之丢失，所以建议在发送频率限制之内多次发送，保证接收端收到的概率。
     * - 调用本接口时，默认使用主流通道发送 SEI。
     * </pre>
     * @param {ArrayBuffer} data 自定义 SEI 数据
     * @return {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    sendSEIMsg(data: ArrayBuffer): number;
    /**
     * 发送媒体补充增强信息（SEI）。
     * @since 4.1.110
     * <pre>
     * 在本端推流传输视频流数据同时，发送流媒体补充增强信息来同步一些其他附加信息。当推流方发送 SEI 后，拉流方可通过监听 onReceSEIMsg 的回调获取 SEI 内容。
     * - 调用时机：视频流（主流）开启后，可调用此函数。
     * - 数据长度限制： SEI 最大数据长度为 4096 字节，超限会发送失败。如果频繁发送大量数据会导致视频码率增大，可能会导致视频画质下降甚至卡顿。
     * - 发送频率限制：最高为视频发送的帧率，建议不超过 10 次/秒。
     * - 生效时间：调用本接口之后，最快在下一帧视频数据帧之后发送 SEI 数据，最慢在接下来的 5 帧视频之后发送。
     * <b>NOTE:</b>
     * - SEI 数据跟随视频帧发送，由于在弱网环境下可能丢帧，SEI 数据也可能随之丢失，所以建议在发送频率限制之内多次发送，保证接收端收到的概率。
     * - 调用本接口时，默认使用主流通道发送 SEI。
     * </pre>
     * @param {ArrayBuffer} data 自定义 SEI 数据
     * @param {number} type 发送 SEI 时，使用的流通道类型：
     * <pre>
     * - 0: 主流通道
     * - 1: 辅流通道
     * </pre>
     * @return {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    sendSEIMsgEx(data: ArrayBuffer, type: NERtcStreamChannelType): number;
    /**
     * 拉取外部音频数据。
     * @since 4.1.110
     * <pre>
     * - 该方法将从内部引擎拉取音频数据。 通过 setExternalAudioRender 启用外部音频数据渲染功能成功后，可以使用 pullExternalAudioFrame 接口获取音频 PCM 数据。
     * <b>NOTE:</b>
     * - 该方法需要在加入房间后调用。
     * - 数据帧时长建议匹配 10ms 周期。
     * - 该方法在音频渲染设备关闭后不再生效，此时会返回空数据。例如通话结束、通话前扬声器设备测试关闭等情况下，该设置不再生效。
     * </pre>
     * @param {boolean} enable 是否外部数据输出
     * <pre>
     * - true: 开启外部数据渲染
     * - false: 关闭外部数据渲染 (默认)
     * </pre>
     * @param {number} sampleRate 数据采样率，后续数据按该格式返回。注意：调用接口关闭功能时可传入任意合法值，此时设置不会生效
     * @param {number} channels channels 数据声道数，后续数据按该格式返回。注意：调用接口关闭功能时可传入任意合法值，此时设置不会生效。
     * @return {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    setExternalAudioRender(enable: boolean, sampleRate: number, channels: number): number;
    /**
     * 拉取外部音频数据。
     * @since 4.1.110
     * <pre>
     * - 该方法将从内部引擎拉取音频数据。 通过 setExternalAudioRender 启用外部音频数据渲染功能成功后，可以使用 pullExternalAudioFrame 接口获取音频 PCM 数据。
     * <b>NOTE:</b>
     * - 该方法需要在加入房间后调用。
     * - 数据帧时长建议匹配 10ms 周期。
     * - 该方法在音频渲染设备关闭后不再生效，此时会返回空数据。例如通话结束、通话前扬声器设备测试关闭等情况下，该设置不再生效。
     * </pre>
     * @param {number} pullLength 待拉取音频数据的字节数，单位为 byte
     * @param {function} cb 拉取数据的回调函数
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    pullExternalAudioFrame(pullLength: number, cb: NERtcPullExternalAudioFrameCb): number;
    /**
     * 查询 SDK 版本号。
     * @returns {String} 当前的 SDK 版本号，格式为字符串，如1.0.0.
     */
    getVersion(): String;
    /**
     * 获取错误描述。
     * @param {number} errorCode 错误码
     * @returns {number} 详细错误码描述
     */
    getErrorDescription(errorCode: number): String;
    /**
     * 上传SDK 信息（如log文件和Audio dump文件）。
     */
    uploadSdkInfo(): void;
    /**
     * 添加房间推流任务，成功添加后当前用户可以收到该直播流的状态通知。通话中有效。
     * @fires NERtcEngine#onAddLiveStreamTask
     * @param {Object} info 直播任务信息。
     * @param {String} info.task_id 推流任务ID，为推流任务的唯一标识，用于过程中增删任务操作 <64 chars
     * @param {String} info.stream_url 直播推流地址 <256 chars
     * @param {Boolean} info.server_record_enabled 服务器录制功能是否开启
     * @param {Number} info.ls_mode 直播推流模式:
     * <pre>
     * - 0 推流带视频
     * - 1 推流纯音频
     * </pre>
     * @param {Object} info.config 音视频流编码参数等设置:
     * @param {boolean} info.config.singleVideoPassThrough 音频编码规格。默认值为 NERtcLiveStreamAudioCodecProfileLCAAC 普通编码规格。
     * @param {boolean} info.config.audioBitrate 音频推流码率。单位为 kbps，取值范围为 10~192。语音场景建议设置为 64 及以上码率，音乐场景建议设置为 128 及以上码率。
     * @param {boolean} info.config.sampleRate 音频推流采样率。默认值为 NERtcLiveStreamAudioSampleRate48000 48K。
     * @param {boolean} info.config.channels 音频推流声道数。1：单声道。2：（默认）双声道。
     * @param {boolean} info.config.audioCodecProfile 音频编码规格。默认值为 NERtcLiveStreamAudioCodecProfileLCAAC 普通编码规格。
     * <pre>
     * - 0 HE-AAC 规格，表示高效音频编码规格。
     * - 1 （默认）LC-AAC 规格，表示基本音频编码规格。
     * </pre>
     * @param {Object} info.layout 视频布局:
     * @param {number} info.layout.width 视频推流宽度
     * @param {number} info.layout.height 视频推流高度
     * @param {number} info.layout.background_color 视频推流背景色，(R & 0xff) << 16 | (G & 0xff) << 8 | (B & 0xff)
     * @param {number} info.layout.user_count 成员布局个数
     * @param {object[]} info.layout.users 成员布局数组:
     * @param {number} info.layout.users[].uid 用户id
     * @param {boolean} info.layout.users[].video_push 是否推送该用户视频流，info.ls_mode=1时无效
     * @param {number} info.layout.users[].adaption 视频流裁剪模式:
     * <pre>
     * - 0 视频尺寸等比缩放。优先保证视频内容全部显示。因视频尺寸与显示视窗尺寸不一致造成的视窗未被填满的区域填充背景色
     * - 1 视频尺寸等比缩放。优先保证视窗被填满。因视频尺寸与显示视窗尺寸不一致而多出的视频将被截掉
     * </pre>
     * @param {number} info.layout.users[].x 画面离主画面左边距
     * @param {number} info.layout.users[].y 画面离主画面上边距
     * @param {number} info.layout.users[].width 画面在主画面的显示宽度，画面右边超出主画面会失败
     * @param {number} info.layout.users[].height 画面在主画面的显示高度，画面底边超出主画面会失败
     * @param {boolean} info.layout.users[].audio_push 是否推送该用户音频流
     * @param {object} info.layout.users[].bg_image 背景图信息:
     * @param {String} info.layout.users[].bg_image.url 图片地址
     * @param {number} info.layout.users[].bg_image.x 画面离主画面左边距
     * @param {number} info.layout.users[].bg_image.y 画面离主画面上边距
     * @param {number} info.layout.users[].bg_image.width 画面在主画面的显示宽度，画面右边超出主画面会失败
     * @param {number} info.layout.users[].bg_image.height 画面在主画面的显示高度，画面底边超出主画面会失败
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    addLiveStreamTask(info: NERtcLiveStreamTaskInfo): number;
    /**
     * 更新修改房间推流任务。通话中有效。
     * @fires NERtcEngine#onUpdateLiveStreamTask
     * @param {Object} info 直播任务信息。
     * @param {String} info.task_id 推流任务ID，为推流任务的唯一标识，用于过程中增删任务操作 <64 chars
     * @param {String} info.stream_url 直播推流地址 <256 chars
     * @param {Boolean} info.server_record_enabled 服务器录制功能是否开启
     * @param {Number} info.ls_mode 直播推流模式:
     * <pre>
     * - 0 推流带视频
     * - 1 推流纯音频
     * </pre>
     * @param {Object} info.layout 视频布局:
     * @param {number} info.layout.width 视频推流宽度
     * @param {number} info.layout.height 视频推流高度
     * @param {number} info.layout.background_color 视频推流背景色，(R & 0xff) << 16 | (G & 0xff) << 8 | (B & 0xff)
     * @param {number} info.layout.user_count 成员布局个数
     * @param {object[]} info.layout.users 成员布局数组:
     * @param {number} info.layout.users[].uid 用户id
     * @param {boolean} info.layout.users[].video_push 是否推送该用户视频流，info.ls_mode=1时无效
     * @param {number} info.layout.users[].adaption 视频流裁剪模式:
     * <pre>
     * - 0 视频尺寸等比缩放。优先保证视频内容全部显示。因视频尺寸与显示视窗尺寸不一致造成的视窗未被填满的区域填充背景色
     * - 1 视频尺寸等比缩放。优先保证视窗被填满。因视频尺寸与显示视窗尺寸不一致而多出的视频将被截掉
     * </pre>
     * @param {number} info.layout.users[].x 画面离主画面左边距
     * @param {number} info.layout.users[].y 画面离主画面上边距
     * @param {number} info.layout.users[].width 画面在主画面的显示宽度，画面右边超出主画面会失败
     * @param {number} info.layout.users[].height 画面在主画面的显示高度，画面底边超出主画面会失败
     * @param {boolean} info.layout.users[].audio_push 是否推送该用户音频流
     * @param {object} info.layout.users[].bg_image 背景图信息:
     * @param {String} info.layout.users[].bg_image.url 图片地址
     * @param {number} info.layout.users[].bg_image.x 画面离主画面左边距
     * @param {number} info.layout.users[].bg_image.y 画面离主画面上边距
     * @param {number} info.layout.users[].bg_image.width 画面在主画面的显示宽度，画面右边超出主画面会失败
     * @param {number} info.layout.users[].bg_image.height 画面在主画面的显示高度，画面底边超出主画面会失败
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    updateLiveStreamTask(info: NERtcLiveStreamTaskInfo): number;
    /**
     * 删除房间推流任务。通话中有效。
     * @fires NERtcEngine#onRemoveLiveStreamTask
     * @param {String} taskId  直播任务id
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    removeLiveStreamTask(taskId: String): number;
    /**
     * 获取系统中所有的音频采集设备列表。
     * <pre>
     *  该方法返回 NERtcDevice[] 包含系统中所有的音频采集设备。
     * </pre>
     * @returns {NERtcDevice[] | null}
     * <pre>
     * - NERtcDevice[] : 方法调用成功；
     * <table style="width:100%;">
     * <tr><th>Name</th><th>Type</th><th>Description</th></tr>
     * <tr><td>NERtcDevice.device_id</td><td>String</td><td>设备ID</td></tr>
     * <tr><td>NERtcDevice.device_name</td><td>String</td><td>设备名称</td></tr>
     * <tr><td>NERtcDevice.transport_type</td><td>Number</td><td>设备链接类型:
     * - audio:
     *   - 0 未知设备
     *   - 1 蓝牙设备
     *   - 2 蓝牙立体声设备
     *   - 3 蓝牙低功耗设备
     *   - 4 USB设备
     *   - 5 HDMI设备
     *   - 6 内置设备
     *   - 7 雷电接口设备
     *   - 8 AirPlay设备
     *   - 9 虚拟设备
     *   - 10 其他设备
     * - video:
     *   - 0 未知设备
     *   - 1 USB设备
     *   - 2 虚拟设备
     *   - 3 其他设备</td></tr>
     * <tr><td>NERtcDevice.suspected_unavailable</td><td>Boolean</td><td>是否是不推荐设备</td></tr>
     * <tr><td>NERtcDevice.system_default_device</td><td>Boolean</td><td>是否是系统默认设备</td></tr>
     * </table>
     * - NULL: 方法调用失败。
     * </pre>
     */
    enumerateRecordDevices(): Array<NERtcDevice>;
    /**
     * 指定音频采集设备。
     * @param {String} id 音频采集设备的设备 ID。可通过 {@link NERtcEngine#enumerateRecordDevices} 获取。插拔设备不会影响 deviceId。
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    setRecordDevice(id: String): number;
    /**
     * 获取当前使用的音频采集设备 ID。
     * @returns {String}
     * <pre>
     * - 设备ID： 调用成功
     * - null: 调用失败。
     * </pre>
     */
    getRecordDevice(): String;
    /**
     * 获取系统中所有的播放设备列表。
     * <pre>
     * 该方法返回 NERtcDevice[]，包含系统中所有的播放设备。
     * </pre>
     * @returns {NERtcDevice[]}
     * <pre>
     * - Array<NERtcDevice>: 调用成功；
     * - null：调用失败。
     * <table style="width:100%;">
     * <tr><th>Name</th><th>Type</th><th>Description</th></tr>
     * <tr><td>NERtcDevice.device_id</td><td>String</td><td>设备ID</td></tr>
     * <tr><td>NERtcDevice.device_name</td><td>String</td><td>设备名称</td></tr>
     * <tr><td>NERtcDevice.transport_type</td><td>Number</td><td>设备链接类型:
     * - audio:
     *   - 0 未知设备
     *   - 1 蓝牙设备
     *   - 2 蓝牙立体声设备
     *   - 3 蓝牙低功耗设备
     *   - 4 USB设备
     *   - 5 HDMI设备
     *   - 6 内置设备
     *   - 7 雷电接口设备
     *   - 8 AirPlay设备
     *   - 9 虚拟设备
     *   - 10 其他设备
     * - video:
     *   - 0 未知设备
     *   - 1 USB设备
     *   - 2 虚拟设备
     *   - 3 其他设备</td></tr>
     * <tr><td>NERtcDevice.suspected_unavailable</td><td>Boolean</td><td>是否是不推荐设备</td></tr>
     * <tr><td>NERtcDevice.system_default_device</td><td>Boolean</td><td>是否是系统默认设备</td></tr>
     * </table>
     - null: 调用失败。
     */
    enumeratePlayoutDevices(): Array<NERtcDevice>;
    /**
     * 指定播放设备。
     * @param {String} id 音频播放设备的设备 ID。可以通过 {@link NERtcEngine#enumeratePlayoutDevices} 获取。插拔设备不会影响 deviceId。
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    setPlayoutDevice(id: String): number;
    /**
     * 获取当前使用的音频播放设备 ID。
     * @returns {String} deviceID
     */
    getPlayoutDevice(): String;
    /**
     * 设置音频采集设备音量。
     * @param {number} volume 音频采集设备音量。取值范围为 0~255。
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    setRecordDeviceVolume(volume: number): number;
    /**
     * 获取音频采集设备音量。
     * @returns {number}
     * <pre>
     * - >=0: 音频采集设备音量。取值范围为 0~255。
     * - 其他: 方法调用失败。
     * </pre>
     */
    getRecordDeviceVolume(): number;
    /**
     * 设置音频播放设备音量。
     * @param {number} volume 音频播放设备音量。取值范围为 0~255。
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    setPlayoutDeviceVolume(volume: number): number;
    /**
     * 获取音频播放设备音量。
     * @returns {number}
     * <pre>
     * - >=0: 音频播放设备音量。取值范围为 0~255。
     * - 其他: 方法调用失败。
     * </pre>
     */
    getPlayoutDeviceVolume(): number;
    /**
     * 静音或取消静音音频播放设备。
     * @param mute 是否静音音频播放设备。
     * <pre>
     * - true: 静音音频播放设备。
     * - false: 取消静音音频播放设备。
     * </pre>
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    setPlayoutDeviceMute(mute: boolean): number;
    /**
     * 获取音频播放设备静音状态。
     * @returns {boolean}
     * <pre>
     * - true: 静音状态。
     * - false: 非静音状态。
     * </pre>
     */
    getPlayoutDeviceMute(): boolean;
    /**
     * 静音或取消静音音频采集设备。
     * @param {boolean} mute 是否静音音频采集设备。
     * <pre>
     * - true: 静音音频采集设备。
     * - false: 取消静音音频采集设备。
     * </pre>
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    setRecordDeviceMute(mute: boolean): number;
    /**
     * 获取音频采集设备静音状态。
     * @returns {boolean}
     * <pre>
     * - true: 静音状态。
     * - false: 非静音状态。
     * </pre>
     */
    getRecordDeviceMute(): boolean;
    /**
     * 调节录音音量。
     * @param {number} volume 录音信号音量可在 0 ~ 400 范围内进行调节:
     * <pre>
     * - 0: 静音；
     * - 100: 原始音量；
     * - 400: 最大可为原始音量的 4 倍（自带溢出保护）。
     * </pre>
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    adjustRecordingSignalVolume(volume: number): number;
    /**
     * 调节播放音量。
     * @param {number} volume 播放音量可在 0 ~ 400 范围内进行调节:
     * <pre>
     * - 0:  静音；
     * - 100: 原始音量；
     * - 400: 最大可为原始音量的 4 倍（自带溢出保护）。
     * </pre>
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    adjustPlaybackSignalVolume(volume: number): number;
    /**
     * 启动音频采集设备测试。
     * <pre>
     * 该方法测试音频采集设备是否能正常工作。调用该方法后，可以通过{@link NERtcEngine#on}方法监听 onLocalAudioVolumeIndication 报告采集设备的音量信息事件。
     * <b>NOTE:</b>
     * - 该方法需在加入房间前调用。
     * - 音频采集设备测试会在通话开始后自动结束，您也可以手动调用 {@link NERtcEngine#stopRecordDeviceTest} 停止音频采集设备测试。
     * </pre>
     * @param {number} interval 事件上报的时间间隔，单位为毫秒。
     * @fires NERtcEngine#onLocalAudioVolumeIndication
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    startRecordDeviceTest(interval: number): number;
    /**
     * 停止音频采集设备测试。
     * <pre>
     * 该方法停止音频采集设备测试。
     * <b>NOTE:</b>
     * - 该方法需在加入房间前调用。
     * - 音频采集设备测试会在通话开始后自动结束，您也可以手动调用 {@link NERtcEngine#stopRecordDeviceTest} 停止音频采集设备测试。
     * </pre>
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    stopRecordDeviceTest(): number;
    /**
     * 启动音频播放设备测试。
     * <pre>
     * 该方法测试音频播放设备是否能正常工作。启动测试后，SDK 播放指定的音频文件，测试者如果能听到声音，说明播放设备能正常工作。调用该方法后，可以通过{@link NERtcEngine#on}方法监听 onLocalAudioVolumeIndication (每隔 100 ms)
     * <b>NOTE:</b>
     * - 该方法需在加入房间前调用。
     * - 音频播放设备测试会在通话开始后自动结束，您也可以手动调用 {@link NERtcEngine#stopPlayoutDeviceTest} 停止音频播放设备测试。
     * - 支持文件格式包括 wav、mp3、aac。
     * </pre>
     * @param {string} path 音频文件的绝对路径，路径字符串使用 UTF-8 编码格式。
     * @fires NERtcEngine#onLocalAudioVolumeIndication
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功，并且可以听到所设置文件的声音。
     * - 其他: 方法调用失败。
     * </pre>
     */
    startPlayoutDeviceTest(path: String): number;
    /**
     * 停止播放设备测试。
     * <pre>
     * <b>NOTE:</b>
     * - 该方法需在加入房间前调用。
     * </pre>
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    stopPlayoutDeviceTest(): number;
    /**
     * 开始音频设备回路测试。
     * <pre>
     * 该方法测试音频采集和播放设备是否能正常工作。一旦测试开始，音频采集设备会采集本地音频，然后使用音频播放设备播放出来。 可以通过 {@link NERtcEngine#on}监听 onLocalAudioVolumeIndication 报告音量信息的事件。
     * <b>NOTE:</b>
     * - 该方法需在加入房间前调用。
     * - 音频设备回路测试会在通话开始后自动结束，您也可以手动调用 {@link NERtcEngine#stopAudioDeviceLoopbackTest} 停止音频设备回路测试。
     * - 该方法仅在本地进行音频设备测试，不涉及网络连接。
     * </pre>
     * @param {number} interval 事件上报的时间间隔，单位为毫秒。
     * @fires NERtcEngine#onLocalAudioVolumeIndication
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    startAudioDeviceLoopbackTest(interval: number): number;
    /**
     * 停止音频设备回路测试。
     * <pre>
     * <b>NOTE:</b>
     * - 该方法需在加入房间前调用。
     * </pre>
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    stopAudioDeviceLoopbackTest(): number;
    /**
     * 获取系统中所有的视频采集设备列表。
     * <pre>
     * 该方法返回 NERtcDevice[] 包含系统中所有的音频采集设备。
     * </pre>
     @returns {NERtcDevice[]}
     * <pre>
     * - NERtcDevice[] : 调用成功；
     * <table style="width:100%;">
     * <tr><th>Name</th><th>Type</th><th>Description</th></tr>
     * <tr><td>NERtcDevice.device_id</td><td>String</td><td>设备ID</td></tr>
     * <tr><td>NERtcDevice.device_name</td><td>String</td><td>设备名称</td></tr>
     * <tr><td>NERtcDevice.transport_type</td><td>Number</td><td>设备链接类型:
     * - audio:
     *   - 0 未知设备
     *   - 1 蓝牙设备
     *   - 2 蓝牙立体声设备
     *   - 3 蓝牙低功耗设备
     *   - 4 USB设备
     *   - 5 HDMI设备
     *   - 6 内置设备
     *   - 7 雷电接口设备
     *   - 8 AirPlay设备
     *   - 9 虚拟设备
     *   - 10 其他设备
     * - video:
     *   - 0 未知设备
     *   - 1 USB设备
     *   - 2 虚拟设备
     *   - 3 其他设备</td></tr>
     * <tr><td>NERtcDevice.suspected_unavailable</td><td>Boolean</td><td>是否是不推荐设备</td></tr>
     * <tr><td>NERtcDevice.system_default_device</td><td>Boolean</td><td>是否是系统默认设备</td></tr>
     * </table>
     * - NULL: 调用失败。
     * </pre>
     */
    enumerateVideoCaptureDevices(): Array<NERtcDevice>;
    /**
     * 枚举屏幕分享源信息。
     * @since 4.1.110
     * @param {number} thumbWidth 缩略图宽度 px。
     * @param {number} thumbHeight 缩略图高度 px。
     * @param {number} iconWidth 图标宽度 px。
     * @param {number} iconHeight 图标高度 px。
     * @returns {Object[]}
     * <pre>
     * - Object[] : 调用成功；
     * <table style="width:100%;">
     * <tr><th>Name</th><th>Type</th><th>Description</th></tr>
     * <tr><td>Object.sourceId</td><td>number</td><td>信息源ID</td></tr>
     * <tr><td>Object.displayId</td><td>String</td><td>如果是屏幕设备则为屏幕 ID</td></tr>
     * <tr><td>Object.sourceName</td><td>String</td><td>信息源名称</td></tr>
     * <tr><td>Object.type</td><td>int</td><td>信息源类型:1-屏幕 2-窗口</td></tr>
     * <tr><td>Object.isMinimizeWindow</td><td>boolean</td><td>窗口是否最小化状态</td></tr>
     * <tr><td>Object.thumbBGRA</td><td>object</td><td>缩略图信息,使用前需要判断是否undefined:
     * - buffer - BGRA二进制数据
     * - length - 数据大小 byte
     * - width - 图片宽度 px
     * - height - 图片高度 px</td></tr>
     * <tr><td>Object.iconBGRA</td><td>object</td><td>图标信息,使用前需要判断是否undefined:
     * - buffer - BGRA二进制数据
     * - length - 数据大小 byte
     * - width - 图片宽度 px
     * - height - 图片高度 px</td></tr>
     * </table>
     * - NULL: 调用失败。
     * </pre>
     */
    enumerateScreenCaptureSourceInfo(thumbWidth: number, thumbHeight: number, iconWidth: number, iconHeight: number): Array<Object>;
    /**
     * 指定视频采集设备。
     * @param {string} id 视频采集设备的设备 ID。可以通过 {@link NERtcEngine#enumerateCaptureDevices}获取。
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    setVideoDevice(id: String): number;
    /**
     * 获取当前使用的视频采集设备信息。
     * @returns {String} 设备ID
     */
    getVideoDevice(): String;
    /**
     * 设置 SDK 预设的人声的变声音效。
     * @since 4.1.110
     * @param {number} type 预设的变声音效。默认关闭变声音效：
     * <pre>
     * - 0: 默认关闭
     * - 1: 机器人
     * - 2: 巨人
     * - 3: 恐怖
     * - 4: 成熟
     * - 5: 男变女
     * - 6: 女变男
     * - 7: 男变萝莉
     * - 8: 女变萝莉
     * </pre>
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功
     * - 其他: 方法调用失败。
     * </pre>
     */
    setAudioEffectPreset(type: NERtcVoiceChangerType): number;
    /**
     * 设置 SDK 预设的美声效果。调用该方法可以为本地发流用户设置 SDK 预设的人声美声效果。
     * @since 4.1.110
     * <pre>
     * <b>NOTE:</b>
     * - 通话结束后重置为默认关闭
     * </pre>
     * @param {number} type 预设的美声效果模式。默认关闭美声效果：
     * <pre>
     * - 0: 默认关闭
     * - 1: 低沉
     * - 2: 圆润
     * - 3: 清澈
     * - 4: 磁性
     * - 5: 录音棚
     * - 6: 天籁
     * - 7: KTV
     * - 8: 悠远
     * - 9: 教堂
     * - 10: 卧室
     * - 11: Live
     * </pre>
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功
     * - 其他: 方法调用失败。
     * </pre>
     */
    setVoiceBeautifierPreset(type: NERtcVoiceBeautifierType): number;
    /**
     * 设置本地语音音调。该方法改变本地说话人声音的音调。
     * @since 4.1.110
     * <pre>
     * <b>NOTE:</b>
     * - 通话结束后该设置会重置，默认为 1.0。
     * - 此方法与 setAudioEffectPreset 互斥，调用此方法后，已设置的变声效果会被取消。
     * </pre>
     * @param {number} pitch 语音频率。可以在 [0.5, 2.0] 范围内设置。取值越小，则音调越低。默认值为 1.0，表示不需要修改音调。
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功
     * - 其他: 方法调用失败。
     * </pre>
     */
    setLocalVoicePitch(pitch: number): number;
    /**
     * 设置本地语音音效均衡，即自定义设置本地人声均衡波段的中心频率。
     * @since 4.1.110
     * <pre>
     * <b>NOTE:</b>
     * - 该方法在加入房间前后都能调用，通话结束后重置为默认关闭状态。
     * </pre>
     * @param {number} bandFrequency 频谱子带索引，取值范围是 [0-9]，分别代表 10 个频带，对应的中心频率是 [31，62，125，250，500，1k，2k，4k，8k，16k] Hz：
     * <pre>
     * - 0: 31Hz
     * - 1: 62Hz
     * - 2: 125Hz
     * - 3: 250Hz
     * - 4: 500Hz
     * - 5: 1kHz
     * - 6: 2kHz
     * - 7: 4kHz
     * - 8: 8kHz
     * - 9: 16kHz
     * </pre>
     * @param {number} bandGain 每个 band 的增益，单位是 dB，每一个值的范围是 [-15，15]，默认值为 0。
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功
     * - 其他: 方法调用失败。
     * </pre>
     */
    setLocalVoiceEqualization(bandFrequency: NERtcVoiceEqualizationBand, bandGain: number): number;
    /**
     * 设置远端用户音频流高优先级
     * @since 4.1.110
     * <pre>
     * - 支持在音频自动订阅的情况下，设置某一个远端用户的音频为最高优先级，可以优先听到该用户的音频
     * </pre>
     * @param  {boolean} enable 开启或关闭
     * @param  {number} uid 用户 ID
     * @param  {NERtcAudioStreamType} streamType 音频类型：主流、辅流
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功
     * - 其他: 方法调用失败。
     * </pre>
     */
    setRemoteHighPriorityAudioStream(enable: boolean, uid: number, streamType: NERtcAudioStreamType): number;
    /**
     * 取消或恢复订阅指定远端用户的音频辅流
     * @since 4.1.110
     * <pre>
     * - 加入房间时，默认不订阅所有远端用户的音频辅流流，您可以通过此方法取消或恢复订阅指定远端用户的音频辅流。
     * <b>NOTE:</b>
     * - 该方法需要在加入房间，远端用户开启音频后调用。
     * </pre>
     * @param  {number} uid 指定用户的 ID
     * @param  {boolean} subscribe
     * <pre>
     * - true: 订阅指定音频流
     * - false: 取消订阅指定音频流（默认）
     * </pre>
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功
     * - 其他: 方法调用失败。
     * </pre>
     */
    subscribeRemoteAudioSubStream(uid: number, subscribe: boolean): number;
    /**
     * 检查mac虚拟声卡是否安装。
     * <pre>
     * only for macOS。
     * </pre>
     * @returns {Boolean}
     * <pre>
     * - false: 虚拟声卡未安装
     * - true: 虚拟声卡已安装
     * </pre>
     */
    checkNeCastAudio(): boolean;
    /**
     * 开关本地音频发送。
     * @since 4.1.110
     * <pre>
     * - 该方法用于允许或禁止向网络发送本地音频流。
     * <b>NOTE:</b>
     * - 该方法不影响音频采集状态，功能与enableLocalAudio（audioDevice + enableLocalAudioStream）类似，但不会主动打开音频采集设备，适用于需要发送音频流但是不需要开启麦克风的场景。
     * - 静音状态会在通话结束后被重置为非静音。
     * </pre>
     * @param  {boolean} enable 是否开启本地音频发送。
     * @param  {NERtcAudioStreamType} streamType 音频类型：主流、辅流
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功
     * - 其他: 方法调用失败。
     * </pre>
     */
    enableLocalAudioStream(enable: boolean, streamType: NERtcAudioStreamType): number;
    /**
     * 开启声卡采集
     * @since 4.1.110
     * <pre>
     * - 启用声卡采集功能后，声卡播放的声音会被合到本地音频流中，从而可以发送到远端。
     * <b>NOTE:</b>
     * - 该方法仅适用于 macOS 和 Windows 平台。
     * - macOS 系统默认声卡不支持采集功能，如需开启此功能需要 App 自己启用一个虚拟声卡，并将该虚拟声卡的名字作为 deviceName 传入 SDK。
     * - 该方法在加入频道前后都能调用。
     * </pre>
     * @param  {boolean} enable
     * <pre>
     * - true: 开启声卡采集
     * - false: 关闭声卡采集（默认）
     * </pre>
     * @param  {String} deviceName 声卡的设备名。默认设为空，即使用当前声卡采集。如果用户使用虚拟声卡，如 “NeCastAudio”，可以将虚拟声卡名称 “NeCastAudio” 作为参数，SDK 会找到对应的虚拟声卡设备，并开始采集，若参数为空则在 macOS 下默认使用 NeCastAudio 设备名称 。
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功
     * - 其他: 方法调用失败。
     * </pre>
     */
    enableLoopbackRecording(enable: boolean, deviceName?: String): number;
    /**
     * 调节声卡采集信号音量。
     * @since 4.1.110
     * <pre>
     * - 调用 {@link nertc::IRtcEngineEx::enableLoopbackRecording} "enableLoopbackRecording" 开启声卡采集后，你可以调用该方法调节声卡采集的信号音量。
     * </pre>
     * @param  {number} volume 声卡采集信号音量。取值范围为 [0,100]。默认值为 100，表示原始音量 。
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功
     * - 其他: 方法调用失败。
     * </pre>
     */
    adjustLoopbackRecordingSignalVolume(volume: number): number;
    /**
     * 调节本地播放的指定远端用户的指定流类型的信号音量
     * @since 4.1.110
     * <pre>
     * - 加入房间后，您可以多次调用该方法设置本地播放的不同远端用户的音量；也可以反复调节本地播放的某个远端用户的音量。
     * <b>NOTE:</b>
     * - 请在成功加入房间后调用该方法。
     * - 该方法在本次通话中有效。如果远端用户中途退出房间，则再次加入此房间时仍旧维持该设置，通话结束后设置失效。
     * - 该方法调节的是本地播放的指定远端用户混音后的音量，且每次只能调整一位远端用户。若需调整多位远端用户在本地播放的音量，则需多次调用该方法。
     * </pre>
     * @param  {number} uid 远端用户 ID。
     * @param  {number} volume volume 播放音量，取值范围为 [0,100]。
     * <pre>
     * - 0：静音。
     * - 100：原始音量。
     * </pre>
     * @param  {number} streamType 音频类型：主流、辅流
     * <pre>
     * - 0: 音频流主流
     * - 1: 音频流辅流
     * </pre>
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功
     * - 其他: 方法调用失败。
     * </pre>
     */
    adjustUserPlaybackSignalVolume(uid: number, volume: number, streamType: NERtcAudioStreamType): number;
    checkNECastAudioDriver(): number;
    /**
     * init event handler
     * @private
     */
    initEventHandler(): void;
    /**
     * @private
     * @ignore
     * check if WebGL will be available with appropriate features
     * @returns {number} {boolean}
     */
    _checkWebGL(): boolean;
    /**
     * @private
     * @ignore
     * @param {number} type 0-local 1-remote 2-localsubstream 3-remotesubstream
     * @param {number} uid uid get from native engine, differ from electron engine's uid
     */
    _getRenderer(type: number, uid: number): IRenderer | undefined;
    /**
     * check if data is valid
     * @private
     * @ignore
     * @param {*} header
     * @param {*} ydata
     * @param {*} udata
     * @param {*} vdata
     */
    _checkData(header: ArrayBuffer, ydata: ArrayBuffer, udata: ArrayBuffer, vdata: ArrayBuffer): boolean;
    /**
     * register renderer for target info
     * @private
     * @ignore
     * @param {number} infos
     */
    doVideoFrameReceived(infos: any): void;
    /**
     * @private
     * @ignore
     * Initializes the renderer.
     * @param key Key for the map that store the renderers,
     * e.g, uid or `local`
     * @param view The Dom elements to render the video.
     */
    initRender(key: 'local' | number, view: Element): void;
    /**
     * @private
     * @ignore
     * Initializes the renderer of substream.
     * @param key Key for the map that store the renderers,
     * e.g, uid or `local`
     * @param view The Dom elements to render the substream video.
     */
    initSubStreamRender(key: 'local' | number, view: Element): void;
    captureRender(key: 'local' | number, streamType?: NERtcVideoStreamType): string;
    /**
     * Destroys the renderer.
     * @private
     * @ignore
     * @param key Key for the map that store the renderers,
     * e.g, `uid` or `local`.
     * @param onFailure The error callback for the {@link destroyRenderer}
     * method.
     */
    destroyRender(key: 'local' | number, onFailure?: (err: Error) => void): void;
    /**
     * Destroys the renderer of substream.
     * @private
     * @ignore
     * @param key Key for the map that store the renderers,
     * e.g, `uid` or `local`.
     * @param onFailure The error callback for the {@link destroySubStreamRender}
     * method.
     */
    destroySubStreamRender(key: 'local' | number, onFailure?: (err: Error) => void): void;
}
/** The NERtcEngine interface. */
declare interface NERtcEngine {
    /** 发生错误回调。

     该回调方法表示 SDK 运行时出现了（网络或媒体相关的）错误。通常情况下，SDK上报的错误意味着SDK无法自动恢复，需要 App 干预或提示用户。

     @param error_code 错误代码: #NERtcDMErrorCode.
     @param msg 错误描述。
     */
    on(event: 'onError', cb: (errorCode: number, msg: string) => void): this;
    /** 发生警告回调。

     该回调方法表示 SDK 运行时出现了（网络或媒体相关的）警告。通常情况下，SDK 上报的警告信息 App 可以忽略，SDK 会自动恢复。

     @param warn_code 错误代码: #NERtcWarnCode.
     @param msg 警告描述。
     */
    on(event: 'onWarning', cb: (warnCode: number, msg: string) => void): this;
    /** 释放硬件资源的回调。

     SDK提示释放硬件资源是否成功。

     @param result 返回结果。
     */
    on(event: 'onReleasedHwResources', cb: (result: NERtcErrorCode) => void): this;
    /** 加入频道回调。

     @param cid  频道 ID。
     @param uid  用户 ID。
     @param result  返回结果。
     @param elapsed 从 joinChannel 开始到发生此事件过去的时间（毫秒）。
     */
    on(event: 'onJoinChannel', cb: (cid: number, uid: number, result: NERtcErrorCode, elapsed: number) => void): this;
    /** 触发重连。

      有时候由于网络原因，客户端可能会和服务器失去连接，SDK会进行自动重连，开始自动重连后触发此回调。

     @param cid  频道 ID。
     @param uid  用户 ID。
     */
    on(event: 'onReconnectingStart', cb: (cid: number, uid: number) => void): this;
    /** 连接状态变更。

      有时候由于通话流程、用户行为、网络原因等，客户端通话状态变更，触发此回调。

     @param state  变更后通话状态。
     @param reason  变更原因。
     */
    on(event: 'onConnectionStateChange', cb: (state: NERtcConnectionStateType, reason: NERtcReasonConnectionChangedType) => void): this;
    /** 重新加入频道回调。

      有时候由于网络原因，客户端可能会和服务器失去连接，SDK会进行自动重连，自动重连后触发此回调方法。

     @param cid  频道 ID。
     @param uid  用户 ID。
     @param result  返回结果。
     @param elapsed 从开始重连到发生此事件过去的时间（毫秒）。
     */
    on(event: 'onRejoinChannel', cb: (cid: number, uid: number, result: NERtcErrorCode, elapsed: number) => void): this;
    /** 离开频道回调。

     App 调用 \ref IRtcEngine::leaveChannel "leaveChannel" 方法时，SDK提示 App 离开频道是否成功。

     @param result 返回结果。
     */
    on(event: 'onLeaveChannel', cb: (result: NERtcErrorCode) => void): this;
    /** 掉线回调。

      由于非网络原因，客户端可能会和服务器失去连接，此时SDK无需自动重连，直接触发此回调方法。

     @param reason  返回结果。
     */
    on(event: 'onDisconnect', cb: (result: NERtcErrorCode) => void): this;
    /** 参会者角色类型变更回调。
    
    本地用户加入房间后，通过 \ref IRtcEngine::setClientRole "setClientRole" 切换用户角色后会触发此回调。例如从主播切换为观众、从观众切换为主播。

    直播场景下，如果您在加入房间后调用该方法切换用户角色，调用成功后，会触发以下回调：
    - 主播切观众，本端触发onClientRoleChanged回调，远端触发\ref nertc::IRtcEngineEventHandler::onUserLeft "onUserLeft"回调。
    - 观众切主播，本端触发onClientRoleChanged回调，远端触发\ref nertc::IRtcEngineEventHandler::onUserJoined "onUserJoined"回调。

     @param oldRole  原角色类型。
     @param newRole  新角色类型。
     */
    on(event: 'onClientRoleChanged', cb: (oldRole: NERtcClientRole, newRole: NERtcClientRole) => void): this;
    /** 远端用户加入当前频道回调。

     - 通信模式下，该回调提示有远端用户加入了频道，并返回新加入用户的 ID；如果加入之前，已经有其他用户在频道中了，新加入的用户也会收到这些已有用户加入频道的回调。

     @param uid 新加入频道的远端用户ID。
     @param user_name 新加入频道的远端用户名(无效)。
     */
    on(event: 'onUserJoined', cb: (uid: number, userName: string) => void): this;
    /** 远端用户离开当前频道回调。

     提示有远端用户离开了频道（或掉线）。

     @param uid 远端用户ID。
     @param reason 远端用户离开原因。
     */
    on(event: 'onUserLeft', cb: (uid: number, reason: NERtcSessionLeaveReason) => void): this;
    /** 远端用户开启音频回调。
    
     @param uid 远端用户ID。
     */
    on(event: 'onUserAudioStart', cb: (uid: number) => void): this;
    /** 远端用户停用音频回调。

     @param uid 远端用户ID。
     */
    on(event: 'onUserAudioStop', cb: (uid: number) => void): this;
    /** 远端用户开启视频回调。

     @param uid 远端用户ID。
     @param max_profile 最大分辨率。
     */
    on(event: 'onUserVideoStart', cb: (uid: number, maxProfile: NERtcVideoProfileType) => void): this;
    /** 远端用户停用视频回调。

     @param uid 远端用户ID。
     */
    on(event: 'onUserVideoStop', cb: (uid: number) => void): this;
    /** 当前通话统计回调。

    SDK 定期向 App 报告当前通话的统计信息，每 2 秒触发一次。

     @param stats NERTC 引擎统计数据: NERtcStats
     */
    on(event: 'onRtcStats', cb: (stats: NERtcStats) => void): this;
    /** 本地音频流统计信息回调。

    该回调描述本地设备发送音频流的统计信息，每 2 秒触发一次。

     @param stats 本地音频流统计信息。详见 NERtcAudioSendStats.
     */
    on(event: 'onLocalAudioStats', cb: (stats: NERtcAudioSendStats) => void): this;
    /** 通话中远端音频流的统计信息回调。

     该回调描述远端用户在通话中端到端的音频流统计信息，每 2 秒触发一次。

     @param stats 每个远端用户音频统计信息的数组。详见 NERtcAudioRecvStats.
     @param user_count stats 数组的大小。
     */
    on(event: 'onRemoteAudioStats', cb: (uc: number, stats: NERtcAudioRecvStats) => void): this;
    /** 本地视频流统计信息回调。

    该回调描述本地设备发送视频流的统计信息，每 2 秒触发一次。

     @param stats 本地视频流统计信息。详见 NERtcVideoSendStats.
     */
    on(event: 'onLocalVideoStats', cb: (stats: NERtcVideoSendStats) => void): this;
    /** 通话中远端视频流的统计信息回调。

     该回调描述远端用户在通话中端到端的视频流统计信息，每 2 秒触发一次。

     @param stats 每个远端用户视频统计信息的数组。详见 NERtcVideoRecvStats.
     @param user_count stats 数组的大小。
     */
    on(event: 'onRemoteVideoStats', cb: (uc: number, stats: NERtcVideoRecvStats) => void): this;
    /** 通话中每个用户的网络上下行质量报告回调。

     该回调描述每个用户在通话中的网络状态，每 2 秒触发一次，只上报状态有变更的成员。

     @param infos 每个用户 ID 和网络上下行质量信息的数组: NERtcNetworkQualityInfo
     @param user_count infos 数组的大小，即用户数。
     */
    on(event: 'onNetworkQuality', cb: (uc: number, stats: NERtcNetworkQualityInfo) => void): this;
    /** 远端用户开启辅流视频回调。

     @param uid 远端用户ID。
     @param max_profile 最大分辨率。
     */
    on(event: 'onUserSubStreamVideoStart', cb: (uid: number, max_profile: NERtcVideoProfileType) => void): this;
    /** 远端用户停用辅流视频回调。

     @param uid 远端用户ID。
     */
    on(event: 'onUserSubStreamVideoStop', cb: (uid: number) => void): this;
    /** 远端用户视频配置更新回调。

     @param uid 远端用户ID。
     @param max_profile 最大分辨率。
     */
    on(event: 'onUserVideoProfileUpdate', cb: (uid: number, max_profile: NERtcVideoProfileType) => void): this;
    /** 远端用户是否静音回调。

     @param uid 远端用户ID。
     @param mute 是否静音。
     */
    on(event: 'onUserAudioMute', cb: (uid: number, mute: boolean) => void): this;
    /** 远端用户是否禁视频流回调。

     @param uid 远端用户ID。
     @param mute 是否禁视频流。
     */
    on(event: 'onUserVideoMute', cb: (uid: number, mute: boolean) => void): this;
    /** 音频设备状态更改回调。

     @param device_id 设备ID。
     @param device_type 音频设备类型。
     @param device_state 音频设备状态。
     */
    on(event: 'onAudioDeviceStateChanged', cb: (device_id: string, device_type: NERtcAudioDeviceType, device_state: NERtcAudioDeviceState) => void): this;
    /** 音频默认设备更改回调。

     @param device_id 设备ID。
     @param device_type 音频设备类型。
     */
    on(event: 'onAudioDefaultDeviceChanged', cb: (device_id: string, device_type: NERtcAudioDeviceType) => void): this;
    /** 视频设备状态更改回调。

     @param device_id 设备ID。
     @param device_type 视频设备类型。
     @param device_state 视频设备状态。
     */
    on(event: 'onVideoDeviceStateChanged', cb: (device_id: string, device_type: NERtcAudioDeviceType, device_state: NERtcAudioDeviceState) => void): this;
    /** 已接收到远端音频首帧回调。

     @param uid 发送音频帧的远端用户的用户 ID。
     */
    on(event: 'onFirstAudioDataReceived', cb: (uid: number) => void): this;
    /** 已显示首帧远端视频回调。

    第一帧远端视频显示在视图上时，触发此调用。

     @param uid 用户 ID，指定是哪个用户的视频流。
     */
    on(event: 'onFirstVideoDataReceived', cb: (uid: number) => void): this;
    /** 已解码远端音频首帧的回调。

     @param uid 远端用户 ID。
     */
    on(event: 'onFirstAudioFrameDecoded', cb: (uid: number) => void): this;
    /** 已接收到远端视频并完成解码回调。

    引擎收到第一帧远端视频流并解码成功时，触发此调用。 App 可在此回调中设置该用户的 video canvas。

     @param uid 用户 ID，指定是哪个用户的视频流。
     @param width 视频流宽（px）。
     @param height 视频流高（px）。

     */
    on(event: 'onFirstVideoFrameDecoded', cb: (uid: number, width: number, height: number) => void): this;
    /** 本地用户的音乐文件播放状态改变回调。

    调用 startAudioMixing 播放混音音乐文件后，当音乐文件的播放状态发生改变时，会触发该回调。

    - 如果播放音乐文件正常结束，state 会返回相应的状态码 kNERtcAudioMixingStateFinished，error_code 返回 kNERtcAudioMixingErrorOK。
    - 如果播放出错，则返回状态码 kNERtcAudioMixingStateFailed，error_code 返回相应的出错原因。
    - 如果本地音乐文件不存在、文件格式不支持、无法访问在线音乐文件 URL，error_code都会返回 kNERtcAudioMixingErrorCanNotOpen。

    @param state 音乐文件播放状态，详见 #NERtcAudioMixingState.
    @param error_code 错误码，详见 #NERtcAudioMixingErrorCode.
    */
    on(event: 'onAudioMixingStateChanged', cb: (state: NERtcAudioMixingState, error_code: NERtcAudioMixingErrorCode) => void): this;
    /** 本地用户的音乐文件播放进度回调。

    调用 startAudioMixing 播放混音音乐文件后，当音乐文件的播放进度改变时，会触发该回调。
    @param timestamp_ms 音乐文件播放进度，单位为毫秒
    */
    on(event: 'onAudioMixingTimestampUpdate', cb: (timestamp_ms: number) => void): this;
    /** 本地音效文件播放已结束回调。

    当播放音效结束后，会触发该回调。
    @param effect_id 指定音效的 ID。每个音效均有唯一的 ID。
    */
    on(event: 'onAudioEffectFinished', cb: (effect_id: number) => void): this;
    /** 提示频道内本地用户瞬时音量的回调。

     该回调默认禁用。可以通过 enableAudioVolumeIndication 方法开启；
     开启后，本地用户说话，SDK 会按 enableAudioVolumeIndication 方法中设置的时间间隔触发该回调。
     如果本地用户将自己静音（调用了 muteLocalAudioStream），SDK 将音量设置为 0 后回调给应用层。

     @param volume （混音后的）音量，取值范围为 [0,100]。
     */
    on(event: 'onLocalAudioVolumeIndication', cb: (volume: number) => void): this;
    /** 提示频道内谁正在说话及说话者瞬时音量的回调。

     该回调默认禁用。可以通过 enableAudioVolumeIndication 方法开启；
     开启后，无论频道内是否有人说话，SDK 都会按 enableAudioVolumeIndication 方法中设置的时间间隔触发该回调。

     在返回的 speakers 数组中:

     - 如果有 uid 出现在上次返回的数组中，但不在本次返回的数组中，则默认该 uid 对应的远端用户没有说话。
     - 如果volume 为 0，表示该用户没有说话。
     - 如果speakers 数组为空，则表示此时远端没有人说话。

     @param speakers 每个说话者的用户 ID 和音量信息的数组: NERtcAudioVolumeInfo
     @param speaker_number speakers 数组的大小，即说话者的人数。
     @param total_volume （混音后的）总音量，取值范围为 [0,100]。
     */
    on(event: 'onRemoteAudioVolumeIndication', cb: (speakers: Array<NERtcAudioVolumeInfo>, speaker_number: number, total_volume: number) => void): this;
    /** 通知添加直播任务结果。

     该回调异步返回 \ref IRtcEngineEx::addLiveStreamTask "addLiveStreamTask" 接口的调用结果；实际推流状态参考 \ref IRtcEngineEventHandlerEx::onLiveStreamState "onLiveStreamState"

     @param task_id 任务id
     @param url 推流地址
     @param error_code 结果
     - 0: 调用成功；
     - 其他: 调用失败。
     */
    on(event: 'onAddLiveStreamTask', cb: (task_id: string, url: string, error: number) => void): this;
    /** 通知更新直播任务结果。

     该回调异步返回 \ref IRtcEngineEx::updateLiveStreamTask "updateLiveStreamTask" 接口的调用结果；实际推流状态参考 \ref IRtcEngineEventHandlerEx::onLiveStreamState "onLiveStreamState"

     @param task_id 任务id
     @param url 推流地址
     @param error_code 结果
     - 0: 调用成功；
     - 其他: 调用失败。
     */
    on(event: 'onUpdateLiveStreamTask', cb: (task_id: string, url: string, error: number) => void): this;
    /** 通知删除直播任务结果。

     该回调异步返回 \ref IRtcEngineEx::removeLiveStreamTask "removeLiveStreamTask" 接口的调用结果；实际推流状态参考 \ref IRtcEngineEventHandlerEx::onLiveStreamState "onLiveStreamState"

     @param task_id 任务id
     @param error_code 结果
     - 0: 调用成功；
     - 其他: 调用失败。
     */
    on(event: 'onRemoveLiveStreamTask', cb: (task_id: string, error: number) => void): this;
    /** 通知直播推流状态

     @param task_id 任务id
     @param url 推流地址
     @param state #NERtcLiveStreamStateCode, 直播推流状态
     - 505: 推流中；
     - 506: 推流失败；
     - 511: 推流结束；
     */
    on(event: 'onLiveStreamState', cb: (task_id: string, url: string, state: NERtcLiveStreamStateCode) => void): this;
    /** 监测音频啸叫的回调。

     @param howling 是否出现啸叫
     - true: 啸叫；
     - false: 正常；。
     */
    on(event: 'onAudioHowling', cb: (howling: boolean) => void): this;
    /** 收到远端流的 SEI 内容回调。

     * @param uid 发送该 sei 的用户 id
     * @param data 接收到的 sei 数据
     */
    on(event: 'onReceSEIMsg', cb: (uid: number, data: ArrayBuffer) => void): this;
    /** 安装声卡回调。

     @param result  返回结果。
     */
    on(event: 'onCheckNECastAudioDriverResult', cb: (result: NERtcInstallCastAudioDriverResult) => void): this;
}
export default NERtcEngine;
