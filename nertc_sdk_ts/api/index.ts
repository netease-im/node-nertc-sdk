import {
    SoftwareRenderer,
    GlRenderer,
    IRenderer,
    CustomRenderer
} from '../renderer';
import {
    NERtcEngineAPI,
    NERtcEngineContext,
    NERtcChannelProfileType,
    NERtcRemoteVideoStreamType,
    NERtcVideoCanvas,
    NERtcErrorCode,
    NERtcSessionLeaveReason,
    NERtcVideoProfileType,
    NERtcAudioProfileType,
    NERtcAudioScenarioType,
    NERtcVideoConfig,
    NERtcAudioFrameRequestFormat,
    NERtcCreateAudioMixingOption,
    NERtcCreateAudioEffectOption,
    NERtcRectangle,
    NERtcScreenCaptureParameters,
    NERtcDevice,
    NERtcStats,
    NERtcAudioSendStats,
    NERtcAudioRecvStats,
    NERtcVideoSendStats,
    NERtcVideoRecvStats,
    NERtcNetworkQualityInfo,
    NERtcClientRole,
    NERtcConnectionStateType,
    NERtcReasonConnectionChangedType,
    NERtcAudioDeviceType,
    NERtcAudioDeviceState,
    NERtcAudioMixingState,
    NERtcAudioMixingErrorCode,
    NERtcAudioVolumeInfo,
    NERtcLiveStreamStateCode,
    NERtcLiveStreamTaskInfo,
    NERtcVideoMirrorMode,
    NERtcVideoScalingMode,
    NERtcVoiceChangerType,
    NERtcVoiceBeautifierType,
    NERtcVoiceEqualizationBand,
    NERtcStreamChannelType,
    NERtcPullExternalAudioFrameCb,
    NERtcChannelMediaRelayConfiguration,
    NERtcVideoStreamType,
    NERtcMediaPriorityType,
    NERtcScreenCaptureWindowParam,
    NERtcAudioRecordingQuality,
    NERtcEncryptionConfig,
    NERtcJoinChannelOptions,
    NERtcJoinChannelOptionsEx,
    NERtcCameraCaptureConfig,
    NERtcVideoOutputOrientationMode,
    NERtcDistanceRolloffModel,
    NERtcSpatializerPositionInfo,
    NERtcSpatializerRoomProperty,
    NERtcSpatializerRenderMode,
    NERtcAudioRecordingConfiguration,
    VirtualBackgroundSource,
    NERtcReverbParam,
    NERtcSize,
    CaptureSoureInfo,
    NERtcFeatureType,
    NERtcScreenCaptureSourceData
} from './defs'
import { EventEmitter } from 'events'
import process from 'process';
// const nertc = require('bindings')('nertc-electron-sdk');
const nertc = require('../../build/Release/nertc-electron-sdk.node');
const NERtcChannel = require('./channel').default;

/**
 * @class NERtcEngine
 */
class NERtcEngine extends EventEmitter {
    nertcEngine: NERtcEngineAPI;
    renderers: Map<string, IRenderer>;
    substreamRenderers: Map<string, IRenderer>;
    renderMode: 1 | 2 | 3;
    customRenderer: any;

    /**
     * NERtcEngine类构造函数
     * @returns {NERtcEngine}
     */
    constructor() {
        super();
        this.nertcEngine = new nertc.NertcNodeEngine();
        this.initEventHandler();
        this.renderers = new Map();
        this.substreamRenderers = new Map();
        this.renderMode = this._checkWebGL() ? 1 : 2;
        this.customRenderer = CustomRenderer;
    }

    /**
     * 创建一个 NERtcChannel 对象
     * @since V5.4.0
     * <pre>
     * 设置相同房间名称的用户会进入同一个通话房间。
     * </pre>
     * @param {string} name  房间名 字符串格式，长度为1~ 64 字节。支持以下89个字符：a-z, A-Z, 0-9, space, !#$%&()+-:;≤.,>? @[]^_{|}~”
     * @returns {object} NERtcChannel对象
     */
    createChannel(channelName: string): any {
        let nertcChannel = new nertc.NertcNodeChannel(channelName);
        return new NERtcChannel(channelName, nertcChannel);
    }

    /**
     * 初始化 NERTC SDK 服务。
     * <pre>
     * 该方法用来进行初始化 NERTC 服务。传入云信为开发者签发的厂商秘钥进行初始化。在使用其他方法前，必须先调用该方法进行初始化。
     * 初始化成功后，默认处于音视频通话模式。
     * <b>NOTE:</b>
     * - 必须使用同一个 App Key 才能进行通话。
     * - 一个 NERtcEngine 实例对象只能使用一个 App Key。如需更换 App Key，必须先调用 {@link NERtcEngine#release} 方法释放当前实例资源再调用本方法初始化实例。
     * </pre>
     * @param {Object} context 传入的 RTC engine context 对象: NERtcEngineContext。
     * @param {String} context.app_key 用户注册云信的 APP Key。如果你的开发包里面缺少 APP Key，请申请注册一个新的 APP Key。
     * @param {String} context.log_dir_path 日志目录的完整路径，采用 UTF-8 编码。
     * @param {number} [context.log_level=3] 日志级别，默认级别为 kNERtcLogLevelInfo。
     * <pre>
     * - 0 Fatal 级别日志信息。
     * - 1 Error 级别日志信息。
     * - 2 Warning 级别日志信息。
     * - 3 Info 级别日志信息。默认级别。
     * - 4 DetailInfo 级别日志信息。
     * - 5 Verbos 级别日志信息。
     * - 6 Debug 级别日志信息。如果你想获取最完整的日志，可以将日志级别设为该等级。
     * - 7 不输出日志信息。
     * </pre>
     * @param {number} [context.log_file_max_size_KBytes=20480] 指定 SDK 输出日志文件的大小上限，单位为 KB。如果设置为 0，则默认为 20 M。
     * @param {Object} context.server_config 私有化服务器地址，默认需要置空, 如需启用私有化功能，请联系技术支持获取详情。
     * @param {String} context.server_config.channel_server 获取通道信息服务器。
     * @param {String} context.server_config.statistics_server 统计上报服务器。
     * @param {String} context.server_config.room_server roomServer服务器。
     * @param {String} context.server_config.compat_server 兼容性配置服务器
     * @param {String} context.server_config.nos_lbs_server nos 域名解析服务器
     * @param {String} context.server_config.nos_upload_sever 默认nos 上传服务器
     * @param {String} context.server_config.nos_token_server 获取NOS token 服务器
     * @param {Boolean} context.server_config.use_ipv6 是否使用IPv6（默认false)
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    initialize(context: NERtcEngineContext): number {
        return this.nertcEngine.initialize(context);
    }

    /**
     * 释放资源。
     */
    release(): void {
        return this.nertcEngine.release();
    }

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
    setClientRole(role: NERtcClientRole): number {
        return this.nertcEngine.setClientRole(role);
    }

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
    setChannelProfile(profile: NERtcChannelProfileType): number {
        return this.nertcEngine.setChannelProfile(profile);
    } 

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
    joinChannel(token: String, channelName: String, uid: number): number {
        return this.nertcEngine.joinChannel(token, channelName, uid);
    }

    /**
     * 加入频道。如果频道还未创建，会自动尝试创建频道。
     * @since V5.4.0
     * <pre>
     * 该方法让用户加入通话频道，在同一个频道内的用户可以互相通话，多个用户加入同一个频道，可以群聊。 使用不同 App Key 的 App 是不能互通的。如果已在通话中，用户必须调用 {@link NERtcEngine#leaveChannel} 退出当前通话，才能进入下一个频道。
     * 频道内每个用户的用户 ID 必须是唯一的。
     * </pre>
     * @param {String} token 动态秘钥。安全要求不高: 将值设为 空字符串。安全要求高: 将值设置为 Token。如果你已经启用了 App Certificate, 请务必使用 Token。
     * @param {String} channelName 标识通话的频道名称，长度在 64 字节以内的字符串。以下为支持的字符集范围（共 89 个字符）: a-z, A-Z, 0-9, space, !#$%&()+-:;&le;.,>? @[]^_{|}~”
     * @param {number} uid 用户ID。
     * @param {Object} channelOptions 加入音视频房间时的一些可选信息。
     * @param {string} channelOptions.custom_info 自定义信息，最长支持 127 个字符。
     * @param {string} channelOptions.permission_key 权限密钥。能控制通话时长及媒体权限能力。
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    joinChannelWithOptions(token: string, channelName: string, uid: number, channelOptions: NERtcJoinChannelOptions): number {
        return this.nertcEngine.joinChannelWithOptions(token, channelName, uid, channelOptions);
    }

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
    leaveChannel(): number {
        return this.nertcEngine.leaveChannel();
    }

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
    enableLocalAudio(enabled: Boolean): number {
        return this.nertcEngine.enableLocalAudio(enabled);
    }

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
    setupLocalVideoCanvas(canvas: NERtcVideoCanvas): number {
        if (canvas.view) {
            //bind
            this.initRender('local', canvas.view);
            this.setRenderMode('local', canvas.mode);
            return this.nertcEngine.setupVideoCanvas(0, true);
        } else {
            //unbind
            this.destroyRender('local');
            return this.nertcEngine.setupVideoCanvas(0, false);
        }
    }

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
    setupRemoteVideoCanvas(uid: number, canvas: NERtcVideoCanvas): number {
        if (canvas.view) {
            //bind
            this.initRender(uid, canvas.view);
            this.setRenderMode(uid, canvas.mode);
            return this.nertcEngine.setupVideoCanvas(uid, true);
        } else {
            //unbind
            this.destroyRender(uid);
            return this.nertcEngine.setupVideoCanvas(uid, false);
        }
    }

    /**
     * 在指定用户的画布上截图。
     * @param {'local'|number} uid 指定用户的 uid，其中在本地用户画布上截图时，此参数为 local。
     * @param {NERtcVideoStreamType} streamType 视频流类型。
     * <pre>
     * - 0：视频流主流。
     * - 1：视频流辅流。
     * </pre>
     * @returns string 返回 base64 截图数据。
     */
    captureImageByUid(uid: 'local' | number, streamType: NERtcVideoStreamType = NERtcVideoStreamType.kNERtcVideoStreamMain): string {
        return this.captureRender(uid, streamType)
    }

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
    enableLocalVideo(enabled: Boolean): number {
        return this.nertcEngine.enableLocalVideo(enabled);
    }

    /**
     * 开启或关闭本地视频采集和渲染
     * @since V5.4.0
     * <pre>
     * 该方法启用本地视频采集功能。
     * 该方法设置内部引擎为启用状态，在 {@link NERtcEngine#leaveChannel} 后仍然有效。
     * </pre>
     * @param {number} streamType 视频通道类型：
     * <pre>
     * - 0: 主流；
     * - 1: 辅流。
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
    enableLocalVideoWithType(streamType: NERtcVideoStreamType, enabled: boolean): number {
        return this.nertcEngine.enableLocalVideoWithType(streamType, enabled);
    }

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
    subscribeRemoteVideoStream(uid: number, type: NERtcRemoteVideoStreamType, subscribe: Boolean): number {
        return this.nertcEngine.subscribeRemoteVideoStream(uid, type, subscribe);
    }

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
    getConnectionState(): number {
        return this.nertcEngine.getConnectionState();
    }

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
    muteLocalAudioStream(enabled: Boolean): number {
        return this.nertcEngine.muteLocalAudioStream(enabled);
    }

    /**
     * 开启或关闭音频辅流。
     * @since V5.4.0
     * <pre>
     * 开启时远端会收到onUserSubStreamAudioStart，关闭时远端会收到onUserSubStreamAudioStop。
     * <b>NOTE:</b>
     * - 该方法设置内部引擎为启用状态，在{@link NERtcEngine#leaveChannel}后仍然有效。
     * </pre>
     * @param {boolean} enabled 是否开启音频辅流：
     * <pre>
     * - true: 开启音频辅流。
     * - false: 关闭音频辅流。
     * </pre>
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    enableLocalSubStreamAudio(enabled: boolean): number {
        return this.nertcEngine.enableLocalSubStreamAudio(enabled);
    }

    /**
     * 静音或解除静音本地上行的音频辅流。
     * @since V5.4.0
     * <pre>
     * <b>NOTE:</b>
     * - 静音状态会在通话结束后被重置为非静音。
     * - 该方法仅可在加入房间后调用。
     * </pre>
     * @param {boolean} mute 是否静音本地音频辅流发送。
     * <pre>
     * - true: 静音本地音频辅流（默认）。
     * - false: 取消静音本地音频辅流。
     * </pre>
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    muteLocalSubStreamAudio(enabled: boolean): number {
        return this.nertcEngine.muteLocalSubStreamAudio(enabled);
    }

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
    setAudioProfile(profile: NERtcAudioProfileType, scenario: NERtcAudioScenarioType): number {
        return this.nertcEngine.setAudioProfile(profile, scenario);
    }

    /**
     * 订阅／取消订阅指定音频主流。
     * @param {number} uid 指定用户的 ID
     * @param {boolean} subscribe
     * <pre>
     * - true: 订阅指定音频主流（默认）。
     * - false: 取消订阅指定音频主流。
     * </pre>
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    subscribeRemoteAudioStream(uid: number, enabled: Boolean): number {
        return this.nertcEngine.subscribeRemoteAudioStream(uid, enabled);
    }

    /**
     * 订阅／取消订阅指定音频辅流
     * @since V5.4.0
     * @param {number} uid 指定用户的 ID
     * @param {boolean} subscribe
     * <pre>
     * - true: 订阅指定音频辅流（默认）。
     * - false: 取消订阅指定音频辅流。
     * </pre>
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    subscribeRemoteSubStreamAudio(uid: number, subscribe: boolean): number {
        return this.nertcEngine.subscribeRemoteSubStreamAudio(uid, subscribe);
    }

    /**
     * 取消或恢复订阅所有远端用户的音频主流。
     * @since V5.4.0
     * <pre>
     * 加入房间时，默认订阅所有远端用户的音频主流。
     * <b>NOTE:</b>
     * - 设置该方法的 subscribe 参数为 true 后，对后续加入房间的用户同样生效。
     * - 在开启自动订阅（默认）时，设置该方法的 subscribe 参数为 false 可以实现取消订阅所有远端用户的音频流，但此时无法再调用{@link NERtcEngine#subscribeRemoteAudioStream}方法单独订阅指定远端用户的音频流。
     * </pre>
     * @param {boolean} subscribe
     * <pre>
     * - true: 订阅所有远端用户的音频主流。
     * - false: 取消订阅所有远端用户的音频主流。
     * </pre>
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    subscribeAllRemoteAudioStream(subscribe: boolean): number {
        return this.nertcEngine.subscribeAllRemoteAudioStream(subscribe);
    }

    /**
     * 设置自己的音频只能被房间内指定的人订阅。
     * @since V5.4.0
     * <pre>
     * 默认房间所有其他人都可以订阅自己的音频。
     * <b>NOTE:</b>
     * - 此接口需要在加入房间成功后调用。
     * - 对于调用接口时不在房间的 uid 不生效。
     * </pre>
     * @param {Array<Number>} uids 用户id数组
     * @param {number} size 数组长度
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    setAudioSubscribeOnlyBy(uids: Array<Number>, size: number): number { //[12, 34, 56]
        return this.nertcEngine.setAudioSubscribeOnlyBy(uids, size);
    }

    /**
     * 你可以调用该方法指定只订阅的音频流。
     * <pre>
     *  - 此接口需要在加入房间成功后调用。
     *  - 对于调用接口时不在房间的 uid 不生效。
     * </pre>
     * @param {Array<Number>} uids 只订阅此用户uid列表 的音频.此列表为全量列表。如果列表为空或 null，取消订阅白名单。例如：[uid1,uid2...]。
     * @param {number} size 数组长度
     * @return {number}
     * <pre>
     * - 0: 方法调用成功。
     * - 其他：方法调用失败。
     * </pre>
     */
    setSubscribeAudioAllowlist(uids: Array<Number>, size: number): number {
        return this.nertcEngine.setSubscribeAudioAllowlist(uids, size);
    }

    /**
     * 你可以调用该方法指定不订阅的音频流。
     * <pre>
     *  - 此接口需要在加入房间成功后调用。
     *  - 对于调用接口时不在房间的 uid 不生效。
     * </pre>
     * @param {number} audioStreamType 音频流类型。
     * <pre>
     * - 0: 主流
     * - 1: 辅流
     * </pre>
     * @param {Array<Number>} uids 只订阅此用户uid列表 的音频.此列表为全量列表。如果列表为空或 null，取消订阅白名单。例如：[uid1,uid2...]。
     * @param {number} size 数组长度
     * @return {number}
     * <pre>
     * - 0: 方法调用成功。
     * - 其他：方法调用失败。
     * </pre>
     */
    setSubscribeAudioBlocklist(audioStreamType: number, uids: Array<Number>, size: number): number {
        return this.nertcEngine.setSubscribeAudioBlocklist(audioStreamType, uids, size);
    }

    /**
     * 开启精准对齐。
     * @since V5.4.0
     * <pre>
     * 通过此接口可以实现精准对齐功能，对齐本地系统与服务端的时间。
     * <b>NOTE:</b>
     * - 请在引擎初始化之后调用此接口，且该方法仅可在加入房间前调用。
     * - 适用于 KTV 实时合唱的场景。
     * </pre>
     * @param {boolean} enable 是否开启精准对齐功能:
     * <pre>
     * true：开启精准对齐功能。
     * false：关闭精准对齐功能。
     * </pre>
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    setStreamAlignmentProperty(enable: boolean): number {
        return this.nertcEngine.setStreamAlignmentProperty(enable);
    }

    /**
     * 获取本地系统时间与服务端时间差值。
     * @since V5.4.0
     * <pre>
     * 可以用于做时间对齐，通过 (毫秒级系统时间 - offset) 可能得到当前服务端时间。
     * </pre>
     * @returns {number}
     * <pre>
     * - 本地与服务端时间差值，单位为毫秒（ms）。如果没有成功加入音视频房间，返回 0。
     * </pre>
     */
    getNtpTimeOffset(): number {
        return this.nertcEngine.getNtpTimeOffset();
    }

     /**
     * 设置本地摄像头的视频主流采集配置。
     * @since V5.4.0
     * <pre>
     * 通过此接口可以设置本地摄像头采集的主流视频宽度、高度、旋转角度等。
     * <b>NOTE:</b>
     * - 纯音频 SDK 禁用该接口。
     * - 该方法仅适用于视频主流。
     * - 该方法支持在加入房间后动态调用，设置成功后，会自动重启摄像头采集模块。
     * - 若系统相机不支持您设置的分辨率，会自动调整为最相近一档的分辨率，因此建议您设置为常规标准的分辨率。
     * - 设置较高的采集分辨率会增加性能消耗，例如 CPU 和内存占用等，尤其是在开启视频前处理的场景下。
     * </pre>
     * @param {object} config 摄像头采集配置:
     * @param {number} config.captureWidth 本地采集的视频宽度，单位为 px。
     * @param {number} config.captureHeight 本地采集的视频高度，单位为 px。
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    setCameraCaptureConfig(config: NERtcCameraCaptureConfig): number {
        return this.nertcEngine.setCameraCaptureConfig(config);
    }

    /**
     * 设置本地摄像头的视频主流或辅流采集配置。
     * @since V5.4.0
     * <pre>
     * 通过此接口可以设置本地摄像头采集的主流或辅流视频宽度、高度、旋转角度等。
     * <b>NOTE:</b>
     * - 纯音频 SDK 禁用该接口。
     * - 调用该接口设置成功后，会自动重启摄像头采集模块。
     * </pre>
     * @param {NERtcVideoStreamType} streamType 视频通道类型。
     * <pre>
     * - 0: 主流。
     * - 1: 辅流。
     * </pre>
     * @param {object} config 摄像头采集配置:
     * @param {number} config.captureWidth 本地采集的视频宽度，单位为 px。
     * @param {number} config.captureHeight 本地采集的视频高度，单位为 px。
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    setCameraCaptureConfigWithType(streamType: NERtcVideoStreamType, config: NERtcCameraCaptureConfig): number {
        return this.nertcEngine.setCameraCaptureConfigWithType(streamType, config);
    }

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
     * @param {number} config.mirror_mode 设置本地视频编码的镜像模式，即本地发送视频的镜像模式，只影响远端用户看到的视频画面:
     * <pre>
     * - 0 Windows/macOS SDK 启用镜像模式。在 iOS/Android 平台中：如果你使用前置摄像头，SDK 默认启用镜像模式；如果你使用后置摄像头，SDK 默认关闭镜像模式。
     * - 1 启用镜像模式。
     * - 2 清晰度优先
     * - 3 关闭镜像模式(默认)。
     * </pre>
     * @param {number} config.orientation_mode 编码策略:
     * <pre>
     * - 0 该模式下 SDK 输出的视频方向与采集到的视频方向一致。接收端会根据收到的视频旋转信息对视频进行旋转（默认）。
     * - 1 该模式下 SDK 固定输出横屏模式的视频。如果采集到的视频是竖屏模式，则视频编码器会对其进行裁剪。
     * - 2 该模式下 SDK 固定输出竖屏模式的视频，如果采集到的视频是横屏模式，则视频编码器会对其进行裁剪。
     * </pre>
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    setVideoConfig(config: NERtcVideoConfig): number {
        return this.nertcEngine.setVideoConfig(config);
    }

    /**
     * 设置视频配置。
     * @since V5.4.0
     * <pre>
     * 该方法设置视频配置。每个属性对应一套视频参数，如分辨率等，会在摄像头重启后生效。 所有设置的参数均为理想情况下的最大值。当视频引擎因网络环境等原因无法达到设置的分辨率的最大值时，会取最接近最大值的那个值。
     * </pre>
     * @param {number} streamType 视频通道类型。
     * <pre>
     * - 0: 主流。
     * - 1: 辅流。
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
     * @param {number} config.mirror_mode 设置本地视频编码的镜像模式，即本地发送视频的镜像模式，只影响远端用户看到的视频画面:
     * <pre>
    * - 0 Windows/macOS SDK 启用镜像模式。
     * - 1 启用镜像模式。
     * - 2 （默认）关闭镜像模式。
     * </pre>
     * @param {NERtcVideoOutputOrientationMode} config.orientation_mode 编码策略:
     * <pre>
     * - 0 该模式下 SDK 输出的视频方向与采集到的视频方向一致。接收端会根据收到的视频旋转信息对视频进行旋转（默认）。
     * - 1 该模式下 SDK 固定输出横屏模式的视频。如果采集到的视频是竖屏模式，则视频编码器会对其进行裁剪。
     * - 2 该模式下 SDK 固定输出竖屏模式的视频，如果采集到的视频是横屏模式，则视频编码器会对其进行裁剪。
     * </pre>
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    setVideoConfigWithType(streamType: NERtcVideoStreamType, config: NERtcVideoConfig): number {
        return this.nertcEngine.setVideoConfigWithType(streamType, config);
    }

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
    enableDualStreamMode(enabled: Boolean): number {
        return this.nertcEngine.enableDualStreamMode(enabled);
    }

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
    setupLocalSubStreamVideoCanvas(canvas: NERtcVideoCanvas): number {
        if (canvas.view) {
            //bind
            this.initSubStreamRender('local', canvas.view);
            this.setSubStreamRenderMode('local', canvas.mode);
            return this.nertcEngine.setupSubStreamVideoCanvas(0, true);
        } else {
            //unbind
            this.destroySubStreamRender('local');
            return this.nertcEngine.setupSubStreamVideoCanvas(0, false);
        }
    }

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
    setRenderMode(uid: 'local' | number, mode: NERtcVideoScalingMode): number {
        if (this.renderers.has(String(uid))) {
            let renderer = this.renderers.get(String(uid));
            (renderer as IRenderer).setContentMode(mode);
            return 0;
        }
        else {
            return -1;
        }
    }

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
    setSubStreamRenderMode(uid: 'local' | number, mode: NERtcVideoScalingMode): number {
        if (this.substreamRenderers.has(String(uid))) {
            let renderer = this.substreamRenderers.get(String(uid));
            (renderer as IRenderer).setContentMode(mode);
            return 0;
        }
        else {
            return -1;
        }
    }

    /**
     * 设置本地视频镜像模式。
     * <pre>
     *  该方法设置本地视频镜像模式。 App 可以多次调用此方法更改镜像模式。
     * </pre>
     * @param {number} mode  视频镜像模式:
     * <pre>
     * - 0 默认关闭镜像模式。
     * - 1 启用镜像模式。
     * - 2 关闭镜像模式。
     * </pre>
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    setLocalVideoMirrorMode(mode: NERtcVideoMirrorMode): number {
        return this.nertcEngine.setLocalVideoMirrorMode(mode);
    }

     /**
     * 设置本地视频镜像模式。
     * @since V5.4.0
     * <pre>
     * 通过此接口可以设置本地视频是否开启镜像模式，即画面是否左右翻转。
     * <b>NOTE:</b>
     * - 纯音频 SDK 禁用该接口
     * - 本地视频画布的镜像模式仅影响本地用户所见，不影响远端用户所见。您的应用层可以多次调用此方法更改镜像模式。
     * </pre>
     * @param {number} streamType 视频通道类型。
     * <pre>
     * - 0: 主流。
     * - 1: 辅流。
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
     setLocalVideoMirrorModeWithType(streamType: NERtcVideoStreamType, mode: NERtcVideoMirrorMode): number {
        return this.nertcEngine.setLocalVideoMirrorModeWithType(streamType, mode);
    }

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
    setupRemoteSubStreamVideoCanvas(uid: number, canvas: NERtcVideoCanvas): number {
        if (canvas.view) {
            //bind
            this.initSubStreamRender(uid, canvas.view);
            this.setSubStreamRenderMode(uid, canvas.mode);
            return this.nertcEngine.setupSubStreamVideoCanvas(uid, true);
        } else {
            //unbind
            this.destroySubStreamRender(uid);
            return this.nertcEngine.setupSubStreamVideoCanvas(uid, false);
        }
    }

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
    subscribeRemoteVideoSubStream(uid: number, sub: boolean): number {
        return this.nertcEngine.subscribeRemoteVideoSubStream(uid, sub);
    }

    /**
     * 开启视频预览。
     * <pre>
     * 该方法用于在进入频道前启动本地视频预览。调用该 API 前，必须:
     * - 调用 {@link NERtcEngine#setupLocalVideoCanvas} 设置预览窗口；
     * - 调用 {@link NERtcEngine#setVideoDevice} 前必须先设置设备id；
     * <b>NOTE:</b> 启用了本地视频预览后，在进入频道前，本地预览必须先关闭，需要调用 {@link NERtcEngine#stopVideoPreview}。
     * </pre>
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    startVideoPreview(): number {
        return this.nertcEngine.startVideoPreview();
    }

    /**
     * 开启视频预览。
     * @since V5.4.0
     * <pre>
     * 该方法用于在进入频道前启动本地视频预览。调用该 API 前，必须:
     * - 调用 {@link NERtcEngine#setupLocalVideoCanvas} 设置预览窗口；
     * - 调用 {@link NERtcEngine#setVideoDevice} 前必须先设置设备id；
     * <b>NOTE:</b> 启用了本地视频预览后，在进入频道前，本地预览必须先关闭，需要调用 {@link NERtcEngine#stopVideoPreview}。
     * </pre>
     * @param {number} streamType 视频通道类型。
     * <pre>
     * - 0: 主流。
     * - 1: 辅流。
     * </pre>
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    startVideoPreviewWithType(streamType: number): number {
        return this.nertcEngine.startVideoPreviewWithType(streamType);
    }

    /**
     * 停止视频预览。
     * <pre>
     * 通过此接口可以实现在预览本地视频后关闭预览。
     * </pre>
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    stopVideoPreview(): number {
        return this.nertcEngine.stopVideoPreview();
    }

    /**
     * 停止视频预览。
     * <pre>
     * 通过此接口可以实现在预览本地视频后关闭预览。
     * </pre>
     * @param {number} streamType 视频通道类型。
     * <pre>
     * - 0: 主流。
     * - 1: 辅流。
     * </pre>
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    stopVideoPreviewWithType(streamType: number): number {
        return this.nertcEngine.stopVideoPreviewWithType(streamType);
    }

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
    muteLocalVideoStream(enabled: Boolean): number {
        return this.nertcEngine.muteLocalVideoStream(enabled);
    }

    /**
     * 开关本地视频发送。
     * <pre>
     * 调用该方法禁视频流时，SDK 不再发送本地视频流，但摄像头仍然处于工作状态。相比于 {@link NERtcEngine#enableLocalVideo} (false) 用于控制本地视频流发送的方法，该方法响应速度更快。该方法不影响本地视频流获取，没有禁用摄像头。
     * </pre>
     * @param {number} streamType 视频通道类型。
     * <pre>
     * - 0: 视频主流。
     * - 1: 视频辅流。
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
    muteLocalVideoStreamWithType(streamType: number, enabled: boolean): number {
        return this.nertcEngine.muteLocalVideoStreamWithType(streamType, enabled);
    }

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
    setParameters(parameters: String): number {
        return this.nertcEngine.setParameters(parameters);
    }

    /**
     * 以String 的形式获取一些内部参数。
     * @since V5.3.0
     * <pre>
    * <b>NOTE:</b>
     * - 此接口为隐藏接口，需要特定参数及特定时机，详情联系技术支持。
     * - 请在初始化后调用该方法，且该方法在加入房间前后均可调用。
     * </pre>
     * @param {String} parameters 音视频通话的参数集合.
     * @param {String} extra_info 额外的信息。
     * @returns {String} 内部参数信息。
     */
    getParameters(parameters: String, extra_info: String): String {
        return this.nertcEngine.getParameters(parameters, extra_info);
    }

