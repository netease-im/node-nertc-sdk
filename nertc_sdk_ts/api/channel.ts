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
  NERtcSize,
  CaptureSoureInfo,
  NERtcScreenCaptureSourceData
} from './defs'
import { EventEmitter } from 'events'

/**
 * @class NERtcChannel
 */
class NERtcChannel extends EventEmitter {
    channelName: string;
    rtcChannel: any;
    renderers: Map<string, IRenderer>;
    substreamRenderers: Map<string, IRenderer>;
    renderMode: 1 | 2 | 3;
    customRenderer: any;
  
    /**
     * NERtcChannel
     * @returns {NERtcChannel}
     */
    constructor(name: string, rtcChannel: any) {
        super();
        this.channelName = name;
        this.rtcChannel = rtcChannel;
        this.renderers = new Map();
        this.substreamRenderers = new Map();
        this.renderMode = this._checkWebGL() ? 1 : 2;
        this.customRenderer = CustomRenderer;
        this.initEventHandler();
    }
  
    /**
     * 释放资源。
     */
    release(): void {
      return this.rtcChannel.release();
    }

    /**
     * 释放资源。
     * @returns {string} 房间名
     */
    getChannelName(): string {
        return this.rtcChannel.getChannelName(this.channelName);
    }

    /**
     * 加入频道。如果频道还未创建，会自动尝试创建频道。
     * <pre>
     * 该方法让用户加入通话频道，在同一个频道内的用户可以互相通话，多个用户加入同一个频道，可以群聊。 使用不同 App Key 的 App 是不能互通的。如果已在通话中，用户必须调用 {@link NERtcChannel#leaveChannel} 退出当前通话，才能进入下一个频道。
     * 频道内每个用户的用户 ID 必须是唯一的。
     * </pre>
     * @param {String} token 动态秘钥。安全要求不高: 将值设为 空字符串。安全要求高: 将值设置为 Token。如果你已经启用了 App Certificate, 请务必使用 Token。
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    joinChannel(token: string): number {
        return this.rtcChannel.joinChannel(token);
    }

    /**
     * 加入频道。如果频道还未创建，会自动尝试创建频道。
     * <pre>
     * 该方法让用户加入通话频道，在同一个频道内的用户可以互相通话，多个用户加入同一个频道，可以群聊。 使用不同 App Key 的 App 是不能互通的。如果已在通话中，用户必须调用 {@link NERtcChannel#leaveChannel} 退出当前通话，才能进入下一个频道。
     * 频道内每个用户的用户 ID 必须是唯一的。
     * </pre>
     * @param {String} token 动态秘钥。安全要求不高: 将值设为 空字符串。安全要求高: 将值设置为 Token。如果你已经启用了 App Certificate, 请务必使用 Token。
     * @param {number} uid 用户ID。
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    joinChannelWithUid(token: string, uid: number): number {
        return this.rtcChannel.joinChannelWithUid(token, uid);
    }

    /**
     * 加入频道。如果频道还未创建，会自动尝试创建频道。
     * @since V5.4.0
     * <pre>
     * 该方法让用户加入通话频道，在同一个频道内的用户可以互相通话，多个用户加入同一个频道，可以群聊。 使用不同 App Key 的 App 是不能互通的。如果已在通话中，用户必须调用 {@link NERtcChannel#leaveChannel} 退出当前通话，才能进入下一个频道。
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
    joinChannelWithOptions(token: string, uid: number, channelOptions: NERtcJoinChannelOptions): number {
        return this.rtcChannel.joinChannelWithOptions(token, uid, channelOptions);
    }

    /**
     * 离开频道。
     * <pre>
     * 离开频道，即挂断或退出通话。
     * 当调用 {@link NERtcChannel#joinChannel} 方法后，必须调用 {@link NERtcChannel#leaveChannel} 结束通话，否则无法开始下一次通话。 不管当前是否在通话中，都可以调用 leaveChannel，没有副作用。该方法会把会话相关的所有资源释放掉。
     * 该方法是异步操作，调用返回时并没有真正退出频道。在真正退出频道后，SDK 会触发 onLeaveChannel 事件。
     * 如果你调用了 {@link NERtcChannel#leaveChannel} 后立即调用 {@link NERtcChannel#release} , SDK 将无法触发 onLeaveChannel 事件。
     * </pre>
     * @fires NERtcChannel#onLeaveChannel
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    leaveChannel(): number {
        return this.rtcChannel.leaveChannel();
    }

    /**
     * 开启或关闭本地语音采集或处理
     * <pre>
     * 该方法可以重新开启本地语音功能，重新开始本地音频采集及处理。
     * 该方法不影响接收或播放远端音频流。
     * <b>NOTE:</b>
     * - 该方法与 {@link NERtcChannel#muteLocalAudioStream} 的区别在于:
     * - {@link NERtcChannel#enableLocalAudio}: 开启本地语音采集及处理
     * - {@link NERtcChannel#muteLocalAudioStream}: 停止或继续发送本地音频流
     * 该方法设置内部引擎为启用状态，在 {@link NERtcChannel#leaveChannel} 后仍然有效。
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
    enableLocalAudio(enabled: boolean): number {
        return this.rtcChannel.enableLocalAudio(enabled);
    }

    /**
     * 开启或关闭音频辅流。
     * @since V5.4.0
     * <pre>
     * 开启时远端会收到onUserSubStreamAudioStart，关闭时远端会收到onUserSubStreamAudioStop。
     * <b>NOTE:</b>
     * - 该方法设置内部引擎为启用状态，在{@link NERtcChannel#leaveChannel}后仍然有效。
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
        return this.rtcChannel.enableLocalSubStreamAudio(enabled);
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
    muteLocalAudioStream(mute: boolean): number {
        return this.rtcChannel.muteLocalAudioStream(mute);
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
    muteLocalSubStreamAudio(mute: boolean): number {
        return this.rtcChannel.muteLocalSubStreamAudio(mute);
    }

    /**
     * 开启或关闭本地视频采集和渲染
     * <pre>
     * 该方法启用本地视频采集功能。
     * 该方法设置内部引擎为启用状态，在 {@link NERtcChannel#leaveChannel} 后仍然有效。
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
    enableLocalVideo(enabled: boolean): number {
        return this.rtcChannel.enableLocalVideo(enabled);
    }
  
    /**
     * 开启或关闭本地视频采集和渲染
     * @since V5.4.0
     * <pre>
     * 该方法启用本地视频采集功能。
     * 该方法设置内部引擎为启用状态，在 {@link NERtcChannel#leaveChannel} 后仍然有效。
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
    enableLocalVideoWithType(type: number, enabled: boolean): number {
        return this.rtcChannel.enableLocalVideoWithType(type, enabled);
    }

    /**
     * 开关本地视频发送。
     * <pre>
     * 调用该方法禁视频流时，SDK 不再发送本地视频流，但摄像头仍然处于工作状态。相比于 {@link NERtcChannel#enableLocalVideo} (false) 用于控制本地视频流发送的方法，该方法响应速度更快。该方法不影响本地视频流获取，没有禁用摄像头。
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
    muteLocalVideoStream(enabled: boolean): number {
        return this.rtcChannel.muteLocalVideoStream(enabled);
    }

    /**
     * 开关本地视频发送。
     * <pre>
     * 调用该方法禁视频流时，SDK 不再发送本地视频流，但摄像头仍然处于工作状态。相比于 {@link NERtcChannel#enableLocalVideo} (false) 用于控制本地视频流发送的方法，该方法响应速度更快。该方法不影响本地视频流获取，没有禁用摄像头。
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
    muteLocalVideoStreamWithType(type: number, enabled: boolean): number {
        return this.rtcChannel.muteLocalVideoStreamWithType(type, enabled);
    }

    /**
     * 启用说话者音量提示。该方法允许 SDK 定期向 App 反馈当前谁在说话以及说话者的音量。
     * <pre>
     * 启用该方法后，无论频道内是否有人说话，可以通过{@link NERtcChannel#on}方法监听 onRemoteAudioVolumeIndication，根据设置的间隔时间返回音量提示事件。
     * </pre>
     * @param {boolean} enable 是否启用说话者音量提示。
     * @param {number} interval 指定音量提示的时间间隔，单位为毫秒。必须设置为 100 毫秒的整数倍值。
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    enableAudioVolumeIndication(enabled: boolean, interval: number, enableVad: boolean): number {
        return this.rtcChannel.enableAudioVolumeIndication(enabled, interval, enableVad);
    }

    /**
     * 枚举屏幕分享源信息。
     * @since 5.5.20
     * @param {Object} thumbSize 屏幕或窗口的缩略图的目标尺寸（宽高单位为像素）:
     * @param {number} thumbSize.width 缩略图缩略图宽度px默认0
     * @param {number} thumbSize.height 缩略图高度px默认0
     * <pre>
     * <b>NOTE:</b>
     * - SDK 会在保证原图不变形的前提下，缩放原图，使图片最长边和目标尺寸的最长边的长度一致。
     * - 如果目标尺寸大于原图尺寸，缩略图即为原图，SDK 不进行缩放操作。
     * </pre>
     * @param {Object} iconSize 程序所对应的图标的目标尺寸px。
     * @param {number} iconSize.width 程序所对应的图标宽度px默认0
     * @param {number} iconSize.height 程序所对应的图标高度px默认0
     * <b>NOTE:</b>
     * - SDK 会在保证原图不变形的前提下，缩放原图，使图片最长边和目标尺寸的最长边的长度一致。
     * - 如果目标尺寸大于原图尺寸，缩略图即为原图，SDK 不进行缩放操作。
     * </pre>
     * @param includeScreen 除了窗口信息外，SDK 是否还返回屏幕信息：
     * <pre>
     * - true: SDK 返回屏幕和窗口信息；
     * - false: SDK 仅返回窗口信息。
     * </pre>
     * @returns {Object[]}
     * <pre>
     * - Object[] : 调用成功；
     * <table style="width:100%;">
     * <tr><td>Object.type</td><td>int</td><td>信息源类型:-1-未知 0-窗口 1-屏幕 2-自定义</td></tr>
     * <tr><td>Object.source_id</td><td>number</td><td>信息源ID</td></tr>
     * <tr><td>Object.source_name</td><td>String</td><td>信息源名称</td></tr>
     * <tr><td>Object.thumb_image</td><td>object</td><td>缩略图信息,使用前需要判断是否undefined:
     * - buffer - BGRA二进制数据
     * - length - 数据大小 byte
     * - width - 图片宽度 px
     * - height - 图片高度 px</td></tr>
     * <tr><td>Object.icon_image</td><td>object</td><td>图标信息,使用前需要判断是否undefined:
     * - buffer - BGRA二进制数据
     * - length - 数据大小 byte
     * - width - 图片宽度 px
     * - height - 图片高度 px</td></tr>
     * </table>
     * - NULL: 调用失败。
     * </pre>
     */
    getScreenCaptureSources(thumbSize: NERtcSize, iconSize: NERtcSize, includeScreen: boolean): Array<Object> {
        return this.rtcChannel.getScreenCaptureSources(thumbSize, iconSize, includeScreen);
    }

