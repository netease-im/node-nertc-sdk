#ifndef NERTC_NODE_ENGINE_EVENT_HANDLER_H
#define NERTC_NODE_ENGINE_EVENT_HANDLER_H

#include <napi.h>
#include <unordered_map>
#include "nertc_node_engine_helper.h"
#include "nertc_engine_event_handler_ex.h"
#include "nertc_engine_media_stats_observer.h"
#include "nertc_engine_video_encoder_qos_observer.h"


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
    virtual void onError(int error_code, const char* msg) override;
    virtual void onWarning(int warn_code, const char* msg) override;
    virtual void onReleasedHwResources(nertc::NERtcErrorCode result) override;
    virtual void onJoinChannel(nertc::channel_id_t cid, nertc::uid_t uid, nertc::NERtcErrorCode result, uint64_t elapsed) override;
    virtual void onConnectionStateChange(nertc::NERtcConnectionStateType state, nertc::NERtcReasonConnectionChangedType reason) override;
    virtual void onReconnectingStart(nertc::channel_id_t cid, nertc::uid_t uid) override;
    virtual void onRejoinChannel(nertc::channel_id_t cid, nertc::uid_t uid, nertc::NERtcErrorCode result, uint64_t elapsed) override;
    virtual void onLeaveChannel(nertc::NERtcErrorCode result) override;
    virtual void onDisconnect(nertc::NERtcErrorCode reason) override;
    virtual void onClientRoleChanged(nertc::NERtcClientRole oldRole, nertc::NERtcClientRole newRole) override;
    virtual void onUserJoined(nertc::uid_t uid, const char * user_name) override;
    virtual void onUserJoined(nertc::uid_t uid, const char* user_name, nertc::NERtcUserJoinExtraInfo join_extra_info) override;
    virtual void onUserLeft(nertc::uid_t uid, nertc::NERtcSessionLeaveReason reason) override;
    virtual void onUserLeft(nertc::uid_t uid, nertc::NERtcSessionLeaveReason reason, nertc::NERtcUserJoinExtraInfo leave_extra_info) override;
    virtual void onUserAudioStart(nertc::uid_t uid) override;
    virtual void onUserAudioStop(nertc::uid_t uid) override;
    virtual void onUserVideoStart(nertc::uid_t uid, nertc::NERtcVideoProfileType max_profile) override;
    virtual void onUserVideoStop(nertc::uid_t uid) override;
    virtual void onUserSubStreamVideoStart(nertc::uid_t uid, nertc::NERtcVideoProfileType max_profile) override;
    virtual void onUserSubStreamVideoStop(nertc::uid_t uid) override;
    virtual void onUserVideoProfileUpdate(nertc::uid_t uid, nertc::NERtcVideoProfileType max_profile) override;
    virtual void onUserAudioMute(nertc::uid_t uid, bool mute) override;
    virtual void onUserVideoMute(nertc::uid_t uid, bool mute) override;
    virtual void onAudioDeviceStateChanged(const char device_id[kNERtcMaxDeviceIDLength],
        nertc::NERtcAudioDeviceType device_type,
        nertc::NERtcAudioDeviceState device_state) override;
    virtual void onAudioDefaultDeviceChanged(const char device_id[kNERtcMaxDeviceIDLength],
        nertc::NERtcAudioDeviceType device_type) override;
    virtual void onVideoDeviceStateChanged(const char device_id[kNERtcMaxDeviceIDLength],
        nertc::NERtcVideoDeviceType device_type,
        nertc::NERtcVideoDeviceState device_state) override;
    virtual void onFirstAudioDataReceived(nertc::uid_t uid) override;
    virtual void onFirstVideoDataReceived(nertc::uid_t uid) override;
    virtual void onFirstVideoDataReceived(nertc::NERtcVideoStreamType type, nertc::uid_t uid) override;
    virtual void onFirstAudioFrameDecoded(nertc::uid_t uid) override;
    virtual void onFirstVideoFrameDecoded(nertc::uid_t uid, uint32_t width, uint32_t height) override;
    virtual void onFirstVideoFrameDecoded(nertc::NERtcVideoStreamType type, nertc::uid_t uid, uint32_t width, uint32_t height) override;
    virtual void onCaptureVideoFrame(void *data,
        nertc::NERtcVideoType type, 
        uint32_t width, 
        uint32_t height,
        uint32_t count,
        uint32_t offset[kNERtcMaxPlaneCount],
        uint32_t stride[kNERtcMaxPlaneCount],
        nertc::NERtcVideoRotation rotation) override;
    virtual void onAudioMixingStateChanged(nertc::NERtcAudioMixingState state, nertc::NERtcAudioMixingErrorCode error_code) override;
    virtual void onAudioMixingTimestampUpdate(uint64_t timestamp_ms) override;
    virtual void onAudioEffectTimestampUpdate(uint32_t effect_id, uint64_t timestamp_ms) override;
    virtual void onAudioEffectFinished(uint32_t effect_id) override;
    virtual void onLocalAudioVolumeIndication(int volume) override;
    virtual void onLocalAudioVolumeIndication(int volume, bool enable_vad) override;
    virtual void onRemoteAudioVolumeIndication(const nertc::NERtcAudioVolumeInfo *speakers, unsigned int speaker_number, int total_volume) override;
    virtual void onAddLiveStreamTask(const char* task_id, const char* url, int error_code) override;
    virtual void onUpdateLiveStreamTask(const char* task_id, const char* url, int error_code) override;
    virtual void onRemoveLiveStreamTask(const char* task_id, int error_code) override;
    virtual void onLiveStreamState(const char* task_id, const char* url, nertc::NERtcLiveStreamStateCode state) override; 
    virtual void onAudioHowling(bool howling) override;
    virtual void onRecvSEIMsg(nertc::uid_t uid, const char* data, uint32_t dataSize) override;
    virtual void onScreenCaptureStatus(nertc::NERtcScreenCaptureStatus status) override;
    virtual void onAudioRecording(nertc::NERtcAudioRecordingCode code, const char* file_path) override;
    virtual void onMediaRelayStateChanged(nertc::NERtcChannelMediaRelayState state, const char* channel_name) override;
    virtual void onMediaRelayEvent(nertc::NERtcChannelMediaRelayEvent event, const char* channel_name, nertc::NERtcErrorCode error) override;
    virtual void onLocalPublishFallbackToAudioOnly(bool is_fallback, nertc::NERtcVideoStreamType stream_type) override;
    virtual void onRemoteSubscribeFallbackToAudioOnly(nertc::uid_t uid, bool is_fallback, nertc::NERtcVideoStreamType stream_type) override;
    virtual void onLastmileQuality(nertc::NERtcNetworkQualityType quality) override;
    virtual void onLastmileProbeResult(const nertc::NERtcLastmileProbeResult& result) override;
    virtual void onUserSubStreamAudioStart(nertc::uid_t uid) override;
    virtual void onUserSubStreamAudioStop(nertc::uid_t uid) override;
    virtual void onUserSubStreamAudioMute(nertc::uid_t uid, bool mute) override;
    virtual void onUserVideoMute(nertc::NERtcVideoStreamType videoStreamType, nertc::uid_t uid, bool mute) override;
    virtual void onMediaRightChange(bool is_audio_banned, bool is_video_banned) override;
    virtual void onCheckNECastAudioDriverResult(nertc::NERtcInstallCastAudioDriverResult result) override;
    virtual void onVirtualBackgroundSourceEnabled(bool enabled, nertc::NERtcVirtualBackgroundSourceStateReason reason) override;
    // virtual void onLocalVideoWatermarkState(nertc::NERtcVideoStreamType videoStreamType, nertc::NERtcLocalVideoWatermarkState state) override;
    virtual void onUserDataReceiveMessage(nertc::uid_t uid, void* pData, uint64_t size) override;
    virtual void onUserDataStart(nertc::uid_t uid) override;
    virtual void onUserDataStop(nertc::uid_t uid) override;
    virtual void onUserDataStateChanged(nertc::uid_t uid) override;
    virtual void onUserDataBufferedAmountChanged(nertc::uid_t uid, uint64_t previousAmount) override;
    virtual void onPermissionKeyWillExpire() override;
    virtual void onUpdatePermissionKey(const char* key, nertc::NERtcErrorCode error, int timeout) override;
    virtual void onApiCallExecuted(const char* api_name, nertc::NERtcErrorCode error, const char* message) override;
    virtual void onRemoteVideoReceiveSizeChanged(nertc::uid_t uid, nertc::NERtcVideoStreamType type, uint32_t width, uint32_t height) override;
    virtual void onLocalVideoRenderSizeChanged(nertc::NERtcVideoStreamType type, uint32_t width, uint32_t height) override;
    virtual void onFirstVideoFrameRender(nertc::NERtcVideoStreamType type, nertc::uid_t uid, uint32_t width, uint32_t height, uint64_t elapsed) override;
    virtual void onLabFeatureCallback(const char* key, const char* param) override;
    
