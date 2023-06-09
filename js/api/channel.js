"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const renderer_1 = require("../renderer");
const defs_1 = require("./defs");
const events_1 = require("events");

/**
 * @class NERtcChannel
 */
class NERtcChannel extends events_1.EventEmitter {
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
    setLocalVideoMirrorMode(mode) {
        return this.rtcChannel.setLocalVideoMirrorMode(mode);
    }
    setLocalVideoMirrorModeEx(type, mode) {
        return this.rtcChannel.setLocalVideoMirrorModeEx(type, mode);
    }
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
    setClientRole(role) {
        return this.rtcChannel.setClientRole(role);
    }
    setLocalMediaPriority(priority, preemptive) {
        return this.rtcChannel.setLocalMediaPriority(priority, preemptive);
    }
    getConnectionState() {
        return this.rtcChannel.getConnectionState();
    }
    setCameraCaptureConfig(config) {
        return this.rtcChannel.setCameraCaptureConfig(config);
    }
    setCameraCaptureConfigEx(type, config) {
        return this.rtcChannel.setCameraCaptureConfigEx(type, config);
    }
    setVideoConfig(config) {
        return this.rtcChannel.setVideoConfig(config);
    }
    setVideoConfigEx(type, config) {
        return this.rtcChannel.setVideoConfigEx(type, config);
    }
    enableDualStreamMode(enabled) {
        return this.rtcChannel.enableDualStreamMode(enabled);
    }
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
    subscribeRemoteVideoStream(uid, type, subscribe) {
        return this.rtcChannel.subscribeRemoteVideoStream(uid, type, subscribe);
    }
    subscribeRemoteVideoSubStream(uid, sub) {
        return this.rtcChannel.subscribeRemoteVideoSubStream(uid, sub);
    }
    addLiveStreamTask(info) {
        return this.rtcChannel.addLiveStreamTask(info);
    }
    updateLiveStreamTask(info) {
        return this.rtcChannel.updateLiveStreamTask(info);
    }
    removeLiveStreamTask(taskId) {
        return this.rtcChannel.removeLiveStreamTask(taskId);
    }
    sendSEIMsg(data) {
        return this.rtcChannel.sendSEIMsg(data);
    }
    sendSEIMsgEx(data, type) {
        return this.rtcChannel.sendSEIMsgEx(data, type);
    }
    captureImageByUid(uid, streamType = defs_1.NERtcVideoStreamType.kNERtcVideoStreamMain) {
        return this.captureRender(uid, streamType);
    }
    adjustUserPlaybackSignalVolume(uid, volume) {
        return this.rtcChannel.adjustUserPlaybackSignalVolume(uid, volume);
    }
    adjustChannelPlaybackSignalVolume(volume) {
        return this.rtcChannel.adjustChannelPlaybackSignalVolume(volume);
    }
    startChannelMediaRelay(config) {
        return this.rtcChannel.startChannelMediaRelay(config);
    }
    updateChannelMediaRelay(config) {
        return this.rtcChannel.updateChannelMediaRelay(config);
    }
    stopChannelMediaRelay() {
        return this.rtcChannel.stopChannelMediaRelay();
    }
    setLocalPublishFallbackOption(option) {
        return this.rtcChannel.setLocalPublishFallbackOption(option);
    }
    setRemoteSubscribeFallbackOption(option) {
        return this.rtcChannel.setRemoteSubscribeFallbackOption(option);
    }
    setRemoteHighPriorityAudioStream(enabled, uid) {
        return this.rtcChannel.setRemoteHighPriorityAudioStream(enabled, uid);
    }
    enableMediaPub(enabled, type) {
        return this.rtcChannel.enableMediaPub(enabled, type);
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
         this.rtcChannel.onEvent('onUserVideoMuteEx', function (uid, mute) {
            fire('onUserVideoMuteEx', uid, mute);
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
         * @event NERtcChannel#onScreenCaptureStatus
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
        
        this.rtcChannel.onVideoFrame( (infos)=>{ //function
            this.doVideoFrameReceived(infos);
        });


        // /**
        //  * 当前通话统计回调。
        //  * <pre>
        //  * SDK 定期向 App 报告当前通话的统计信息，每 2 秒触发一次。
        //  * </pre>
        //  * @event NERtcChannel#onRtcStats
        //  * @param {object} stats NERTC 引擎统计数据:
        //  * @param {number} stats.cpu_app_usage 当前 App 的 CPU 使用率 (%)。
        //  * @param {number} stats.cpu_idle_usage 当前系统的 CPU 空闲率 (%)。
        //  * @param {number} stats.cpu_total_usage 当前系统的 CPU 使用率 (%)。
        //  * @param {number} stats.memory_app_usage 当前App的内存使用率 (%)。
        //  * @param {number} stats.memory_total_usage 当前系统的内存使用率 (%)。
        //  * @param {number} stats.memory_app_kbytes 当前App的内存使用量 (KB)。
        //  * @param {number} stats.total_duration 通话时长（秒）。
        //  * @param {number} stats.tx_bytes 发送字节数，累计值。(bytes)
        //  * @param {number} stats.rx_bytes 接收字节数，累计值。(bytes)
        //  * @param {number} stats.tx_audio_bytes 音频发送字节数，累计值。(bytes)
        //  * @param {number} stats.tx_video_bytes 视频发送字节数，累计值。(bytes)
        //  * @param {number} stats.rx_audio_bytes 音频接收字节数，累计值。(bytes)
        //  * @param {number} stats.rx_video_bytes 视频接收字节数，累计值。(bytes)
        //  * @param {number} stats.tx_audio_kbitrate 音频发送码率。(kbps)
        //  * @param {number} stats.rx_audio_kbitrate 音频接收码率。(kbps)
        //  * @param {number} stats.tx_video_kbitrate 视频发送码率。(kbps)
        //  * @param {number} stats.rx_video_kbitrate 视频接收码率。(kbps)
        //  * @param {number} stats.up_rtt 上行平均往返时延rtt(ms)
        //  * @param {number} stats.down_rtt 下行平均往返时延rtt(ms)
        //  * @param {number} stats.tx_audio_packet_loss_rate 本地上行音频实际丢包率。(%)
        //  * @param {number} stats.tx_video_packet_loss_rate 本地上行视频实际丢包率。(%)
        //  * @param {number} stats.tx_audio_packet_loss_sum  本地上行音频实际丢包数。
        //  * @param {number} stats.tx_video_packet_loss_sum  本地上行视频实际丢包数。
        //  * @param {number} stats.tx_audio_jitter           本地上行音频抖动计算。(ms)
        //  * @param {number} stats.tx_video_jitter           本地上行视频抖动计算。(ms)
        //  * @param {number} stats.rx_audio_packet_loss_rate 本地下行音频实际丢包率。(%)
        //  * @param {number} stats.rx_video_packet_loss_rate 本地下行视频实际丢包率。(%)
        //  * @param {number} stats.rx_audio_packet_loss_sum  本地下行音频实际丢包数。
        //  * @param {number} stats.rx_video_packet_loss_sum  本地下行视频实际丢包数。
        //  * @param {number} stats.rx_audio_jitter           本地下行音频抖动计算。(ms)
        //  * @param {number} stats.rx_video_jitter           本地下行视频抖动计算。(ms)
        //  */
        this.rtcChannel.onStatsObserver('onRtcStats', true, function (stats) {
            fire('onRtcStats', stats);
        });

        // /**
        //  * 本地音频流统计信息回调。
        //  * <pre>
        //  * 该回调描述本地设备发送音频流的统计信息，每 2 秒触发一次。
        //  * </pre>
        //  * @event NERtcChannel#onLocalAudioStats
        //  * @param {object} stats 本地音频流统计信息:
        //  * @param {number} stats.num_channels 当前采集声道数。
        //  * @param {number} stats.sent_sample_rate 本地上行音频采样率。
        //  * @param {number} stats.sent_bitrate （上次统计后）发送码率(Kbps)。
        //  * @param {number} stats.audio_loss_rate 特定时间内的音频丢包率 (%)。
        //  * @param {number} stats.rtt RTT。
        //  * @param {number} stats.volume 音量，范围为 0（最低）- 100（最高）。
        //  */
        this.rtcChannel.onStatsObserver('onLocalAudioStats', true, function (stats) {
            fire('onLocalAudioStats', stats);
        });

        // /**
        //  * 通话中远端音频流的统计信息回调。
        //  * <pre>
        //  * 该回调描述远端用户在通话中端到端的音频流统计信息，每 2 秒触发一次。
        //  * </pre>
        //  * @event NERtcChannel#onRemoteAudioStats
        //  * @param {number} uc stats 数组的大小
        //  * @param {object[]} stats 每个远端用户音频统计信息的数组:
        //  * @param {number} stats[].uid 用户 ID，指定是哪个用户的音频流
        //  * @param {number} stats[].received_bitrate （上次统计后）接收到的码率(Kbps)
        //  * @param {number} stats[].total_frozen_time 用户的下行音频卡顿累计时长(ms)
        //  * @param {number} stats[].frozen_rate 用户的下行音频平均卡顿率(%)。
        //  * @param {number} stats[].audio_loss_rate 特定时间内的音频丢包率 (%)。
        //  * @param {number} stats[].volume 音量，范围为 0（最低）- 100（最高）。
        //  */
        this.rtcChannel.onStatsObserver('onRemoteAudioStats', true, function (uc, stats) {
            fire('onRemoteAudioStats', uc, stats);
        });

        // /**
        //  * 本地视频流统计信息回调。
        //  * <pre>
        //  * 该回调描述本地设备发送视频流的统计信息，每 2 秒触发一次。
        //  * </pre>
        //  * @event NERtcChannel#onLocalVideoStats
        //  * @param {object} stats 本地视频流统计信息:
        //  * @param {number} stats.video_layers_count 视频流条数。
        //  * @param {object[]} stats.video_layers_list 视频流信息数组:
        //  * @param {number} stats.video_layers_list[].layer_type 流类型： 1、主流，2、辅流。
        //  * @param {number} stats.video_layers_list[].width 视频流宽（像素）
        //  * @param {number} stats.video_layers_list[].height 视频流高（像素）
        //  * @param {number} stats.video_layers_list[].capture_frame_rate 视频采集帧率。
        //  * @param {number} stats.video_layers_list[].render_frame_rate 视频渲染帧率。
        //  * @param {number} stats.video_layers_list[].encoder_frame_rate 编码帧率。
        //  * @param {number} stats.video_layers_list[].sent_frame_rate 发送帧率。
        //  * @param {number} stats.video_layers_list[].sent_bitrate 发送码率(Kbps)。
        //  * @param {number} stats.video_layers_list[].target_bitrate 编码器目标码率(Kbps)。
        //  * @param {number} stats.video_layers_list[].encoder_bitrate 编码器实际编码码率(Kbps)。
        //  * @param {String} stats.video_layers_list[].codec_name 视频编码器名字。
        //  */
        this.rtcChannel.onStatsObserver('onLocalVideoStats', true, function (stats) {
            fire('onLocalVideoStats', stats);
        });

        // /**
        //  * 通话中远端视频流的统计信息回调。
        //  * <pre>
        //  * 该回调描述远端用户在通话中端到端的视频流统计信息，每 2 秒触发一次。
        //  * </pre>
        //  * @event NERtcChannel#onRemoteVideoStats
        //  * @param {number} uc stats 数组的大小。
        //  * @param {object} stats 每个远端用户视频统计信息的数组:
        //  * @param {number} stats.uid 用户 ID，指定是哪个用户的视频流。
        //  * @param {number} stats.video_layers_count 视频流条数。
        //  * @param {object[]} stats.video_layers_list 视频流信息数组:
        //  * @param {number} stats.video_layers_list[].layer_type 流类型： 1、主流，2、辅流。
        //  * @param {number} stats.video_layers_list[].width 视频流宽（像素）。
        //  * @param {number} stats.video_layers_list[].height 视频流高（像素）。
        //  * @param {number} stats.video_layers_list[].received_frame_rate 接收到的帧率 (fps)。
        //  * @param {number} stats.video_layers_list[].render_frame_rate 视频渲染帧率。
        //  * @param {number} stats.video_layers_list[].decoder_frame_rate 视频解码帧率（fps）。
        //  * @param {number} stats.video_layers_list[].frozen_rate 用户的下行视频平均卡顿率（%）。
        //  * @param {number} stats.video_layers_list[].packet_loss_rate 下行丢包率（%）。
        //  * @param {number} stats.video_layers_list[].received_bitrate 接收到的码率（Kbps）。
        //  * @param {number} stats.video_layers_list[].total_frozen_time 用户的下行视频卡顿累计时长（ms）。
        //  * @param {String} stats.video_layers_list[].codec_name 视频编码器名字。
        //  */
        this.rtcChannel.onStatsObserver('onRemoteVideoStats', true, function (uc, stats) {
            fire('onRemoteVideoStats', uc, stats);
        });

        // /**
        //  * 通话中每个用户的网络上下行质量报告回调。
        //  * <pre>
        //  * 该回调描述每个用户在通话中的网络状态，每 2 秒触发一次，只上报状态有变更的成员。
        //  * </pre>
        //  * @event NERtcChannel#onNetworkQuality
        //  * @param {number} uc 数组的大小，即用户数。
        //  * @param {object} stats 每个用户 ID 和网络上下行质量信息的数组:
        //  * @param {number} stats[].uid 用户 ID，指定是哪个用户的视频流。
        //  * @param {number} stats[].tx_quality 该用户的上行网络质量:
        //  * <pre>
        //  * - 0 网络质量未知
        //  * - 1 网络质量极好
        //  * - 2 用户主观感觉和 excellent 差不多，但码率可能略低于 excellent
        //  * - 3 用户主观感受有瑕疵但不影响沟通
        //  * - 4 勉强能沟通但不顺畅
        //  * - 5 网络质量非常差，基本不能沟通
        //  * - 6 完全无法沟通
        //  * </pre>
        //  * @param {number} stats[].rx_quality 该用户的下行网络质量:
        //  * <pre>
        //  * - 0 网络质量未知
        //  * - 1 网络质量极好
        //  * - 2 用户主观感觉和 excellent 差不多，但码率可能略低于 excellent
        //  * - 3 用户主观感受有瑕疵但不影响沟通
        //  * - 4 勉强能沟通但不顺畅
        //  * - 5 网络质量非常差，基本不能沟通
        //  * - 6 完全无法沟通
        //  * </pre>
        //  */
        this.rtcChannel.onStatsObserver('onNetworkQuality', true, function (uc, stats) {
            fire('onNetworkQuality', uc, stats);
        });
    
    }
    
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
    
    doVideoFrameReceived(infos) {
        const len = infos.length;
        for (let i = 0; i < len; i++) {
            const info = infos[i];
            const { type, uid, channelId, header, ydata, udata, vdata } = info;
            if (!header || !ydata || !udata || !vdata) {
                console.log('Invalid data param : ' +
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
        let renderer = null;
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