    /**
     * 枚举屏幕分享源信息。
     * @since 5.5.20
     * @param {CaptureSoureInfo} source 屏幕或窗口的缩略图的目标尺寸（宽高单位为像素）:
     * @param {number} source.type 信息源类型:-1-未知 0-窗口 1-屏幕 2-自定义
     * @param {number} source.source_id 信息源ID
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
     */
    setScreenCaptureSource(source: CaptureSoureInfo, regionRect: NERtcRectangle, capture_params: NERtcScreenCaptureParameters): number {
        return this.rtcChannel.setScreenCaptureSource(source, regionRect, capture_params);
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
     * @param {boolean} param.window_focus 调用 {@link NERtcChannel#startScreenCaptureByWindowId} 方法共享窗口时，是否将该窗口前置
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
        return this.rtcChannel.startScreenCaptureByScreenRect(screenRect, regionRect, param);
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
     * @param {boolean} param.window_focus 调用 {@link NERtcChannel#startScreenCaptureByWindowId} 方法共享窗口时，是否将该窗口前置
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
     * - -100: 需要先调用 {@link NERtcChannel#enumerateScreenCaptureSourceInfo} 缓存桌面信息
     * - 其他: 方法调用失败。
     * </pre>
     */
    startScreenCaptureByDisplayId(displayId: string, regionRect: NERtcRectangle, param: NERtcScreenCaptureParameters): number {
        return this.rtcChannel.startScreenCaptureByDisplayId(displayId, regionRect, param);
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
     * @param {boolean} param.window_focus 调用 {@link NERtcChannel#startScreenCaptureByWindowId} 方法共享窗口时，是否将该窗口前置
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
    startScreenCaptureByWindowId(windowid: string, regionRect: NERtcRectangle, param: NERtcScreenCaptureParameters): number {
        return this.rtcChannel.startScreenCaptureByWindowId(windowid, regionRect, param);
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
     * @param {boolean} param.window_focus 调用 {@link NERtcChannel#startScreenCaptureByWindowId} 方法共享窗口时，是否将该窗口前置
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
        return this.rtcChannel.updateScreenCaptureRegion(regionRect);
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
    setScreenCaptureMouseCursor(): number {
        return this.rtcChannel.setScreenCaptureMouseCursor();
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
        return this.rtcChannel.stopScreenCapture();
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
        return this.rtcChannel.pauseScreenCapture();
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
        return this.rtcChannel.resumeScreenCapture();
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
    setExcludeWindowList(param: NERtcScreenCaptureWindowParam): number {
        return this.rtcChannel.setExcludeWindowList(param);
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
    updateScreenCaptureParameters(captureParams: NERtcScreenCaptureWindowParam): number {
        return this.rtcChannel.updateScreenCaptureParameters(captureParams);
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
            return this.rtcChannel.setupVideoCanvas(0, true);
        }
        else {
            //unbind
            this.destroyRender('local');
            return this.rtcChannel.setupVideoCanvas(0, false);
        }
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
            return this.rtcChannel.setupSubStreamVideoCanvas(0, true);
        }
        else {
            //unbind
            this.destroySubStreamRender('local');
            return this.rtcChannel.setupSubStreamVideoCanvas(0, false);
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
    setLocalVideoMirrorMode(mode: NERtcVideoMirrorMode): number {
        return this.rtcChannel.setLocalVideoMirrorMode(mode);
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
    setLocalVideoMirrorModeWithType(type: number, mode: NERtcVideoMirrorMode): number {
        return this.rtcChannel.setLocalVideoMirrorModeWithType(type, mode);
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
            return this.rtcChannel.setupVideoCanvas(uid, true);
        }
        else {
            //unbind
            this.destroyRender(uid);
            return this.rtcChannel.setupVideoCanvas(uid, false);
        }
    }

    /**
     * 设置远端用户辅流视图。
     * <pre>
     * 该方法绑定远端用户和辅流显示视图，即设定 uid 指定的用户用哪个视图显示。调用该接口时需要指定远端视频的 uid，一般可以在用户加入后设置好。
     * <b>NODE:</b>
     * - 如果 App 不能事先知道对方的 uid，可以通过{@link NERtcChannel#on}方法监听 onUserJoined 事件时设置。
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
            return this.rtcChannel.setupSubStreamVideoCanvas(uid, true);
        }
        else {
            //unbind
            this.destroySubStreamRender(uid);
            return this.rtcChannel.setupSubStreamVideoCanvas(uid, false);
        }
    }

    /**
     * 设置参会者角色
     * <pre>
     * {@link NERtcChannel#setClientRole} 接口用于在直播场景中设置用户角色。默认情况下用户以主播角色加入房间。
     * 在加入房间前，用户需要调用setClientRole 接口设置本端模式为观众或主播模式。在加入房间后，用户可以通过本接口切换用户模式。
     * 用户角色支持设置为主播或观众，主播和观众的权限不同。默认情况下用户以主播角色加入房间。
     * 可以在通话前后设置，通话前设置会在加入频道之后生效。如果设置观众模式。则会停止音视频设备。
     * </pre>
     * @param {number} role 参会者模式
     * <pre>
     * - 主播(0)：可以开关摄像头等设备、可以发布流、可以操作互动直播推流相关接口、上下线对其他房间内用户可见
     * - 观众(1)：不可以开关摄像头等设备、不可以发布流、不可以操作互动直播推流相关接口、上下线对其他房间内用户不可见
     * </pre>
     * @fires NERtcChannel#onClientRoleChanged
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    setClientRole(role: NERtcClientRole): number {
        return this.rtcChannel.setClientRole(role);
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
    setLocalMediaPriority(priority: NERtcMediaPriorityType, preemptive:boolean): number {
        return this.rtcChannel.setLocalMediaPriority(priority, preemptive);
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
        return this.rtcChannel.getConnectionState();
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
    setCameraCaptureConfig(config: any): number {
        return this.rtcChannel.setCameraCaptureConfig(config);
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
    setCameraCaptureConfigWithType(type: number, config: any): number {
        return this.rtcChannel.setCameraCaptureConfigWithType(type, config);
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
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    setVideoConfig(config: NERtcVideoConfig): number {
        return this.rtcChannel.setVideoConfig(config);
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
    setVideoConfigWithType(type: number, config: NERtcVideoConfig): number {
        return this.rtcChannel.setVideoConfigWithType(type, config);
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
    enableDualStreamMode(enabled: boolean): number {
        return this.rtcChannel.enableDualStreamMode(enabled);
    }

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
    subscribeRemoteAudioStream(uid: number, enabled: boolean): number {
        return this.rtcChannel.subscribeRemoteAudioStream(uid, enabled);
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
    subscribeRemoteSubStreamAudio(uid: number, enabled: boolean): number {
        return this.rtcChannel.subscribeRemoteSubStreamAudio(uid, enabled);
    }

    /**
     * 取消或恢复订阅所有远端用户的音频主流。
     * @since V5.4.0
     * <pre>
     * 加入房间时，默认订阅所有远端用户的音频主流。
     * <b>NOTE:</b>
     * - 设置该方法的 subscribe 参数为 true 后，对后续加入房间的用户同样生效。
     * - 在开启自动订阅（默认）时，设置该方法的 subscribe 参数为 false 可以实现取消订阅所有远端用户的音频流，但此时无法再调用{@link NERtcChannel#subscribeRemoteAudioStream}方法单独订阅指定远端用户的音频流。
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
        return this.rtcChannel.subscribeAllRemoteAudioStream(subscribe);
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
    setAudioSubscribeOnlyBy(subscribe: boolean): number {
        return this.rtcChannel.setAudioSubscribeOnlyBy(subscribe);
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
    subscribeRemoteVideoStream(uid: number, type: number, subscribe: boolean) {
        return this.rtcChannel.subscribeRemoteVideoStream(uid, type, subscribe);
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
    subscribeRemoteVideoSubStream(uid: number, sub: boolean) {
        return this.rtcChannel.subscribeRemoteVideoSubStream(uid, sub);
    }

    /**
     * 添加房间推流任务，成功添加后当前用户可以收到该直播流的状态通知。通话中有效。
     * @fires NERtcChannel#onAddLiveStreamTask
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
        return this.rtcChannel.addLiveStreamTask(info);
    }

    /**
     * 更新修改房间推流任务。通话中有效。
     * @fires NERtcChannel#onUpdateLiveStreamTask
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
    updateLiveStreamTask(info: NERtcLiveStreamTaskInfo): number {
        return this.rtcChannel.updateLiveStreamTask(info);
    }

    /**
     * 删除房间推流任务。通话中有效。
     * @fires NERtcChannel#onRemoveLiveStreamTask
     * @param {String} taskId  直播任务id
     * @returns {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    removeLiveStreamTask(taskId: String): number {
        return this.rtcChannel.removeLiveStreamTask(taskId);
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
     * @return {number}
     * <pre>
     * - 0: 方法调用成功；
     * - 其他: 方法调用失败。
     * </pre>
     */
    sendSEIMsg(data: ArrayBuffer): number {
        return this.rtcChannel.sendSEIMsg(data);
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
        return this.rtcChannel.sendSEIMsgWithType(data, type);
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
        return this.rtcChannel.adjustUserPlaybackSignalVolume(uid, volume)
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
        return this.rtcChannel.adjustChannelPlaybackSignalVolume(volume);
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
        return this.rtcChannel.startChannelMediaRelay(config);
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
        return this.rtcChannel.updateChannelMediaRelay(config);
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
        return this.rtcChannel.stopChannelMediaRelay();
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
    setLocalPublishFallbackOption(option: number): number {
        return this.rtcChannel.setLocalPublishFallbackOption(option);
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
    setRemoteSubscribeFallbackOption(option: number): number {
        return this.rtcChannel.setRemoteSubscribeFallbackOption(option);
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
    setRemoteHighPriorityAudioStream(enabled: boolean, uid: number): number {
        return this.rtcChannel.setRemoteHighPriorityAudioStream(enabled, uid);
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
    enableMediaPub(enabled: boolean, type: number): number {
        return this.rtcChannel.enableMediaPub(enabled, type);
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
        return this.rtcChannel.updatePermissionKey(key);
    }
  
    /**
     * 你可以调用该方法指定只订阅的音频流。
     * <pre>
     *  - 此接口需要在加入房间成功后调用。
     *  - 对于调用接口时不在房间的 uid 不生效。
     * </pre>
     * @param {Array<Number>} uids 只订阅此用户uid列表 的音频.此列表为全量列表。如果列表为空或 null，取消订阅白名单。例如：[uid1,uid2...]。
     * @return {number}
     * <pre>
     * - 0: 方法调用成功。
     * - 其他：方法调用失败。
     * </pre>
     */
    setSubscribeAudioAllowlist(uids: Array<Number>, size: number): number {
        return this.rtcChannel.setSubscribeAudioAllowlist(uids, size);
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
     * @return {number}
     * <pre>
     * - 0: 方法调用成功。
     * - 其他：方法调用失败。
     * </pre>
     */
    setSubscribeAudioBlocklist(audioStreamType: number, uids: Array<Number>, size: number): number {
        return this.rtcChannel.setSubscribeAudioBlocklist(audioStreamType, uids, size);
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
        return this.rtcChannel.setRangeAudioMode(mode);
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
        return this.rtcChannel.setRangeAudioTeamID(team_id);
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
        return this.rtcChannel.setAudioRecvRange(audible_distance, conversational_distance, roll_off);
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
        return this.rtcChannel.updateSelfPosition(info);
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
        return this.rtcChannel.enableSpatializerRoomEffects(enable);
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
        return this.rtcChannel.updateSelfPosition(config);
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
        return this.rtcChannel.setSpatializerRenderMode(mode);
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
        return this.rtcChannel.initSpatializer();
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
        return this.rtcChannel.enableSpatializer(enable, apply_to_team);
    }
  
    initEventHandler() {
        const self = this;
        const fire = (event: string, ...args: Array<any>) => {
            setImmediate(() => {
                this.emit(event, ...args);
            });
        };
        
        /**
         * 发生错误回调。
         * @event NERtcChannel#onError
         * @param {NERtcErrorCode} errorCode 错误码
         * @param {string} msg 错误描述
         */
        this.rtcChannel.onEvent('onError', function (
            errorCode: number,
            msg: string
        ) {
            fire('onError', errorCode, msg);
        });
  
        /**
         * 发生警告回调。
         * @event NERtcChannel#onWarning
         * @param {NERtcErrorCode} warnCode 错误码
         * @param {string} msg 错误描述
         */
        this.rtcChannel.onEvent('onWarning', function (
            warnCode: number,
            msg: string
        ) {
            fire('onWarning', warnCode, msg);
        });
  
         /**
         * API调用结束回调。
         * @since V5.4.0
         * @event NERtcChannel#onApiCallExecuted
         * @param {string} apiName API名称
         * @param {NERtcErrorCode} code API执行结果错误码
         * @param {string} msg API执行结果描述
         */
        this.rtcChannel.onEvent('onApiCallExecuted', function (
          apiName: string, 
          code: number, 
          msg: string
        ) {
            fire('onApiCallExecuted', apiName, code, msg);
        });
  
        /**
         * 加入频道回调
         * @event NERtcChannel#onJoinChannel
         * @param {number} cid 频道 ID
         * @param {number} uid 用户 ID
         * @param {NERtcErrorCode} result 错误码
         * @param {number} elapsed 从 joinChannel 开始到发生此事件过去的时间（毫秒）
         */
        this.rtcChannel.onEvent('onJoinChannel', function (
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
          * @event NERtcChannel#onReconnectingStart
          * @param cid  频道 ID。
          * @param uid  用户 ID。
          */
        this.rtcChannel.onEvent('onReconnectingStart', function (
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
          * @event NERtcChannel#onConnectionStateChange
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
        this.rtcChannel.onEvent('onConnectionStateChange', function (
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
         * @event NERtcChannel#onRejoinChannel
         * @param {number} cid 频道 ID
         * @param {number} uid 用户 ID
         * @param {NERtcErrorCode} result 错误码
         * @param {number} elapsed 从 joinChannel 开始到发生此事件过去的时间（毫秒）
         */
        this.rtcChannel.onEvent('onRejoinChannel', function (
            cid: number,
            uid: number,
            result: NERtcErrorCode,
            elapsed: number
        ) {
            fire('onRejoinChannel', cid, uid, result, elapsed);
        });
  
        /**
         * 离开频道回调。
         * App 调用 {@link NERtcChannel#leaveChannel} 方法时，SDK提示 App 离开频道是否成功。
         * @event NERtcChannel#onLeaveChannel
         * @param {NERtcErrorCode} result 错误码
         */
        this.rtcChannel.onEvent('onLeaveChannel', function (
            result: NERtcErrorCode
        ) {
            fire('onLeaveChannel', result);
        });
  
        /**
         * 掉线回调。
         * <pre>
         * 由于非网络原因，客户端可能会和服务器失去连接，此时SDK无需自动重连，直接触发此回调方法。
         * </pre>
         * @event NERtcChannel#onDisconnect
         * @param {NERtcErrorCode} result 错误码
         */
        this.rtcChannel.onEvent('onDisconnect', function (
            result: NERtcErrorCode
        ) {
            fire('onDisconnect', result);
        });
  
        /** 参会者角色类型变更回调。
         * <pre>
         * 本地用户加入房间后，通过 {@link NERtcChannel#setClientRole} 切换用户角色后会触发此回调。例如从主播切换为观众、从观众切换为主播。
         * <b>NOTE:</b>
         * 直播场景下，如果您在加入房间后调用该方法切换用户角色，调用成功后，会触发以下回调：
         * - 主播切观众，本端触发onClientRoleChanged回调，远端触发 onUserLeft 回调。
         * - 观众切主播，本端触发onClientRoleChanged回调，远端触发 onUserJoined 回调。
         * @event NERtcChannel#onClientRoleChanged
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
        this.rtcChannel.onEvent('onClientRoleChanged', function (
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
         * @event NERtcChannel#onUserJoined
         * @param {number} uid 新加入频道的远端用户ID。
         * @param {string} userName 新加入频道的远端用户名(无效)。
         */
        this.rtcChannel.onEvent('onUserJoined', function (
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
         * @event NERtcChannel#onUserJoinedEx
         * @param {number} uid 新加入频道的远端用户ID。
         * @param {string} userName 新加入频道的远端用户名(无效)。
         * @param {object} extra_info 一些可选信息:
         * @param {string} extra_info.custom_info 自定义信息，来源于远端用户joinChannel时填的 {@link NERtcJoinChannelOptions#custom_info}参数，默认为空字符串。
         */
        this.rtcChannel.onEvent('onUserJoinedWithExtraInfo', function (
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
         * @event NERtcChannel#onUserLeft
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
        this.rtcChannel.onEvent('onUserLeft', function (
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
         * @event NERtcChannel#onUserLeftEx
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
        this.rtcChannel.onEvent('onUserLeftWithExtraInfo', function (
            uid: number, 
            reason: number, 
            extra_info: any) {
            fire('onUserLeftWithExtraInfo', uid, reason, extra_info);
        });

        /**
         * 远端用户开启音频回调。
         * @event NERtcChannel#onUserAudioStart
         * @param {number} uid 远端用户ID。
         */
        this.rtcChannel.onEvent('onUserAudioStart', function (
            uid: number
        ) {
            fire('onUserAudioStart', uid);
        });
  
        /**
         * 远端用户停用音频回调。
         * @event NERtcChannel#onUserAudioStop
         * @param {number} uid 远端用户ID。
         */
        this.rtcChannel.onEvent('onUserAudioStop', function (
            uid: number
        ) {
            fire('onUserAudioStop', uid);
        });
  
        /**
         * 远端用户是否静音回调。
         * @event NERtcChannel#onUserAudioMute
         * @param {number} uid 远端用户ID。
         * @param {boolean} mute 是否静音。
         */
        this.rtcChannel.onEvent('onUserAudioMute', function (
            uid: number,
            mute: boolean
        ) {
            fire('onUserAudioMute', uid, mute);
        });
        
         /**
         * 远端用户开启音频辅流回调。
         * @since V5.4.0
         * @event NERtcChannel#onUserSubStreamAudioStart 
         * @param {number} uid 远端用户的 ID。
         */
        this.rtcChannel.onEvent('onUserSubStreamAudioStart', function (uid: number) {
            fire('onUserSubStreamAudioStart', uid);
        });
  
        /**
         * 远端用户停用音频辅流回调。
         * @since V5.4.0
         * @event NERtcChannel#onUserSubStreamAudioStop 
         * @param {number} uid 远端用户的 ID。
         */
        this.rtcChannel.onEvent('onUserSubStreamAudioStop', function (uid: number) {
            fire('onUserSubStreamAudioStop', uid);
        });
  
         /**
         * 远端用户是否静音的回调。
         * @since V5.4.0
         * @event NERtcChannel#onUserSubStreamAudioMute 
         * @param {number} uid 远端用户的 ID。
         * </pre>
         */
        this.rtcChannel.onEvent('onUserSubStreamAudioMute', function (uid: number, mute: boolean) {
            fire('onUserSubStreamAudioMute', uid, mute);
        });
  
        /**
         * 远端用户开启视频回调。
         * @event NERtcChannel#onUserVideoStart
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
        this.rtcChannel.onEvent('onUserVideoStart', function (
            uid: number,
            maxProfile: NERtcVideoProfileType
        ) {
            fire('onUserVideoStart', uid, maxProfile);
        });
  
        /**
         * 远端用户停用视频回调。
         * @event NERtcChannel#onUserVideoStop
         * @param {number} uid 远端用户ID。
         */
        this.rtcChannel.onEvent('onUserVideoStop', function (
            uid: number
        ) {
            fire('onUserVideoStop', uid);
        });
  
        /**
         * 远端用户是否禁视频流回调。
         * @event NERtcChannel#onUserVideoMute
         * @param {number} uid 远端用户ID。
         * @param {boolean} mute 是否禁视频流。
         */
        this.rtcChannel.onEvent('onUserVideoMute', function (
          uid: number, 
          mute: boolean
        ) {
            fire('onUserVideoMute', uid, mute);
        });
  
        /**
         * 远端用户是否禁视频流回调。
         * @event NERtcChannel#onUserVideoMuteEx
         * @param {number} uid 远端用户ID。
         * @param {boolean} mute 是否禁视频流。
         */
        this.rtcChannel.onEvent('onUserVideoMuteWithType', function (
          streamType: number, 
          uid: number, 
          mute: boolean
        ) {
            fire('onUserVideoMuteWithType', streamType, uid, mute);
        });
  
        /**
         * 远端用户开启辅流视频回调。
         * @event NERtcChannel#onUserSubStreamVideoStart
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
        this.rtcChannel.onEvent('onUserSubStreamVideoStart', function (
            uid: number,
            max_profile: NERtcVideoProfileType
        ) {
            fire('onUserSubStreamVideoStart', uid, max_profile);
        });
  
        /**
         * 远端用户停用辅流视频回调。
         * @event NERtcChannel#onUserSubStreamVideoStop
         * @param {number} uid 远端用户ID。
         */
        this.rtcChannel.onEvent('onUserSubStreamVideoStop', function (
            uid: number
        ) {
            fire('onUserSubStreamVideoStop', uid);
        });
  
        /**
         * 屏幕共享暂停/恢复/开始/结束等回调
         * @event NERtcChannel#onScreenCaptureStatusonScreenCaptureStatus
         * @param {number} status 屏幕共享状态。
         * <pre>
         * - 1 开始屏幕共享。
         * - 2 暂停屏幕共享。
         * - 3 恢复屏幕共享。
         * - 4 停止屏幕共享。
         * - 5 屏幕分享的目标窗口被覆盖。
         * </pre>
         */
        this.rtcChannel.onEvent('onScreenCaptureStatus', function (status: number) {
            fire('onScreenCaptureStatus', status);
        });
  
        /**
         * 已接收到远端音频首帧回调。
         * @event NERtcChannel#onFirstAudioDataReceived
         * @param {number} uid 发送音频帧的远端用户的用户 ID。
         */
        this.rtcChannel.onEvent('onFirstAudioDataReceived', function (
            uid: number
        ) {
            fire('onFirstAudioDataReceived', uid);
        });
  
        /**
         * 已显示首帧远端视频回调。
         * <pre>
         * 第一帧远端视频显示在视图上时，触发此调用。
         * </pre>
         * @event NERtcChannel#onFirstVideoDataReceived
         * @param {number} uid 用户 ID，指定是哪个用户的视频流。
         */
        this.rtcChannel.onEvent('onFirstVideoDataReceived', function (
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
         * @event NERtcChannel#onFirstVideoDataReceivedEx
         * @param {number} streamType 视频流类型
         * <pre>
         * - 0 主流
         * - 1 辅流
         * </pre>
         * @param {number} uid 用户 ID，指定是哪个用户的视频流。
         */
        this.rtcChannel.onEvent('onFirstVideoDataReceivedWithType', function (
          type: number, 
          uid: number
        ) {
            fire('onFirstVideoDataReceivedWithType', type, uid);
        });
  
        /**
         * 已解码远端音频首帧的回调。
         * @event NERtcChannel#onFirstAudioFrameDecoded
         * @param {number} uid 远端用户 ID。
         */
        this.rtcChannel.onEvent('onFirstAudioFrameDecoded', function (
            uid: number
        ) {
            fire('onFirstAudioFrameDecoded', uid);
        });
  
        /**
         * 已显示首帧远端视频回调。
         * <pre>
         * 引擎收到第一帧远端视频流并解码成功时，触发此调用。 App 可在此回调中设置该用户的 video canvas。
         * </pre>
         * @event NERtcChannel#onFirstVideoFrameDecoded
         * @param {number} uid 用户 ID，指定是哪个用户的视频流。
         * @param {number} width 视频流宽（px）。
         * @param {number} height 视频流高（px）。
         */
        this.rtcChannel.onEvent('onFirstVideoFrameDecoded', function (
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
         * @event NERtcChannel#onFirstVideoFrameDecodedEx
         * @param {number} streamType 视频流类型
         * <pre>
         * - 0 主流
         * - 1 辅流
         * </pre>
         * @param {number} uid 用户 ID，指定是哪个用户的视频流。
         * @param {number} width 视频流宽（px）。
         * @param {number} height 视频流高（px）。
         */
        this.rtcChannel.onEvent('onFirstVideoFrameDecodedWithType', function (
            type: number, 
            uid: number, 
            width: number, 
            height: number
        ) {
            fire('onFirstVideoFrameDecodedWithType', type, uid, width, height);
        });
  
        /**
         * 提示频道内本地用户瞬时音量的回调。
         * <pre>
         * 该回调默认禁用。可以通过 {@link NERtcChannel#enableAudioVolumeIndication} 方法开启；
         * 开启后，本地用户说话，SDK 会按 {@link NERtcChannel#enableAudioVolumeIndication} 方法中设置的时间间隔触发该回调。
         * 如果本地用户将自己静音（调用了 {@link NERtcChannel#muteLocalAudioStream}），SDK 将音量设置为 0 后回调给应用层。
         * </pre>
         * @event NERtcChannel#onLocalAudioVolumeIndication
         * @param {number} volume （混音后的）音量，取值范围为 [0,100]。
         */
        this.rtcChannel.onEvent('onLocalAudioVolumeIndication', function (
            volume: number
        ) {
            fire('onLocalAudioVolumeIndication', volume);
        });

        /**
         * 提示频道内本地用户瞬时音量的回调。
         * @since V5.4.0
         * <pre>
         * 该回调默认禁用。可以通过 {@link NERtcChannel#enableAudioVolumeIndication} 方法开启；
         * 开启后，本地用户说话，SDK 会按 {@link NERtcChannel#enableAudioVolumeIndication} 方法中设置的时间间隔触发该回调。
         * 如果本地用户将自己静音（调用了 {@link NERtcChannel#muteLocalAudioStream}），SDK 将音量设置为 0 后回调给应用层。
         * </pre>
         * @event NERtcChannel#onLocalAudioVolumeIndicationEx
         * @param {number} volume （混音后的）音量，取值范围为 [0,100]。
         * @param {number} enable_vad 是否检测到人声。
         */
        this.rtcChannel.onEvent('onLocalAudioVolumeIndicationEx', function (
            volume: number, 
            enable_vad: boolean
        ) {
            fire('onLocalAudioVolumeIndicationEx', volume, enable_vad);
        });
  
        /**
         * 提示频道内谁正在说话及说话者瞬时音量的回调。
         * <pre>
         * 该回调默认禁用。可以通过 {@link NERtcChannel#enableAudioVolumeIndication} 方法开启；
         * 开启后，无论频道内是否有人说话，SDK 都会按 {@link NERtcChannel#enableAudioVolumeIndication} 方法中设置的时间间隔触发该回调。
         * 在返回的 speakers 数组中:
         * - 如果有 uid 出现在上次返回的数组中，但不在本次返回的数组中，则默认该 uid 对应的远端用户没有说话。
         * - 如果volume 为 0，表示该用户没有说话。
         * - 如果speakers 数组为空，则表示此时远端没有人说话。
         * </pre>
         * @event NERtcChannel#onRemoteAudioVolumeIndication
         * @param {object[]} speakers 每个说话者的用户 ID 和音量信息的数组:
         * @param {number} speakers[].uid 说话者的用户 ID。如果返回的 uid 为 0，则默认为本地用户
         * @param {number} speakers[].volume 说话者的音量，范围为 0（最低）- 100（最高）
         * @param {number} speaker_number speakers 数组的大小，即说话者的人数。
         * @param {number} total_volume （混音后的）总音量，取值范围为 [0,100]。
         */
        this.rtcChannel.onEvent('onRemoteAudioVolumeIndication', function (
            speakers: Array<NERtcAudioVolumeInfo>,
            speaker_number: number,
            total_volume: number
        ) {
            fire('onRemoteAudioVolumeIndication', speakers, speaker_number, total_volume);
        });
  
        /**
         * 通知添加直播任务结果。
         * <pre>
         * 该回调异步返回 {@link NERtcChannel#addLiveStreamTask} 接口的调用结果；实际推流状态参考 onLiveStreamState 事件
         * </pre>
         * @event NERtcChannel#onAddLiveStreamTask
         * @param {String} task_id 任务id
         * @param {String} url 推流地址
         * @param {number} error 结果
         */
        this.rtcChannel.onEvent('onAddLiveStreamTask', function (
            task_id: string,
            url: string,
            error: number
        ) {
            fire('onAddLiveStreamTask', task_id, url, error);
        });
  
        /**
         * 通知更新直播任务结果。
         * <pre>
         * 该回调异步返回 {@link NERtcChannel#updateLiveStreamTask} 接口的调用结果；实际推流状态参考 onLiveStreamState 事件
         * </pre>
         * @event NERtcChannel#onUpdateLiveStreamTask
         * @param {String} task_id 任务id
         * @param {String} url 推流地址
         * @param {number} error 结果
         */
        this.rtcChannel.onEvent('onUpdateLiveStreamTask', function (
            task_id: string,
            url: string,
            error: number
        ) {
            fire('onUpdateLiveStreamTask', task_id, url, error);
        });
  
        /**
         * 通知删除直播任务结果。
         * <pre>
         * 该回调异步返回 {@link NERtcChannel#removeLiveStreamTask} 接口的调用结果；实际推流状态参考 onLiveStreamState 事件
         * </pre>
         * @event NERtcChannel#onRemoveLiveStreamTask
         * @param {String} task_id 任务id
         * @param {number} error 结果
         */
        this.rtcChannel.onEvent('onRemoveLiveStreamTask', function (
            task_id: string,
            error: number
        ) {
            fire('onRemoveLiveStreamTask', task_id, error);
        });
  
        /**
         * 通知直播推流状态
         * @event NERtcChannel#onLiveStreamState
         * @param {string} task_id 任务id
         * @param {string} url 推流地址
         * @param {number} state 直播推流状态:
         * <pre>
         * - 505: 推流中；
         * - 506: 推流失败；
         * - 511: 推流结束；
         * </pre>
         */
        this.rtcChannel.onEvent('onLiveStreamState', function (
            task_id: string,
            url: string,
            state: NERtcLiveStreamStateCode
        ) {
            fire('onLiveStreamState', task_id, url, state);
        });

        /**
         * 监听 SEI 数据回调
         * @since 4.1.110
         * @event NERtcChannel#onRecvSEIMsg
         * @param {number} uid 发送该 sei 的用户 id
         * @param {ArrayBuffer} data 接收到的 sei 数据
         */
        this.rtcChannel.onEvent('onRecvSEIMsg', function (
            uid: number,
            data: ArrayBuffer,
        ) {
            fire('onRecvSEIMsg', uid, data);
        });
  
        /** 
         * 跨房间媒体流转发状态发生改变回调。
         * @event NERtcChannel#onMediaRelayStateChanged
         * @param {number} state 当前跨房间媒体流转发状态。
         * <pre>
         * - 0 初始状态。在成功调用 stopChannelMediaRelay 停止跨房间媒体流转发后， onMediaRelayStateChanged 会回调该状态。
         * - 1 尝试跨房间转发媒体流。
         * - 2 源房间主播角色成功加入目标房间。
         * - 3 发生异常，详见 onMediaRelayEvent 的 error 中提示的错误信息。
         * </pre>
         * @param channel_name  媒体流转发的目标房间名。
         */
        this.rtcChannel.onEvent('onMediaRelayStateChanged', function (state: number, channel_name: String) {
            fire('onMediaRelayStateChanged', state, channel_name);
        });
  
        /** 
         * 媒体流相关转发事件回调。
         * @event NERtcChannel#onMediaRelayEvent
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
         this.rtcChannel.onEvent('onMediaRelayEvent', function (event: number, channel_name: string, error: number) {
            fire('onMediaRelayEvent', event, channel_name, error);
        });
  
        /**
         * 本地发布流已回退为音频流、或已恢复为音视频流回调。
         * <br>如果您调用了设置本地推流回退选项 setLocalPublishFallbackOption 接口，并将 option 设置为 #kNERtcStreamFallbackAudioOnly 后，当上行网络环境不理想、本地发布的媒体流回退为音频流时，或当上行网络改善、媒体流恢复为音视频流时，会触发该回调。 
         * @event NERtcChannel#onLocalPublishFallbackToAudioOnly
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
        this.rtcChannel.onEvent('onLocalPublishFallbackToAudioOnly', function (
          is_fallback: boolean, 
          stream_type: number
        ) {
            fire('onLocalPublishFallbackToAudioOnly', is_fallback, stream_type);
        });
  
        /**
         * 订阅的远端流已回退为音频流、或已恢复为音视频流回调。
         * <pre>
         * 如果你调用了设置远端订阅流回退选项 setRemoteSubscribeFallbackOption 接口并将 option 设置 #kNERtcStreamFallbackAudioOnly 后，当下行网络环境不理想、仅接收远端音频流时，或当下行网络改善、恢复订阅音视频流时，会触发该回调。
         * </pre>
         * @event NERtcChannel#onRemoteSubscribeFallbackToAudioOnly 
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
         this.rtcChannel.onEvent('onRemoteSubscribeFallbackToAudioOnly', function (
            uid: number, 
            is_fallback: boolean, 
            stream_type: number
          ) {
            fire('onRemoteSubscribeFallbackToAudioOnly', uid, is_fallback, stream_type);
        });
  
        /**
         * 服务端禁言音视频权限变化回调。
         * @since V5.4.0
         * @event NERtcChannel#onMediaRightChange 
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
        this.rtcChannel.onEvent('onMediaRightChange', function (
            is_audio_banned: boolean, 
            is_video_banned: boolean
        ) {
            fire('onMediaRightChange', is_audio_banned, is_video_banned);
        });
  
        /**
         * 权限密钥即将过期事件回调
         * <pre>
         * - 由于 PermissionKey 具有一定的时效，在通话过程中如果 PermissionKey 即将失效，SDK 会提前 30 秒触发该回调，提醒用户更新 PermissionKey。
         * </pre>
         * @since V5.4.0
         * @event NERtcChannel#onPermissionKeyWillExpire 
         */
        this.rtcChannel.onEvent('onPermissionKeyWillExpire', function () {
            fire('onPermissionKeyWillExpire');
        });
  
        /**
         * 更新权限密钥事件回调
         * @since V5.4.0
         * @event NERtcChannel#onUpdatePermissionKey 
         * @param {string} key 新的权限密钥
         * @param {number} code 错误码
         * @param {number} time 更新后的权限密钥剩余有效时间。单位为秒。
         */
        this.rtcChannel.onEvent('onUpdatePermissionKey', function (
            key: string, 
            code: number, 
            time: number
        ) {
            fire('onUpdatePermissionKey', key, code, time);
        });
        
        this.rtcChannel.onVideoFrame( (infos: any)=>{ //function
            this.doVideoFrameReceived(infos);
        });
  
  
        /*****mediaStats*****/
        /**
         * 当前通话统计回调。
         * <pre>
         * SDK 定期向 App 报告当前通话的统计信息，每 2 秒触发一次。
         * </pre>
         * @event NERtcChannel#onRtcStats
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
        this.rtcChannel.onStatsObserver('onRtcStats', true, function (stats: NERtcStats) {
          fire('onRtcStats', stats);
        });

        /**
         * 本地音频流统计信息回调。
         * <pre>
         * 该回调描述本地设备发送音频流的统计信息，每 2 秒触发一次。
         * </pre>
         * @event NERtcChannel#onLocalAudioStats
         * @param {object} stats 本地音频流统计信息:
         * @param {number} stats.num_channels 当前采集声道数。
         * @param {number} stats.sent_sample_rate 本地上行音频采样率。
         * @param {number} stats.sent_bitrate （上次统计后）发送码率(Kbps)。
         * @param {number} stats.audio_loss_rate 特定时间内的音频丢包率 (%)。
         * @param {number} stats.rtt RTT。
         * @param {number} stats.volume 音量，范围为 0（最低）- 100（最高）。
         */
        this.rtcChannel.onStatsObserver('onLocalAudioStats', true, function (stats: NERtcAudioSendStats) {
            fire('onLocalAudioStats', stats);
        });
  
        /**
         * 通话中远端音频流的统计信息回调。
         * <pre>
         * 该回调描述远端用户在通话中端到端的音频流统计信息，每 2 秒触发一次。
         * </pre>
         * @event NERtcChannel#onRemoteAudioStats
         * @param {number} uc stats 数组的大小
         * @param {object[]} stats 每个远端用户音频统计信息的数组:
         * @param {number} stats[].uid 用户 ID，指定是哪个用户的音频流
         * @param {number} stats[].received_bitrate （上次统计后）接收到的码率(Kbps)
         * @param {number} stats[].total_frozen_time 用户的下行音频卡顿累计时长(ms)
         * @param {number} stats[].frozen_rate 用户的下行音频平均卡顿率(%)。
         * @param {number} stats[].audio_loss_rate 特定时间内的音频丢包率 (%)。
         * @param {number} stats[].volume 音量，范围为 0（最低）- 100（最高）。
         */
        this.rtcChannel.onStatsObserver('onRemoteAudioStats', true, function (uc: number, stats: Array<NERtcAudioRecvStats>) {
            fire('onRemoteAudioStats', uc, stats);
        });
  
        /**
         * 本地视频流统计信息回调。
         * <pre>
         * 该回调描述本地设备发送视频流的统计信息，每 2 秒触发一次。
         * </pre>
         * @event NERtcChannel#onLocalVideoStats
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
        this.rtcChannel.onStatsObserver('onLocalVideoStats', true, function (stats: NERtcVideoSendStats) {
            fire('onLocalVideoStats', stats);
        });
  
        /**
         * 通话中远端视频流的统计信息回调。
         * <pre>
         * 该回调描述远端用户在通话中端到端的视频流统计信息，每 2 秒触发一次。
         * </pre>
         * @event NERtcChannel#onRemoteVideoStats
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
        this.rtcChannel.onStatsObserver('onRemoteVideoStats', true, function (uc: number, stats: Array<NERtcVideoRecvStats>) {
            fire('onRemoteVideoStats', uc, stats);
        });
  
        /**
         * 通话中每个用户的网络上下行质量报告回调。
         * <pre>
         * 该回调描述每个用户在通话中的网络状态，每 2 秒触发一次，只上报状态有变更的成员。
         * </pre>
         * @event NERtcChannel#onNetworkQuality
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
        this.rtcChannel.onStatsObserver('onNetworkQuality', true, function (uc: number, stats: Array<NERtcNetworkQualityInfo>) {
            fire('onNetworkQuality', uc, stats);
        });

        /**
         * 接收的远端视频分辨率变化回调。
         * @since V5.4.1
         * <pre>
          * 当远端用户视频流的分辨率发生变化时，会触发此回调，例如推流端调用 SetVideoConfig 更改了编码分辨率设置，本地会收到该远端用户分辨率变化通知。
          * </pre>
         * @event NERtcChannel#onRemoteVideoReceiveSizeChanged
         * @param {number} uid 远端用户ID，指定是哪个用户的视频流
         * @param {number} type 视频通道类型
         * <pre>
         * - 0 主流
         * - 1 辅流
         * </pre>
         * @param {number} width 视频采集的宽，单位为 px
         * @param {number} height 视频采集的高，单位为 px
         */
        this.rtcChannel.onEvent('onRemoteVideoReceiveSizeChanged', function (
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
         * @event NERtcChannel#onLocalVideoRenderSizeChanged
         * @param {number} type 视频通道类型
         * <pre>
         * - 0 主流
         * - 1 辅流
         * </pre>
         * @param {number} width 视频采集的宽，单位为 px
         * @param {number} height 视频采集的高，单位为 px
         */
        this.rtcChannel.onEvent('onLocalVideoRenderSizeChanged', function (
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
         * @event NERtcChannel#onFirstVideoFrameRender
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
        this.rtcChannel.onEvent('onFirstVideoFrameRender', function (
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
         * @event NERtcChannel#onLabFeatureCallback
         * @param {string} key 返回回调类型。
         * @param {string} param 值内容。对应字符串的参数值，如果是结构体对象，需要转成json格式。
         */
        this.rtcChannel.onEvent('onLabFeatureCallback', function (
            key: string, 
            param: string
        ) {
            fire('onLabFeatureCallback', key, param);
        });

        /**
         * 屏幕共享源采集范围等变化的回调。
         * @event NERtcChannel#onLabFeatureCallback
         * @param {Object} data 返回回调类型。
         * @param {number} data.type 屏幕分享类型。
         * @param {number} data.source_id 屏幕分享源的ID。
         * @param {number} data.status 屏幕分享状态。
         * @param {number} data.action 屏幕分享自定义高亮框的设置动作，结合capture_rect使用。
         * @param {number} data.regionRect.x 左上角的横向偏移
         * @param {number} data.regionRect.y 左上角的纵向偏移
         * @param {number} data.regionRect.width 待共享区域的宽
         * @param {number} data.regionRect.height 待共享区域的高
         * @param {number} data.level 屏幕分享源的层级，仅用于macOS
         */
        this.rtcChannel.onEvent('onScreenCaptureSourceDataUpdate', function (
            data: NERtcScreenCaptureSourceData
        ) {
            fire('onScreenCaptureSourceDataUpdate', data);
        });



    }
    
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

declare interface NERtcChannel {
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
    on(event: 'onUserJoined', cb: (uid: number, userName: string, extra_info: any) => void): this;

    on(event: 'onUserJoinedEx', cb: (uid: number, userName: string, extra_info: any) => void): this;

    /** 远端用户离开当前频道回调。

     提示有远端用户离开了频道（或掉线）。

     @param uid 远端用户ID。
     @param reason 远端用户离开原因。
     */
     on(event: 'onUserLeft', cb: (uid: number, reason: NERtcSessionLeaveReason) => void): this;

     on(event: 'onUserLeftEx', cb: (uid: number, reason: NERtcSessionLeaveReason, extra_info: any) => void): this;

    /** 远端用户开启音频回调。
    
     @param uid 远端用户ID。
     */
    on(event: 'onUserAudioStart', cb: (uid: number) => void): this;

    /** 远端用户停用音频回调。

     @param uid 远端用户ID。
     */
     on(event: 'onUserAudioStop', cb: (uid: number) => void): this;

     /** 远端用户是否静音回调。

     @param uid 远端用户ID。
     @param mute 是否静音。
     */
    on(event: 'onUserAudioMute', cb: (uid: number, mute: boolean) => void): this;

    on(event: 'onUserSubStreamAudioStart', cb: (uid: number) => void): this;

    on(event: 'onUserSubStreamAudioStop', cb: (uid: number) => void): this;

    on(event: 'onUserSubStreamAudioMute', cb: (uid: number, numte: boolean) => void): this;

    /** 远端用户开启视频回调。

     @param uid 远端用户ID。
     @param max_profile 最大分辨率。
     */
     on(event: 'onUserVideoStart', cb: (uid: number, maxProfile: NERtcVideoProfileType) => void): this;

     /** 远端用户停用视频回调。

     @param uid 远端用户ID。
     */
    on(event: 'onUserVideoStop', cb: (uid: number) => void): this;

    /** 远端用户是否禁视频流回调。

     @param uid 远端用户ID。
     @param mute 是否禁视频流。
     */
    on(event: 'onUserVideoMute', cb: (uid: number, mute: boolean) => void): this;

    on(event: 'onUserVideoMuteEx', cb: (streamType: number, uid: number, mute: boolean) => void): this;

    /** 远端用户开启辅流视频回调。

     @param uid 远端用户ID。
     @param max_profile 最大分辨率。
     */
     on(event: 'onUserSubStreamVideoStart', cb: (uid: number, max_profile: NERtcVideoProfileType) => void): this;

    /** 远端用户停用辅流视频回调。

     @param uid 远端用户ID。
     */
     on(event: 'onUserSubStreamVideoStop', cb: (uid: number) => void): this;

     /**
     * 屏幕共享暂停/恢复/开始/结束等回调
     */
    on(event: 'onScreenCaptureStatus', cb: (status: number) => void): this;

    /** 已接收到远端音频首帧回调。

     @param uid 发送音频帧的远端用户的用户 ID。
     */
     on(event: 'onFirstAudioDataReceived', cb: (uid: number) => void): this;

    /** 已显示首帧远端视频回调。

    第一帧远端视频显示在视图上时，触发此调用。

     @param uid 用户 ID，指定是哪个用户的视频流。
     */
    on(event: 'onFirstVideoDataReceived', cb: (uid: number) => void): this;

    on(event: 'onFirstVideoDataReceivedEx', cb: (type:number, uid: number) => void): this;

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

    on(event: 'onFirstVideoFrameDecodedEx', cb: (type: number, uid: number, width: number, height: number) => void): this;

    /** 提示频道内本地用户瞬时音量的回调。

     该回调默认禁用。可以通过 enableAudioVolumeIndication 方法开启；
     开启后，本地用户说话，SDK 会按 enableAudioVolumeIndication 方法中设置的时间间隔触发该回调。
     如果本地用户将自己静音（调用了 muteLocalAudioStream），SDK 将音量设置为 0 后回调给应用层。

     @param volume （混音后的）音量，取值范围为 [0,100]。
     */
    on(event: 'onLocalAudioVolumeIndication', cb: (volume: number) => void): this;

    on(event: 'onLocalAudioVolumeIndicationEx', cb: (volume: number, enable_vad: boolean) => void): this;

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

    /** 收到远端流的 SEI 内容回调。

     * @param uid 发送该 sei 的用户 id
     * @param data 接收到的 sei 数据
     */
    on(event: 'onRecvSEIMsg', cb: (uid: number, data: ArrayBuffer) => void): this;

    on(event: 'onMediaRelayStateChanged', cb: (state: number, channel_name: String) => void): this;
    
    on(event: 'onMediaRelayEvent', cb: (event: number, channel_name: String, error: number) => void): this;
    on(event: 'onLocalPublishFallbackToAudioOnly', cb: (is_fallback: boolean, stream_type: number) => void): this;
    on(event: 'onRemoteSubscribeFallbackToAudioOnly', cb: (uid: number, is_fallback: boolean, stream_type: number) => void): this;
    
    on(event: 'onMediaRightChange', cb: (is_audio_banned: boolean, is_video_banned: boolean) => void): this;
    on(event: 'onPermissionKeyWillExpire', cb: () => void): this;
    on(event: 'onUpdatePermissionKey', cb: (key: string, code: number, time: number) => void): this;
    
    











}

export default NERtcChannel;