public:
    void onPullExternalAudioFrame(Napi::FunctionReference&& function, const std::shared_ptr<unsigned char>& data, uint32_t length);

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
    void Node_onUserJoined(nertc::uid_t uid, std::string user_name, nertc::NERtcUserJoinExtraInfo join_extra_info);
    void Node_onUserLeft(nertc::uid_t uid, nertc::NERtcSessionLeaveReason reason);
    void Node_onUserLeft(nertc::uid_t uid, nertc::NERtcSessionLeaveReason reason, nertc::NERtcUserJoinExtraInfo leave_extra_info);
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
    void Node_onFirstVideoDataReceived(nertc::NERtcVideoStreamType type, nertc::uid_t uid);
    void Node_onFirstAudioFrameDecoded(nertc::uid_t uid);
    void Node_onFirstVideoFrameDecoded(nertc::uid_t uid, uint32_t width, uint32_t height);
    void Node_onFirstVideoFrameDecoded(nertc::NERtcVideoStreamType type, nertc::uid_t uid, uint32_t width, uint32_t height);
    void Node_onAudioMixingStateChanged(nertc::NERtcAudioMixingState state, nertc::NERtcAudioMixingErrorCode error_code);
    void Node_onAudioMixingTimestampUpdate(uint64_t timestamp_ms);
    void Node_onAudioEffectTimestampUpdate(uint32_t effect_id, uint64_t timestamp_ms);
    void Node_onAudioEffectFinished(uint32_t effect_id);
    void Node_onLocalAudioVolumeIndication(int volume);
    void Node_onLocalAudioVolumeIndication(int volume, bool enable_vad);
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
    void Node_onLastmileQuality(nertc::NERtcNetworkQualityType quality);
    void Node_onLastmileProbeResult(const nertc::NERtcLastmileProbeResult& result);
    void Node_onUserSubStreamAudioStart(nertc::uid_t uid);
    void Node_onUserSubStreamAudioStop(nertc::uid_t uid);
    void Node_onUserSubStreamAudioMute(nertc::uid_t uid, bool mute);
    void Node_onUserVideoMute(nertc::NERtcVideoStreamType videoStreamType, nertc::uid_t uid, bool mute);
    void Node_onMediaRightChange(bool is_audio_banned, bool is_video_banned);
    void Node_onCheckNECastAudioDriverResult(nertc::NERtcInstallCastAudioDriverResult result);
    void Node_onVirtualBackgroundSourceEnabled(bool enabled, nertc::NERtcVirtualBackgroundSourceStateReason reason);
    void Node_onLocalVideoWatermarkState(nertc::NERtcVideoStreamType videoStreamType, nertc::NERtcLocalVideoWatermarkState state);
    void Node_onUpdatePermissionKey(std::string key, nertc::NERtcErrorCode error, int timeout);
    void Node_onUserDataReceiveMessage(nertc::uid_t uid, void* pData, uint64_t size);
    void Node_onUserDataStart(nertc::uid_t uid);
    void Node_onUserDataStop(nertc::uid_t uid);
    void Node_onUserDataStateChanged(nertc::uid_t uid);
    void Node_onUserDataBufferedAmountChanged(nertc::uid_t uid, uint64_t previousAmount);
    void Node_onPermissionKeyWillExpire();
    void Node_onApiCallExecuted(std::string api_name, nertc::NERtcErrorCode error, std::string message);
    void Node_onRemoteVideoReceiveSizeChanged(nertc::uid_t uid, nertc::NERtcVideoStreamType type, uint32_t width, uint32_t height);
    void Node_onLocalVideoRenderSizeChanged(nertc::NERtcVideoStreamType type, uint32_t width, uint32_t height);
    void Node_onFirstVideoFrameRender(nertc::NERtcVideoStreamType type, nertc::uid_t uid, uint32_t width, uint32_t height, uint64_t elapsed);
    void Node_onLabFeatureCallback(std::string key, std::string param);
    
    
};

