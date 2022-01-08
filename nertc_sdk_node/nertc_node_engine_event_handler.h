#ifndef NERTC_NODE_ENGINE_EVENT_HANDLER_H
#define NERTC_NODE_ENGINE_EVENT_HANDLER_H

#include <napi.h>
#include <unordered_map>
#include "nertc_node_engine_helper.h"
#include "nertc_engine_event_handler_ex.h"
#include "nertc_engine_media_stats_observer.h"


namespace nertc_node
{

class EventHandler
{
public:
    typedef struct NodeEventCallback {
        Napi::FunctionReference function;
    } EventCallback;

public:
    EventHandler(){};
    ~EventHandler(){};
public:
    void addEvent(const std::string& eventName, Napi::FunctionReference&& function);
    int removeEventHandler(const std::string &eventName);
    int removeAll();

protected:
    std::unordered_map<std::string, std::shared_ptr<EventCallback>> _callbacks;
};


class NertcNodeEventHandler : public EventHandler, public nertc::IRtcEngineEventHandlerEx
{

public:
    NertcNodeEventHandler(){};
    ~NertcNodeEventHandler(){};

public:
    /** 发生错误回调。
     该回调方法表示 SDK 运行时出现了（网络或媒体相关的）错误。通常情况下，SDK上报的错误意味着SDK无法自动恢复，需要 App 干预或提示用户。
     @param error_code 错误代码: #NERtcDMErrorCode.
     @param msg 错误描述。
     */
    virtual void onError(int error_code, const char* msg);

    /** 发生警告回调。
     该回调方法表示 SDK 运行时出现了（网络或媒体相关的）警告。通常情况下，SDK 上报的警告信息 App 可以忽略，SDK 会自动恢复。
     @param warn_code 错误代码: #NERtcWarnCode.
     @param msg 警告描述。
     */
    virtual void onWarning(int warn_code, const char* msg);

    /** 释放硬件资源的回调。
     SDK提示释放硬件资源是否成功。
     @param result 返回结果。
     */
    virtual void onReleasedHwResources(nertc::NERtcErrorCode result);

    /** 加入频道回调。
     @param cid  频道 ID。
     @param uid  用户 ID。
     @param result  返回结果。
     @param elapsed 从 joinChannel 开始到发生此事件过去的时间（毫秒）。
     */
    virtual void onJoinChannel(nertc::channel_id_t cid, nertc::uid_t uid, nertc::NERtcErrorCode result, uint64_t elapsed);

    /** 连接状态变更。
      有时候由于通话流程、用户行为、网络原因等，客户端通话状态变更，触发此回调。
     @param state  变更后通话状态。
     @param reason  变更原因。
     */
    virtual void onConnectionStateChange(nertc::NERtcConnectionStateType state, nertc::NERtcReasonConnectionChangedType reason);

    /** 触发重连。
      有时候由于网络原因，客户端可能会和服务器失去连接，SDK会进行自动重连，开始自动重连后触发此回调。
     @param cid  频道 ID。
     @param uid  用户 ID。
     */
    virtual void onReconnectingStart(nertc::channel_id_t cid, nertc::uid_t uid);

    /** 重新加入频道回调。
	  有时候由于网络原因，客户端可能会和服务器失去连接，SDK会进行自动重连，自动重连后触发此回调方法。
     @param cid  频道 ID。
     @param uid  用户 ID。
     @param result  返回结果。
     @param elapsed 从开始重连到发生此事件过去的时间（毫秒）。
     */
    virtual void onRejoinChannel(nertc::channel_id_t cid, nertc::uid_t uid, nertc::NERtcErrorCode result, uint64_t elapsed);

    /** 离开频道回调。
     App 调用 \ref IRtcEngine::leaveChannel "leaveChannel" 方法时，SDK提示 App 离开频道是否成功。
     @param result 返回结果。
     */
    virtual void onLeaveChannel(nertc::NERtcErrorCode result);

    /** 掉线回调。
	  由于非网络原因，客户端可能会和服务器失去连接，此时SDK无需自动重连，直接触发此回调方法。
     @param reason  返回结果。
     */
    virtual void onDisconnect(nertc::NERtcErrorCode reason);