    //  /**
    //  * 设置采集的音频格式。
    //  * @since V5.4.0
    //  * <pre>
    //  * <b>NOTE:</b>
    //  * - 请在初始化后调用该方法，且该方法在加入房间前后均可调用。
    //  * - 适用于需要监听音频 PCM 采集数据回调并指定回调的数据格式的场景。
    //  * - 若您希望使用音频的原始格式，format 参数传 NULL 即可。
    //  * </pre>
    //  * @param {object} format 音频帧请求格式
    //  * @param {Number} format.channels 音频声道数量:
    //  * <pre>
    //  * - 1 单声道
    //  * - 2 双声道
    //  * </pre>
    //  * @param {Number} format.sample_rate 采样率。
    //  * @param {number} format.mode 读写模式：
    //  * <pre>
    //  * 0 返回数据只读模式
    //  * 1 返回数据可读写
    //  * </pre>
    //  * @returns {number}
    //  * <pre>
    //  * - 0: 方法调用成功；
    //  * - 其他: 方法调用失败。
    //  * </pre>
    //  */
    // setRecordingAudioFrameParameters(format: NERtcAudioFrameRequestFormat): number {
    //     return this.nertcEngine.setRecordingAudioFrameParameters(format);
    // }

    // /**
    //  * 设置音频播放回调的声音格式。
    //  * @since V5.4.0
    //  * <pre>
    //  * <b>NOTE:</b>
    //  * - 请在初始化后调用该方法，且该方法仅可在加入房间后调用。
    //  * - 适用于需要自行对待播放的声音进行二次处理的场景。
    //  * </pre>
    //  * @param {object} format 音频帧请求格式
    //  * @param {Number} format.channels 音频声道数量:
    //  * <pre>
    //  * - 1 单声道
    //  * - 2 双声道
    //  * </pre>
    //  * @param {Number} format.sample_rate 采样率。
    //  * @param {number} format.mode 读写模式：
    //  * <pre>
    //  * 0 返回数据只读模式
    //  * 1 返回数据可读写
    //  * </pre>
    //  * @returns {number}
    //  * <pre>
    //  * - 0: 方法调用成功；
    //  * - 其他: 方法调用失败。
    //  * </pre>
    //  */
    // setPlaybackAudioFrameParameters(format: NERtcAudioFrameRequestFormat): number {
    //     return this.nertcEngine.setPlaybackAudioFrameParameters(format);
    // }

    //  /**
    //  * 设置采集和播放声音混音后的音频数据采样率。
    //  * @since V5.4.0
    //  * <pre>
    //  * 通过本接口可以实现设置 onMixedAudioFrame回调的混音音频采样率
    //  * <b>NOTE:</b>
    //  * - 请在初始化后调用该方法，且该方法在加入房间前后均可调用。
    //  * - 适用于需要获取本地用户和远端所有用户的声音的场景，比如通话录音的场景。
    //  * - 该方法设置内部引擎为启用状态，在离开房间后设置会重置为默认状态。
    //  * - 未调用该接口设置返回的音频数据格式时，回调中的采样率取默认值。
    //  * </pre>
    //  * @param {Number} sample_rate 采样率。
    //  * @returns {number}
    //  * <pre>
    //  * - 0: 方法调用成功；
    //  * - 其他: 方法调用失败。
    //  * </pre>
    //  */
    // setMixedAudioFrameParameters(sample_rate: number): number {
    //     return this.nertcEngine.setMixedAudioFrameParameters(sample_rate);
    // }

    /**
     * 开启音频dump。
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    startAudioDump(): number {
        return this.nertcEngine.startAudioDump();
    }

    /**
     * 开启音频dump。
     * @since V5.4.0
     * @param {number} type 音频dump类型。
     * <pre>
     * - 0: 仅输出.dump文件（默认）。
     * - 1: 输出.dump和.wav文件。
     * - 2: 仅输出.wav文件.
     * </pre>
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    startAudioDumpWithType(type: number): number {
        return this.nertcEngine.startAudioDumpWithType(type);
    }

    /**
     * 结束音频dump。
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    stopAudioDump(): number {
        return this.nertcEngine.stopAudioDump();
    }

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
     * @param {number} [option.start_timestamp] 音乐文件开始播放的时间，UTC 时间戳，即从1970 年 1 月 1 日 0 点 0 分 0 秒开始到事件发生时的毫秒数。默认值为 0，表示立即播放。
     * @param {number} [option.NERtcAudioStreamType] 伴音跟随音频主流还是辅流，默认跟随主流。
     * @param {number} [option.progress_interval] 伴音播放进度回调间隔，单位ms，取值范围为 100~10000, 默认1000ms。
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    startAudioMixing(opt: NERtcCreateAudioMixingOption): number {
        return this.nertcEngine.startAudioMixing(opt);
    }

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
    stopAudioMixing(): number {
        return this.nertcEngine.stopAudioMixing();
    }

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
    pauseAudioMixing(): number {
        return this.nertcEngine.pauseAudioMixing();
    }

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
    resumeAudioMixing(): number {
        return this.nertcEngine.resumeAudioMixing();
    }

    /**
     * 调节伴奏发送音量。
     * <pre>
     * 该方法调节混音里伴奏的发送音量大小。请在频道内调用该方法。
     * </pre>
     * @param {number} volume 伴奏音量范围为 0~200。默认 100 为原始文件音量。
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    setAudioMixingSendVolume(volume: number): number {
        return this.nertcEngine.setAudioMixingSendVolume(volume);
    }

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
    getAudioMixingSendVolume(): number {
        return this.nertcEngine.getAudioMixingSendVolume();
    }

    /**
     * 调节伴奏播放音量。
     * <pre>
     * 该方法调节混音里伴奏的播放音量大小。请在频道内调用该方法。
     * </pre>
     * @param {number} volume 伴奏音量范围为 0~200。默认 100 为原始文件音量。
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    setAudioMixingPlaybackVolume(volume: number): number {
        return this.nertcEngine.setAudioMixingPlaybackVolume(volume);
    }

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
    getAudioMixingPlaybackVolume(): number {
        return this.nertcEngine.getAudioMixingPlaybackVolume();
    }

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
    getAudioMixingDuration(): number {
        return this.nertcEngine.getAudioMixingDuration();
    }

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
    getAudioMixingCurrentPosition(): number {
        return this.nertcEngine.getAudioMixingCurrentPosition();
    }

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
    setAudioMixingPosition(pos: number): number {
        return this.nertcEngine.setAudioMixingPosition(pos);
    }

    /**
     * 设置当前伴音文件的音调。
     * @since v5.4.0
     * <pre>
     * 通过此接口可以实现当本地人声和播放的音乐文件混音时，仅调节音乐文件的音调。
     * <b>NOTE:</b>
     * - 当前伴音任务结束后，此接口的设置会恢复至默认。
     * </pre>
     * @param {number} pitch 当前伴音文件的音调。默认值为 0，即不调整音调，取值范围为 -12 ~ 12
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    setAudioMixingPitch(pitch: number): number {
        return this.nertcEngine.setAudioMixingPitch(pitch);
    }

     /**
     * 获取当前伴音文件的音调。
     * @since v5.4.0
     * <pre>
     * 请先调用startAudioMixing方法开启伴音。
     * @returns {number} 当前伴音文件音调
     */
    getAudioMixingPitch(): number {
        return this.nertcEngine.getAudioMixingPitch();
    }

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
     * @param {number} [opt[].send_volume] 发送音量。最大为 100（默认）含义（0%-100%）
     * @param {boolean} [opt[].playback_enabled=true] 是否可回放，默认为 true
     * @param {number} [opt[].playback_volume]  回放音量。最大为 100（默认）
     * @param {number} [opt[].start_timestamp] 音乐文件开始播放的时间，UTC 时间戳，即从1970 年 1 月 1 日 0 点 0 分 0 秒开始到事件发生时的毫秒数。默认值为 0，表示立即播放。
     * @param {number} [opt[].NERtcAudioStreamType] 伴音跟随音频主流还是辅流，默认跟随主流。
     * @param {number} [opt[].progress_interval] 伴音播放进度回调间隔，单位ms，取值范围为 100~10000, 默认1000ms。
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    playEffect(effectId: number, opt: Array<NERtcCreateAudioEffectOption>): number {
        return this.nertcEngine.playEffect(effectId, opt);
    }

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
    stopEffect(effectId: number): number {
        return this.nertcEngine.stopEffect(effectId);
    }

    /**
     * 设置指定音效文件的音调。
     * @since v5.4.0
     * <pre>
     * 通过此接口可以实现当本地人声和播放的音乐文件混音时，仅调节音乐文件的音调。
     * <b>NOTE:</b>
     * - 当前音效任务结束后，此接口的设置会恢复至默认。
     * </pre>
     * @param {number} 指定音效文件的 ID。每个音效文件均对应唯一的 ID。
     * @param {number} pitch 当前音效文件的音调。默认值为 0，即不调整音调，取值范围为 -12 ~ 12。
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    setEffectPitch(effectId: number, pitch: number): number {
        return this.nertcEngine.setEffectPitch(effectId, pitch);
    }

    /**
     * 获取指定音效文件的音调。
     * @since v5.4.0
     * <pre>
     * 请先调用playEffect方法开启伴音。
     * </pre>
     * @returns {number} 当前音效文件音调
     */
    getEffectPitch(effectId: number): number {
        return this.nertcEngine.getEffectPitch(effectId);
    }

    /**
     * 停止播放所有音效文件。
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    stopAllEffects(): number {
        return this.nertcEngine.stopAllEffects();
    }

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
    pauseEffect(effectId: number): number {
        return this.nertcEngine.pauseEffect(effectId);
    }

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
    resumeEffect(effectId: number): number {
        return this.nertcEngine.resumeEffect(effectId);
    }

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
    pauseAllEffects(): number {
        return this.nertcEngine.pauseAllEffects();
    }

    /**
     * 设置指定音效文件的播放位置。
     * @since v5.4.0
     * <pre>
     * 通过此接口可以实现根据实际情况播放音效文件，而非从头到尾播放整个文件。
     * <b>NOTE:</b>
     * - 请在引擎初始化之后调用此接口，且该方法仅可在加入房间后调用。
     * </pre>
     * @param {number} effectId 指定音效文件的 ID。每个音效文件均对应唯一的 ID。
     * @param {number} timestamp_ms 指定音效文件的起始播放位置。单位为毫秒。
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    setEffectPosition(effectId: number, pos: number): number {
        return this.nertcEngine.setEffectPosition(effectId, pos);
    }

    /**
     * 获取指定音效文件的播放进度。
     * @since v5.4.0
     * <pre>
     * 请先调用playEffect方法开启伴音。
     * @returns {number} 当前音效文件音调
     */
    getEffectCurrentPosition(effectId: number): number {
        return this.nertcEngine.getEffectCurrentPosition(effectId);
    }

    /**
     * 获取指定音效文件的时长。
     * @since v5.4.0
     * @returns {number} 当前音效文件的时长.
     */
    getEffectDuration(effectId: number): number {
        return this.nertcEngine.getEffectDuration(effectId);
    }

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
    resumeAllEffects(): number {
        return this.nertcEngine.resumeAllEffects();
    }

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
    setEffectSendVolume(effectId: number, volume: number): number {
        return this.nertcEngine.setEffectSendVolume(effectId, volume);
    }

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
    getEffectSendVolume(effectId: number): number {
        return this.nertcEngine.getEffectSendVolume(effectId);
    }

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
    setEffectPlaybackVolume(effectId: number, volume: number): number {
        return this.nertcEngine.setEffectPlaybackVolume(effectId, volume);
    }

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
    getEffectPlaybackVolume(effectId: number): number {
        return this.nertcEngine.getEffectPlaybackVolume(effectId);
    }

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
    enableEarback(enabled: boolean, volume: number): number {
        return this.nertcEngine.enableEarback(enabled, volume);
    }

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
    setEarbackVolume(volume: number): number {
        return this.nertcEngine.setEarbackVolume(volume);
    }

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
    enableAudioVolumeIndication(enabled: boolean, interval: number): number {
        return this.nertcEngine.enableAudioVolumeIndication(enabled, interval);
    }

    /**
     * 启用说话者音量提示。该方法允许 SDK 定期向 App 反馈当前谁在说话以及说话者的音量。
     * @since V4.5.d
     * <pre>
     * 启用该方法后，无论频道内是否有人说话，可以通过{@link NERtcEngine#on}方法监听 onRemoteAudioVolumeIndication，根据设置的间隔时间返回音量提示事件。
     * </pre>
     * @param {boolean} enable 是否启用说话者音量提示。
     * <pre>
     * - true 启用说话者音量提示
     * - false 关闭说话者音量提示
     * </pre>
     * @param {number} interval 指定音量提示的时间间隔，单位为毫秒。必须设置为 100 毫秒的整数倍值。
     * @param {boolean} intervaenableVadl 是否启用本地采集人声监测:
     * <pre>
     * - true 启用本地采集人声监测
     * - false 关闭本地采集人声监测
     * </pre>
     * @returns {number}
     * <pre>
     * - d: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    enableAudioVolumeIndicationEx(enabled: boolean, interval: number, enableVad: boolean): number {
        return this.nertcEngine.enableAudioVolumeIndicationEx(enabled, interval, enableVad);
    }

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
    startScreenCaptureByScreenRect(screenRect: NERtcRectangle, regionRect: NERtcRectangle, param: NERtcScreenCaptureParameters): number {
        return this.nertcEngine.startScreenCaptureByScreenRect(screenRect, regionRect, param);
    }

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
    startScreenCaptureByDisplayId(displayId: number, regionRect: NERtcRectangle, param: NERtcScreenCaptureParameters): number {
        return this.nertcEngine.startScreenCaptureByDisplayId(displayId, regionRect, param);
    }

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
    startScreenCaptureByWindowId(windowid: number, regionRect: NERtcRectangle, param: NERtcScreenCaptureParameters): number {
        return this.nertcEngine.startScreenCaptureByWindowId(windowid, regionRect, param);
    }

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
    updateScreenCaptureRegion(regionRect: NERtcRectangle): number {
        return this.nertcEngine.updateScreenCaptureRegion(regionRect);
    }

    /**
     * 在共享屏幕或窗口时，更新是否显示鼠标。
     * @since V5.4.0
     * @param {boolean} capture_cursor  屏幕共享时是否捕捉鼠标光标。
     * <pre>
     * - true 共享屏幕时显示鼠标
     * - false 共享屏幕时不显示鼠标
     * </pre>
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    setScreenCaptureMouseCursor(capture_cursor: boolean): number {
        return this.nertcEngine.setScreenCaptureMouseCursor(capture_cursor);
    }

    /**
     * 停止屏幕共享。
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    stopScreenCapture(): number {
        return this.nertcEngine.stopScreenCapture();
    }

    /**
     * 暂停屏幕共享。
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    pauseScreenCapture(): number {
        return this.nertcEngine.pauseScreenCapture();
    }

    /**
     * 恢复屏幕共享。
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    resumeScreenCapture(): number {
        return this.nertcEngine.resumeScreenCapture();
    }


    // /**
    //  * 关闭声音共享。
    //  * @deprecated 该接口已经在 4.1.x 版本后废弃
    //  * <pre>
    //  * <b>NOTE:</b>
    //  * - 该方法仅适用于 Windows。
    //  * - 请在频道内调用该方法，通话结束后自动关闭。
    //  * </pre>
    //  * @return {number}
    //  * <pre>
    //  * - 0: 方法调用成功；
    //  * - 其他: 方法调用失败。
    //  * </pre>
    //  */
    // stopSystemAudioLoopbackCapture(): number {
    //     return this.nertcEngine.stopSystemAudioLoopbackCapture();
    // }

    /**
     * 发送媒体补充增强信息（SEI）。
     * @since 4.1.110
     * <pre>
     * 在本端推流传输视频流数据同时，发送流媒体补充增强信息来同步一些其他附加信息。当推流方发送 SEI 后，拉流方可通过监听 onRecvSEIMsg 的回调获取 SEI 内容。
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
    sendSEIMsg(data: ArrayBuffer): number {
        return this.nertcEngine.sendSEIMsg(data);
    }

    /**
     * 发送媒体补充增强信息（SEI）。
     * @since 4.1.110
     * <pre>
     * 在本端推流传输视频流数据同时，发送流媒体补充增强信息来同步一些其他附加信息。当推流方发送 SEI 后，拉流方可通过监听 onRecvSEIMsg 的回调获取 SEI 内容。
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
    sendSEIMsgWithType(data: ArrayBuffer, type: NERtcStreamChannelType) {
        return this.nertcEngine.sendSEIMsgWithType(data, type);
    }

    // /**
    //  * 拉取外部音频数据。
    //  * @since 4.1.110
    //  * <pre>
    //  * - 该方法将从内部引擎拉取音频数据。 通过 setExternalAudioRender 启用外部音频数据渲染功能成功后，可以使用 pullExternalAudioFrame 接口获取音频 PCM 数据。
    //  * <b>NOTE:</b>
    //  * - 该方法需要在加入房间后调用。
    //  * - 数据帧时长建议匹配 10ms 周期。
    //  * - 该方法在音频渲染设备关闭后不再生效，此时会返回空数据。例如通话结束、通话前扬声器设备测试关闭等情况下，该设置不再生效。
    //  * </pre>
    //  * @param {boolean} enable 是否外部数据输出
    //  * <pre>
    //  * - true: 开启外部数据渲染
    //  * - false: 关闭外部数据渲染 (默认)
    //  * </pre>
    //  * @param {number} sampleRate 数据采样率，后续数据按该格式返回。注意：调用接口关闭功能时可传入任意合法值，此时设置不会生效
    //  * @param {number} channels channels 数据声道数，后续数据按该格式返回。注意：调用接口关闭功能时可传入任意合法值，此时设置不会生效。
    //  * @return {number}
    //  * <pre>
    //  * - 0: 方法调用成功；
    //  * - 其他: 方法调用失败。
    //  * </pre>
    //  */
    // setExternalAudioRender(enable: boolean, sampleRate: number, channels: number): number {
    //     return this.nertcEngine.setExternalAudioRender(enable, sampleRate, channels);
    // }

    // /**
    //  * 拉取外部音频数据。
    //  * @since 4.1.110
    //  * <pre>
    //  * - 该方法将从内部引擎拉取音频数据。 通过 setExternalAudioRender 启用外部音频数据渲染功能成功后，可以使用 pullExternalAudioFrame 接口获取音频 PCM 数据。
    //  * <b>NOTE:</b>
    //  * - 该方法需要在加入房间后调用。
    //  * - 数据帧时长建议匹配 10ms 周期。
    //  * - 该方法在音频渲染设备关闭后不再生效，此时会返回空数据。例如通话结束、通话前扬声器设备测试关闭等情况下，该设置不再生效。
    //  * </pre>
    //  * @param {number} pullLength 待拉取音频数据的字节数，单位为 byte
    //  * @param {function} cb 拉取数据的回调函数
    //  * @returns {number}
    //  * <pre>
    //  * - 0: 方法调用成功；
    //  * - 其他: 方法调用失败。
    //  * </pre>
    //  */
    // pullExternalAudioFrame(pullLength: number, cb: NERtcPullExternalAudioFrameCb): number {
    //     return this.nertcEngine.pullExternalAudioFrame(pullLength, cb);
    // }

    /**
     * 查询 SDK 版本号。
     * @returns {String} 当前的 SDK 版本号，格式为字符串，如1.0.0.
     */
    getVersion(): String {
        return this.nertcEngine.getVersion();
    }

    /**
     * 获取错误描述。
     * @param {number} errorCode 错误码
     * @returns {number} 详细错误码描述
     */
    getErrorDescription(errorCode: number): String {
        return this.nertcEngine.getErrorDescription(errorCode);
    }

    /**
     * 上传SDK 信息（如log文件和Audio dump文件）。
     */
    uploadSdkInfo(): void {
        return this.nertcEngine.uploadSdkInfo();
    }

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
    addLiveStreamTask(info: NERtcLiveStreamTaskInfo): number {
        return this.nertcEngine.addLiveStreamTask(info);
    }

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
     * @param {object} info.layout.users[].bg_image 背景图信息
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
    updateLiveStreamTask(info: NERtcLiveStreamTaskInfo): number {
        return this.nertcEngine.updateLiveStreamTask(info);
    }

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
    removeLiveStreamTask(taskId: String): number {
        return this.nertcEngine.removeLiveStreamTask(taskId);
    }

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
    enumerateRecordDevices(): Array<NERtcDevice> {
        return this.nertcEngine.enumerateRecordDevices();
    }