class NertcNodeRtcMediaStatsHandler : public EventHandler, public nertc::IRtcMediaStatsObserver
{
private:
    /* data */
public:
    NertcNodeRtcMediaStatsHandler(){};
    ~NertcNodeRtcMediaStatsHandler(){};


    virtual void onRtcStats(const nertc::NERtcStats &stats) override;
    virtual void onLocalAudioStats(const nertc::NERtcAudioSendStats &stats) override;
    virtual void onRemoteAudioStats(const nertc::NERtcAudioRecvStats *stats, unsigned int user_count) override;
    virtual void onLocalVideoStats(const nertc::NERtcVideoSendStats &stats) override;
    virtual void onRemoteVideoStats(const nertc::NERtcVideoRecvStats *stats, unsigned int user_count) override;
    virtual void onNetworkQuality(const nertc::NERtcNetworkQualityInfo *infos, unsigned int user_count) override;
private:
    void Node_onRtcStats(const nertc::NERtcStats & stats);
    void Node_onLocalAudioStats(const nertc::NERtcAudioSendStats & stats);
    void Node_onRemoteAudioStats(const nertc::NERtcAudioRecvStats *stats, unsigned int user_count);
    void Node_onLocalVideoStats(const nertc::NERtcVideoSendStats & ss);
    void Node_onRemoteVideoStats(const nertc::NERtcVideoRecvStats *ss, unsigned int user_count);
    void Node_onNetworkQuality(const nertc::NERtcNetworkQualityInfo *ss, unsigned int user_count);

};