    /** 参会者角色类型变更回调。
    本地用户加入房间后，通过 \ref IRtcEngine::setClientRole "setClientRole" 切换用户角色后会触发此回调。例如从主播切换为观众、从观众切换为主播。
    @note 直播场景下，如果您在加入房间后调用该方法切换用户角色，调用成功后，会触发以下回调：
    - 主播切观众，本端触发onClientRoleChanged回调，远端触发\ref nertc::IRtcEngineEventHandler::onUserLeft "onUserLeft"回调。
    - 观众切主播，本端触发onClientRoleChanged回调，远端触发\ref nertc::IRtcEngineEventHandler::onUserJoined "onUserJoined"回调。
     @param oldRole  原角色类型。
     @param newRole  新角色类型。
     */
    virtual void onClientRoleChanged(nertc::NERtcClientRole oldRole, nertc::NERtcClientRole newRole);


    /** 远端用户加入当前频道回调。

     - 通信模式下，该回调提示有远端用户加入了频道，并返回新加入用户的 ID；如果加入之前，已经有其他用户在频道中了，新加入的用户也会收到这些已有用户加入频道的回调。

     @param uid 新加入频道的远端用户ID。
     @param user_name 新加入频道的远端用户名。
     */
    virtual void onUserJoined(nertc::uid_t uid, const char * user_name);

    /** 远端用户离开当前频道回调。

     提示有远端用户离开了频道（或掉线）。

     @param uid 远端用户ID。
     @param reason 远端用户离开原因。
     */
    virtual void onUserLeft(nertc::uid_t uid, nertc::NERtcSessionLeaveReason reason);

    /** 远端用户开启音频回调。
    
     @param uid 远端用户ID。
     */
    virtual void onUserAudioStart(nertc::uid_t uid);

    /** 远端用户停用音频回调。
     @param uid 远端用户ID。
     */
    virtual void onUserAudioStop(nertc::uid_t uid);

    /** 远端用户开启视频回调。

     @param uid 远端用户ID。
     @param max_profile 最大分辨率。
     */
    virtual void onUserVideoStart(nertc::uid_t uid, nertc::NERtcVideoProfileType max_profile);

    /** 远端用户停用视频回调。
     @param uid 远端用户ID。
     */
    virtual void onUserVideoStop(nertc::uid_t uid);


public:
    /** 远端用户开启辅流视频回调。
     @param uid 远端用户ID。
     @param max_profile 最大分辨率。
     */
    virtual void onUserSubStreamVideoStart(nertc::uid_t uid, nertc::NERtcVideoProfileType max_profile);

    /** 远端用户停用辅流视频回调。
     @param uid 远端用户ID。
     */
    virtual void onUserSubStreamVideoStop(nertc::uid_t uid);

    /** 远端用户视频配置更新回调。
     @param uid 远端用户ID。
     @param max_profile 最大分辨率。
     */
    virtual void onUserVideoProfileUpdate(nertc::uid_t uid, nertc::NERtcVideoProfileType max_profile);

    /** 远端用户是否静音回调。
     @param uid 远端用户ID。
     @param mute 是否静音。
     */
    virtual void onUserAudioMute(nertc::uid_t uid, bool mute);

    /** 远端用户是否禁视频流回调。
     @param uid 远端用户ID。
     @param mute 是否禁视频流。
     */
    virtual void onUserVideoMute(nertc::uid_t uid, bool mute);

    /** 音频设备状态更改回调。
     @param device_id 设备ID。
     @param device_type 音频设备类型。
     @param device_state 音频设备状态。
     */
    virtual void onAudioDeviceStateChanged(const char device_id[kNERtcMaxDeviceIDLength],
        nertc::NERtcAudioDeviceType device_type,
        nertc::NERtcAudioDeviceState device_state);

    /** 音频默认设备更改回调。
     @param device_id 设备ID。
     @param device_type 音频设备类型。
     */
    virtual void onAudioDefaultDeviceChanged(const char device_id[kNERtcMaxDeviceIDLength],
        nertc::NERtcAudioDeviceType device_type);

    /** 视频设备状态更改回调。
     @param device_id 设备ID。
     @param device_type 视频设备类型。
     @param device_state 视频设备状态。
     */
    virtual void onVideoDeviceStateChanged(const char device_id[kNERtcMaxDeviceIDLength],
        nertc::NERtcVideoDeviceType device_type,
        nertc::NERtcVideoDeviceState device_state);

    /** 已接收到远端音频首帧回调。
     @param uid 发送音频帧的远端用户的用户 ID。
     */
    virtual void onFirstAudioDataReceived(nertc::uid_t uid);