    /**
     * 指定音频采集设备。
     * @param {String} id 音频采集设备的设备 ID。可通过 {@link NERtcEngine#enumerateRecordDevices} 获取。插拔设备不会影响 deviceId。
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    setRecordDevice(id: String): number {
        return this.nertcEngine.setRecordDevice(id);
    }

    /**
     * 获取当前使用的音频采集设备 ID。
     * @returns {String}
     * <pre>
     * - 设备ID： 调用成功
     * - null: 调用失败。
     * </pre>
     */
    getRecordDevice(): String {
        return this.nertcEngine.getRecordDevice();
    }

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
    enumeratePlayoutDevices(): Array<NERtcDevice> {
        return this.nertcEngine.enumeratePlayoutDevices();
    }

    /**
     * 指定播放设备。
     * @param {String} id 音频播放设备的设备 ID。可以通过 {@link NERtcEngine#enumeratePlayoutDevices} 获取。插拔设备不会影响 deviceId。
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    setPlayoutDevice(id: String): number {
        return this.nertcEngine.setPlayoutDevice(id);
    }

    /**
     * 获取当前使用的音频播放设备 ID。
     * @returns {String} deviceID
     */
    getPlayoutDevice(): String {
        return this.nertcEngine.getPlayoutDevice();
    }

    /**
     * 设置音频采集设备音量。
     * @param {number} volume 音频采集设备音量。取值范围为 0~255。
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    setRecordDeviceVolume(volume: number): number {
        return this.nertcEngine.setRecordDeviceVolume(volume);
    }

    /**
     * 获取音频采集设备音量。
     * @returns {number}
     * <pre>
     * - >=0: 音频采集设备音量。取值范围为 0~255。
     * - 其他: 方法调用失败。
     * </pre>
     */
    getRecordDeviceVolume(): number {
        return this.nertcEngine.getRecordDeviceVolume();
    }

    /**
     * 设置音频播放设备音量。
     * @param {number} volume 音频播放设备音量。取值范围为 0~255。
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    setPlayoutDeviceVolume(volume: number): number {
        return this.nertcEngine.setPlayoutDeviceVolume(volume);
    }

    /**
     * 获取音频播放设备音量。
     * @returns {number}
     * <pre>
     * - >=0: 音频播放设备音量。取值范围为 0~255。
     * - 其他: 方法调用失败。
     * </pre>
     */
    getPlayoutDeviceVolume(): number {
        return this.nertcEngine.getPlayoutDeviceVolume();
    }

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
    setPlayoutDeviceMute(mute: boolean): number {
        return this.nertcEngine.setPlayoutDeviceMute(mute);
    }

    /**
     * 获取音频播放设备静音状态。
     * @returns {boolean}
     * <pre>
     * - true: 静音状态。
     * - false: 非静音状态。
     * </pre>
     */
    getPlayoutDeviceMute(): boolean {
        return this.nertcEngine.getPlayoutDeviceMute();
    }

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
    setRecordDeviceMute(mute: boolean): number {
        return this.nertcEngine.setRecordDeviceMute(mute);
    }

    /**
     * 获取音频采集设备静音状态。
     * @returns {boolean}
     * <pre>
     * - true: 静音状态。
     * - false: 非静音状态。
     * </pre>
     */
    getRecordDeviceMute(): boolean {
        return this.nertcEngine.getRecordDeviceMute();
    }

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
    adjustRecordingSignalVolume(volume: number): number {
        return this.nertcEngine.adjustRecordingSignalVolume(volume);
    }

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
    adjustPlaybackSignalVolume(volume: number): number {
        return this.nertcEngine.adjustPlaybackSignalVolume(volume);
    }

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
    startRecordDeviceTest(interval: number): number {
        return this.nertcEngine.startRecordDeviceTest(interval);
    }

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
    stopRecordDeviceTest(): number {
        return this.nertcEngine.stopRecordDeviceTest();
    }

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
    startPlayoutDeviceTest(path: String): number {
        return this.nertcEngine.startPlayoutDeviceTest(path);
    }

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
    stopPlayoutDeviceTest(): number {
        return this.nertcEngine.stopPlayoutDeviceTest();
    }

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
    startAudioDeviceLoopbackTest(interval: number): number {
        return this.nertcEngine.startAudioDeviceLoopbackTest(interval);
    }

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
    stopAudioDeviceLoopbackTest(): number {
        return this.nertcEngine.stopAudioDeviceLoopbackTest();
    }

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
    enumerateVideoCaptureDevices(): Array<NERtcDevice> {
        return this.nertcEngine.enumerateCaptureDevices();
    }


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
    enumerateScreenCaptureSourceInfo(thumbWidth: number, thumbHeight: number, iconWidth: number, iconHeight: number): Array<Object> {
        return this.nertcEngine.enumerateScreenCaptureSourceInfo(thumbWidth, thumbHeight, iconWidth, iconHeight);
    }

    // /**
    //  * 枚举屏幕分享源信息。
    //  * @since 5.5.20
    //  * @param {Object} thumbSize 屏幕或窗口的缩略图的目标尺寸（宽高单位为像素）:
    //  * @param {number} thumbSize.width 缩略图缩略图宽度px默认0
    //  * @param {number} thumbSize.height 缩略图高度px默认0
    //  * <pre>
    //  * <b>NOTE:</b>
    //  * - SDK 会在保证原图不变形的前提下，缩放原图，使图片最长边和目标尺寸的最长边的长度一致。
    //  * - 如果目标尺寸大于原图尺寸，缩略图即为原图，SDK 不进行缩放操作。
    //  * </pre>
    //  * @param {Object} iconSize 程序所对应的图标的目标尺寸px。
    //  * @param {number} iconSize.width 程序所对应的图标宽度px默认0
    //  * @param {number} iconSize.height 程序所对应的图标高度px默认0
    //  * <b>NOTE:</b>
    //  * - SDK 会在保证原图不变形的前提下，缩放原图，使图片最长边和目标尺寸的最长边的长度一致。
    //  * - 如果目标尺寸大于原图尺寸，缩略图即为原图，SDK 不进行缩放操作。
    //  * </pre>
    //  * @param includeScreen 除了窗口信息外，SDK 是否还返回屏幕信息：
    //  * <pre>
    //  * - true: SDK 返回屏幕和窗口信息；
    //  * - false: SDK 仅返回窗口信息。
    //  * </pre>
    //  * @returns {Object[]}
    //  * <pre>
    //  * - Object[] : 调用成功；
    //  * <table style="width:100%;">
    //  * <tr><td>Object.type</td><td>int</td><td>信息源类型:-1-未知 0-窗口 1-屏幕 2-自定义</td></tr>
    //  * <tr><td>Object.source_id</td><td>number</td><td>信息源ID</td></tr>
    //  * <tr><td>Object.source_name</td><td>String</td><td>信息源名称</td></tr>
    //  * <tr><td>Object.thumb_image</td><td>object</td><td>缩略图信息,使用前需要判断是否undefined:
    //  * - buffer - BGRA二进制数据
    //  * - length - 数据大小 byte
    //  * - width - 图片宽度 px
    //  * - height - 图片高度 px</td></tr>
    //  * <tr><td>Object.icon_image</td><td>object</td><td>图标信息,使用前需要判断是否undefined:
    //  * - buffer - BGRA二进制数据
    //  * - length - 数据大小 byte
    //  * - width - 图片宽度 px
    //  * - height - 图片高度 px</td></tr>
    //  * </table>
    //  * - NULL: 调用失败。
    //  * </pre>
    //  */
    // getScreenCaptureSources(thumbSize: NERtcSize, iconSize: NERtcSize, includeScreen: boolean): Array<Object> {
    //     return this.nertcEngine.getScreenCaptureSources(thumbSize, iconSize, includeScreen);
    // }

    // /**
    //  * 设置屏幕分享参数，该方法在屏幕分享过程中调用，用来快速切换采集源。
    //  * @since 5.5.20
    //  * @param {CaptureSoureInfo} source 屏幕或窗口的缩略图的目标尺寸（宽高单位为像素）:
    //  * @param {number} source.type 信息源类型:-1-未知 0-窗口 1-屏幕 2-自定义
    //  * @param {number} source.source_id 信息源ID
    //  * @param {object} [regionRect=] (可选) 指定待共享区域相对于整个屏幕屏幕的位置。如果设置的共享区域超出了屏幕的边界，则只共享屏幕内的内容；如果将 width 或 height 设为 0, 则共享整个屏幕。
    //  * @param {number} regionRect.x 左上角的横向偏移
    //  * @param {number} regionRect.y 左上角的纵向偏移
    //  * @param {number} regionRect.width 待共享区域的宽
    //  * @param {number} regionRect.height 待共享区域的高
    //  * @param {object} param 屏幕共享的编码参数配置。
    //  * @param {object} [param.profile=2] 屏幕共享编码参数配置:
    //  * <pre>
    //  * - 0 640x480, 5fps
    //  * - 1 1280x720, 5fps
    //  * - 2 1920x1080, 5fps。默认
    //  * - 3 自定义
    //  * </pre>
    //  * @param {object} param.dimensions 屏幕共享视频发送的最大像素值，param.profile=3时生效:
    //  * @param {number} param.dimensions.width  宽度
    //  * @param {number} param.dimensions.height  高度
    //  * @param {number} [param.frame_rate=5] 共享视频的帧率，param.profile=3时生效，单位为 fps；默认值为 5，建议不要超过 15
    //  * @param {number} [param.bitrate=0] 共享视频的码率，单位为 bps；默认值为 0，表示 SDK 根据当前共享屏幕的分辨率计算出一个合理的值
    //  * @param {boolean} param.capture_mouse_cursor 是否采集鼠标用于屏幕共享
    //  * @param {boolean} param.window_focus 调用 {@link NERtcEngine#startScreenCaptureByWindowId} 方法共享窗口时，是否将该窗口前置
    //  * @param {number[]} param.excluded_window_list 待屏蔽窗口的 ID 列表
    //  * @param {number} param.excluded_window_count 待屏蔽窗口的数量
    //  * @param {number} param.prefer 编码策略倾向:
    //  * <pre>
    //  * - 0 动画模式
    //  * - 1 细节模式
    //  * </pre>
    //  */
    // setScreenCaptureSource(source: CaptureSoureInfo, regionRect: NERtcRectangle, capture_params: NERtcScreenCaptureParameters): number {
    //     return this.nertcEngine.setScreenCaptureSource(source, regionRect, capture_params);
    // }

    /**
     * 指定视频采集设备。
     * @param {string} id 视频采集设备的设备 ID。可以通过 {@link NERtcEngine#enumerateCaptureDevices}获取。
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    setVideoDevice(id: String): number {
        return this.nertcEngine.setDevice(id);
    }

     /**
     * 指定视频采集设备。
     * @param {string} id 视频采集设备的设备 ID。可以通过 {@link NERtcEngine#enumerateCaptureDevices}获取。
     * @param {number} streamType 视频流类型。
     * <pre>
     * - 0：视频流主流。
     * - 1：视频流辅流。
     * </pre>
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
     setVideoDeviceWithType(id: string, streamType: NERtcVideoStreamType): number {
        return this.nertcEngine.setDeviceWithType(id, streamType);
    }

    /**
     * 获取当前使用的视频采集设备信息。
     * @returns {String} 设备ID
     */
    getVideoDevice(): String {
        return this.nertcEngine.getDevice();
    }

    /**
     * 获取当前使用的视频采集设备信息。
     * @param {number} streamType 视频流类型。
     * <pre>
     * - 0：视频流主流。
     * - 1：视频流辅流。
     * </pre>
     * @returns {String} 设备ID
     */
    getVideoDeviceWithType(streamType: NERtcVideoStreamType): string {
        return this.nertcEngine.getDeviceWithType(streamType);
    }

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
    setAudioEffectPreset(type: NERtcVoiceChangerType): number {
        return this.nertcEngine.setAudioEffectPreset(type)
    }

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
    setVoiceBeautifierPreset(type: NERtcVoiceBeautifierType): number {
        return this.nertcEngine.setVoiceBeautifierPreset(type)
    }

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
    setLocalVoicePitch(pitch: number): number {
        return this.nertcEngine.setLocalVoicePitch(pitch)
    }

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
    setLocalVoiceEqualization(bandFrequency: NERtcVoiceEqualizationBand, bandGain: number): number {
        return this.nertcEngine.setLocalVoiceEqualization(bandFrequency, bandGain)
    }
    
    /**
     * 开启或关闭声卡采集。
     * @since 4.1.110
     * <pre>
     * - 启用声卡采集功能后，声卡播放的声音会被合到本地音频流中，从而可以发送到远端。
     * <b>NOTE:</b>
     * - 该方法仅适用于 macOS 和 Windows 平台。
     * - macOS 系统默认声卡不支持采集功能，如需开启此功能需要 App 自己启用一个虚拟声卡，并将该虚拟声卡的名字作为 deviceName 传入 SDK。
     * - 该方法在加入频道前后都能调用。
     * </pre>
     * @param  {boolean} enable 是否开启声卡采集功能。
     * <pre>
     * - true: 开启声卡采集。
     * - false: 关闭声卡采集（默认）。
     * </pre>
     * @param  {String} deviceName 声卡的设备名。默认设为 NULL，即使用当前声卡采集。
     * <pre>
     * - 如果用户使用虚拟声卡，如 “Soundflower”，可以将虚拟声卡名称 “Soundflower” 作为参数，SDK 会找到对应的虚拟声卡设备，并开始采集。
     * <pre>
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功。
     * - 其他: 方法调用失败。
     * </pre>
     */
    enableLoopbackRecording(enable: boolean, deviceName: String ): number { //deviceName: String = 'NeCastAudio'
        // if (deviceName === '' && process.platform === 'darwin') {
        //     const playoutDevices = this.nertcEngine.enumeratePlayoutDevices()
        //     let foundDevice = false
        //     for (let i = 0; i < playoutDevices.length; i++) {
        //         if (playoutDevices[i].device_name === 'NeCastAudio A') {
        //             foundDevice = true
        //             break
        //         }
        //     }
        //     if (foundDevice) {
        //         deviceName = 'NeCastAudio'
        //     } else {
        //         return -1
        //     }
        // }
        return this.nertcEngine.enableLoopbackRecording(enable, deviceName)
    }

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
    adjustLoopbackRecordingSignalVolume(volume: number): number {
        return this.nertcEngine.adjustLoopbackRecordingSignalVolume(volume)
    }

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
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功。
     * - 其他: 方法调用失败。
     * </pre>
     */
    adjustUserPlaybackSignalVolume(uid: number, volume: number): number {
        return this.nertcEngine.adjustUserPlaybackSignalVolume(uid, volume)
    }

    /**
     * 调节本地播放的指定房间的所有远端用户的信号音量。
     * @since V5.4.0
     * <pre>
     * -通过此接口可以实现在通话过程中随时调节指定房间内的所有远端用户在本地播放的混音音量。
     * <b>NOTE:</b>
     * - 请在引擎初始化之后调用此接口，该方法在加入房间前后都可调用。
     * - 该方法设置内部引擎为启用状态，在 leaveChannel 后失效，但在本次通话过程中有效。
     * </pre>
     * @param  {number} volume 播放音量，取值范围为 [0,400]。
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功。
     * - 其他: 方法调用失败。
     * </pre>
     */
    adjustChannelPlaybackSignalVolume(volume: number): number {
        return this.nertcEngine.adjustChannelPlaybackSignalVolume(volume);
    }

     /**
    * 快速切换音视频房间。
    * @since 4.4.8
    * <pre>
    * - 房间场景为直播场景时，房间中角色为观众的成员可以调用该方法从当前房间快速切换至另一个房间。
    * - 成功调用该方切换房间后，本端用户会先收到离开房间的回调 onLeaveChannel，再收到成功加入新房间的回调 onJoinChannel。远端用户会收到 onUserLeave 和 onUserJoined 回调。
    * <b>NOTE:</b>
    * - 快速切换房间功能默认关闭。如需使用，请联系技术支持免费开通。
    * - 该方法仅适用于直播场景中，角色为观众的音视频房间成员。即已通过接口 setchannelprofile 设置房间场景为直播，通过 setClientRole 设置房间成员的角色为观众。
    * - 房间成员成功切换房间后，默认订阅房间内所有其他成员的音频流，因此产生用量并影响计费。如果想取消订阅，可以通过调用相应的 subscribeRemoteAudio 方法传入 false 实现。
    * </pre>
    * @param {number}  token 安全认证签名（NERTC Token）。
    * <pre>
    * - null。非安全模式下可设置为 null。安全性不高，建议在产品正式上线前联系对应商务经理转为安全模式。
    * - 已获取的 NERTC Token。安全模式下必须设置为获取到的 Token 。若未传入正确的 Token，用户将无法进入房间。推荐使用安全模式。
    * </pre>
    * @param {string} channel_name 期望切换到的目标房间名称。
    * @return {number}
    * <pre>
    * - 0: 方法调用成功。
    * - 其他：方法调用失败。
    * </pre>
    */
    switchChannel(token: String, channelName: String): number {
        return this.nertcEngine.switchChannel(token, channelName);
    }

   /**
    * 快速切换音视频房间。
    * @since V5.4.0
    * <pre>
    * - 房间场景为直播场景时，房间中角色为观众的成员可以调用该方法从当前房间快速切换至另一个房间。
    * - 成功调用该方切换房间后，本端用户会先收到离开房间的回调 onLeaveChannel，再收到成功加入新房间的回调 onJoinChannel。远端用户会收到 onUserLeave 和 onUserJoined 回调。
    * <b>NOTE:</b>
    * - 快速切换房间功能默认关闭。如需使用，请联系技术支持免费开通。
    * - 该方法仅适用于直播场景中，角色为观众的音视频房间成员。即已通过接口 setchannelprofile 设置房间场景为直播，通过 setClientRole 设置房间成员的角色为观众。
    * - 房间成员成功切换房间后，默认订阅房间内所有其他成员的音频流，因此产生用量并影响计费。如果想取消订阅，可以通过调用相应的 subscribeRemoteAudio 方法传入 false 实现。
    * </pre>
    * @param {number}  token 安全认证签名（NERTC Token）。
    * <pre>
    * - null。非安全模式下可设置为 null。安全性不高，建议在产品正式上线前联系对应商务经理转为安全模式。
    * - 已获取的 NERTC Token。安全模式下必须设置为获取到的 Token 。若未传入正确的 Token，用户将无法进入房间。推荐使用安全模式。
    * </pre>
    * @param {string} channelName 期望切换到的目标房间名称。
    * @param {Object} channelOptions 加入音视频房间时的一些可选信息。
    * @param {string} channelOptions.custom_info 自定义信息，最长支持 127 个字符。
    * @param {string} channelOptions.permission_key 权限密钥。能控制通话时长及媒体权限能力。
    * @return {number}
    * <pre>
    * - 0: 方法调用成功。
    * - 其他：方法调用失败。
    * </pre>
    */
    switchChannelWithOptions(token: string, channelName: string, channelOptions: NERtcJoinChannelOptions): number {
        return this.nertcEngine.switchChannelWithOptions(token, channelName, channelOptions);
    }

    /**
    * 快速切换音视频房间。
    * @since V5.4.0
    * <pre>
    * - 房间场景为直播场景时，房间中角色为观众的成员可以调用该方法从当前房间快速切换至另一个房间。
    * - 成功调用该方切换房间后，本端用户会先收到离开房间的回调 onLeaveChannel，再收到成功加入新房间的回调 onJoinChannel。远端用户会收到 onUserLeave 和 onUserJoined 回调。
    * <b>NOTE:</b>
    * - 快速切换房间功能默认关闭。如需使用，请联系技术支持免费开通。
    * - 该方法仅适用于直播场景中，角色为观众的音视频房间成员。即已通过接口 setchannelprofile 设置房间场景为直播，通过 setClientRole 设置房间成员的角色为观众。
    * - 房间成员成功切换房间后，默认订阅房间内所有其他成员的音频流，因此产生用量并影响计费。如果想取消订阅，可以通过调用相应的 subscribeRemoteAudio 方法传入 false 实现。
    * </pre>
    * @param {number}  token 安全认证签名（NERTC Token）。
    * <pre>
    * - null。非安全模式下可设置为 null。安全性不高，建议在产品正式上线前联系对应商务经理转为安全模式。
    * - 已获取的 NERTC Token。安全模式下必须设置为获取到的 Token 。若未传入正确的 Token，用户将无法进入房间。推荐使用安全模式。
    * </pre>
    * @param {string} channelName 期望切换到的目标房间名称。
    * @param {Object} channelOptions 加入音视频房间时的一些可选信息。
    * @param {string} channelOptions.custom_info 自定义信息，最长支持 127 个字符。
    * @param {string} channelOptions.permission_key 权限密钥。能控制通话时长及媒体权限能力。
    * @return {number}
    * <pre>
    * - 0: 方法调用成功。
    * - 其他：方法调用失败。
    * </pre>
    */
    switchChannelWithOptionsEx(token: string, channelName: string, channelOptions: NERtcJoinChannelOptionsEx): number {
        return this.nertcEngine.switchChannelWithOptionsEx(token, channelName, channelOptions);
    }

    /**
    * 设置本地用户的媒体流优先级。
    * @since 4.4.8
    * <pre>
    * - 如果本地用户的优先级为高，则该用户媒体流的优先级就会高于其他用户，那么弱网环境下 SDK 会优先保证其他用户收到的本地用户媒体流的质量。
    * <b>NOTE:</b>
    * - 请在加入房间（joinChannel）前调用此方法。
    * - 快速切换房间 （switchChannel） 后，媒体优先级会恢复为默认值，即普通优先级。
    * - 一个音视频房间中只有一个高优先级的用户。建议房间中只有一位用户调用 setLocalMediaPriority 将本端媒体流设为高优先级，否则需要开启抢占模式，才能保证本地用户的高优先级设置生效。
    * </pre>
    * @param {number} priority 本地用户的媒体流优先级。
    * <pre>
    * - 50 高优先级。
    * - 100 普通，默认优先级。
    * </pre>
    * @param {boolean} preemptive 是否开启抢占模式。默认为 false，即不开启。
    * <pre>
    * - 抢占模式开启后，本地用户可以抢占其他用户的高优先级，被抢占的用户的媒体流优先级变为普通优先级。在抢占者退出房间后，其他用户的优先级仍旧维持普通优先级。
    * - 抢占模式关闭后，如果房间中已有高优先级用户，则本地用户的高优先级设置不生效，仍旧为普通优先级。
    * </pre
    * @return {number}
    * <pre>
    * - 0: 方法调用成功。
    * - 其他：方法调用失败。
    * </pre>
    */
    setLocalMediaPriority(priority: NERtcMediaPriorityType, preemptive: boolean): number{
        return this.nertcEngine.setLocalMediaPriority(priority, preemptive);
    }

    /**
    * 设置屏幕捕捉时需屏蔽的窗口列表, 该方法在捕捉过程中可动态调用。
    * @since 4.4.8
    * <pre>
    *  - 仅支持Windows
    * </pre>
    * @param {list} window_list 需屏蔽的窗口ID列表, 例如：[id1,id2...]。
    * @return {number}
    * <pre>
    * - 0: 方法调用成功。
    * - 其他：方法调用失败。
    * </pre>
    */
    setExcludeWindowList(param: NERtcScreenCaptureWindowParam): number{
        return this.nertcEngine.setExcludeWindowList(param);
    }

    /**
    * 更新屏幕共享参数。
    * @since V5.4.0
    * <pre>
    *  - 仅支持Windows
    * </pre>
    * @param {list} window_list 需屏蔽的窗口ID列表, 例如：[id1,id2...]。
    * @return {number}
    * <pre>
    * - 0: 方法调用成功。
    * - 其他：方法调用失败。
    * </pre>
    */
    updateScreenCaptureParameters(param: NERtcScreenCaptureWindowParam): number {
        return this.nertcEngine.updateScreenCaptureParameters(param);
    }

    /**
    * 开始客户端录音。
    * @since 4.4.8
    * <pre>
    * - 调用该方法后，客户端会录制房间内所有用户混音后的音频流，并将其保存在本地一个录音文件中。录制开始或结束时，自动触发 onAudioRecording() 回调。
    * - 指定的录音音质不同，录音文件会保存为不同格式：
    * - WAV：音质保真度高，文件大。
    * - AAC：音质保真度低，文件小。
    * <b>NOTE:</b>
    * - 请在加入房间后调用此方法。
    * - 客户端只能同时运行一个录音任务，正在录音时，如果重复调用 startAudioRecording，会结束当前录制任务，并重新开始新的录音任务。
    * - 当前用户离开房间时，自动停止录音。您也可以在通话中随时调用 stopAudioRecording 手动停止录音。
    * </pre>
    * @param {String} filePath 录音文件在本地保存的绝对路径，需要精确到文件名及格式。例如：sdcard/xxx/audio.aac。
    * <pre>
    * - 请确保指定的路径存在并且可写。
    * - 目前仅支持 WAV 或 AAC 文件格式。
    * </pre>
    * @param {number} sampleRate 录音采样率（Hz），可以设为 16000、32000（默认）、44100 或 48000。
    * @param {number} quality 录音音质，只在 AAC 格式下有效。详细说明请参考 NERtcAudioRecordingQuality。
    * @return {number}
    * <pre>
    * - 0: 方法调用成功。
    * - 其他：方法调用失败。
    * </pre>
    */
    startAudioRecording(filePath: String, sampleRate: number, quality: NERtcAudioRecordingQuality): number{
        return this.nertcEngine.startAudioRecording(filePath, sampleRate, quality);
    }