class NertcNodeVideoEncoderQosObserver : public EventHandler, public nertc::INERtcVideoEncoderQosObserver
{

public:
    NertcNodeVideoEncoderQosObserver(){};
    ~NertcNodeVideoEncoderQosObserver(){};

public:
    virtual void onRequestSendKeyFrame(nertc::NERtcVideoStreamType video_stream_type) override;
    virtual void onBitrateUpdated(uint32_t bitrate_bps, nertc::NERtcVideoStreamType video_stream_type) override;
    virtual void onVideoCodecUpdated(nertc::NERtcVideoCodecType video_codec_type, nertc::NERtcVideoStreamType video_stream_type) override;

private:
    void Node_onRequestSendKeyFrame(nertc::NERtcVideoStreamType video_stream_type);
    void Node_onBitrateUpdated(uint32_t bitrate_bps, nertc::NERtcVideoStreamType video_stream_type);
    void Node_onVideoCodecUpdated(nertc::NERtcVideoCodecType video_codec_type, nertc::NERtcVideoStreamType video_stream_type);

};

class NertcNodeAudioFrameObserverHandler : public EventHandler, public nertc::INERtcAudioFrameObserver
{
private:
    /* data */
public:
    NertcNodeAudioFrameObserverHandler(){};
    ~NertcNodeAudioFrameObserverHandler(){};

public:
    virtual void onAudioFrameDidRecord(nertc::NERtcAudioFrame* frame) override;
    
    virtual void onSubStreamAudioFrameDidRecord(nertc::NERtcAudioFrame* frame) override;

    virtual void onAudioFrameWillPlayback(nertc::NERtcAudioFrame* frame) override;

    virtual void onMixedAudioFrame(nertc::NERtcAudioFrame* frame) override;

    virtual void onPlaybackAudioFrameBeforeMixing(uint64_t userID, nertc::NERtcAudioFrame* frame) override;

    virtual void onPlaybackAudioFrameBeforeMixing(uint64_t userID, nertc::NERtcAudioFrame* frame, nertc::channel_id_t cid) override;

    virtual void onPlaybackSubStreamAudioFrameBeforeMixing(uint64_t userID, nertc::NERtcAudioFrame* frame,
                                                           nertc::channel_id_t cid) override;


private:
    void Node_onAudioFrameDidRecord(nertc::NERtcAudioFrame* frame);
    void Node_onMixedAudioFrame(nertc::NERtcAudioFrame* frame);

    void Node_onAudioFrameWillPlayback(nertc::NERtcAudioFrame* frame);


};


}
#endif //NERTC_NODE_ENGINE_EVENT_HANDLER_H