    /** 已显示首帧远端视频回调。
    第一帧远端视频显示在视图上时，触发此调用。
     @param uid 用户 ID，指定是哪个用户的视频流。
     */
    virtual void onFirstVideoDataReceived(nertc::uid_t uid);


    /** 已解码远端音频首帧的回调。
     @param uid 远端用户 ID。
     */
    virtual void onFirstAudioFrameDecoded(nertc::uid_t uid);

    /** 已接收到远端视频并完成解码回调。
    引擎收到第一帧远端视频流并解码成功时，触发此调用。 App 可在此回调中设置该用户的 video canvas。
     @param uid 用户 ID，指定是哪个用户的视频流。
     @param width 视频流宽（px）。
     @param height 视频流高（px）。

     */
    virtual void onFirstVideoFrameDecoded(nertc::uid_t uid, uint32_t width, uint32_t height);

    /** 采集视频数据回调。
     @param data 采集视频数据。
     @param type 视频类型。
     @param width 视频宽度。
     @param height 视频高度。
     @param count 视频Plane Count。
     @param offset 视频offset。
     @param stride 视频stride。
     @param rotation 视频旋转角度。
     */
    virtual void onCaptureVideoFrame(void *data,
        nertc::NERtcVideoType type, 
        uint32_t width, 
        uint32_t height,
        uint32_t count,
        uint32_t offset[kNERtcMaxPlaneCount],
        uint32_t stride[kNERtcMaxPlaneCount],
        nertc::NERtcVideoRotation rotation) ;

    /** 本地用户的音乐文件播放状态改变回调。

    调用 startAudioMixing 播放混音音乐文件后，当音乐文件的播放状态发生改变时，会触发该回调。

    - 如果播放音乐文件正常结束，state 会返回相应的状态码 kNERtcAudioMixingStateFinished，error_code 返回 kNERtcAudioMixingErrorOK。
    - 如果播放出错，则返回状态码 kNERtcAudioMixingStateFailed，error_code 返回相应的出错原因。
    - 如果本地音乐文件不存在、文件格式不支持、无法访问在线音乐文件 URL，error_code都会返回 kNERtcAudioMixingErrorCanNotOpen。

    @param state 音乐文件播放状态，详见 #NERtcAudioMixingState.
    @param error_code 错误码，详见 #NERtcAudioMixingErrorCode.
    */
    virtual void onAudioMixingStateChanged(nertc::NERtcAudioMixingState state, nertc::NERtcAudioMixingErrorCode error_code);

    /** 本地用户的音乐文件播放进度回调。
    调用 startAudioMixing 播放混音音乐文件后，当音乐文件的播放进度改变时，会触发该回调。
    @param timestamp_ms 音乐文件播放进度，单位为毫秒
    */
    virtual void onAudioMixingTimestampUpdate(uint64_t timestamp_ms);


    /** 本地音效文件播放已结束回调。
    当播放音效结束后，会触发该回调。
    @param effect_id 指定音效的 ID。每个音效均有唯一的 ID。
    */
    virtual void onAudioEffectFinished(uint32_t effect_id);

    /** 提示频道内本地用户瞬时音量的回调。
     该回调默认禁用。可以通过 enableAudioVolumeIndication 方法开启；
     开启后，本地用户说话，SDK 会按 enableAudioVolumeIndication 方法中设置的时间间隔触发该回调。
     如果本地用户将自己静音（调用了 muteLocalAudioStream），SDK 将音量设置为 0 后回调给应用层。
     @param volume （混音后的）音量，取值范围为 [0,100]。
     */
    virtual void onLocalAudioVolumeIndication(int volume);

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
    virtual void onRemoteAudioVolumeIndication(const nertc::NERtcAudioVolumeInfo *speakers, unsigned int speaker_number, int total_volume);

    /** 通知添加直播任务结果。
     该回调异步返回 \ref IRtcEngineEx::addLiveStreamTask "addLiveStreamTask" 接口的调用结果；实际推流状态参考 \ref IRtcEngineEventHandlerEx::onLiveStreamState "onLiveStreamState"
     @param task_id 任务id
     @param url 推流地址
     @param error_code 结果
     - 0: 调用成功；
     - 其他: 调用失败。
     */
    virtual void onAddLiveStreamTask(const char* task_id, const char* url, int error_code);