    /**
    * 开始客户端录音。
    * @since V5.4.0
    * <pre>
    * - 调用该方法后，客户端会录制房间内所有用户混音后的音频流，并将其保存在本地一个录音文件中。录制开始或结束时，自动触发 onAudioRecording() 回调。
    * - 指定的录音音质不同，录音文件会保存为不同格式：
    * - WAV：音质保真度高，文件大。
    * - AAC：音质保真度低，文件小。
    * <b>NOTE:</b>
    * - 请在加入房间后调用此方法。
    * - 客户端只能同时运行一个录音任务，正在录音时，如果重复调用 startAudioRecording，会结束当前录制任务，并重新开始新的录音任务。
    * - 当前用户离开房间时，自动停止录音。您也可以在通话中随时调用 stopAudioRecording 手动停止录音。
    * </pre>
    * @param {object} config 录音配置。
    * @param {String} config.filePath 录音文件在本地保存的绝对路径，需要精确到文件名及格式。例如：sdcard/xxx/audio.aac。
    * <pre>
    * - 请确保指定的路径存在并且可写。
    * - 目前仅支持 WAV 或 AAC 文件格式。
    * </pre>
    * @param {number} config.sampleRate 录音采样率（Hz），可以设为 16000、32000（默认）、44100 或 48000。
    * @param {number} config.quality 录音音质，只在 AAC 格式下有效:
    * <pre>
    * - 0 低音质
    * - 1 中音质
    * - 2 高音质
    * </pre>
    * @param {number} config.position 录音文件所包含的内容:
    * <pre>
    * - 0 录制本地和所有远端用户混音后的音频（默认）
    * - 1 仅录制本地用户的音频
    * - 2 仅录制所有远端用户的音频
    * </pre>
    * @param {number} config.cycleTime  录制过程中，循环缓存的最大时间长度，单位(s):
    * <pre>
    * - 0 录制本地和所有远端用户混音后的音频（默认）
    * - 10 音频录制缓存时间为10s，StopAudioRectording()后，将缓存都写到文件，文件数据时间跨度为: [0,10s]
    * - 60 音频录制缓存时间为60s，StopAudioRectording()后，将缓存都写到文件，文件数据时间跨度为: [0,60s]
    * - 360 音频录制缓存时间为360s，StopAudioRectording()后，将缓存都写到文件，文件数据时间跨度为: [0,360s]
    * - 900 音频录制缓存时间为900s，StopAudioRectording()后，将缓存都写到文件，文件数据时间跨度为: [0,900s]
    * </pre>
    * @return {number}
    * <pre>
    * - 0: 方法调用成功。
    * - 其他：方法调用失败。
    * </pre>
    */
    startAudioRecordingWithConfig(config: NERtcAudioRecordingConfiguration): number {
        return this.nertcEngine.startAudioRecordingWithConfig(config);
    }

    /**
    * 停止客户端录音。
    * @since 4.4.8
    * <pre>
    * - 本端离开房间时自动停止录音，您也可以在通话中随时调用 stopAudioRecording 手动停止录音。
    * <b>NOTE:</b>
    * - 该接口需要在 leaveChannel 之前调用。
    * </pre>
    * @return {number}
    * <pre>
    * - 0: 方法调用成功。
    * - 其他：方法调用失败。
    * </pre>
    */
    stopAudioRecording(): number{
        return this.nertcEngine.stopAudioRecording();
    }

    /**
    * 开始跨房间媒体流转发。
    * <pre>
    * - 该方法可用于实现跨房间连麦等场景。支持同时转发到 4 个房间，同一个房间可以有多个转发进来的媒体流。
    * - 成功调用该方法后，SDK 会触发 onMediaRelayStateChanged 和 onMediaRelayEvent 回调，并在回调中报告当前的跨房间媒体流转发状态和事件。
    * <b>NOTE:</b>
    * - 此方法仅适用于用户角色为主播时。
    * - 成功调用该方法后，若您想再次调用该方法，必须先调用 stopChannelMediaRelay 方法退出当前的转发状态。
    * - 成功开始跨房间转发媒体流后，如果您需要修改目标房间，例如添加或删减目标房间等，可以调用方法 updateChannelMediaRelay 更新目标房间信息。
    * </pre>
    * @param {Object} config 跨房间媒体流转发参数配置信息: NERtcChannelMediaRelayConfiguration
    * @param {Object} config.src_infos 源房间信息: NERtcChannelMediaRelayInfo
    * @param {String} config.src_infos.channel_name 源房间名。默认值为 nil，表示 SDK 填充当前的房间名。
    * @param {String} config.src_infos.channel_token 能加入源房间的 Token。
    * @param {String} config.src_infos.uid 标识源房间中的转发媒体流的 UID。
    * @param {Object} config.dest_infos 目标房间信息: NERtcChannelMediaRelayInfo
    * @param {String} config.dest_infos.channel_name 目标房间的房间名。
    * @param {String} config.dest_infos.channel_token 可以加入目标房间的 Token。
    * @param {String} config.dest_infos.uid 标识目标房间中的转发媒体流的 UID。请确保不要将该参数设为目标房间的主播的 UID，并与目标房间中的 所有 UID 都不同。
    * @returns {number}
    * <pre>
    * - 0: 方法调用成功。
    * - 其他: 方法调用失败。
    * </pre>
    */
     startChannelMediaRelay(config: NERtcChannelMediaRelayConfiguration): number{
        return this.nertcEngine.startChannelMediaRelay(config);
    }

    /**
    * 更新媒体流转发的目标房间。
    * <pre>
    * - 成功开始跨房间转发媒体流后，如果您需要修改目标房间，例如添加或删减目标房间等，可以调用此方法。
    * - 成功调用该方法后，SDK 会触发 onMediaRelayStateChanged 和 onMediaRelayEvent 回调，并在回调中报告当前的跨房间媒体流转发状态和事件。
    * <b>NOTE:</b>
    * - 请在加入房间并成功调用 startChannelMediaRelay 开始跨房间媒体流转发后，调用此方法。
    * <pre>
    * @param {Object} config 跨房间媒体流转发参数配置信息: NERtcChannelMediaRelayConfiguration
    * @param {Object} config.src_infos 源房间信息: NERtcChannelMediaRelayInfo
    * @param {String} config.src_infos.channel_name 源房间名。默认值为 nil，表示 SDK 填充当前的房间名。
    * @param {String} config.src_infos.channel_token 能加入源房间的 Token。
    * @param {String} config.src_infos.uid 标识源房间中的转发媒体流的 UID。
    * @param {Object} config.dest_infos 目标房间信息: NERtcChannelMediaRelayInfo
    * @param {String} config.dest_infos.channel_name 目标房间的房间名。
    * @param {String} config.dest_infos.channel_token 可以加入目标房间的 Token。
    * @param {String} config.dest_infos.uid 标识目标房间中的转发媒体流的 UID。请确保不要将该参数设为目标房间的主播的 UID，并与目标房间中的 所有 UID 都不同。
    * @returns {number}
    * <pre>
    * - 0: 方法调用成功。
    * - 其他：方法调用失败。
    * </pre>
    */
     updateChannelMediaRelay(config: NERtcChannelMediaRelayConfiguration): number{
        return this.nertcEngine.updateChannelMediaRelay(config);
    }

    /** 
    * 停止跨房间媒体流转发。
    * <pre>
    * 主播离开房间时，跨房间媒体流转发自动停止，您也可以在需要的时候随时调用 stopChannelMediaRelay 方法，此时主播会退出所有目标房间。
    * 成功调用该方法后，SDK 会触发 onMediaRelayStateChanged 回调。
    * <pre>
    * @return {number}
    * <pre>
    * - 0: 方法调用成功。
    * - 其他：方法调用失败。
    * </pre>
    */
    stopChannelMediaRelay(): number{
        return this.nertcEngine.stopChannelMediaRelay();
    }

    /** 
    * 设置弱网条件下发布的音视频流回退选项。
    * <pre>
    * 在网络不理想的环境下，发布的音视频质量都会下降。使用该接口并将 option 设置为 kNERtcStreamFallbackAudioOnly 后：
    * - SDK 会在上行弱网且音视频质量严重受影响时，自动关断视频流，尽量保证音频质量。
    * - 同时 SDK 会持续监控网络质量，并在网络质量改善时恢复音视频流。
    * - 当本地发布的音视频流回退为音频流时，或由音频流恢复为音视频流时，SDK 会触发本地发布的媒体流已回退为音频流 onLocalPublishFallbackToAudioOnly 回调。
    * <b>NOTE:</b> 请在加入房间（joinChannel）前调用此方法。
    * <pre>
    * @param  {number} option 发布音视频流的回退选项: NERTCStreamFallbackOption。
    * <pre>
    * - 0: 上行或下行网络较弱时，不对音视频流作回退处理，但不能保证音视频流的质量。
    * - 1: 在下行网络条件较差的情况下，SDK 将只接收视频小流，即低分辨率、低码率视频流。
    * - 2: 上行网络较弱时，只发布音频流。下行网络较弱时，先尝试只接收视频小流，即低分辨率、低码率视频流。如果网络环境无法显示视频，则再回退到只接收音频流。
    * </pre>
    * @returns {number}
    * <pre>
    * - 0: 方法调用成功。
    * - 其他: 方法调用失败。
    * </pre>
    */
    setLocalPublishFallbackOption(option: number): number{
        return this.nertcEngine.setLocalPublishFallbackOption(option);
    }

    /** 
    * 设置弱网条件下订阅的音视频流回退选项。
    * <pre>弱网环境下，订阅的音视频质量会下降。使用该接口并将 option 设置为  #kNERtcStreamFallbackVideoStreamLow 或者 #kNERtcStreamFallbackAudioOnly 后：
    * - SDK 会在下行弱网且音视频质量严重受影响时，将视频流切换为小流，或关断视频流，从而保证或提高通信质量。
    * - SDK 会持续监控网络质量，并在网络质量改善时自动恢复音视频流。
    * - 当远端订阅流回退为音频流时，或由音频流恢复为音视频流时，SDK 会触发远端订阅流已回退为音频流 onRemoteSubscribeFallbackToAudioOnly 回调。
    * <b>NOTE:</b>
    * - 请在加入房间（joinChannel）前调用此方法。
    * </pre>
    * @param  {number} option 发布音视频流的回退选项: NERTCStreamFallbackOption。
    * <pre>
    * - 0: 上行或下行网络较弱时，不对音视频流作回退处理，但不能保证音视频流的质量。
    * - 1: 在下行网络条件较差的情况下，SDK 将只接收视频小流，即低分辨率、低码率视频流。
    * - 2: 上行网络较弱时，只发布音频流。下行网络较弱时，先尝试只接收视频小流，即低分辨率、低码率视频流。如果网络环境无法显示视频，则再回退到只接收音频流。
    * </pre>
    * @returns {number}
    * <pre>
    * - 0: 方法调用成功；
    * - 其他: 方法调用失败。
    * </pre>
    */
    setRemoteSubscribeFallbackOption(option: number): number{
        return this.nertcEngine.setRemoteSubscribeFallbackOption(option);
    }

    /** 
    * 启用或停止 AI 超分。
    * @since V4.4.0
    * <pre>
    * <b>NOTE:</b>
    * - 使用 AI 超分功能之前，请联系技术支持开通 AI 超分功能。
    * - AI 超分仅对以下类型的视频流有效：
    * - 必须为本端接收到第一路 360P 的视频流。
    * - 必须为摄像头采集到的主流大流视频。AI 超分功能暂不支持复原重建小流和屏幕共享辅流。
    * <pre>
    * @param enable 是否启用 AI 超分。默认为关闭状态。
    * @return {number}
    * <pre>
    * - 0: 方法调用成功。
    * - 其他：方法调用失败。
    * </pre>
    */
    enableSuperResolution(enable: boolean): number{
        return this.nertcEngine.enableSuperResolution(enable);
    }

    /**
     * 开启或关闭媒体流加密。
     * @since V4.4.0
     * 在金融行业等安全性要求较高的场景下，您可以在加入房间前通过此方法设置媒体流加密模式。
     * <pre>
     *  <b>NOTE:</b>
     * - 请在加入房间前调用该方法，加入房间后无法修改加密模式与密钥。用户离开房间后，SDK 会自动关闭加密。如需重新开启加密，需要在用户再次加入房间前调用此方法。
     * - 同一房间内，所有开启媒体流加密的用户必须使用相同的加密模式和密钥，否则使用不同密钥的成员加入房间时会报错 kNERtcErrEncryptNotSuitable（30113）。
     * - 安全起见，建议每次启用媒体流加密时都更换新的密钥。
     * </pre>
     * @param enable 是否开启媒体流加密。
     * <pre>
     * - true: 开启
     * - false:（默认）关闭
     * </pre>
     * @param config 媒体流加密方案。详细信息请参考 nertc::NERtcEncryptionConfig 。
     * @param {number} config.mode 媒体流加密模式。
     * <pre>
     * - 0: 128 位 SM4 加密，ECB 模式
     * - 1: 自定义加密
     * <b>NOTE:</b>
     * - 自定义加密需要在cpp层开发自定义算法，实现nertc_node_engine_event_handler文件中NertcNodePacketObserver中相关方法
     * - 自定义加密 key设置为null。
     * </pre>
     * @param {String} config.key 媒体流加密密钥。字符串类型，推荐设置为英文字符串。
     * @return
     * <pre>
     * - 0: 方法调用成功
     * - 其他: 调用失败
     * </pre>
     */
    enableEncryption(enable: boolean, config: NERtcEncryptionConfig): number{
        return this.nertcEngine.enableEncryption(enable, config);
    }

    /**
    * 开始通话前网络质量探测。
    * <pre>
    * - 启用该方法后，SDK 会通过回调方式反馈上下行网络的质量状态与质量探测报告，包括带宽、丢包率、网络抖动和往返时延等数据。一般用于通话前的网络质量探测场景，用户加入房间之前可以通过该方法预估音视频通话中本地用户的主观体验和客观网络状态。
    * - 相关回调如下：
    * - `onLastmileQuality`：网络质量状态回调，以打分形式描述上下行网络质量的主观体验。该回调视网络情况在约 5 秒内返回。
    * - `onLastmileProbeResult`：网络质量探测报告回调，报告中通过客观数据反馈上下行网络质量。该回调视网络情况在约 30 秒内返回。
    * <b>NOTE:</b>
    * - 请在加入房间（joinChannel）前调用此方法。
    * - 调用该方法后，在收到 `onLastmileQuality` 和 `onLastmileProbeResult` 回调之前请不要调用其他方法，否则可能会由于 API 操作过于频繁导致此方法无法执行。
    * </pre>
    * @param {Object} config Last mile网络探测配置。
    * @param {boolean} config.probe_uplink 是否探测上行网络,不发流的用户，例如直播房间中的普通观众，无需进行上行网络探测。
    * <pre>
    * - true: 探测
    * - false: 不探测
    * </pre>
    * @param {boolean} config.probe_downlink 是否探测下行网络。
    * <pre>
    * - true: 探测
    * - false: 不探测
    * </pre>
    * @param {number} config.expected_uplink_bitratebps 本端期望的最高发送码率。
    * <pre>
    * - 单位为 bps，范围为 [100000, 5000000]
    * - 推荐参考 setVideoConfig 中的码率值设置该参数的值。
    * </pre>
    * @param {number} config.expected_downlink_bitratebps 本端期望的最高接收码率。
    * <pre>
    * - 单位为 bps，范围为 [100000, 5000000]
    * </pre>
    * @returns {number}
    * <pre>
    * - 0: 方法调用成功；
    * - 其他: 方法调用失败。
    * </pre>
    */
    startLastmileProbeTest(config: any): number {
        return this.nertcEngine.startLastmileProbeTest(config);
    }
    /**
     * 停止通话前网络质量探测。
     * @since V4.5.0
     * @return
     * <pre>
     * - 0: 方法调用成功
     * - 其他: 调用失败
     * </pre>
     */
    stopLastmileProbeTest(): number {
        return this.nertcEngine.stopLastmileProbeTest();
    }

    /**
     * 设置远端用户音频流为高优先级。
     * @since V5.4.0
     * <pre>
     * 支持在音频自动订阅的情况下，设置某一个远端用户的音频为最高优先级，可以优先听到该用户的音频。
     *  <b>NOTE:</b>
     * - 该接口需要通话中设置，并需要自动订阅打开（默认打开）。
     * - 该接口只能设置一个用户的优先级，后设置的会覆盖之前的设置。
     * - 该接口通话结束后，优先级设置重置。
     * </pre>
     * @param enable 是否设置音频订阅优先级。
     * <pre>
     * - true 设置音频订阅优先级。
     * - false 取消设置音频订阅优先级。
     * </pre>
     * @param {number} uid 用户 ID。
     * @return
     * <pre>
     * - 0: 方法调用成功
     * - 其他: 调用失败
     * </pre>
     */
    setRemoteHighPriorityAudioStream(enable: boolean, uid: number): number {
        return this.nertcEngine.setRemoteHighPriorityAudioStream(enable, uid);
    }

     /**
     * 检测虚拟声卡是否安装（仅适用于 Mac 系统）
     * @since V5.4.0
     * <pre>
     * 该接口会检测电脑是否安装最新版本的虚拟声卡。如果未安装，并且应用中已集成NERTCPrivilegedTask库，该接口会弹出安装虚拟声卡对话框，方便用户安装。
     * </pre>
     * @return
     * <pre>
     * - 0 电脑未安装网易虚拟声卡或虚拟声卡不是最新版本
     * - 1 电脑已安装最新版本的网易虚拟声卡
     * </pre>
     */
    checkNECastAudioDriver(): number {
        return this.nertcEngine.checkNECastAudioDriver();
    }

    /**
     * 启用/禁用虚拟背景。
     * @since V5.4.0
     * <pre>
     * 启用虚拟背景功能后，您可以使用自定义背景图片替换本地用户的原始背景图片。
     * 替换后，频道内所有用户都可以看到自定义背景图片。
     *  <b>NOTE:</b>
     * - 您可以通过onVirtualBackgroundSourceEnabled回调查看虚拟背景是否开启成功或出错原因。
     * - 建议您使用配备 i5 CPU 及更高性能的设备。
     * - 建议您在满足以下条件的场景中使用该功能：
     * - 采用高清摄像设备，环境光线均匀。
     * - 捕获的视频图像整洁，用户肖像半长且基本无遮挡，并且背景是与用户衣服颜色不同的单一颜色。
     * - 虚拟背景功能不支持 Texture 格式的视频或通过 Push 方法从自定义视频捕获中获取的视频。
     * </pre>
     * @param enable 是否开启虚拟背景。
     * <pre>
     * - true 开启虚拟背景。
     * - false 关闭虚拟背景
     * </pre>
     * @param {object} backgroundSource 自定义背景图像:
     * @param {number} backgroundSource.background_source_type 自定义背景图片的类型
     * <pre>
     * - 1 背景图像为纯色（默认）
     * - 2 背景图像只支持 PNG 或 JPG 格式的文件
     * </pre>
     * @param {number} backgroundSource.color 自定义背景图像的颜色。格式为RGB定义的十六进制整数，不带#号.
     * @param {string} backgroundSource.source 自定义背景图片的本地绝对路径。支持 PNG 和 JPG 格式。
     * @return
     * <pre>
     * - 0: 方法调用成功
     * - 其他: 调用失败
     * </pre>
     */
    enableVirtualBackground(enable: boolean, backgroundSource: VirtualBackgroundSource): number {
        return this.nertcEngine.enableVirtualBackground(enable, backgroundSource);
    }

    /**
     * 获取当前设备是否支持虚拟背景功能。
     * @since V5.4.0
     * <pre>
     * 此接口在初始化引擎之后调用
     * </pre>
     * @param type 功能类型： 0:-虚拟背景
     * @return
     * <pre>
     * - true 支持虚拟背景。
     * - false 不支持虚拟背景
     * </pre>
     */
    isFeatureSupported(type: NERtcFeatureType): boolean {
        return this.nertcEngine.isFeatureSupported(type);
    }

     /**
     * 开启并设置云代理服务。
     * @since V5.4.0
     * <pre>
     * 在内网环境下，如果用户防火墙开启了网络限制，请参考《使用云代理》将指定 IP 地址和端口号加入防火墙白名单，然后调用此方法开启云代理，并将 proxyType 参数设置为 NERtcTransportTypeUDPProxy(1)，即指定使用 UDP 协议的云代理。
     * <b>NOTE:</b>
     * - 请在加入房间前调用此方法。
     * </pre>
     * @param {number} proxyType 云代理类型。
     * @return {number}
     * <pre>
     * - 0: 方法调用成功
     * - 其他: 调用失败
     * </pre>
     */
    setCloudProxy(type: number): number {
        return this.nertcEngine.setCloudProxy(type);
    }

    /**
     * 开启或关闭本地数据通道。
     * @since V5.4.0
     * <pre>
     * <b>NOTE:</b>
     * - 该方法加入房间后才可调用。
     * - 成功启用或禁用本地数据通道后，远端会触发 onUserDataStop 或 onUserDataStart  回调。
     * </pre>
     * @param {boolean} enable 是否启用本地数据通道:
     * @return {number}
     * <pre>
     * - 0: 方法调用成功
     * - 其他: 调用失败
     * </pre>
     */
    enableLocalData(enable: boolean): number {
        return this.nertcEngine.enableLocalData(enable);
    }

    /**
     * 取消或恢复订阅指定远端用户数据通道流。
     * @since V5.4.0
     * <pre>
     * 加入房间时，默认订阅所有远端用户的数据通道流，您可以通过此方法取消或恢复订阅指定远端用户的数据通道流。
     * <b>NOTE:</b>
     * - 当kNERtcKeyAutoSubscribeData默认打开时，用户不能手动修改数据通道订阅状态
     * </pre>
     * @param {number} uid  指定用户的 ID。
     * @param {boolean} subscribe 是否订阅远端用户数据通道流。
     * @return {number}
     * <pre>
     * - 0: 方法调用成功
     * - 其他: 调用失败
     * </pre>
     */
    subscribeRemoteData(uid: number, subscribe: boolean): number {
        return this.nertcEngine.subscribeRemoteData(uid, subscribe);
    }

    /**
     * 通过数据通道发送数据。
     * @since v5.4.0
     * @param {ArrayBuffer} data 自定义数据。
     * @return {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    sendData(data: ArrayBuffer): number {
        return this.nertcEngine.sendData(data);
    }

     /**
     * 开启美颜功能模块。
     * @since V5.4.0
     * <pre>
     * 调用此接口后，开启美颜引擎。如果后续不再需要使用美颜功能，可以调用 `stopBeauty` 结束美颜功能模块，销毁美颜引擎并释放资源。
     * 开启美颜功能模块后，默认无美颜效果，您需要通过 `setBeautyEffect` 或其他滤镜、贴纸相关接口设置美颜或滤镜效果。
     * <b>NOTE:</b>
     * - 该方法需要在 `enableLocalVideo` 之前设置。
     * - 该方法仅适用于 Windows 平台。
     * </pre>
     * @param {String} file_path 文件文件绝对路径。（例：windows环境下传入xxx\data\beauty\nebeauty）
     * @return {number}
     * <pre>
     * - 0: 方法调用成功
     * - 其他: 调用失败
     * </pre>
     */
    startBeauty(file_path: string): number {
        return this.nertcEngine.startBeauty(file_path);
    }

    /**
     * 结束美颜功能模块。
     * @since V5.4.0
     * <pre>
     * 如果后续不再需要使用美颜功能，可以调用 `stopBeauty` 结束美颜功能模块，SDK 会自动销毁美颜引擎并释放资源。
     * <b>NOTE:</b>
     * - 该方法仅适用于 Windows 平台。
     * </pre>
     * @return {number}
     * <pre>
     * - 0: 方法调用成功
     * - 其他: 调用失败
     * </pre>
     */
    stopBeauty(): number {
        return this.nertcEngine.stopBeauty();
    }

    /**
     * 暂停或恢复美颜效果。
     * @since V5.4.0
     * <pre>
     * 通过此接口实现取消美颜效果后，包括全局美颜、滤镜在内的所有美颜效果都会暂时关闭，直至重新恢复美颜效果。
     * <b>NOTE:</b>
     * - 该方法仅适用于 Windows 平台。
     * </pre>
     * @param {Boolean} enabled 是否恢复美颜效果。
     * <pre>
     * - true 恢复美颜效果。
     * - false 取消美颜效果。
     * </pre>
     * @return {number}
     * <pre>
     * - 0: 方法调用成功
     * - 其他: 调用失败
     * </pre>
     */
    enableBeauty(enable: boolean): number {
        return this.nertcEngine.enableBeauty(enable);
    }

