"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const renderer_1 = require("../renderer");
const defs_1 = require("./defs");
const events_1 = require("events");
/**
 * @class NERtcChannel
 */
class NERtcChannel extends events_1.EventEmitter {
    /**
     * NERtcEngine类构造函数
     * @returns {NERtcEngine}
     */
    constructor(name, rtcChannel) {
        super();
        this.channelName = name;
        this.rtcChannel = rtcChannel;
        this.renderers = new Map();
        this.substreamRenderers = new Map();
        this.renderMode = this._checkWebGL() ? 1 : 2;
        this.customRenderer = renderer_1.CustomRenderer;
        this.initEventHandler();
    }
    release() {
        return this.rtcChannel.release();
    }
    getChannelName() {
        return this.rtcChannel.getChannelName(this.channelName);
    }
    joinChannel(token) {
        return this.rtcChannel.joinChannel(token);
    }
    joinChannelWithUid(token, uid) {
        return this.rtcChannel.joinChannelWithUid(token, uid);
    }
    joinChannelEx(token, uid, opt) {
        return this.rtcChannel.joinChannelEx(token, uid, opt);
    }
    leaveChannel() {
        return this.rtcChannel.leaveChannel();
    }
    enableLocalAudio(enabled) {
        return this.rtcChannel.enableLocalAudio(enabled);
    }
    enableLocalSubStreamAudio(enabled) {
        return this.rtcChannel.enableLocalSubStreamAudio(enabled);
    }
    muteLocalAudioStream(mute) {
        return this.rtcChannel.muteLocalAudioStream(mute);
    }
    muteLocalSubStreamAudio(mute) {
        return this.rtcChannel.muteLocalSubStreamAudio(mute);
    }
    enableLocalVideo(enabled) {
        return this.rtcChannel.enableLocalVideo(enabled);
    }
    enableLocalVideoEx(type, enabled) {
        return this.rtcChannel.enableLocalVideoEx(type, enabled);
    }
    muteLocalVideoStream(enabled) {
        return this.rtcChannel.muteLocalVideoStream(enabled);
    }
    muteLocalVideoStreamEx(type, enabled) {
        return this.rtcChannel.muteLocalVideoStreamEx(type, enabled);
    }
    enableAudioVolumeIndication(enabled, interval, enableVad) {
        return this.rtcChannel.enableAudioVolumeIndication(enabled, interval, enableVad);
    }
    startScreenCaptureByScreenRect(screenRect, regionRect, param) {
        return this.rtcChannel.startScreenCaptureByScreenRect(screenRect, regionRect, param);
    }
    startScreenCaptureByDisplayId(displayId, regionRect, param) {
        return this.rtcChannel.startScreenCaptureByDisplayId(displayId, regionRect, param);
    }
    startScreenCaptureByWindowId(windowid, regionRect, param) {
        return this.rtcChannel.startScreenCaptureByWindowId(windowid, regionRect, param);
    }
    updateScreenCaptureRegion(regionRect) {
        return this.rtcChannel.updateScreenCaptureRegion(regionRect);
    }
    setScreenCaptureMouseCursor() {
        return this.rtcChannel.setScreenCaptureMouseCursor();
    }
    stopScreenCapture() {
        return this.rtcChannel.stopScreenCapture();
    }
    pauseScreenCapture() {
        return this.rtcChannel.pauseScreenCapture();
    }
    resumeScreenCapture() {
        return this.rtcChannel.resumeScreenCapture();
    }
    setExcludeWindowList(param) {
        return this.rtcChannel.setExcludeWindowList(param);
    }
    updateScreenCaptureParameters(captureParams) {
        return this.rtcChannel.updateScreenCaptureParameters(captureParams);
    }
    setupLocalVideoCanvas(canvas) {
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
    setupLocalSubStreamVideoCanvas(canvas) {
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
    setRenderMode(uid, mode) {
        if (this.renderers.has(String(uid))) {
            let renderer = this.renderers.get(String(uid));
            renderer.setContentMode(mode);
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
    setSubStreamRenderMode(uid, mode) {
        if (this.substreamRenderers.has(String(uid))) {
            let renderer = this.substreamRenderers.get(String(uid));
            renderer.setContentMode(mode);
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
    setLocalVideoMirrorMode(mode) {
        return this.rtcChannel.setLocalVideoMirrorMode(mode);
    }
    setLocalVideoMirrorModeEx(type, mode) {
        return this.rtcChannel.setLocalVideoMirrorModeEx(type, mode);
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
    setupRemoteVideoCanvas(uid, canvas) {
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
    setupRemoteSubStreamVideoCanvas(uid, canvas) {
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
    setClientRole(role) {
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
    setLocalMediaPriority(priority, preemptive) {
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
    getConnectionState() {
        return this.rtcChannel.getConnectionState();
    }
    setCameraCaptureConfig(config) {
        return this.rtcChannel.setCameraCaptureConfig(config);
    }
    setCameraCaptureConfigEx(type, config) {
        return this.rtcChannel.setCameraCaptureConfigEx(type, config);
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
    setVideoConfig(config) {
        return this.rtcChannel.setVideoConfig(config);
    }
    setVideoConfigEx(type, config) {
        return this.rtcChannel.setVideoConfigEx(type, config);
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
    enableDualStreamMode(enabled) {
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
    subscribeRemoteAudioStream(uid, enabled) {
        return this.rtcChannel.subscribeRemoteAudioStream(uid, enabled);
    }
    subscribeRemoteSubStreamAudio(uid, enabled) {
        return this.rtcChannel.subscribeRemoteSubStreamAudio(uid, enabled);
    }
    subscribeAllRemoteAudioStream(subscribe) {
        return this.rtcChannel.subscribeAllRemoteAudioStream(subscribe);
    }
    setAudioSubscribeOnlyBy(subscribe) {
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
    subscribeRemoteVideoStream(uid, type, subscribe) {
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
    subscribeRemoteVideoSubStream(uid, sub) {
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
    addLiveStreamTask(info) {
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
    updateLiveStreamTask(info) {
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
    removeLiveStreamTask(taskId) {
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
    sendSEIMsg(data) {
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
    sendSEIMsgEx(data, type) {
        return this.rtcChannel.sendSEIMsgEx(data, type);
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
    captureImageByUid(uid, streamType = defs_1.NERtcVideoStreamType.kNERtcVideoStreamMain) {
        return this.captureRender(uid, streamType);
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
    adjustUserPlaybackSignalVolume(uid, volume) {
        return this.rtcChannel.adjustUserPlaybackSignalVolume(uid, volume);
    }
    adjustChannelPlaybackSignalVolume(volume) {
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
    startChannelMediaRelay(config) {
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
    updateChannelMediaRelay(config) {
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
    stopChannelMediaRelay() {
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
    setLocalPublishFallbackOption(option) {
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
    setRemoteSubscribeFallbackOption(option) {
        return this.rtcChannel.setRemoteSubscribeFallbackOption(option);
    }
    setRemoteHighPriorityAudioStream(enabled, uid) {
        return this.rtcChannel.setRemoteHighPriorityAudioStream(enabled, uid);
    }
    enableMediaPub(enabled, type) {
        return this.rtcChannel.enableMediaPub(enabled, type);
    }
    updatePermissionKey(key) {
        return this.rtcChannel.updatePermissionKey(key);
    }
    enableSpatializer(enable) {
        return this.rtcChannel.enableSpatializer(enable);
    }
    updateSpatializerAudioRecvRange(audible_distance, conversational_distance, roll_off) {
        return this.rtcChannel.updateSpatializerAudioRecvRange(audible_distance, conversational_distance, roll_off);
    }
    updateSpatializerSelfPosition(info) {
        return this.rtcChannel.updateSpatializerSelfPosition(info);
    }
    enableSpatializerRoomEffects(enable) {
        return this.rtcChannel.enableSpatializerRoomEffects(enable);
    }
    setSpatializerRoomProperty(room_property) {
        return this.rtcChannel.setSpatializerRoomProperty(room_property);
    }
    setSpatializerRenderMode(mode) {
        return this.rtcChannel.setSpatializerRenderMode(mode);
    }
    initEventHandler() {
        const self = this;
        const fire = (event, ...args) => {
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
        this.rtcChannel.onEvent('onError', function (errorCode, msg) {
            fire('onError', errorCode, msg);
        });
        /**
         * 发生警告回调。
         * @event NERtcChannel#onWarning
         * @param {NERtcErrorCode} warnCode 错误码
         * @param {string} msg 错误描述
         */
        this.rtcChannel.onEvent('onWarning', function (warnCode, msg) {
            fire('onWarning', warnCode, msg);
        });
        this.rtcChannel.onEvent('onApiCallExecuted', function (apiName, code, msg) {
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
        this.rtcChannel.onEvent('onJoinChannel', function (cid, uid, result, elapsed) {
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
        this.rtcChannel.onEvent('onReconnectingStart', function (cid, uid) {
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
        this.rtcChannel.onEvent('onConnectionStateChange', function (state, reason) {
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
        this.rtcChannel.onEvent('onRejoinChannel', function (cid, uid, result, elapsed) {
            fire('onRejoinChannel', cid, uid, result, elapsed);
        });
        /**
         * 离开频道回调。
         * App 调用 {@link NERtcChannel#leaveChannel} 方法时，SDK提示 App 离开频道是否成功。
         * @event NERtcChannel#onLeaveChannel
         * @param {NERtcErrorCode} result 错误码
         */
        this.rtcChannel.onEvent('onLeaveChannel', function (result) {
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
        this.rtcChannel.onEvent('onDisconnect', function (result) {
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
        this.rtcChannel.onEvent('onClientRoleChanged', function (oldRole, newRole) {
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
        this.rtcChannel.onEvent('onUserJoined', function (uid, userName) {
            fire('onUserJoined', uid, userName);
        });
        this.rtcChannel.onEvent('onUserJoinedEx', function (uid, userName, extra_info) {
            fire('onUserJoinedEx', uid, userName, extra_info);
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
        this.rtcChannel.onEvent('onUserLeft', function (uid, reason) {
            fire('onUserLeft', uid, reason);
        });
        this.rtcChannel.onEvent('onUserLeftEx', function (uid, reason, extra_info) {
            fire('onUserLeftEx', uid, reason, extra_info);
        });
        /**
         * 远端用户开启音频回调。
         * @event NERtcChannel#onUserAudioStart
         * @param {number} uid 远端用户ID。
         */
        this.rtcChannel.onEvent('onUserAudioStart', function (uid) {
            fire('onUserAudioStart', uid);
        });
        /**
         * 远端用户停用音频回调。
         * @event NERtcChannel#onUserAudioStop
         * @param {number} uid 远端用户ID。
         */
        this.rtcChannel.onEvent('onUserAudioStop', function (uid) {
            fire('onUserAudioStop', uid);
        });
        /**
         * 远端用户是否静音回调。
         * @event NERtcChannel#onUserAudioMute
         * @param {number} uid 远端用户ID。
         * @param {boolean} mute 是否静音。
         */
        this.rtcChannel.onEvent('onUserAudioMute', function (uid, mute) {
            fire('onUserAudioMute', uid, mute);
        });
        this.rtcChannel.onEvent('onUserSubStreamAudioStart', function (uid) {
            fire('onUserSubStreamAudioStart', uid);
        });
        this.rtcChannel.onEvent('onUserSubStreamAudioStop', function (uid) {
            fire('onUserSubStreamAudioStop', uid);
        });
        this.rtcChannel.onEvent('onUserSubStreamAudioMute', function (uid, mute) {
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
        this.rtcChannel.onEvent('onUserVideoStart', function (uid, maxProfile) {
            fire('onUserVideoStart', uid, maxProfile);
        });
        /**
         * 远端用户停用视频回调。
         * @event NERtcChannel#onUserVideoStop
         * @param {number} uid 远端用户ID。
         */
        this.rtcChannel.onEvent('onUserVideoStop', function (uid) {
            fire('onUserVideoStop', uid);
        });
        /**
         * 远端用户是否禁视频流回调。
         * @event NERtcChannel#onUserVideoMute
         * @param {number} uid 远端用户ID。
         * @param {boolean} mute 是否禁视频流。
         */
        this.rtcChannel.onEvent('onUserVideoMute', function (uid, mute) {
            fire('onUserVideoMute', uid, mute);
        });
        /**
         * 远端用户是否禁视频流回调。
         * @event NERtcChannel#onUserVideoMuteEx
         * @param {number} uid 远端用户ID。
         * @param {boolean} mute 是否禁视频流。
         */
        this.rtcChannel.onEvent('onUserVideoMuteEx', function (streamType, uid, mute) {
            fire('onUserVideoMuteEx', streamType, uid, mute);
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
        this.rtcChannel.onEvent('onUserSubStreamVideoStart', function (uid, max_profile) {
            fire('onUserSubStreamVideoStart', uid, max_profile);
        });
        /**
         * 远端用户停用辅流视频回调。
         * @event NERtcChannel#onUserSubStreamVideoStop
         * @param {number} uid 远端用户ID。
         */
        this.rtcChannel.onEvent('onUserSubStreamVideoStop', function (uid) {
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
        this.rtcChannel.onEvent('onScreenCaptureStatus', function (status) {
            fire('onScreenCaptureStatus', status);
        });
        /**
         * 已接收到远端音频首帧回调。
         * @event NERtcChannel#onFirstAudioDataReceived
         * @param {number} uid 发送音频帧的远端用户的用户 ID。
         */
        this.rtcChannel.onEvent('onFirstAudioDataReceived', function (uid) {
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
        this.rtcChannel.onEvent('onFirstVideoDataReceived', function (uid) {
            fire('onFirstVideoDataReceived', uid);
        });
        this.rtcChannel.onEvent('onFirstVideoDataReceivedEx', function (type, uid) {
            fire('onFirstVideoDataReceivedEx', type, uid);
        });
        /**
         * 已解码远端音频首帧的回调。
         * @event NERtcChannel#onFirstAudioFrameDecoded
         * @param {number} uid 远端用户 ID。
         */
        this.rtcChannel.onEvent('onFirstAudioFrameDecoded', function (uid) {
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
        this.rtcChannel.onEvent('onFirstVideoFrameDecoded', function (uid, width, height) {
            fire('onFirstVideoFrameDecoded', uid, width, height);
        });
        this.rtcChannel.onEvent('onFirstVideoFrameDecodedEx', function (type, uid, width, height) {
            fire('onFirstVideoFrameDecodedEx', type, uid, width, height);
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
        this.rtcChannel.onEvent('onLocalAudioVolumeIndication', function (volume) {
            fire('onLocalAudioVolumeIndication', volume);
        });
        this.rtcChannel.onEvent('onLocalAudioVolumeIndicationEx', function (volume, enable_vad) {
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
        this.rtcChannel.onEvent('onRemoteAudioVolumeIndication', function (speakers, speaker_number, total_volume) {
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
        this.rtcChannel.onEvent('onAddLiveStreamTask', function (task_id, url, error) {
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
        this.rtcChannel.onEvent('onUpdateLiveStreamTask', function (task_id, url, error) {
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
        this.rtcChannel.onEvent('onRemoveLiveStreamTask', function (task_id, error) {
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
        this.rtcChannel.onEvent('onLiveStreamState', function (task_id, url, state) {
            fire('onLiveStreamState', task_id, url, state);
        });
        /**
         * 监听 SEI 数据回调
         * @since 4.1.110
         * @event NERtcChannel#onRecvSEIMsg
         * @param {number} uid 发送该 sei 的用户 id
         * @param {ArrayBuffer} data 接收到的 sei 数据
         */
        this.rtcChannel.onEvent('onRecvSEIMsg', function (uid, data) {
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
        this.rtcChannel.onEvent('onMediaRelayStateChanged', function (state, channel_name) {
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
        this.rtcChannel.onEvent('onMediaRelayEvent', function (event, channel_name, error) {
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
        this.rtcChannel.onEvent('onLocalPublishFallbackToAudioOnly', function (is_fallback, stream_type) {
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
        this.rtcChannel.onEvent('onRemoteSubscribeFallbackToAudioOnly', function (uid, is_fallback, stream_type) {
            fire('onRemoteSubscribeFallbackToAudioOnly', uid, is_fallback, stream_type);
        });
        this.rtcChannel.onEvent('onMediaRightChange', function (is_audio_banned, is_video_banned) {
            fire('onMediaRightChange', is_audio_banned, is_video_banned);
        });
        this.rtcChannel.onEvent('onPermissionKeyWillExpire', function () {
            fire('onPermissionKeyWillExpire');
        });
        this.rtcChannel.onEvent('onUpdatePermissionKey', function (key, code, time) {
            fire('onUpdatePermissionKey', key, code, time);
        });
        this.rtcChannel.onVideoFrame((infos) => {
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
        this.rtcChannel.onStatsObserver('onRtcStats', true, function (stats) {
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
        this.rtcChannel.onStatsObserver('onLocalAudioStats', true, function (stats) {
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
        this.rtcChannel.onStatsObserver('onRemoteAudioStats', true, function (uc, stats) {
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
        this.rtcChannel.onStatsObserver('onLocalVideoStats', true, function (stats) {
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
        this.rtcChannel.onStatsObserver('onRemoteVideoStats', true, function (uc, stats) {
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
        this.rtcChannel.onStatsObserver('onNetworkQuality', true, function (uc, stats) {
            fire('onNetworkQuality', uc, stats);
        });
    }
    /**
     * @private
     * @ignore
     * check if WebGL will be available with appropriate features
     * @returns {number} {boolean}
     */
    _checkWebGL() {
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
        }
        catch (e) {
            return false;
        }
        if (gl) {
            return true;
        }
        else {
            return false;
        }
    }
    /**
     * @private
     * @ignore
     * @param {number} type 0-local 1-remote 2-localsubstream 3-remotesubstream
     * @param {number} uid uid get from native engine, differ from electron engine's uid
     */
    _getRenderer(type, uid) {
        if (type === 0) {
            return this.renderers.get('local');
        }
        else if (type === 1) {
            return this.renderers.get(String(uid));
        }
        else if (type === 2) {
            return this.substreamRenderers.get('local');
        }
        else if (type === 3) {
            return this.substreamRenderers.get(String(uid));
        }
        else {
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
    _checkData(header, ydata, udata, vdata) {
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
        if (ydata.byteLength != udata.byteLength * 4 ||
            udata.byteLength != vdata.byteLength) {
            console.error('invalid image header ' +
                ydata.byteLength +
                ' ' +
                udata.byteLength +
                ' ' +
                vdata.byteLength);
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
    doVideoFrameReceived(infos) {
        const len = infos.length;
        for (let i = 0; i < len; i++) {
            const info = infos[i];
            const { type, uid, channelId, header, ydata, udata, vdata } = info;
            if (!header || !ydata || !udata || !vdata) {
                console.log('Invalid data param ： ' +
                    header +
                    ' ' +
                    ydata +
                    ' ' +
                    udata +
                    ' ' +
                    vdata);
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
    initRender(key, view) {
        if (this.renderers.has(String(key))) {
            this.destroyRender(key);
        }
        let renderer;
        if (this.renderMode === 1) {
            renderer = new renderer_1.GlRenderer();
        }
        else if (this.renderMode === 2) {
            renderer = new renderer_1.SoftwareRenderer();
        }
        else if (this.renderMode === 3) {
            renderer = new this.customRenderer();
        }
        else {
            console.warn('Unknown render mode, fallback to 1');
            renderer = new renderer_1.GlRenderer();
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
    initSubStreamRender(key, view) {
        if (this.substreamRenderers.has(String(key))) {
            this.destroySubStreamRender(key);
        }
        let renderer;
        if (this.renderMode === 1) {
            renderer = new renderer_1.GlRenderer();
        }
        else if (this.renderMode === 2) {
            renderer = new renderer_1.SoftwareRenderer();
        }
        else if (this.renderMode === 3) {
            renderer = new this.customRenderer();
        }
        else {
            console.warn('Unknown render mode, fallback to 1');
            renderer = new renderer_1.GlRenderer();
        }
        renderer.bind(view);
        this.substreamRenderers.set(String(key), renderer);
    }
    captureRender(key, streamType = defs_1.NERtcVideoStreamType.kNERtcVideoStreamMain) {
        if (streamType === defs_1.NERtcVideoStreamType.kNERtcVideoStreamMain) {
            if (!this.renderers.has(String(key))) {
                return '';
            }
        }
        else {
            if (!this.substreamRenderers.has(String(key))) {
                return '';
            }
        }
        let renderer;
        if (streamType === defs_1.NERtcVideoStreamType.kNERtcVideoStreamMain) {
            renderer = this.renderers.get(String(key));
        }
        else {
            renderer = this.substreamRenderers.get(String(key));
        }
        try {
            return renderer.captureImage();
        }
        catch (err) {
            console.error(`${err.stack}`);
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
    destroyRender(key, onFailure) {
        if (!this.renderers.has(String(key))) {
            return;
        }
        let exception = null;
        let renderer = this.renderers.get(String(key));
        try {
            renderer.unbind();
            this.renderers.delete(String(key));
        }
        catch (err) {
            exception = err;
            console.error(`${err.stack}`);
        }
        if (exception) {
            onFailure && onFailure(exception);
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
    destroySubStreamRender(key, onFailure) {
        if (!this.substreamRenderers.has(String(key))) {
            return;
        }
        let exception = null;
        let renderer = this.substreamRenderers.get(String(key));
        try {
            renderer.unbind();
            this.substreamRenderers.delete(String(key));
        }
        catch (err) {
            exception = err;
            console.error(`${err.stack}`);
        }
        if (exception) {
            onFailure && onFailure(exception);
        }
    }
}
exports.default = NERtcChannel;