    /** 通知更新直播任务结果。
     该回调异步返回 \ref IRtcEngineEx::updateLiveStreamTask "updateLiveStreamTask" 接口的调用结果；实际推流状态参考 \ref IRtcEngineEventHandlerEx::onLiveStreamState "onLiveStreamState"
     @param task_id 任务id
     @param url 推流地址
     @param error_code 结果
     - 0: 调用成功；
     - 其他: 调用失败。
     */
    virtual void onUpdateLiveStreamTask(const char* task_id, const char* url, int error_code);

    /** 通知删除直播任务结果。
     该回调异步返回 \ref IRtcEngineEx::removeLiveStreamTask "removeLiveStreamTask" 接口的调用结果；实际推流状态参考 \ref IRtcEngineEventHandlerEx::onLiveStreamState "onLiveStreamState"
     @param task_id 任务id
     @param error_code 结果
     - 0: 调用成功；
     - 其他: 调用失败。
     */
    virtual void onRemoveLiveStreamTask(const char* task_id, int error_code);

    /** 通知直播推流状态
     @param task_id 任务id
     @param url 推流地址
     @param state #NERtcLiveStreamStateCode, 直播推流状态
     - 505: 推流中；
     - 506: 推流失败；
     - 511: 推流结束；
     */
    virtual void onLiveStreamState(const char* task_id, const char* url, nertc::NERtcLiveStreamStateCode state); 

    /** 监测音频啸叫的回调。
     @param howling 是否出现啸叫
     - true: 啸叫；
     - false: 正常；。
     */
    virtual void onAudioHowling(bool howling);

    /** 收到远端流的 SEI 内容回调。
     当远端成功发送 SEI 后，本端会收到此回调。
     * @param[in] uid 发送该 sei 的用户 id
     * @param[in] data 接收到的 sei 数据
     * @param[in] dataSize 接收到 sei 数据的大小
     */
    void onRecvSEIMsg(nertc::uid_t uid, const char* data, uint32_t dataSize) override;

    /**
     * 屏幕共享暂停/恢复/开始/结束等回调
     */
    virtual void onScreenCaptureStatus(nertc::NERtcScreenCaptureStatus status) override;

    /** 音频录制状态回调。
     * @param code 音频录制状态码。详细信息请参考 NERtcAudioRecordingCode。
     * @param file_path 音频录制文件保存路径。
     */
    virtual void onAudioRecording(nertc::NERtcAudioRecordingCode code, const char* file_path) override;

    /** Occurs when the state of the media stream is relayed. 

     @param state The state of the media stream.
     @param channel_name The name of the destination room where the media streams are relayed. 
     */
    virtual void onMediaRelayStateChanged(nertc::NERtcChannelMediaRelayState state, const char* channel_name) override;

    /** Occurs when events related to media stream relay are triggered.

     @param event The media stream relay event.
     @param channel_name The name of the destination room where the media streams are relayed.
     @param error  Specific error codes.
     */
    virtual void onMediaRelayEvent(nertc::NERtcChannelMediaRelayEvent event, const char* channel_name, nertc::NERtcErrorCode error) override;

    /**
     Occurs when the published local media stream falls back to an audio-only stream due to poor network conditions or switches back to audio and video stream after the network conditions improve.

     If you call setLocalPublishFallbackOption and set option to #kNERtcStreamFallbackAudioOnly, this callback is triggered when the locally published stream falls back to audio-only mode due to poor uplink network conditions, or when the audio stream switches back to the audio and video stream after the uplink network conditions improve. 

     @since V4.3.0
     @param is_fallback  The locally published stream falls back to audio-only mode or switches back to audio and video stream.
                         - true: The published stream from a local client falls back to audio-only mode due to poor uplink network conditions.
                         - false: The audio stream switches back to the audio and video stream after the upstream network condition improves.
     @param stream_type The type of the video stream, such as bigstream and substream. 
     */
    virtual void onLocalPublishFallbackToAudioOnly(bool is_fallback, nertc::NERtcVideoStreamType stream_type) override;
    