    /**
     * 获取指定美颜类型的强度设置。
     * @since V5.4.0
     * <pre>
     * 通过接口 `setBeautyEffect` 设置美颜效果及强度后，可以通过此接口查看指定美颜效果的强度设置。
     * <b>NOTE:</b>
     * - 该方法仅适用于 Windows 平台。
     * </pre>
     * @param {number} type 美颜类型
     * @return {number} 指定美颜类型的强度
     */
    getBeautyEffect(type: number): number {
        return this.nertcEngine.getBeautyEffect(type);
    }

    /**
     * 设置美颜效果
     * @since V5.4.0
     * <pre>
     * 通过此接口可以实现设置磨皮、美白、大眼等多种全局美颜类型和对应的美颜强度。
     * <b>NOTE:</b>
     * - 请在引擎初始化之后调用此接口，且该方法在加入房间前后均可调用。
     * - 您可以多次调用此接口以叠加多种全局美颜效果，也可以在此基础上通过其他方法叠加滤镜等自定义效果。
     * - 该方法仅适用于 Windows 平台。
     * </pre>
     * @param {number} type 美颜类型:
     * <pre>
     * - 0 美牙
     * - 1 亮眼
     * - 2 美白
     * - 3 磨皮
     * - 4 小鼻
     * - 5 眼距调整
     * - 6 眼角调整
     * - 7 嘴型调整
     * - 8 大眼
     * - 9 小脸
     * - 10 下巴调整
     * - 11 瘦脸
     * - 12 红润
     * - 13 长鼻
     * - 14 人中
     * - 15 嘴角
     * - 16 圆眼
     * - 17 开眼角
     * </pre>
     * @param {number} level 对应美颜类型的强度[1, 100]
     * @return {number}
     * <pre>
     * - 0: 方法调用成功
     * - 其他: 调用失败
     * </pre>
     */
    setBeautyEffect(type: number, level: number): number { //level *100
        return this.nertcEngine.setBeautyEffect(type, level);
    }

    /**
     * 添加滤镜效果。
     * @since V5.4.0
     * <pre>
     * 通过此接口可以实现加载滤镜资源，并添加对应的滤镜效果；若您需要更换滤镜，重复调用此接口使用新的滤镜资源即可。
     * <b>NOTE:</b>
     * - 请先调用 startBeauty 方法开启美颜功能模块。
     * - 请在引擎初始化之后调用此接口，且该方法在加入房间前后均可调用。
     * - 该方法仅适用于 Windows 平台。
     * - 使用滤镜、贴纸和美妆等自定义美颜效果之前，请联系商务经理获取美颜资源或模型。
     * - 滤镜效果可以和全局美颜、贴纸、美妆等效果互相叠加，但是不支持叠加多个滤镜。
     * </pre>
     * @param {String} file_path 滤镜资源或模型所在的绝对路径。
     * @return {number}
     * <pre>
     * - 0: 方法调用成功
     * - 其他: 调用失败
     * </pre>
     */
    addBeautyFilter(file_path: string): number {
        return this.nertcEngine.addBeautyFilter(file_path);
    }

    /**
     * 取消滤镜效果。
     * @since V5.4.0
     * <pre>
     * <b>NOTE:</b>
     * - 该方法仅适用于 Windows 平台。
     * </pre>
     * @return {number}
     * <pre>
     * - 0: 方法调用成功
     * - 其他: 调用失败
     * </pre>
     */
    removeBeautyFilter(): number {
        return this.nertcEngine.removeBeautyFilter();
    }

    /**
     * 设置滤镜强度。
     * @since V5.4.0
     * <pre>
     * 取值越大，滤镜强度越大，开发者可以根据业务需求自定义设置滤镜强度。
     * <b>NOTE:</b>
     * - 该方法仅适用于 Windows 平台。
     * - 滤镜强度设置实时生效，更换滤镜后需要重新设置滤镜强度，否则强度取默认值。
     * </pre>
     * @param {number} level 滤镜强度。取值范围为 [0 - 100]，默认值为 50。
     * @return {number}
     * <pre>
     * - 0: 方法调用成功
     * - 其他: 调用失败
     * </pre>
     */
    setBeautyFilterLevel(level: number): number { //level *100
        return this.nertcEngine.setBeautyFilterLevel(level);
    }

    /**
     * （此接口为 beta 版本）添加贴纸效果。
     * @since V5.4.0
     * <pre>
     * 此接口用于加载贴纸资源，添加对应的贴纸效果。需要更换贴纸时，重复调用此接口使用新的贴纸资源即可。
     * <b>NOTE:</b>
     * - 该方法仅适用于 Windows 平台。
     * - 使用滤镜、贴纸和美妆等自定义美颜效果之前，需要先准备好对应的美颜资源或模型。
     * - 贴纸效果可以和全局美颜、滤镜、美妆等效果互相叠加，但是不支持叠加多个贴纸。
     * </pre>
     * @param {String} file_path 贴纸资源或模型所在的绝对路径。
     * @return {number}
     * <pre>
     * - 0: 方法调用成功
     * - 其他: 调用失败
     * </pre>
     */
    addBeautySticker(file_path: string): number {
        return this.nertcEngine.addBeautySticker(file_path);
    }

    /**
     * 此接口为 beta 版本）取消贴纸效果。
     * @since V5.4.0
     * <pre>
     * <b>NOTE:</b>
     * - 该方法仅适用于 Windows 平台。
     * </pre>
     * @return {number}
     * <pre>
     * - 0: 方法调用成功
     * - 其他: 调用失败
     * </pre>
     */
    removeBeautySticker(): number {
        return this.nertcEngine.removeBeautySticker();
    }

    /**
     * （此接口为 beta 版本）添加美妆效果。
     * @since V5.4.0
     * <pre>
     * 此接口用于加载美妆模型，添加对应的美妆效果。需要更换美妆效果时，重复调用此接口使用新的美妆模型即可。
     * <b>NOTE:</b>
     * - 该方法仅适用于 Windows 平台。
     * - 使用滤镜、贴纸和美妆等自定义美颜效果之前，需要先准备好对应的美颜资源或模型。
     * - 美妆效果可以和全局美颜、滤镜、贴纸等效果互相叠加，但是不支持叠加多个美妆效果。
     * </pre>
     * @param {String} file_path 美妆模型所在的绝对路径。
     * @return {number}
     * <pre>
     * - 0: 方法调用成功
     * - 其他: 调用失败
     * </pre>
     */
    addBeautyMakeup(file_path: string): number {
        return this.nertcEngine.addBeautyMakeup(file_path);
    }

    /**
     * （此接口为 beta 版本）取消美妆效果。
     * @since V5.4.0
     * <pre>
     * <b>NOTE:</b>
     * - 该方法仅适用于 Windows 平台。
     * </pre>
     * @return {number}
     * <pre>
     * - 0: 方法调用成功
     * - 其他: 调用失败
     * </pre>
     */
    removeBeautyMakeup(): number {
        return this.nertcEngine.removeBeautyMakeup();
    }

    /**
     * 设置本地语音混响效果。
     * @since V5.4.0
     * <pre>
     * 该方法在加入房间前后都能调用，通话结束后重置为默认的关闭状态。
     * </pre>
     * @param {object} param 混响参数
     * @param {number} param.wetGain 湿信号，取值范围为 0 ~ 1，默认值为 0.0f。
     * @param {number} param.dryGain 干信号，取值范围为 0 ~ 1，默认值为 1.0f。
     * @param {number} param.damping 混响阻尼，取值范围为 0 ~ 1，默认值为 1.0f。
     * @param {number} param.roomSize 房间大小，取值范围为 0.1 ~ 2，默认值为 0.1f。
     * @param {number} param.decayTime 持续强度（余响），取值范围为 0.1 ~ 20，默认值为 0.1f。
     * @param {number} param.preDelay 延迟长度，取值范围为 0 ~ 1，默认值为 0.0f。
     * @return {number}
     * <pre>
     * - 0: 方法调用成功
     * - 其他: 调用失败
     * </pre>
     */
    setLocalVoiceReverbParam(param: NERtcReverbParam): number {
        return this.nertcEngine.setLocalVoiceReverbParam(param);
    }

    /**
     * 开启或关闭本地媒体流（主流）的发送。
     * @since V5.4.0
     * <pre>
     * - 该方法用于开始或停止向网络发送本地音频或视频数据。
     * - 该方法不影响接收或播放远端媒体流，也不会影响本地音频或视频的采集状态。
     * <b>NOTE:</b>
     * - 该方法暂时仅支持控制音频流的发送。
     * - 该方法在加入房间前后均可调用。
     * - 停止发送媒体流的状态会在通话结束后被重置为允许发送。
     * - 成功调用该方法切换本地用户的发流状态后，房间内其他用户会收到onUserAudioStart（开启发送音频）或 onUserAudioStop（停止发送音频）的回调。
     * </pre>
     * @param {boolean} enabled 是否发布本地媒体流。
     * <pre>
     * - true 布本地媒体流(默认)。
     * - false 不发布本地媒体流。
     * </pre>
     * @param {number} media_type 媒体发布类型，暂时仅支持音频。
     * <pre>
     * - 0 音频 pub 类型。
     * </pre>
     * @return {number}
     * <pre>
     * - 0: 方法调用成功
     * - 其他: 调用失败
     * </pre>
     */
    enableMediaPub(enabled: boolean, mediaType: number): number {
        return this.nertcEngine.enableMediaPub(enabled, mediaType);
    }

    /**
     * 更新权限密钥。
     * @since V5.4.0
     * <pre>
     * - 通过本接口可以实现当用户权限被变更，或者收到权限密钥即将过期的回调onPermissionKeyWillExpire时，更新权限密钥。
     * <b>NOTE:</b>
     * - 请确保已开通高级 Token 鉴权功能，具体请联系网易云信商务经理。
     * - 请在引擎初始化之后调用此接口，且该方法仅可在加入房间后调用。
     * - 适用于变更指定用户加入、创建房间或上下麦时发布流相关权限的场景。
     * </pre>
     * @param {string} key 新的权限密钥
     * @return {number}
     * <pre>
     * - 0: 方法调用成功
     * - 其他: 调用失败
     * </pre>
     */
    updatePermissionKey(key: string): number {
        return this.nertcEngine.updatePermissionKey(key);
    }

    /**
     * 设置玩家本人在房间中的范围语音模式，该设置不影响其他人。
     * @since V5.5.10
     * <pre>
     * - 请在引擎初始化后调用此接口，且该方法在加入房间前后均可调用。
     * <b>NOTE:</b>
     * - 离开房间后，此参数不会自动重置为默认模式，所以请在每次加入房间之前都调用此方法设置语音模式。
     * - 加入房间后，可以随时修改语音模式，并立即生效。
     * </pre>
     * @param {number} mode 范围语音模式，包括所有人和仅小队两种模式。
     * <pre>
     * - 0: 默认模式。设置后玩家附近一定范围的人都能听到该玩家讲话，如果范围内也有玩家设置为此模式，则也可以互相通话。
     * - 1: 小组模式。仅TeamID相同的队友可以互相听到
     * </pre>
     * @return {number}
     * <pre>
     * - 0: 方法调用成功
     * - 其他: 调用失败
     * </pre>
     */
    setRangeAudioMode(mode: number): number {
        return this.nertcEngine.setRangeAudioMode(mode);
    }

    /**
     * 设置范围语音的小队ID。
     * @since V5.5.10
     * <pre>
     * - 请在引擎初始化后调用此接口，且该方法在加入房间前后均可调用。
     * <b>NOTE:</b>
     * - 离开房间后，TeamID 失效，需要重新配置TeamID ，请在每次加入房间之前都调用此方法设置 TeamID。
     * - 离开房间后，TeamID 失效，需要重新配置TeamID ，请在每次加入房间之前都调用此方法设置队伍号。
     * - 如果离开房间后再加入房间，请在收到退房成功回调（onLeaveChannel）后，再调用此方法设置队伍号。
     * - 若加入房间后，调用此接口修改队伍号，设置后立即生效。
     * - 请配合 #setRangeAudioMode  接口一起使用。  
     * </pre>
     * @param {number} team_id 小队ID, 有效值: >=0。若team_id = 0，则房间内所有人（不论范围语音的模式是所有人还是仅小队）都可以听到该成员的声音。
     * @return {number}
     * <pre>
     * - 0: 方法调用成功
     * - 其他: 调用失败
     * </pre>
     */
    setRangeAudioTeamID(team_id: number): number {
        return this.nertcEngine.setRangeAudioTeamID(team_id);
    }

    /**
     * 设置空间音效的距离衰减属性和语音范围。
     * @since V5.5.10
     * <pre>
     * - 请在引擎初始化后调用此接口，且该方法在加入房间前后均可调用。
     * <b>NOTE:</b>
     * - 若要使用范围语音或3D音效功能，加入房间前需要调用一次本接口。
     * - 仅使用范围语音时，您只需要设置audible_distance参数，其他参数设置不生效，填写默认值即可。
     * </pre>
     * @param {number} audible_distance 监听器能够听到扬声器并接收其语音的距离扬声器的最大距离。距离有效范围：[1,max int) ，无默认值。
     * @param {number} conversational_distance 范围语音场景中，该参数设置的值不起作用，保持默认值即可。空间音效场景中，需要配置该参数。控制音频保持其原始音量的范围，超出该范围时，语音聊天的响度在被听到时开始淡出。
     * 默认值为 1。
     * @param {number} roll_off 范围语音场景中，该参数设置的值不起作用，保持默认值即可。
     * <pre>
     * - 0: 指数模式
     * - 1: 线性模式
     * - 2: 无衰减
     * - 3: 仅线性衰减,没有方位效果
     * @return {number}
     * <pre>
     * - 0: 方法调用成功
     * - 其他: 调用失败
     * </pre>
     */
    setAudioRecvRange(audible_distance: number, conversational_distance: number, roll_off: number, ): number {
        return this.nertcEngine.setAudioRecvRange(audible_distance, conversational_distance, roll_off);
    }

    /**
     * 更新本地用户的空间位置。
     * @since V5.5.10
     * <pre>
     * - 请在引擎初始化后调用此接口，且该方法在加入房间前后均可调用。
     * </pre>
     * @param {Object} info L通过 info 参数设置空间音效中说话者和接收者的空间位置信息。
     * @param {Array<Number>} info.speaker_position 说话者的位置信息，三个值依次表示X、Y、Z的坐标值。默认值[0,0,0]。
     * @param {Array<Number>} info.speaker_quaternion 说话者的旋转信息，通过四元组来表示，数据格式为[w, x, y, z]。默认值[0,0,0,0]。
     * @param {Array<Number>} info.head_position 接收者的位置信息，三个值依次表示X、Y、Z的坐标值。默认值[0,0,0]。
     * @param {Array<Number>} info.head_quaternion 接收者的旋转信息，通过四元组来表示，数据格式为[w, x, y, z]。默认值[0,0,0,0]。
     * @return {number}
     * <pre>
     * - 0: 方法调用成功
     * - 其他: 调用失败
     * </pre>
     */
    updateSelfPosition(info: any): number {
        return this.nertcEngine.updateSelfPosition(info);
    }

    /**
     * 开启或关闭空间音效的房间混响效果.
     * @since V5.4.0
     * <pre>
     * - 请在引擎初始化后调用此接口，且该方法在加入房间前后均可调用。
     * - 该接口不支持Linux平台
     * - 请先调用enableSpatializer接口启用空间音效，再调用本接口。
     * </pre>
     * @param {boolean} enable 混响效果开关，默认值关闭。
     * @return {number}
     * <pre>
     * - 0: 方法调用成功
     * - 其他: 调用失败
     * </pre>
     */
    enableSpatializerRoomEffects(enable: boolean): number {
        return this.nertcEngine.enableSpatializerRoomEffects(enable);
    }

    /**
     * 设置空间音效的房间混响属性。
     * @since V5.4.0
     * <pre>
     * - 请在引擎初始化后调用此接口，且该方法在加入房间前才可调用。
     * - 该接口不支持 Linux。
     * - 请先调用 \ref  #enableSpatializer 接口启用空间音效，再调用本接口。
     * </pre>
     * @param {Object} config 房间属性。
     * @param {number} config.room_capacity 房间大小。
     * <pre>
     * - 0: 小房间。
     * - 1: 中等大小房间。
     * - 2: 大房间。
     * - 3: 巨大房间。
     * - 4: 无房间效果
     * </pre>
     * @param {number} config.material 房间材质。
     * <pre>
     * - 0: 透明的
     * - 1: 声学天花板，未开放
     * - 2: 砖块，未开放
     * - 3: 涂漆的砖块，未开放
     * - 4: 粗糙的混凝土块，未开放
     * - 5: 涂漆的混凝土块，未开放
     * - 6: 厚重的窗帘
     * - 7: 隔音的玻璃纤维，未开放
     * - 8: 薄的的玻璃，未开放
     * - 9: 茂密的草地，未开放
     * - 10: 草地
     * - 11: 铺装了油毡的混凝土，未开放
     * - 12: 大理石
     * - 13: 金属，未开放
     * - 14: 镶嵌木板的混凝土，未开放
     * - 15: 石膏，未开放
     * - 16: 粗糙石膏，未开放
     * - 17: 光滑石膏，未开放
     * - 18: 木板，未开放
     * - 19: 石膏灰胶纸板，未开放
     * - 20: 水面或者冰面，未开放
     * - 21: 木头天花板，未开放
     * - 22: 木头枪板，未开放
     * - 23: 均匀分布，未开放
     * </pre>
     * @param {number} config.reflection_scalar 反射比例，默认值1.0。
     * @param {number} config.reverb_gain 混响增益比例因子，默认值1.0。
     * @param {number} config.reverb_time 混响时间比例因子，默认值1.0。
     * @param {number} config.reverb_brightness 混响亮度，默认值1.0。
     * @return {number}
     * <pre>
     * - 0: 方法调用成功
     * - 其他: 调用失败
     * </pre>
     */
    setSpatializerRoomProperty(config: any): number {
        return this.nertcEngine.setSpatializerRoomProperty(config);
    }

    /**
     * 设置空间音效的渲染模式。
     * @since V5.4.0
     * <pre>
     * - 请在引擎初始化后调用此接口，且该方法在加入房间前才可调用。
     * <b>NOTE:</b>
     * - 该接口不支持 Linux 平台。
     * - 请先调用 \ref  #enableSpatializer 接口启用空间音效，再调用本接口。
     * </pre>
     * @param {number} mode 渲染模式。
     * <pre>
     * - 0: 立体声
     * - 1: 双声道低
     * - 2: 双声道中
     * - 3: 双声道高
     * - 4: 仅房间音效
     * </pre>
     * @return {number}
     * <pre>
     * - 0: 方法调用成功
     * - 其他: 调用失败
     * </pre>
     */
    setSpatializerRenderMode(mode: number): number {
        return this.nertcEngine.setSpatializerRenderMode(mode);
    }

    /**
     * 初始化引擎3D音效算法。
     * @since V5.5.10
     * <pre>
     * - 此接口在加入房间前调用后均可调用。
     * <b>NOTE:</b>
     * - 该接口不支持 Linux 平台。
     * </pre>
     * @return {number}
     * <pre>
     * - 0: 方法调用成功
     * - 其他: 调用失败
     * </pre>
     */
    initSpatializer(): number {
        return this.nertcEngine.initSpatializer();
    }

    /**
     * 开启或关闭空间音效。
     * @since V5.4.0
     * <pre>
     * - 请在引擎初始化后调用此接口，且该方法在加入房间前后均可调用。
     * <b>NOTE:</b>
     * - 该接口不支持 Linux 平台。
     * - 开启空间音效后，通话结束时仍保留该开关状态，不重置。
     * - 请先调用 \ref  #initSpatializer 接口初始化空间音效算法，再调用本接口。
     * </pre>
     * @param {boolean} enable 是否打开3D音效算法功能，默认为关闭状态。
     * <pre>
     * - true: 开启空间音效。
     * - false: 关闭空间音效。
     * </pre>
     * @param {boolean} apply_to_team 是否仅本小队开启3D音效。默认为 false。
     * <pre>
     * - true: 仅在接收本小队的语音时有3D音效。
     * - false: 接收到所有的语音都有3D音效。
     * </pre>
     * @return {number}
     * <pre>
     * - 0: 方法调用成功
     * - 其他: 调用失败
     * </pre>
     */
    enableSpatializer(enable: boolean, apply_to_team: boolean): number {
        return this.nertcEngine.enableSpatializer(enable, apply_to_team);
    }

    /**
     * 设置视频水印，水印在本地预览及发送过程中均生效
     * @since V5.5.20
     * <pre>
     * - 请在引擎初始化后调用此接口，且该方法在加入房间前才可调用。
     * <b>NOTE:</b>
     * - 该接口不支持 Linux 平台。
     * - 设置水印后，建议关注水印状态回调 \ref nertc::IRtcEngineEventHandlerEx::onLocalVideoWatermarkState "onLocalVideoWatermarkState"。
     * </pre>
     * @param {boolean} enbale 渲染模式。
     * <pre>
     * - true: 添加水印
     * - false: 删除水印
     * </pre>
     * @param {number} type 视频流类型
     * <pre>
     * - 0: 主流
     * - 1: 辅流
     * </pre>
     * @param {Object} config 
     * @param {number} config.watermark_type 视频水印类型。
     * @param {Object} config.image_watermarks 图片水印。
     * @return {number}
     * <pre>
     * - 0: 方法调用成功
     * - 其他: 调用失败
     * </pre>
     */
    setLocalVideoWatermarkConfigs(enbale: boolean, type: number, config: any): number {
        return this.nertcEngine.setLocalVideoWatermarkConfigs(enbale, type, config);
    }

    // setAudioFrameObserver(enable: boolean) {
    //     const self = this;
    //     const fire = (event: string, ...args: Array<any>) => {
    //         setImmediate(() => {
    //             this.emit(event, ...args);
    //         });
    //     };

    //     /**
    //      * 采集音频数据回调。
    //      * <pre>
    //      * 有本地音频数据驱动就会回调。
    //      * </pre>
    //      * @event NERtcEngine#onAudioFrameEvent
    //      * @param {object} frame 音频帧。
    //      * @param {number} frame.format 音频格式。
    //      * @param {number} frame.format.type 音频类型, 默认：0 PCM 音频格式。
    //      * @param {number} frame.format.channels 音频声道数量。如果是立体声，数据是交叉的。单声道: 1；双声道 : 2。
    //      * @param {number} frame.format.sample_rate 采样率。
    //      * @param {number} frame.format.bytes_per_sample 每个采样点的字节数。对于 PCM 来说，一般使用 16 bit，即两个字节。
    //      * @param {number} frame.format.samples_per_channel 每个房间的样本数量。
    //      * @param {number} frame.data 数据缓冲区。有效数据长度为：samples_per_channel * channels * bytes_per_sample。
    //      * @param {number} frame.sync_timestamp 同步音频主辅流的时间戳，一般只有在同时开启外部音频主流及辅流输入时用到。
    //      */
    //     this.nertcEngine.onAudioFrameEvent('onAudioFrameDidRecord', function (frame: any) {
    //         fire('onAudioFrameDidRecord', frame);
    //     }, enable);
    //     /**
    //      * 获取本地用户和所有远端用户混音后的原始音频数据。
    //      * <pre>
    //      * 返回音频数据只读，有本地音频数据驱动就会回调。
    //      * </pre>
    //      * @event NERtcEngine#onMixedAudioFrame
    //      * @param {object} frame 音频帧。
    //      * @param {number} frame.format 音频格式。
    //      * @param {number} frame.format.type 音频类型, 默认：0 PCM 音频格式。
    //      * @param {number} frame.format.channels 音频声道数量。如果是立体声，数据是交叉的。单声道: 1；双声道 : 2。
    //      * @param {number} frame.format.sample_rate 采样率。
    //      * @param {number} frame.format.bytes_per_sample 每个采样点的字节数。对于 PCM 来说，一般使用 16 bit，即两个字节。
    //      * @param {number} frame.format.samples_per_channel 每个房间的样本数量。
    //      * @param {number} frame.data 数据缓冲区。有效数据长度为：samples_per_channel * channels * bytes_per_sample。
    //      * @param {number} frame.sync_timestamp 同步音频主辅流的时间戳，一般只有在同时开启外部音频主流及辅流输入时用到。
    //      */
    //     this.nertcEngine.onAudioFrameEvent('onMixedAudioFrame', function (frame: any) {
    //         fire('onMixedAudioFrame', frame);
    //     }, enable);
    //     /**
    //      *播放音频数据回调，用于声音处理等操作。
    //      * <pre>
    //      * 有本地音频数据驱动就会回调。
    //      * </pre>
    //      * @event NERtcEngine#onAudioFrameEvent
    //      * @param {object} frame 音频帧。
    //      * @param {number} frame.format 音频格式。
    //      * @param {number} frame.format.type 音频类型, 默认：0 PCM 音频格式。
    //      * @param {number} frame.format.channels 音频声道数量。如果是立体声，数据是交叉的。单声道: 1；双声道 : 2。
    //      * @param {number} frame.format.sample_rate 采样率。
    //      * @param {number} frame.format.bytes_per_sample 每个采样点的字节数。对于 PCM 来说，一般使用 16 bit，即两个字节。
    //      * @param {number} frame.format.samples_per_channel 每个房间的样本数量。
    //      * @param {number} frame.data 数据缓冲区。有效数据长度为：samples_per_channel * channels * bytes_per_sample。
    //      * @param {number} frame.sync_timestamp 同步音频主辅流的时间戳，一般只有在同时开启外部音频主流及辅流输入时用到。
    //      */
    //     this.nertcEngine.onAudioFrameEvent('onAudioFrameWillPlayback', function (frame: any) {
    //         fire('onAudioFrameWillPlayback', frame);
    //     }, enable);

    // }

    /**
     * init event handler
     * @private
     */
    initEventHandler(): void {
        const self = this;

        const fire = (event: string, ...args: Array<any>) => {
            setImmediate(() => {
                this.emit(event, ...args);
            });
        };

        /**
         * 发生错误回调。
         * @event NERtcEngine#onError
         * @param {NERtcErrorCode} errorCode 错误码
         * @param {string} msg 错误描述
         */
        this.nertcEngine.onEvent('onError', function (
            errorCode: number,
            msg: string
        ) {
            fire('onError', errorCode, msg);
        });

        /**
         * 发生警告回调。
         * @event NERtcEngine#onWarning
         * @param {NERtcErrorCode} warnCode 错误码
         * @param {string} msg 错误描述
         */
        this.nertcEngine.onEvent('onWarning', function (
            warnCode: number,
            msg: string
        ) {
            fire('onWarning', warnCode, msg);
        });

        /**
         * API调用结束回调。
         * @since V5.4.0
         * @event NERtcEngine#onApiCallExecuted
         * @param {string} apiName API名称
         * @param {NERtcErrorCode} code API执行结果错误码
         * @param {string} msg API执行结果描述
         */
        this.nertcEngine.onEvent('onApiCallExecuted', function (
            apiName: string, 
            code: number, 
            msg: string
        ) {
            fire('onApiCallExecuted', apiName, code, msg);
        });

        /**
         * 接收的远端视频分辨率变化回调。
         * @since V5.4.1
         * <pre>
          * 当远端用户视频流的分辨率发生变化时，会触发此回调，例如推流端调用 SetVideoConfig 更改了编码分辨率设置，本地会收到该远端用户分辨率变化通知。
          * </pre>
         * @event NERtcEngine#onRemoteVideoReceiveSizeChanged
         * @param {number} uid 远端用户ID，指定是哪个用户的视频流
         * @param {number} type 视频通道类型
         * <pre>
         * - 0 主流
         * - 1 辅流
         * </pre>
         * @param {number} width 视频采集的宽，单位为 px
         * @param {number} height 视频采集的高，单位为 px
         */
        this.nertcEngine.onEvent('onRemoteVideoReceiveSizeChanged', function (
            uid: number, 
            type: number, 
            width: number,
            height: number
        ) {
            fire('onRemoteVideoReceiveSizeChanged', uid, type, width, height);
        });

        /**
         * 本地视频预览的分辨率变化回调, 与是否进入房间的状态无关，与硬件状态有关，也适用于预览。
         * @since V5.4.1
         * <pre>
          * 当本地视频的分辨率发生变化，会触发此回调。
          * 当调用 SetCaptureConfig 设置采集分辨率或调用 SetVideoConfig 设置编码属性时可以触发该回调。回调的分辨率宽和高为本地预览的宽和高，和实际编码发送的分辨率不一定一致
          * 开发者可以根据该回调的分辨率来动态调整预览视图的比例等。
          * </pre>
         * @event NERtcEngine#onLocalVideoRenderSizeChanged
         * @param {number} type 视频通道类型
         * <pre>
         * - 0 主流
         * - 1 辅流
         * </pre>
         * @param {number} width 视频采集的宽，单位为 px
         * @param {number} height 视频采集的高，单位为 px
         */
        this.nertcEngine.onEvent('onLocalVideoRenderSizeChanged', function (
            type: number, 
            width: number,
            height: number
        ) {
            fire('onLocalVideoRenderSizeChanged', type, width, height);
        });

        /**
         * 已接收到远端视频首帧并完成渲染的回调。
         * <pre>
          * 当 SDK 收到远端视频的第一帧并渲染成功时，会触发该回调。
          * </pre>
         * @event NERtcEngine#onFirstVideoFrameRender
         * @param {number} type 视频通道类型
         * <pre>
         * - 0 主流
         * - 1 辅流
         * </pre>
         * @param {number} uid 用户 ID，提示是哪个用户的视频流。
         * @param {number} width 首帧视频的宽度，单位为 px。
         * @param {number} height 视频采集的高，单位为 px
         * * @param {number} 首帧视频的高度，单位为 px。
         */
        this.nertcEngine.onEvent('onFirstVideoFrameRender', function (
            type: number, 
            uid: number, 
            width: number,
            height: number,
            elapsed: number
        ) {
            fire('onFirstVideoFrameRender', type, uid, type, width, height, elapsed);
        });

        /**
         * 实验功能回调接口，用于回调一些非正式的事件及数据通知。
         * @event NERtcEngine#onLabFeatureCallback
         * @param {string} key 返回回调类型。
         * @param {string} param 值内容。对应字符串的参数值，如果是结构体对象，需要转成json格式。
         */
        this.nertcEngine.onEvent('onLabFeatureCallback', function (
            key: string, 
            param: string
        ) {
            fire('onLabFeatureCallback', key, param);
        });

        // /**
        //  * 屏幕共享源采集范围等变化的回调。
        //  * @event NERtcEngine#onLabFeatureCallback
        //  * @param {Object} data 返回回调类型。
        //  * @param {number} data.type 屏幕分享类型。
        //  * @param {number} data.source_id 屏幕分享源的ID。
        //  * @param {number} data.status 屏幕分享状态。
        //  * @param {number} data.action 屏幕分享自定义高亮框的设置动作，结合capture_rect使用。
        //  * @param {number} data.regionRect.x 左上角的横向偏移
        //  * @param {number} data.regionRect.y 左上角的纵向偏移
        //  * @param {number} data.regionRect.width 待共享区域的宽
        //  * @param {number} data.regionRect.height 待共享区域的高
        //  * @param {number} data.level 屏幕分享源的层级，仅用于macOS
        //  */
        // this.nertcEngine.onEvent('onScreenCaptureSourceDataUpdate', function (
        //     data: NERtcScreenCaptureSourceData
        // ) {
        //     fire('onScreenCaptureSourceDataUpdate', data);
        // });

        /**
         * 释放硬件资源的回调。
         * @event NERtcEngine#onReleasedHwResources
         * @param {NERtcErrorCode} result 错误码
         */
        this.nertcEngine.onEvent('onReleasedHwResources', function (
            result: NERtcErrorCode
        ) {
            fire('onReleasedHwResources', result);
        });

        /**
         * 加入频道回调
         * @event NERtcEngine#onJoinChannel
         * @param {number} cid 频道 ID
         * @param {number} uid 用户 ID
         * @param {NERtcErrorCode} result 错误码
         * @param {number} elapsed 从 joinChannel 开始到发生此事件过去的时间（毫秒）
         */
        this.nertcEngine.onEvent('onJoinChannel', function (
            cid: number,
            uid: number,
            result: NERtcErrorCode,
            elapsed: number
        ) {
            fire('onJoinChannel', cid, uid, result, elapsed);
        });

        /**
          * 触发重连。
          * <pre>
          * 有时候由于网络原因，客户端可能会和服务器失去连接，SDK会进行自动重连，开始自动重连后触发此回调。
          * </pre>
          * @event NERtcEngine#onReconnectingStart
          * @param cid  频道 ID。
          * @param uid  用户 ID。
          */
        this.nertcEngine.onEvent('onReconnectingStart', function (
            cid: number,
            uid: number
        ) {
            fire('onReconnectingStart', cid, uid);
        });

        /**
          * 连接状态变更。
          * <pre>
          * 有时候由于通话流程、用户行为、网络原因等，客户端通话状态变更，触发此回调。
          * </pre>
          * @event NERtcEngine#onConnectionStateChange
          * @param {number} state  变更后通话状态:
          * <pre>
          * - 1 没加入频道
          * - 2 正在加入频道
          * - 3 加入频道成功
          * - 4 正在尝试重新加入频道
          * - 5 加入频道失败
          * </pre>
          * @param {number} reason  变更原因:
          * <pre>
          * - 1 离开房间
          * - 2 房间被关闭
          * - 3 用户被踢
          * - 4 服务超时
          * - 5 加入房间
          * - 6 加入房间成功
          * - 7 重新加入房间成功（重连）
          * - 8 媒体连接断开
          * - 9 信令连接断开
          * - 10 请求频道失败
          * - 11 加入频道失败
          * </pre>
          */
        this.nertcEngine.onEvent('onConnectionStateChange', function (
            state: NERtcConnectionStateType,
            reason: NERtcReasonConnectionChangedType
        ) {
            fire('onConnectionStateChange', state, reason);
        });

        /**
         * 重新加入频道回调。
         * <pre>
         * 有时候由于网络原因，客户端可能会和服务器失去连接，SDK会进行自动重连，自动重连后触发此回调方法。
         * </pre>
         * @event NERtcEngine#onRejoinChannel
         * @param {number} cid 频道 ID
         * @param {number} uid 用户 ID
         * @param {NERtcErrorCode} result 错误码
         * @param {number} elapsed 从 joinChannel 开始到发生此事件过去的时间（毫秒）
         */
        this.nertcEngine.onEvent('onRejoinChannel', function (
            cid: number,
            uid: number,
            result: NERtcErrorCode,
            elapsed: number
        ) {
            fire('onRejoinChannel', cid, uid, result, elapsed);
        });

        /**
         * 离开频道回调。
         * App 调用 {@link NERtcEngine#leaveChannel} 方法时，SDK提示 App 离开频道是否成功。
         * @event NERtcEngine#onLeaveChannel
         * @param {NERtcErrorCode} result 错误码
         */
        this.nertcEngine.onEvent('onLeaveChannel', function (
            result: NERtcErrorCode
        ) {
            fire('onLeaveChannel', result);
        });

        /**
         * 掉线回调。
         * <pre>
         * 由于非网络原因，客户端可能会和服务器失去连接，此时SDK无需自动重连，直接触发此回调方法。
         * </pre>
         * @event NERtcEngine#onDisconnect
         * @param {NERtcErrorCode} result 错误码
         */
        this.nertcEngine.onEvent('onDisconnect', function (
            result: NERtcErrorCode
        ) {
            fire('onDisconnect', result);
        });

        /** 参会者角色类型变更回调。
         * <pre>
         * 本地用户加入房间后，通过 {@link NERtcEngine#setClientRole} 切换用户角色后会触发此回调。例如从主播切换为观众、从观众切换为主播。
         * <b>NOTE:</b>
         * 直播场景下，如果您在加入房间后调用该方法切换用户角色，调用成功后，会触发以下回调：
         * - 主播切观众，本端触发onClientRoleChanged回调，远端触发 onUserLeft 回调。
         * - 观众切主播，本端触发onClientRoleChanged回调，远端触发 onUserJoined 回调。
         * @event NERtcEngine#onClientRoleChanged
         * @param {number} oldRole 原角色类型。
         * <pre>
         * - 0 主播
         * - 1 观众
         * </pre>
         * @param {number} newRole 新角色类型。
         * <pre>
         * - 0 主播
         * - 1 观众
         * </pre>
         */
        this.nertcEngine.onEvent('onClientRoleChanged', function (
            oldRole: NERtcClientRole,
            newRole: NERtcClientRole
        ) {
            fire('onClientRoleChanged', oldRole, newRole);
        });

        /**
         * 远端用户加入当前频道回调。
         * <pre>
         * - 通信模式下，该回调提示有远端用户加入了频道，并返回新加入用户的 ID；如果加入之前，已经有其他用户在频道中了，新加入的用户也会收到这些已有用户加入频道的回调。
         * </pre>
         * @event NERtcEngine#onUserJoined
         * @param {number} uid 新加入频道的远端用户ID。
         * @param {string} userName 新加入频道的远端用户名(无效)。
         */
        this.nertcEngine.onEvent('onUserJoined', function (
            uid: number,
            userName: string
        ) {
            fire('onUserJoined', uid, userName);
        });

          /**
         * 远端用户加入当前频道回调。
         * @since V5.4.0
         * <pre>
         * - 通信模式下，该回调提示有远端用户加入了频道，并返回新加入用户的 ID；如果加入之前，已经有其他用户在频道中了，新加入的用户也会收到这些已有用户加入频道的回调。
         * </pre>
         * @event NERtcEngine#onUserJoinedEx
         * @param {number} uid 新加入频道的远端用户ID。
         * @param {string} userName 新加入频道的远端用户名(无效)。
         * @param {object} extra_info 一些可选信息:
         * @param {string} extra_info.custom_info 自定义信息，来源于远端用户joinChannel时填的 {@link NERtcJoinChannelOptions#custom_info}参数，默认为空字符串。
         */
        this.nertcEngine.onEvent('onUserJoinedWithExtraInfo', function (
            uid: number, 
            userName: number, 
            extra_info: any) {
            fire('onUserJoinedWithExtraInfo', uid, userName, extra_info);
        });

        /**
         * 远端用户离开当前频道回调。
         * <pre>
         * 提示有远端用户离开了频道（或掉线）。
         * </pre>
         * @event NERtcEngine#onUserLeft
         * @param {number} uid 远端用户ID。
         * @param {number} reason 远端用户离开原因:
         * <pre>
         * - 0 正常离开
         * - 1 用户断线导致离开
         * - 2 用户 Failover 过程中产生的 leave
         * - 3 用户被踢导致离开
         * - 4 用户超时导致离开
         * </pre>
         */
        this.nertcEngine.onEvent('onUserLeft', function (
            uid: number,
            reason: NERtcSessionLeaveReason
        ) {
            fire('onUserLeft', uid, reason);
        });

          /**
         * 远端用户离开当前频道回调。
         * @since V5.4.0
         * <pre>
         * 提示有远端用户离开了频道（或掉线）。
         * </pre>
         * @event NERtcEngine#onUserLeftEx
         * @param {number} uid 远端用户ID。
         * @param {number} reason 远端用户离开原因:
         * <pre>
         * - 0 正常离开
         * - 1 用户断线导致离开
         * - 2 用户 Failover 过程中产生的 leave
         * - 3 用户被踢导致离开
         * - 4 用户超时导致离开
         * </pre>
         * @param {object} extra_info 一些可选信息:
         * @param {string} extra_info.custom_info 自定义信息，来源于远端用户joinChannel时填的 {@link NERtcJoinChannelOptions#custom_info}参数，默认为空字符串。
         */
        this.nertcEngine.onEvent('onUserLeftWithExtraInfo', function (
            uid: number, 
            reason: number, 
            extra_info: any) {
            fire('onUserLeftWithExtraInfo', uid, reason, extra_info);
        });

        /**
         * 远端用户开启音频回调。
         * @event NERtcEngine#onUserAudioStart
         * @param {number} uid 远端用户ID。
         */
        this.nertcEngine.onEvent('onUserAudioStart', function (
            uid: number
        ) {
            fire('onUserAudioStart', uid);
        });

        /**
         * 远端用户停用音频回调。
         * @event NERtcEngine#onUserAudioStop
         * @param {number} uid 远端用户ID。
         */
        this.nertcEngine.onEvent('onUserAudioStop', function (
            uid: number
        ) {
            fire('onUserAudioStop', uid);
        });

        /**
         * 远端用户开启视频回调。
         * @event NERtcEngine#onUserVideoStart
         * @param {number} uid 远端用户ID。
         * @param {number} maxProfile 最大分辨率:
         * <pre>
         * - 0 160x90/120, 15fps
         * - 1 320x180/240, 15fps
         * - 2 640x360/480, 30fps
         * - 3 1280x720, 30fps
         * - 4 1920x1080, 30fps
         * - 5 none
         * - 6 FakeVideo 标识，仅在回调中显示。请勿主动设置，否则 SDK 会按照STANDARD处理。
         * </pre>
         */
        this.nertcEngine.onEvent('onUserVideoStart', function (
            uid: number,
            maxProfile: NERtcVideoProfileType
        ) {
            fire('onUserVideoStart', uid, maxProfile);
        });

        /**
         * 远端用户停用视频回调。
         * @event NERtcEngine#onUserVideoStop
         * @param {number} uid 远端用户ID。
         */
        this.nertcEngine.onEvent('onUserVideoStop', function (
            uid: number
        ) {
            fire('onUserVideoStop', uid);
        });

        /**
         * 远端用户开启辅流视频回调。
         * @event NERtcEngine#onUserSubStreamVideoStart
         * @param {number} uid 远端用户ID。
         * @param {number} maxProfile 最大分辨率:
         * <pre>
         * - 0 160x90/120, 15fps
         * - 1 320x180/240, 15fps
         * - 2 640x360/480, 30fps
         * - 3 1280x720, 30fps
         * - 4 1920x1080, 30fps
         * </pre>
         */
        this.nertcEngine.onEvent('onUserSubStreamVideoStart', function (
            uid: number,
            max_profile: NERtcVideoProfileType
        ) {
            fire('onUserSubStreamVideoStart', uid, max_profile);
        });

        /**
         * 远端用户停用辅流视频回调。
         * @event NERtcEngine#onUserSubStreamVideoStop
         * @param {number} uid 远端用户ID。
         */
        this.nertcEngine.onEvent('onUserSubStreamVideoStop', function (
            uid: number
        ) {
            fire('onUserSubStreamVideoStop', uid);
        });

        /**
         * 远端用户视频配置更新回调。
         * @event NERtcEngine#onUserVideoProfileUpdate
         * @param {number} uid 远端用户ID。
         * @param {number} maxProfile 最大分辨率:
         * <pre>
         * - 0 160x90/120, 15fps
         * - 1 320x180/240, 15fps
         * - 2 640x360/480, 30fps
         * - 3 1280x720, 30fps
         * - 4 1920x1080, 30fps
         * </pre>
         */
        this.nertcEngine.onEvent('onUserVideoProfileUpdate', function (
            uid: number,
            max_profile: NERtcVideoProfileType
        ) {
            fire('onUserVideoProfileUpdate', uid, max_profile);
        });

        /**
         * 远端用户是否静音回调。
         * @event NERtcEngine#onUserAudioMute
         * @param {number} uid 远端用户ID。
         * @param {boolean} mute 是否静音。
         */
        this.nertcEngine.onEvent('onUserAudioMute', function (
            uid: number,
            mute: boolean
        ) {
            fire('onUserAudioMute', uid, mute);
        });

        /**
         * 远端用户是否禁视频流回调。
         * @event NERtcEngine#onUserVideoMute
         * @param {number} uid 远端用户ID。
         * @param {boolean} mute 是否禁视频流。
         */
        this.nertcEngine.onEvent('onUserVideoMute', function (
            uid: number,
            mute: boolean
        ) {
            fire('onUserVideoMute', uid, mute);
        });

        /**
         * 远端用户是否禁视频流回调。
         * @since V5.4.0
         * @event NERtcEngine#onUserVideoMuteEx
         * @param {number} streamType 视频流类型
         * <pre>
         * - 0 主流
         * - 1 辅流
         * </pre>
         * @param {number} uid 远端用户ID。
         * @param {boolean} mute 是否禁视频流。
         */
        this.nertcEngine.onEvent('onUserVideoMuteWithType', function (
            streamType: number, 
            uid: number, 
            mute: boolean) {
            fire('onUserVideoMuteWithType', streamType, uid, mute);
        });

        /**
         * 音频设备状态更改回调。
         * @event NERtcEngine#onAudioDeviceStateChanged
         * @param {string} device_id 设备ID。
         * @param {number} device_type 音频设备类型:
         * <pre>
         * - 0 未知音频设备
         * - 1 音频采集设备
         * - 2 音频播放设备
         * </pre>
         * @param {number} device_state 音频设备状态:
         * <pre>
         * - 0 音频设备已激活
         * - 1 音频设备未激活
         * </pre>
         */
        this.nertcEngine.onEvent('onAudioDeviceStateChanged', function (
            device_id: string,
            device_type: NERtcAudioDeviceType,
            device_state: NERtcAudioDeviceState
        ) {
            fire('onAudioDeviceStateChanged', device_id, device_type, device_state);
        });

        /**
         * 音频默认设备更改回调。
         * @event NERtcEngine#onAudioDefaultDeviceChanged
         * @param {string} device_id 设备ID。
         * @param {number} device_type 音频设备类型:
         * <pre>
         * - 0 未知音频设备
         * - 1 音频采集设备
         * - 2 音频播放设备
         * </pre>
         */
        this.nertcEngine.onEvent('onAudioDefaultDeviceChanged', function (
            device_id: string,
            device_type: NERtcAudioDeviceType
        ) {
            fire('onAudioDefaultDeviceChanged', device_id, device_type);
        });

        /**
         * 视频设备状态更改回调。
         * @event NERtcEngine#onVideoDeviceStateChanged
         * @param {string} device_id 设备ID。
         * @param {number} device_type 视频设备类型:
         * <pre>
         * - 0 未知音频设备
         * - 1 视频采集设备
         * </pre>
         * @param {number} device_state 视频设备状态:
         * <pre>
         * - 0 视频设备已添加
         * - 1 视频设备已拔除
         * </pre>
         */
        this.nertcEngine.onEvent('onVideoDeviceStateChanged', function (
            device_id: string,
            device_type: NERtcAudioDeviceType,
            device_state: NERtcAudioDeviceState
        ) {
            fire('onVideoDeviceStateChanged', device_id, device_type, device_state);
        });

        /**
         * 已接收到远端音频首帧回调。
         * @event NERtcEngine#onFirstAudioDataReceived
         * @param {number} uid 发送音频帧的远端用户的用户 ID。
         */
        this.nertcEngine.onEvent('onFirstAudioDataReceived', function (
            uid: number
        ) {
            fire('onFirstAudioDataReceived', uid);
        });

        /**
         * 已显示首帧远端视频回调。
         * <pre>
         * 第一帧远端视频显示在视图上时，触发此调用。
         * </pre>
         * @event NERtcEngine#onFirstVideoDataReceived
         * @param {number} uid 用户 ID，指定是哪个用户的视频流。
         */
        this.nertcEngine.onEvent('onFirstVideoDataReceived', function (
            uid: number
        ) {
            fire('onFirstVideoDataReceived', uid);
        });

        /**
         * 已显示首帧远端视频回调。
         * @since V5.4.0
         * <pre>
         * 第一帧远端视频显示在视图上时，触发此调用。
         * </pre>
         * @event NERtcEngine#onFirstVideoDataReceivedEx
         * @param {number} streamType 视频流类型
         * <pre>
         * - 0 主流
         * - 1 辅流
         * </pre>
         * @param {number} uid 用户 ID，指定是哪个用户的视频流。
         */
        this.nertcEngine.onEvent('onFirstVideoDataReceivedWithType', function (
            streamType: number, 
            uid: number
        ) {
            fire('onFirstVideoDataReceivedWithType', streamType, uid);
        });

        /**
         * 已解码远端音频首帧的回调。
         * @event NERtcEngine#onFirstAudioFrameDecoded
         * @param {number} uid 远端用户 ID。
         */
        this.nertcEngine.onEvent('onFirstAudioFrameDecoded', function (
            uid: number
        ) {
            fire('onFirstAudioFrameDecoded', uid);
        });

        /**
         * 已显示首帧远端视频回调。
         * <pre>
         * 引擎收到第一帧远端视频流并解码成功时，触发此调用。 App 可在此回调中设置该用户的 video canvas。
         * </pre>
         * @event NERtcEngine#onFirstVideoFrameDecoded
         * @param {number} uid 用户 ID，指定是哪个用户的视频流。
         * @param {number} width 视频流宽（px）。
         * @param {number} height 视频流高（px）。
         */
        this.nertcEngine.onEvent('onFirstVideoFrameDecoded', function (
            uid: number,
            width: number,
            height: number
        ) {
            fire('onFirstVideoFrameDecoded', uid, width, height);
        });

        /**
         * 已显示首帧远端视频回调。
         * @since V5.4.0
         * <pre>
         * 引擎收到第一帧远端视频流并解码成功时，触发此调用。 App 可在此回调中设置该用户的 video canvas。
         * </pre>
         * @event NERtcEngine#onFirstVideoFrameDecodedEx
         * @param {number} streamType 视频流类型
         * <pre>
         * - 0 主流
         * - 1 辅流
         * </pre>
         * @param {number} uid 用户 ID，指定是哪个用户的视频流。
         * @param {number} width 视频流宽（px）。
         * @param {number} height 视频流高（px）。
         */
        this.nertcEngine.onEvent('onFirstVideoFrameDecodedWithType', function (
            streamType: number, 
            uid: number, 
            width: number, 
            height: number
        ) {
            fire('onFirstVideoFrameDecodedWithType', streamType, uid, width, height);
        });

        /**
         * 本地用户的音乐文件播放状态改变回调。
         * <pre>
         * 调用 {@link NERtcEngine#startAudioMixing} 播放混音音乐文件后，当音乐文件的播放状态发生改变时，会触发该回调。
         * - 如果播放音乐文件正常结束，state 会返回相应的状态码 0，error_code 返回 0。
         * - 如果播放出错，则返回状态码 1，error_code 返回相应的出错原因。
         * - 如果本地音乐文件不存在、文件格式不支持、无法访问在线音乐文件 URL，error_code都会返回 100。
         * </pre>
         * @event NERtcEngine#onAudioMixingStateChanged
         * @param {number} state 音乐文件播放状态:
         * <pre>
         * - 0 音乐文件播放结束。
         * - 1 音乐文件报错。
         * </pre>
         * @param {number} error_code 错误码:
         * <pre>
         * - 0 没有错误。
         * - 1 通用错误。
         * - 100 音乐文件打开出错。
         * </pre>
         */
        this.nertcEngine.onEvent('onAudioMixingStateChanged', function (
            state: NERtcAudioMixingState,
            error_code: NERtcAudioMixingErrorCode
        ) {
            fire('onAudioMixingStateChanged', state, error_code);
        });

        /**
         * 本地用户的音乐文件播放进度回调
         * <pre>
         * 调用 {@link NERtcEngine#startAudioMixing} 播放混音音乐文件后，当音乐文件的播放进度改变时，会触发该回调。
         * </pre>
         * @event NERtcEngine#onAudioMixingTimestampUpdate
         * @param {number} timestamp_ms 音乐文件播放进度，单位为毫秒。
         */
        this.nertcEngine.onEvent('onAudioMixingTimestampUpdate', function (
            timestamp_ms: number
        ) {
            fire('onAudioMixingTimestampUpdate', timestamp_ms);
        });

        /**
         * 本地音效文件播放已结束回调。
         * <pre>
         * 当播放音效结束后，会触发该回调。
         * </pre>
         * @event NERtcEngine#onAudioEffectFinished
         * @param {number} effect_id 音效ID
         */
        this.nertcEngine.onEvent('onAudioEffectFinished', function (
            effect_id: number
        ) {
            fire('onAudioEffectFinished', effect_id);
        });

        this.nertcEngine.onEvent('onAudioEffectTimestampUpdate', function (
            effecct_id: number, 
            timestamp_ms: number
        ) {
            fire('onAudioEffectTimestampUpdate', effecct_id, timestamp_ms);
        });

        /**
         * 提示频道内本地用户瞬时音量的回调。
         * <pre>
         * 该回调默认禁用。可以通过 {@link NERtcEngine#enableAudioVolumeIndication} 方法开启；
         * 开启后，本地用户说话，SDK 会按 {@link NERtcEngine#enableAudioVolumeIndication} 方法中设置的时间间隔触发该回调。
         * 如果本地用户将自己静音（调用了 {@link NERtcEngine#muteLocalAudioStream}），SDK 将音量设置为 0 后回调给应用层。
         * </pre>
         * @event NERtcEngine#onLocalAudioVolumeIndication
         * @param {number} volume （混音后的）音量，取值范围为 [0,100]。
         */
        this.nertcEngine.onEvent('onLocalAudioVolumeIndication', function (
            volume: number
        ) {
            fire('onLocalAudioVolumeIndication', volume);
        });

        /**
         * 提示频道内本地用户瞬时音量的回调。
         * @since V5.4.0
         * <pre>
         * 该回调默认禁用。可以通过 {@link NERtcEngine#enableAudioVolumeIndication} 方法开启；
         * 开启后，本地用户说话，SDK 会按 {@link NERtcEngine#enableAudioVolumeIndication} 方法中设置的时间间隔触发该回调。
         * 如果本地用户将自己静音（调用了 {@link NERtcEngine#muteLocalAudioStream}），SDK 将音量设置为 0 后回调给应用层。
         * </pre>
         * @event NERtcEngine#onLocalAudioVolumeIndicationEx
         * @param {number} volume （混音后的）音量，取值范围为 [0,100]。
         * @param {number} enable_vad 是否检测到人声。
         */
        this.nertcEngine.onEvent('onLocalAudioVolumeIndicationEx', function (
            volume: number, 
            enable_vad: boolean
        ) {
            fire('onLocalAudioVolumeIndicationEx', volume, enable_vad);
        });

        /**
         * 提示频道内谁正在说话及说话者瞬时音量的回调。
         * <pre>
         * 该回调默认禁用。可以通过 {@link NERtcEngine#enableAudioVolumeIndication} 方法开启；
         * 开启后，无论频道内是否有人说话，SDK 都会按 {@link NERtcEngine#enableAudioVolumeIndication} 方法中设置的时间间隔触发该回调。
         * 在返回的 speakers 数组中:
         * - 如果有 uid 出现在上次返回的数组中，但不在本次返回的数组中，则默认该 uid 对应的远端用户没有说话。
         * - 如果volume 为 0，表示该用户没有说话。
         * - 如果speakers 数组为空，则表示此时远端没有人说话。
         * </pre>
         * @event NERtcEngine#onRemoteAudioVolumeIndication
         * @param {object[]} speakers 每个说话者的用户 ID 和音量信息的数组:
         * @param {number} speakers[].uid 说话者的用户 ID。如果返回的 uid 为 0，则默认为本地用户
         * @param {number} speakers[].volume 说话者的音量，范围为 0（最低）- 100（最高）
         * @param {number} speaker_number speakers 数组的大小，即说话者的人数。
         * @param {number} total_volume （混音后的）总音量，取值范围为 [0,100]。
         */
        this.nertcEngine.onEvent('onRemoteAudioVolumeIndication', function (
            speakers: Array<NERtcAudioVolumeInfo>,
            speaker_number: number,
            total_volume: number
        ) {
            fire('onRemoteAudioVolumeIndication', speakers, speaker_number, total_volume);
        });

        /**
         * 通知添加直播任务结果。
         * <pre>
         * 该回调异步返回 {@link NERtcEngine#addLiveStreamTask} 接口的调用结果；实际推流状态参考 onLiveStreamState 事件
         * </pre>
         * @event NERtcEngine#onAddLiveStreamTask
         * @param {String} task_id 任务id
         * @param {String} url 推流地址
         * @param {number} error 结果
         */
        this.nertcEngine.onEvent('onAddLiveStreamTask', function (
            task_id: string,
            url: string,
            error: number
        ) {
            fire('onAddLiveStreamTask', task_id, url, error);
        });

        /**
         * 通知更新直播任务结果。
         * <pre>
         * 该回调异步返回 {@link NERtcEngine#updateLiveStreamTask} 接口的调用结果；实际推流状态参考 onLiveStreamState 事件
         * </pre>
         * @event NERtcEngine#onUpdateLiveStreamTask
         * @param {String} task_id 任务id
         * @param {String} url 推流地址
         * @param {number} error 结果
         */
        this.nertcEngine.onEvent('onUpdateLiveStreamTask', function (
            task_id: string,
            url: string,
            error: number
        ) {
            fire('onUpdateLiveStreamTask', task_id, url, error);
        });

        /**
         * 通知删除直播任务结果。
         * <pre>
         * 该回调异步返回 {@link NERtcEngine#removeLiveStreamTask} 接口的调用结果；实际推流状态参考 onLiveStreamState 事件
         * </pre>
         * @event NERtcEngine#onRemoveLiveStreamTask
         * @param {String} task_id 任务id
         * @param {number} error 结果
         */
        this.nertcEngine.onEvent('onRemoveLiveStreamTask', function (
            task_id: string,
            error: number
        ) {
            fire('onRemoveLiveStreamTask', task_id, error);
        });

        /**
         * 通知直播推流状态
         * @event NERtcEngine#onLiveStreamState
         * @param {string} task_id 任务id
         * @param {string} url 推流地址
         * @param {number} state 直播推流状态:
         * <pre>
         * - 505: 推流中；
         * - 506: 推流失败；
         * - 511: 推流结束；
         * </pre>
         */
        this.nertcEngine.onEvent('onLiveStreamState', function (
            task_id: string,
            url: string,
            state: NERtcLiveStreamStateCode
        ) {
            fire('onLiveStreamState', task_id, url, state);
        });

        /**
         * 监测音频啸叫的回调。
         * @event NERtcEngine#onAudioHowling
         * @param {boolean} howling 是否出现啸叫
         */
        this.nertcEngine.onEvent('onAudioHowling', function (
            howling: boolean
        ) {
            fire('onAudioHowling', howling);
        });

        /**
         * 监听 SEI 数据回调
         * @since 4.1.110
         * @event NERtcEngine#onRecvSEIMsg
         * @param {number} uid 发送该 sei 的用户 id
         * @param {ArrayBuffer} data 接收到的 sei 数据
         */
        this.nertcEngine.onEvent('onRecvSEIMsg', function (
            uid: number,
            data: ArrayBuffer,
        ) {
            fire('onRecvSEIMsg', uid, data);
        });

       /**
         * 屏幕共享暂停/恢复/开始/结束等回调
         * @event NERtcEngine#onScreenCaptureStatusonScreenCaptureStatus
         * @param {number} status 屏幕共享状态。
         * <pre>
         * - 1 开始屏幕共享。
         * - 2 暂停屏幕共享。
         * - 3 恢复屏幕共享。
         * - 4 停止屏幕共享。
         * - 5 屏幕分享的目标窗口被覆盖。
         * </pre>
         */
        this.nertcEngine.onEvent('onScreenCaptureStatus', function (status: number) {
            fire('onScreenCaptureStatus', status);
        });

       /** 音频录制状态回调。
         * @event NERtcEngine#onAudioRecording
         * @param code 音频录制状态码。详细信息请参考 NERtcAudioRecordingCode。
         * @param file_path 音频录制文件保存路径。
         */
        this.nertcEngine.onEvent('onAudioRecording', function (code: number, file_path: String) {
            fire('onAudioRecording', code, file_path);
        });
        
       /** 
         * 跨房间媒体流转发状态发生改变回调。
         * @event NERtcEngine#onMediaRelayStateChanged
         * @param {number} state 当前跨房间媒体流转发状态。
         * <pre>
         * - 0 初始状态。在成功调用 stopChannelMediaRelay 停止跨房间媒体流转发后， onMediaRelayStateChanged 会回调该状态。
         * - 1 尝试跨房间转发媒体流。
         * - 2 源房间主播角色成功加入目标房间。
         * - 3 发生异常，详见 onMediaRelayEvent 的 error 中提示的错误信息。
         * </pre>
         * @param channel_name  媒体流转发的目标房间名。
         */
        this.nertcEngine.onEvent('onMediaRelayStateChanged', function (state: number, channel_name: String) {
            fire('onMediaRelayStateChanged', state, channel_name);
        });

       /** 
         * 媒体流相关转发事件回调。
         * @event NERtcEngine#onMediaRelayEvent
         * @param {number} event 当前媒体流转发事件。详细信息请参考 #NERtcChannelMediaRelayEvent 。
         * <pre>
         * - 0 媒体流转发停止。
         * - 1 正在连接服务器，开始尝试转发媒体流。
         * - 2 连接服务器成功。
         * - 3 视频音频媒体流成功转发到目标房间。
         * - 4 音频媒体流成功转发到目标房间。
         * - 5 媒体流屏幕共享等其他流成功转发到目标房间。
         * - 100 媒体流转发失败。原因包括：
         * - 414 请求参数错误。
         * - 30110 重复调用 startChannelMediaRelay。
         * - 30111 媒体流转发权限不足。例如调用 startChannelMediaRelay 的房间成员为主播角色、或房间为双人通话房间，不支持转发媒体流。
         * - 30112 调用 stopChannelMediaRelay 前，未调用 startChannelMediaRelay。
         * </pre>
         * @param channel_name  转发的目标房间名。
         * @param error         相关错误码。详细信息请参考 #NERtcErrorCode 。
         */
        this.nertcEngine.onEvent('onMediaRelayEvent', function (event: number, channel_name: String, error: number) {
            fire('onMediaRelayEvent', event, channel_name, error);
        });

        /**
         * 本地发布流已回退为音频流、或已恢复为音视频流回调。
         * <br>如果您调用了设置本地推流回退选项 setLocalPublishFallbackOption 接口，并将 option 设置为 #kNERtcStreamFallbackAudioOnly 后，当上行网络环境不理想、本地发布的媒体流回退为音频流时，或当上行网络改善、媒体流恢复为音视频流时，会触发该回调。 
         * @event NERtcEngine#onLocalPublishFallbackToAudioOnly
         * @param {boolean} is_fallback   本地发布流已回退或已恢复。
         * <pre>
         * - true： 由于网络环境不理想，发布的媒体流已回退为音频流。
         * - false：由于网络环境改善，从音频流恢复为音视频流。
         * </pre>
         * @param stream_type   对应的视频流类型，即主流或辅流。
         * <pre>
         * - 0 主流
         * - 1 辅流
         * </pre>
         */
        this.nertcEngine.onEvent('onLocalPublishFallbackToAudioOnly', function (is_fallback: boolean, stream_type: number) {
            fire('onLocalPublishFallbackToAudioOnly', is_fallback, stream_type);
        });

        /**
         * 订阅的远端流已回退为音频流、或已恢复为音视频流回调。
         * <pre>
         * 如果你调用了设置远端订阅流回退选项 setRemoteSubscribeFallbackOption 接口并将 option 设置 #kNERtcStreamFallbackAudioOnly 后，当下行网络环境不理想、仅接收远端音频流时，或当下行网络改善、恢复订阅音视频流时，会触发该回调。
         * </pre>
         * @event NERtcEngine#onRemoteSubscribeFallbackToAudioOnly 
         * @param {number} uid 远端用户的 ID。
         * @param {boolean} is_fallback 远端订阅流已回退或恢复：
         * <pre>
         * - true： 由于网络环境不理想，发布的媒体流已回退为音频流。
         * - false：由于网络环境改善，从音频流恢复为音视频流。
         * </pre>
         * @param stream_type   对应的视频流类型，即主流或辅流。
         * <pre>
         * - 0 主流
         * - 1 辅流
         * </pre>
         */
        this.nertcEngine.onEvent('onRemoteSubscribeFallbackToAudioOnly', function (uid: number, is_fallback: boolean, stream_type: number) {
            fire('onRemoteSubscribeFallbackToAudioOnly', uid, is_fallback, stream_type);
        });

        /**
         * 远端用户开启音频辅流回调。
         * @since V5.4.0
         * @event NERtcEngine#onUserSubStreamAudioStart 
         * @param {number} uid 远端用户的 ID。
         */
        this.nertcEngine.onEvent('onUserSubStreamAudioStart', function (uid: number) {
            fire('onUserSubStreamAudioStart', uid);
        });

        /**
         * 远端用户停用音频辅流回调。
         * @since V5.4.0
         * @event NERtcEngine#onUserSubStreamAudioStop 
         * @param {number} uid 远端用户的 ID。
         */
        this.nertcEngine.onEvent('onUserSubStreamAudioStop', function (uid: number) {
            fire('onUserSubStreamAudioStop', uid);
        });

        /**
         * 远端用户是否静音的回调。
         * @since V5.4.0
         * @event NERtcEngine#onUserSubStreamAudioMute 
         * @param {number} uid 远端用户的 ID。
         * </pre>
         */
        this.nertcEngine.onEvent('onUserSubStreamAudioMute', function (uid: number, mute: number) {
            fire('onUserSubStreamAudioMute', uid, mute);
        });

        /**
         * 通话前网络上下行 last mile 质量状态回调。
         * <pre>
         * 该回调描述本地用户在加入房间前的 last mile 网络探测的结果，以打分形式描述上下行网络质量的主观体验，您可以通过该回调预估本地用户在音视频通话中的网络体验。
         * 在调用 startLastmileProbeTest 之后，SDK 会在约 5 秒内返回该回调。
         * </pre>
         * @since V4.5.0
         * @event NERtcEngine#onLastmileQuality
         * @param {number} quality  网络上下行质量，基于上下行网络的丢包率和抖动计算，探测结果主要反映上行网络的状态。
         * <pre>
         * - 0: 网络质量未知。
         * - 1: 网络质量极好。
         * - 2: 用户主观感觉和 `kNERtcNetworkQualityExcellent` 类似，但码率可能略低于 `kNERtcNetworkQualityExcellent`。
         * - 3: 用户主观感受有瑕疵但不影响沟通。
         * - 4: 勉强能沟通但不顺畅。
         * - 5: 网络质量非常差，基本不能沟通。
         * - 6: 完全无法沟通。
         * </pre>
         */
        this.nertcEngine.onEvent('onLastmileQuality', function (quality: number) {
            fire('onLastmileQuality', quality);
        });
        /**
        * 通话前网络上下行 Last mile 质量探测报告回调。
        * <pre>
        * 该回调描述本地用户在加入房间前的 last mile 网络探测详细报告，报告中通过客观数据反馈上下行网络质量，包括网络抖动、丢包率等数据。您可以通过该回调客观预测本地用户在音视频通话中的网络状态。
        * 在调用 startLastmileProbeTest 之后，SDK 会在约 30 秒内返回该回调。
        * </pre>
        * @since V4.5.0
        * @event NERtcEngine#onLastmileProbeResult
        * @param {object} result  上下行 Last mile 质量探测结果。
        * @param {number} result.rtt 往返时延，单位为毫秒（ms）。
        * @param {number} result.state 质量探测结果的状态。
        * <pre>
        * - 1: 表示本次 last mile 质量探测的结果是完整的。
        * - 2: 表示本次 last mile 质量探测未进行带宽预测，因此结果不完整。通常原因为测试资源暂时受限。
        * - 3: 未进行 last mile 质量探测。通常原因为网络连接中断。
        * </pre>
        * @param {number} result.uplink_report 上行网络质量报告。
        * @param {number} result.uplink_report.jitter 网络抖动，单位为毫秒 (ms)。
        * @param {number} result.uplink_report.packet_loss_rate 丢包率（%）。
        * @param {number} result.uplink_report.available_band_width 可用网络带宽预估，单位为 bps。
        * @param {number} result.downlink_report下行网络质量报告。
        * @param {number} result.downlink_report.jitter 网络抖动，单位为毫秒 (ms)。
        * @param {number} result.downlink_report.packet_loss_rate 丢包率（%）。
        * @param {number} result.downlink_report.available_band_width 可用网络带宽预估，单位为 bps。
        */
        this.nertcEngine.onEvent('onLastmileProbeResult', function (result: any) {
            fire('onLastmileProbeResult', result);
        });

        /**
         * 服务端禁言音视频权限变化回调。
         * @since V5.4.0
         * @event NERtcEngine#onMediaRightChange 
         * @param {boolean} is_audio_banned 是否禁用音频：
         * <pre>
         * - true 禁用音频
         * - false 取消禁用音频
         * </pre>
         * @param {boolean} is_video_banned 是否禁用视频:
         * <pre>
         * - true 禁用视频
         * - false 取消禁用视频
         * </pre>
         */
        this.nertcEngine.onEvent('onMediaRightChange', function (
            is_audio_banned: boolean, 
            is_video_banned: boolean
        ) {
            fire('onMediaRightChange', is_audio_banned, is_video_banned);
        });

        /**
         * 收到检测安装声卡的内容回调（仅适用于 Mac 系统）。
         * @since V5.4.0
         * <pre>
         * 在 Mac 系统上，您可以通过调用 checkNECastAudioDriver 为当前系统安装一个音频驱动，并让 SDK 通过该音频驱动捕获当前 Mac 系统播放出的声音。
         * </pre>
         * @event NERtcEngine#onCheckNECastAudioDriverResult 
         * @param {number} result 安装虚拟声卡的结果:
         * <pre>
         * - 0 安装音频驱动插件成功
         * - 1 安装音频驱动插件未授
         * - 2 安装音频驱动插件失败
         * </pre>
         */
        this.nertcEngine.onEvent('onCheckNECastAudioDriverResult', function (
            result: number
        ) {
            fire('onCheckNECastAudioDriverResult', result);
        });

        /**
         * 通知虚拟背景功能是否成功启用的回调。
         * @since V5.4.0
         * @event NERtcEngine#onVirtualBackgroundSourceEnabled 
         * @param {boolean} enabled 是否成功启用虚拟背景
         * <pre>
         * - true 成功启用
         * - false 未成功启用
         * </pre>
         * @param {number} reason 虚拟背景功能未成功启用的原因或成功启用虚拟背景功能的通知:
         * <pre>
         * - 0 虚拟背景开启成功。
         * - 1 自定义背景图片不存在。
         * - 2 自定义背景图片的图片格式无效。
         * - 3 自定义背景图片的颜色格式无效。
         * </pre>
         */
        this.nertcEngine.onEvent('onVirtualBackgroundSourceEnabled', function (
            enabled: boolean, reason: number
        ) {
            fire('onVirtualBackgroundSourceEnabled', enabled, reason);
        });

        /**
         * 本地视频水印生效结果回调。
         * @since V5.4.0
         * @event NERtcEngine#onLocalVideoWatermarkState 
         * @param {number} videoStreamType 对应的视频流类型
         * <pre>
         * - 0 主流
         * - 1 辅流
         * </pre>
         * @param {number} state 水印状态
         * <pre>
         * - 0 虚拟背景开启成功。
         * - 1 自定义背景图片不存在。
         * - 2 自定义背景图片的图片格式无效。
         * - 3 自定义背景图片的颜色格式无效。
         * </pre>
         * </pre>
         */
        this.nertcEngine.onEvent('onLocalVideoWatermarkState', function (
            videoStreamType: number, 
            state: number
        ) {
            fire('onLocalVideoWatermarkState', videoStreamType, state);
        });

        /**
         * 权限密钥即将过期事件回调
         * <pre>
         * - 由于 PermissionKey 具有一定的时效，在通话过程中如果 PermissionKey 即将失效，SDK 会提前 30 秒触发该回调，提醒用户更新 PermissionKey。
         * </pre>
         * @since V5.4.0
         * @event NERtcEngine#onPermissionKeyWillExpire 
         */
        this.nertcEngine.onEvent('onPermissionKeyWillExpire', function () {
            fire('onPermissionKeyWillExpire');
        });

        /**
         * 更新权限密钥事件回调
         * @since V5.4.0
         * @event NERtcEngine#onUpdatePermissionKey 
         * @param {string} key 新的权限密钥
         * @param {number} code 错误码
         * @param {number} time 更新后的权限密钥剩余有效时间。单位为秒。
         */
        this.nertcEngine.onEvent('onUpdatePermissionKey', function (
            key: string, 
            code: number, 
            time: number
        ) {
            fire('onUpdatePermissionKey', key, code, time);
        });

        
        /**
         * 远端用户通过数据通道发送数据的回调
         * @since V5.4.0
         * @event NERtcEngine#onUserDataReceiveMessage 
         * @param {number} uid 远端用户id
         * @param {ArrayBuffer} data 数据。
         */
        this.nertcEngine.onEvent('onUserDataReceiveMessage', function (
            uid: number, 
            data: ArrayBuffer
        ) {
            fire('onUserDataReceiveMessage', uid, data);
        });

         /**
         * 远端用户开启数据通道的回调
         * @since V5.4.0
         * @event NERtcEngine#onUserDataStart 
         * @param {number} uid 远端用户id
         */
        this.nertcEngine.onEvent('onUserDataStart', function (uid: number) {
            fire('onUserDataStart', uid);
        });

         /**
         * 远端用户停用数据通道的回调
         * @since V5.4.0
         * @event NERtcEngine#onUserDataStop 
         * @param {number} uid 远端用户id
         */
        this.nertcEngine.onEvent('onUserDataStop', function (uid: number) {
            fire('onUserDataStop', uid);
        });

         /**
         * 远端用户数据通道状态变更回调
         * @since V5.4.0
         * @event NERtcEngine#onUserDataStateChanged 
         * @param {number} uid 远端用户id
         */
        this.nertcEngine.onEvent('onUserDataStateChanged', function (uid: number) {
            fire('onUserDataStateChanged', uid);
        });

        /**
         * 远端用户数据通道buffer变更回调
         * @since V5.4.0
         * @event NERtcEngine#onUserDataBufferedAmountChanged 
         * @param {number} uid 远端用户id
         * @param {number} amount 变更前大小
         */
        this.nertcEngine.onEvent('onUserDataBufferedAmountChanged', function (
            uid: number, 
            amount: number) {
            fire('onUserDataBufferedAmountChanged', uid, amount);
        });

        this.nertcEngine.onVideoFrame(function (infos: any) {
            self.doVideoFrameReceived(infos);
        });

        /*****mediaStats*****/
        /**
         * 当前通话统计回调。
         * <pre>
         * SDK 定期向 App 报告当前通话的统计信息，每 2 秒触发一次。
         * </pre>
         * @event NERtcEngine#onRtcStats
         * @param {object} stats NERTC 引擎统计数据:
         * @param {number} stats.cpu_app_usage 当前 App 的 CPU 使用率 (%)。
         * @param {number} stats.cpu_idle_usage 当前系统的 CPU 空闲率 (%)。
         * @param {number} stats.cpu_total_usage 当前系统的 CPU 使用率 (%)。
         * @param {number} stats.memory_app_usage 当前App的内存使用率 (%)。
         * @param {number} stats.memory_total_usage 当前系统的内存使用率 (%)。
         * @param {number} stats.memory_app_kbytes 当前App的内存使用量 (KB)。
         * @param {number} stats.total_duration 通话时长（秒）。
         * @param {number} stats.tx_bytes 发送字节数，累计值。(bytes)
         * @param {number} stats.rx_bytes 接收字节数，累计值。(bytes)
         * @param {number} stats.tx_audio_bytes 音频发送字节数，累计值。(bytes)
         * @param {number} stats.tx_video_bytes 视频发送字节数，累计值。(bytes)
         * @param {number} stats.rx_audio_bytes 音频接收字节数，累计值。(bytes)
         * @param {number} stats.rx_video_bytes 视频接收字节数，累计值。(bytes)
         * @param {number} stats.tx_audio_kbitrate 音频发送码率。(kbps)
         * @param {number} stats.rx_audio_kbitrate 音频接收码率。(kbps)
         * @param {number} stats.tx_video_kbitrate 视频发送码率。(kbps)
         * @param {number} stats.rx_video_kbitrate 视频接收码率。(kbps)
         * @param {number} stats.up_rtt 上行平均往返时延rtt(ms)
         * @param {number} stats.down_rtt 下行平均往返时延rtt(ms)
         * @param {number} stats.tx_audio_packet_loss_rate 本地上行音频实际丢包率。(%)
         * @param {number} stats.tx_video_packet_loss_rate 本地上行视频实际丢包率。(%)
         * @param {number} stats.tx_audio_packet_loss_sum  本地上行音频实际丢包数。
         * @param {number} stats.tx_video_packet_loss_sum  本地上行视频实际丢包数。
         * @param {number} stats.tx_audio_jitter           本地上行音频抖动计算。(ms)
         * @param {number} stats.tx_video_jitter           本地上行视频抖动计算。(ms)
         * @param {number} stats.rx_audio_packet_loss_rate 本地下行音频实际丢包率。(%)
         * @param {number} stats.rx_video_packet_loss_rate 本地下行视频实际丢包率。(%)
         * @param {number} stats.rx_audio_packet_loss_sum  本地下行音频实际丢包数。
         * @param {number} stats.rx_video_packet_loss_sum  本地下行视频实际丢包数。
         * @param {number} stats.rx_audio_jitter           本地下行音频抖动计算。(ms)
         * @param {number} stats.rx_video_jitter           本地下行视频抖动计算。(ms)
         */
        this.nertcEngine.onStatsObserver('onRtcStats', true, function (stats: NERtcStats) {
            fire('onRtcStats', stats);
        });

        /**
         * 本地音频流统计信息回调。
         * <pre>
         * 该回调描述本地设备发送音频流的统计信息，每 2 秒触发一次。
         * </pre>
         * @event NERtcEngine#onLocalAudioStats
         * @param {object} stats 本地音频流统计信息:
         * @param {number} stats.num_channels 当前采集声道数。
         * @param {number} stats.sent_sample_rate 本地上行音频采样率。
         * @param {number} stats.sent_bitrate （上次统计后）发送码率(Kbps)。
         * @param {number} stats.audio_loss_rate 特定时间内的音频丢包率 (%)。
         * @param {number} stats.rtt RTT。
         * @param {number} stats.volume 音量，范围为 0（最低）- 100（最高）。
         */
        this.nertcEngine.onStatsObserver('onLocalAudioStats', true, function (stats: NERtcAudioSendStats) {
            fire('onLocalAudioStats', stats);
        });

        /**
         * 通话中远端音频流的统计信息回调。
         * <pre>
         * 该回调描述远端用户在通话中端到端的音频流统计信息，每 2 秒触发一次。
         * </pre>
         * @event NERtcEngine#onRemoteAudioStats
         * @param {number} uc stats 数组的大小
         * @param {object[]} stats 每个远端用户音频统计信息的数组:
         * @param {number} stats[].uid 用户 ID，指定是哪个用户的音频流
         * @param {number} stats[].received_bitrate （上次统计后）接收到的码率(Kbps)
         * @param {number} stats[].total_frozen_time 用户的下行音频卡顿累计时长(ms)
         * @param {number} stats[].frozen_rate 用户的下行音频平均卡顿率(%)。
         * @param {number} stats[].audio_loss_rate 特定时间内的音频丢包率 (%)。
         * @param {number} stats[].volume 音量，范围为 0（最低）- 100（最高）。
         */
        this.nertcEngine.onStatsObserver('onRemoteAudioStats', true, function (uc: number, stats: Array<NERtcAudioRecvStats>) {
            fire('onRemoteAudioStats', uc, stats);
        });

        /**
         * 本地视频流统计信息回调。
         * <pre>
         * 该回调描述本地设备发送视频流的统计信息，每 2 秒触发一次。
         * </pre>
         * @event NERtcEngine#onLocalVideoStats
         * @param {object} stats 本地视频流统计信息:
         * @param {number} stats.video_layers_count 视频流条数。
         * @param {object[]} stats.video_layers_list 视频流信息数组:
         * @param {number} stats.video_layers_list[].layer_type 流类型： 1、主流，2、辅流。
         * @param {number} stats.video_layers_list[].width 视频流宽（像素）
         * @param {number} stats.video_layers_list[].height 视频流高（像素）
         * @param {number} stats.video_layers_list[].capture_frame_rate 视频采集帧率。
         * @param {number} stats.video_layers_list[].render_frame_rate 视频渲染帧率。
         * @param {number} stats.video_layers_list[].encoder_frame_rate 编码帧率。
         * @param {number} stats.video_layers_list[].sent_frame_rate 发送帧率。
         * @param {number} stats.video_layers_list[].sent_bitrate 发送码率(Kbps)。
         * @param {number} stats.video_layers_list[].target_bitrate 编码器目标码率(Kbps)。
         * @param {number} stats.video_layers_list[].encoder_bitrate 编码器实际编码码率(Kbps)。
         * @param {String} stats.video_layers_list[].codec_name 视频编码器名字。
         */
        this.nertcEngine.onStatsObserver('onLocalVideoStats', true, function (stats: NERtcVideoSendStats) {
            fire('onLocalVideoStats', stats);
        });

        /**
         * 通话中远端视频流的统计信息回调。
         * <pre>
         * 该回调描述远端用户在通话中端到端的视频流统计信息，每 2 秒触发一次。
         * </pre>
         * @event NERtcEngine#onRemoteVideoStats
         * @param {number} uc stats 数组的大小。
         * @param {object} stats 每个远端用户视频统计信息的数组:
         * @param {number} stats.uid 用户 ID，指定是哪个用户的视频流。
         * @param {number} stats.video_layers_count 视频流条数。
         * @param {object[]} stats.video_layers_list 视频流信息数组:
         * @param {number} stats.video_layers_list[].layer_type 流类型： 1、主流，2、辅流。
         * @param {number} stats.video_layers_list[].width 视频流宽（像素）。
         * @param {number} stats.video_layers_list[].height 视频流高（像素）。
         * @param {number} stats.video_layers_list[].received_frame_rate 接收到的帧率 (fps)。
         * @param {number} stats.video_layers_list[].render_frame_rate 视频渲染帧率。
         * @param {number} stats.video_layers_list[].decoder_frame_rate 视频解码帧率（fps）。
         * @param {number} stats.video_layers_list[].frozen_rate 用户的下行视频平均卡顿率（%）。
         * @param {number} stats.video_layers_list[].packet_loss_rate 下行丢包率（%）。
         * @param {number} stats.video_layers_list[].received_bitrate 接收到的码率（Kbps）。
         * @param {number} stats.video_layers_list[].total_frozen_time 用户的下行视频卡顿累计时长（ms）。
         * @param {String} stats.video_layers_list[].codec_name 视频编码器名字。
         */
        this.nertcEngine.onStatsObserver('onRemoteVideoStats', true, function (uc: number, stats: Array<NERtcVideoRecvStats>) {
            fire('onRemoteVideoStats', uc, stats);
        });

        /**
         * 通话中每个用户的网络上下行质量报告回调。
         * <pre>
         * 该回调描述每个用户在通话中的网络状态，每 2 秒触发一次，只上报状态有变更的成员。
         * </pre>
         * @event NERtcEngine#onNetworkQuality
         * @param {number} uc 数组的大小，即用户数。
         * @param {object} stats 每个用户 ID 和网络上下行质量信息的数组:
         * @param {number} stats[].uid 用户 ID，指定是哪个用户的视频流。
         * @param {number} stats[].tx_quality 该用户的上行网络质量:
         * <pre>
         * - 0 网络质量未知
         * - 1 网络质量极好
         * - 2 用户主观感觉和 excellent 差不多，但码率可能略低于 excellent
         * - 3 用户主观感受有瑕疵但不影响沟通
         * - 4 勉强能沟通但不顺畅
         * - 5 网络质量非常差，基本不能沟通
         * - 6 完全无法沟通
         * </pre>
         * @param {number} stats[].rx_quality 该用户的下行网络质量:
         * <pre>
         * - 0 网络质量未知
         * - 1 网络质量极好
         * - 2 用户主观感觉和 excellent 差不多，但码率可能略低于 excellent
         * - 3 用户主观感受有瑕疵但不影响沟通
         * - 4 勉强能沟通但不顺畅
         * - 5 网络质量非常差，基本不能沟通
         * - 6 完全无法沟通
         * </pre>
         */
        this.nertcEngine.onStatsObserver('onNetworkQuality', true, function (uc: number, stats: Array<NERtcNetworkQualityInfo>) {
            fire('onNetworkQuality', uc, stats);
        });

        /*****qs*****/
        /**
         * I 帧请求事件回调
         * @since V5.4.0
         * @event NERtcEngine#onRequestSendKeyFrame 
         * @param {number} type 视频通道类型
         */
        this.nertcEngine.onQsObserver('onRequestSendKeyFrame',true,  function (type: number) {
            fire('onRequestSendKeyFrame', type);
        });

        /**
         * 视频码率信息回调
         * @since V5.4.0
         * @event NERtcEngine#onBitrateUpdated 
         * @param {number} bps 码率
         * @param {number} type 视频通道类型
         */
        this.nertcEngine.onQsObserver('onBitrateUpdated', true, function (bps: number, type: number) {
            fire('onBitrateUpdated', bps, type);
        });

        /**
         * 视频码率信息回调
         * @since V5.4.0
         * @event NERtcEngine#onVideoCodecUpdated 
         * @param {number} codecType 视频编码器类型：
         * <pre>
         * - 3 H264 编解码器。
         * </pre>
         * @param {number} type 视频通道类型
         */
        this.nertcEngine.onQsObserver('onVideoCodecUpdated', true, function (codecType: number, type: number) {
            fire('onVideoCodecUpdated', codecType, type);
        });
    }