    /**
     Occurs when the subscribed remote media stream falls back to an audio-only stream due to poor network conditions or switches back to the audio and video stream after the network condition improves.

     If you call setLocalPublishFallbackOption and set option to #kNERtcStreamFallbackAudioOnly, this callback is triggered when the locally published stream falls back to audio-only mode due to poor uplink network conditions, or when the audio stream switches back to the audio and video stream after the uplink network condition improves.
     
     @since V4.3.0
     @param uid The ID of a remote user.
     @param is_fallback The subscribed remote media stream falls back to audio-only mode or switches back to the audio and video stream. 
                           - true: The subscribed remote media stream falls back to audio-only mode due to poor downstream network conditions.
                           - false: The subscribed remote media stream switches back to the audio and video stream after the downstream network condition improves.
     @param stream_type  The type of the video stream, such as bigstream and substream. 
     */
    virtual void onRemoteSubscribeFallbackToAudioOnly(nertc::uid_t uid, bool is_fallback, nertc::NERtcVideoStreamType stream_type) override;

public:
    void onPullExternalAudioFrame(Napi::FunctionReference&& function, const std::shared_ptr<unsigned char>& data, uint32_t length);


// public:
//     void addEvent(const std::string& eventName, Napi::FunctionReference&& function);
//     int removeAll();
// private:
//     std::unordered_map<std::string, EventCallback*> _callbacks;

private:
    void Node_onError(int error_code, std::string msg);

    void Node_onWarning(int warn_code, std::string msg);

    void Node_onReleasedHwResources(nertc::NERtcErrorCode result);

    void Node_onJoinChannel(nertc::channel_id_t cid, nertc::uid_t uid, nertc::NERtcErrorCode result, uint64_t elapsed);

    void Node_onConnectionStateChange(nertc::NERtcConnectionStateType state, nertc::NERtcReasonConnectionChangedType reason);

    void Node_onReconnectingStart(nertc::channel_id_t cid, nertc::uid_t uid);

    void Node_onRejoinChannel(nertc::channel_id_t cid, nertc::uid_t uid, nertc::NERtcErrorCode result, uint64_t elapsed);

    void Node_onLeaveChannel(nertc::NERtcErrorCode result);

    void Node_onDisconnect(nertc::NERtcErrorCode reason);

    void Node_onClientRoleChanged(nertc::NERtcClientRole oldRole, nertc::NERtcClientRole newRole);

    void Node_onUserJoined(nertc::uid_t uid, std::string user_name);
    
    void Node_onUserLeft(nertc::uid_t uid, nertc::NERtcSessionLeaveReason reason);
    
    void Node_onUserAudioStart(nertc::uid_t uid);

    void Node_onUserAudioStop(nertc::uid_t uid);

    void Node_onUserVideoStart(nertc::uid_t uid, nertc::NERtcVideoProfileType max_profile);

    void Node_onUserVideoStop(nertc::uid_t uid);

    void Node_onUserSubStreamVideoStart(nertc::uid_t uid, nertc::NERtcVideoProfileType max_profile);

    void Node_onUserSubStreamVideoStop(nertc::uid_t uid);

    void Node_onUserVideoProfileUpdate(nertc::uid_t uid, nertc::NERtcVideoProfileType max_profile);

    void Node_onUserAudioMute(nertc::uid_t uid, bool mute);

    void Node_onUserVideoMute(nertc::uid_t uid, bool mute);

    void Node_onAudioDeviceStateChanged(std::string device_id, nertc::NERtcAudioDeviceType device_type, nertc::NERtcAudioDeviceState device_state);

    void Node_onAudioDefaultDeviceChanged(std::string device_id, nertc::NERtcAudioDeviceType device_type);

    void Node_onVideoDeviceStateChanged(std::string device_id, nertc::NERtcVideoDeviceType device_type, nertc::NERtcVideoDeviceState device_state);

    void Node_onFirstAudioDataReceived(nertc::uid_t uid);
 
    void Node_onFirstVideoDataReceived(nertc::uid_t uid);

    void Node_onFirstAudioFrameDecoded(nertc::uid_t uid);

    void Node_onFirstVideoFrameDecoded(nertc::uid_t uid, uint32_t width, uint32_t height);

    void Node_onAudioMixingStateChanged(nertc::NERtcAudioMixingState state, nertc::NERtcAudioMixingErrorCode error_code);

    void Node_onAudioMixingTimestampUpdate(uint64_t timestamp_ms);

    void Node_onAudioEffectFinished(uint32_t effect_id);

    void Node_onLocalAudioVolumeIndication(int volume);

    void Node_onRemoteAudioVolumeIndication(const nertc::NERtcAudioVolumeInfo *speakers, unsigned int speaker_number, int total_volume);

    void Node_onAddLiveStreamTask(std::string task_id, std::string url, int error_code);