    // /**
    //  * Decide whether to use webgl/software/custom rendering.
    //  * @private
    //  * @ignore
    //  * @param {1|2|3} mode:
    //  * - 1 for old webgl rendering.
    //  * - 2 for software rendering.
    //  * - 3 for custom rendering.
    //  */
    // setRendererMode(mode: 1 | 2 | 3 = 1): void {
    //     this.renderMode = mode;
    // }

    // /**
    //  * Use this method to set custom Renderer when set renderMode in the 
    //  * {@link setRenderMode} method to 3.
    //  * CustomRender should be a class.
    //  * @param {IRenderer} customRenderer Customizes the video renderer.
    //  */
    // setCustomRenderer(customRenderer: IRenderer) {
    //     this.customRenderer = customRenderer;
    // }

    /*****test api*****/
    // /**
    //  * @private
    //  * @ignore
    //  * sdk test interface, external cannot be used
    //  * @returns {number} {boolean}
    //  */
    // setupLocalVideoCanvasTest(bind) {
    //     let canvas = {}
    //     canvas.mode = 0
    //     if(bind){
    //         canvas.view = document.getElementById('localView')
    //     }else {
    //         canvas.view = null
    //     }
    //     return this.setupLocalVideoCanvas(canvas)  
    // }
    // /**
    //  * @private
    //  * @ignore 
    //  * sdk test interface, external cannot be used
    //  * @returns {number} {boolean}
    //  */
    // setupRemoteVideoCanvasTest(uid, bind) {
    //     let canvas = {}
    //     canvas.mode = 0
    //     if(bind){
    //         canvas.view = document.getElementById('remoteView')
    //     }else {
    //         canvas.view = null
    //     }
    //     return this.setupRemoteVideoCanvas(uid, canvas)  
    // }
    // /**
    //  * @private
    //  * @ignore 
    //  * sdk test interface, external cannot be used
    //  * @returns {number} {boolean}
    //  */
    // setupRemoteSubVideoCanvasTest(uid, bind) {
    //     let canvas = {}
    //     canvas.mode = 0
    //     if(bind){
    //         canvas.view = document.getElementById('remoteSubView')
    //     }else {
    //         canvas.view = null
    //     }
    //     return this.setupRemoteSubStreamVideoCanvas(uid, canvas)  
    // }

    // /**
    //  * @private
    //  * @ignore
    //  * sdk test interface, external cannot be used
    //  * @returns {number}
    //  */
    //  appQuitTest() {
    //     this.emit('onAppQuit', 0, 'app quit');
    // }

    /**
     * @private
     * @ignore
     * check if WebGL will be available with appropriate features
     * @returns {number} {boolean}
     */
    _checkWebGL(): boolean {
        const canvas = document.createElement('canvas');
        let gl;

        canvas.width = 1;
        canvas.height = 1;

        const options = {
            // Turn off things we don't need
            alpha: false,
            depth: false,
            stencil: false,
            antialias: false,
            preferLowPowerToHighPerformance: true

            // Still dithering on whether to use this.
            // Recommend avoiding it, as it's overly conservative
            // failIfMajorPerformanceCaveat: true
        };

        try {
            gl =
                canvas.getContext('webgl', options) ||
                canvas.getContext('experimental-webgl', options);
        } catch (e) {
            return false;
        }
        if (gl) {
            return true;
        } else {
            return false;
        }
    }

    /**
     * @private
     * @ignore
     * @param {number} type 0-local 1-remote 2-localsubstream 3-remotesubstream
     * @param {number} uid uid get from native engine, differ from electron engine's uid
     */
    _getRenderer(type: number, uid: number): IRenderer | undefined {
        if (type === 0) {
            return this.renderers.get('local');
        } else if (type === 1) {
            return this.renderers.get(String(uid));
        } else if (type === 2) {
            return this.substreamRenderers.get('local');
        } else if (type === 3) {
            return this.substreamRenderers.get(String(uid));
        } else {
            console.warn('Invalid type for getRenderer, only accept 0~3.');
            return;
        }
    }

    /**
     * check if data is valid
     * @private
     * @ignore
     * @param {*} header
     * @param {*} ydata
     * @param {*} udata
     * @param {*} vdata
     */
    _checkData(
        header: ArrayBuffer,
        ydata: ArrayBuffer,
        udata: ArrayBuffer,
        vdata: ArrayBuffer
    ) {
        if (header.byteLength != 20) {
            console.error('invalid image header ' + header.byteLength);
            return false;
        }
        if (ydata.byteLength === 20) {
            console.error('invalid image yplane ' + ydata.byteLength);
            return false;
        }
        if (udata.byteLength === 20) {
            console.error('invalid image uplanedata ' + udata.byteLength);
            return false;
        }
        if (
            ydata.byteLength != udata.byteLength * 4 ||
            udata.byteLength != vdata.byteLength
        ) {
            console.error(
                'invalid image header ' +
                ydata.byteLength +
                ' ' +
                udata.byteLength +
                ' ' +
                vdata.byteLength
            );
            return false;
        }
        return true;
    }

    /**
     * register renderer for target info
     * @private
     * @ignore
     * @param {number} infos
     */
    doVideoFrameReceived(infos: any) {
        const len = infos.length;
        for (let i = 0; i < len; i++) {
            const info = infos[i];
            const { type, uid, channelId, header, ydata, udata, vdata } = info;
            if (!header || !ydata || !udata || !vdata) {
                console.log(
                    'Invalid data param ： ' +
                    header +
                    ' ' +
                    ydata +
                    ' ' +
                    udata +
                    ' ' +
                    vdata
                );
                continue;
            }
            let renderer = this._getRenderer(type, uid);
            if (!renderer) {
                console.warn(`Can't find renderer for uid : ${uid}`);
                continue;
            }

            if (this._checkData(header, ydata, udata, vdata)) {
                renderer.drawFrame({
                    header,
                    yUint8Array: ydata,
                    uUint8Array: udata,
                    vUint8Array: vdata
                });
            }
        }
    }

    // /**
    //  * Resizes the renderer.
    //  *
    //  * When the size of the view changes, this method refresh the zoom level so 
    //  * that video is sized appropriately while waiting for the next video frame 
    //  * to arrive.
    //  * 
    //  * Calling this method prevents a view discontinutity.
    //  * @param key Key for the map that store the renderers, 
    //  * e.g, uid or `local`.
    //  */
    // resizeRender(key: 'local' | 'substream' | number) {
    //     if (this.renderers.has(String(key))) {
    //         let renderer = this.renderers.get(String(key));
    //         (renderer as IRenderer).refreshCanvas();
    //     }
    // }

    /**
     * @private
     * @ignore
     * Initializes the renderer.
     * @param key Key for the map that store the renderers, 
     * e.g, uid or `local`
     * @param view The Dom elements to render the video.
     */
    initRender(key: 'local' | number, view: Element) {

        if (this.renderers.has(String(key))) {
            this.destroyRender(key);
        }
        let renderer: IRenderer;
        if (this.renderMode === 1) {
            renderer = new GlRenderer();
        } else if (this.renderMode === 2) {
            renderer = new SoftwareRenderer();
        } else if (this.renderMode === 3) {
            renderer = new this.customRenderer();
        } else {
            console.warn('Unknown render mode, fallback to 1');
            renderer = new GlRenderer();
        }
        renderer.bind(view);
        this.renderers.set(String(key), renderer);
    }

    /**
     * @private
     * @ignore
     * Initializes the renderer of substream.
     * @param key Key for the map that store the renderers, 
     * e.g, uid or `local`
     * @param view The Dom elements to render the substream video.
     */
    initSubStreamRender(key: 'local' | number, view: Element) {

        if (this.substreamRenderers.has(String(key))) {
            this.destroySubStreamRender(key);
        }
        let renderer: IRenderer;
        if (this.renderMode === 1) {
            renderer = new GlRenderer();
        } else if (this.renderMode === 2) {
            renderer = new SoftwareRenderer();
        } else if (this.renderMode === 3) {
            renderer = new this.customRenderer();
        } else {
            console.warn('Unknown render mode, fallback to 1');
            renderer = new GlRenderer();
        }
        renderer.bind(view);
        this.substreamRenderers.set(String(key), renderer);
    }

    captureRender(
        key: 'local' | number,
        streamType: NERtcVideoStreamType = NERtcVideoStreamType.kNERtcVideoStreamMain
      ): string {
          if (streamType === NERtcVideoStreamType.kNERtcVideoStreamMain) {
              if (!this.renderers.has(String(key))) {
                  return '';
              }
          } else {
              if (!this.substreamRenderers.has(String(key))) {
                  return '';
              }
          }
          let renderer: IRenderer | undefined;
          if (streamType === NERtcVideoStreamType.kNERtcVideoStreamMain) {
              renderer = this.renderers.get(String(key));
          } else {
              renderer = this.substreamRenderers.get(String(key));
          }
          try {
              return (renderer as IRenderer).captureImage()
          } catch (err) {
              console.error(`${err.stack}`)
              return '';
          }
      }

    /**
     * Destroys the renderer.
     * @private
     * @ignore
     * @param key Key for the map that store the renderers, 
     * e.g, `uid` or `local`.
     * @param onFailure The error callback for the {@link destroyRenderer} 
     * method.
     */
    destroyRender(
        key: 'local' | number, onFailure?: (err: Error) => void
    ) {
        if (!this.renderers.has(String(key))) {
            return;
        }
        let exception = null
        let renderer = this.renderers.get(String(key));
        try {
            (renderer as IRenderer).unbind();
            this.renderers.delete(String(key));
        } catch (err) {
            exception = err
            console.error(`${err.stack}`)
        }
        if (exception) {
            onFailure && onFailure(exception)
        }
    }

    /**
     * Destroys the renderer of substream.
     * @private
     * @ignore
     * @param key Key for the map that store the renderers, 
     * e.g, `uid` or `local`.
     * @param onFailure The error callback for the {@link destroySubStreamRender} 
     * method.
     */
    destroySubStreamRender(
        key: 'local' | number, onFailure?: (err: Error) => void
    ) {
        if (!this.substreamRenderers.has(String(key))) {
            return;
        }
        let exception = null
        let renderer = this.substreamRenderers.get(String(key));
        try {
            (renderer as IRenderer).unbind();
            this.substreamRenderers.delete(String(key));
        } catch (err) {
            exception = err
            console.error(`${err.stack}`)
        }
        if (exception) {
            onFailure && onFailure(exception)
        }
    }
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

    
    on(event: 'onApiCallExecuted', cb: (apiName: string, code: number, msg: string) => void): this;

    
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

    
    on(event: 'onUserSubStreamAudioStart', cb: (uid: number) => void): this;

    on(event: 'onUserSubStreamAudioStop', cb: (uid: number) => void): this;

    on(event: 'onUserSubStreamAudioMute', cb: (uid: number, numte: boolean) => void): this;


    /** 远端用户是否禁视频流回调。

     @param uid 远端用户ID。
     @param mute 是否禁视频流。
     */
    on(event: 'onUserVideoMute', cb: (uid: number, mute: boolean) => void): this;

    on(event: 'onUserVideoMuteEx', cb: (streamType: number, uid: number, mute: boolean) => void): this;


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
    on(event: 'onRecvSEIMsg', cb: (uid: number, data: ArrayBuffer) => void): this;

    /**
     * 屏幕共享暂停/恢复/开始/结束等回调
     */
    on(event: 'onScreenCaptureStatus', cb: (status: number) => void): this;

   /** 音频录制状态回调。
    * @param code 音频录制状态码。详细信息请参考 NERtcAudioRecordingCode。
    * @param file_path 音频录制文件保存路径。
    */
    on(event: 'onAudioRecording', cb: (code: number, file_path: String) => void): this;

   /** Occurs when the state of the media stream is relayed. 
    * @param state The state of the media stream.
    * @param channel_name The name of the destination room where the media streams are relayed. 
    */
    on(event: 'onMediaRelayStateChanged', cb: (state: number, channel_name: String) => void): this;

   /** Occurs when events related to media stream relay are triggered.
    * @param event The media stream relay event.
    * @param channel_name The name of the destination room where the media streams are relayed.
    * @param error  Specific error codes.
    */
    on(event: 'onMediaRelayEvent', cb: (event: number, channel_name: String, error: number) => void): this;

   /**
    * Occurs when the published local media stream falls back to an audio-only stream due to poor network conditions or switches back to audio and video stream after the network conditions improve.
    * If you call setLocalPublishFallbackOption and set option to #kNERtcStreamFallbackAudioOnly, this callback is triggered when the locally published stream falls back to audio-only mode due to poor uplink network conditions, or when the audio stream switches back to the audio and video stream after the uplink network conditions improve. 
    * @since V4.3.0
    * @param is_fallback  The locally published stream falls back to audio-only mode or switches back to audio and video stream.
    * - true: The published stream from a local client falls back to audio-only mode due to poor uplink network conditions.
    * - false: The audio stream switches back to the audio and video stream after the upstream network condition improves.
    * @param stream_type The type of the video stream, such as bigstream and substream. 
    */
    on(event: 'onLocalPublishFallbackToAudioOnly', cb: (is_fallback: boolean, stream_type: number) => void): this;

   /**
    * Occurs when the subscribed remote media stream falls back to an audio-only stream due to poor network conditions or switches back to the audio and video stream after the network condition improves.
    * If you call setLocalPublishFallbackOption and set option to #kNERtcStreamFallbackAudioOnly, this callback is triggered when the locally published stream falls back to audio-only mode due to poor uplink network conditions, or when the audio stream switches back to the audio and video stream after the uplink network condition improves.
    * @since V4.3.0
    * @param uid The ID of a remote user.
    * @param is_fallback The subscribed remote media stream falls back to audio-only mode or switches back to the audio and video stream. 
    * - true: The subscribed remote media stream falls back to audio-only mode due to poor downstream network conditions.
    * - false: The subscribed remote media stream switches back to the audio and video stream after the downstream network condition improves.
    * @param stream_type  The type of the video stream, such as bigstream and substream. 
    */
    on(event: 'onRemoteSubscribeFallbackToAudioOnly', cb: (uid: number, is_fallback: boolean, stream_type: number) => void): this;

}

export default NERtcEngine;