    void Node_onUpdateLiveStreamTask(std::string task_id, std::string url, int error_code);

    void Node_onRemoveLiveStreamTask(std::string task_id, int error_code);

    void Node_onLiveStreamState(std::string task_id, std::string url, nertc::NERtcLiveStreamStateCode state);

    void Node_onAudioHowling(bool howling);

    void Node_onRecvSEIMsg(nertc::uid_t uid, const char* data, uint32_t dataSize);

    void Node_onScreenCaptureStatus(nertc::NERtcScreenCaptureStatus status);

    void Node_onAudioRecording(nertc::NERtcAudioRecordingCode code, std::string file_path);

    void Node_onMediaRelayStateChanged(nertc::NERtcChannelMediaRelayState state, std::string channel_name);

    void Node_onMediaRelayEvent(nertc::NERtcChannelMediaRelayEvent event, std::string channel_name, nertc::NERtcErrorCode error);

    void Node_onLocalPublishFallbackToAudioOnly(bool is_fallback, nertc::NERtcVideoStreamType stream_type);

    void Node_onRemoteSubscribeFallbackToAudioOnly(nertc::uid_t uid, bool is_fallback, nertc::NERtcVideoStreamType stream_type);
    
};

class NertcNodeRtcMediaStatsHandler : public EventHandler, public nertc::IRtcMediaStatsObserver
{
private:
    /* data */
public:
    NertcNodeRtcMediaStatsHandler(){};
    ~NertcNodeRtcMediaStatsHandler(){};

public:
    /** 当前通话统计回调。

    SDK 定期向 App 报告当前通话的统计信息，每 2 秒触发一次。

     @param stats NERTC 引擎统计数据: NERtcStats
     */
    virtual void onRtcStats(const nertc::NERtcStats &stats);

    /** 本地音频流统计信息回调。

    该回调描述本地设备发送音频流的统计信息，每 2 秒触发一次。

     @param stats 本地音频流统计信息。详见 NERtcAudioSendStats.
     */
    virtual void onLocalAudioStats(const nertc::NERtcAudioSendStats &stats);

    /** 通话中远端音频流的统计信息回调。

     该回调描述远端用户在通话中端到端的音频流统计信息，每 2 秒触发一次。

     @param stats 每个远端用户音频统计信息的数组。详见 NERtcAudioRecvStats.
     @param user_count stats 数组的大小。
     */
    virtual void onRemoteAudioStats(const nertc::NERtcAudioRecvStats *stats, unsigned int user_count);

    /** 本地视频流统计信息回调。

    该回调描述本地设备发送视频流的统计信息，每 2 秒触发一次。

     @param stats 本地视频流统计信息。详见 NERtcVideoSendStats.
     */
    virtual void onLocalVideoStats(const nertc::NERtcVideoSendStats &stats) ;

    /** 通话中远端视频流的统计信息回调。

     该回调描述远端用户在通话中端到端的视频流统计信息，每 2 秒触发一次。

     @param stats 每个远端用户视频统计信息的数组。详见 NERtcVideoRecvStats.
     @param user_count stats 数组的大小。
     */
    virtual void onRemoteVideoStats(const nertc::NERtcVideoRecvStats *stats, unsigned int user_count);

    /** 通话中每个用户的网络上下行质量报告回调。

     该回调描述每个用户在通话中的网络状态，每 2 秒触发一次，只上报状态有变更的成员。

     @param infos 每个用户 ID 和网络上下行质量信息的数组: NERtcNetworkQualityInfo
     @param user_count infos 数组的大小，即用户数。
     */
    virtual void onNetworkQuality(const nertc::NERtcNetworkQualityInfo *infos, unsigned int user_count);


private:
    void Node_onRtcStats(const nertc::NERtcStats & stats);

    void Node_onLocalAudioStats(const nertc::NERtcAudioSendStats & stats);

    void Node_onRemoteAudioStats(const nertc::NERtcAudioRecvStats *stats, unsigned int user_count);

    void Node_onLocalVideoStats(nertc::NERtcVideoSendStats & ss);

    void Node_onRemoteVideoStats(nertc::NERtcVideoRecvStats *ss, unsigned int user_count);

    void Node_onNetworkQuality(const nertc::NERtcNetworkQualityInfo *ss, unsigned int user_count);

}; 





}
#endif //NERTC_NODE_ENGINE_EVENT_HANDLER_H