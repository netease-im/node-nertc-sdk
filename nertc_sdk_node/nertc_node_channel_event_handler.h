#ifndef NERTC_NODE_CHANNEL_EVENT_HANDLER_H
#define NERTC_NODE_CHANNEL_EVENT_HANDLER_H

#include <napi.h>
#include <unordered_map>
#include "nertc_node_engine_helper.h"
#include "./nertc_node_engine_event_handler.h"
#include "nertc_channel_event_handler.h"

namespace nertc_node
{

class NertcChannelEventHandler : public EventHandler, public nertc::IRtcChannelEventHandler
{

public:
    NertcChannelEventHandler(){};
    ~NertcChannelEventHandler(){};

public:
    virtual void onError(int error_code, const char* msg) override;
    virtual void onWarning(int warn_code, const char* msg) override;
    virtual void onJoinChannel(nertc::channel_id_t cid, nertc::uid_t uid, nertc::NERtcErrorCode result, uint64_t elapsed) override;
    virtual void onReconnectingStart(nertc::channel_id_t cid, nertc::uid_t uid) override;
    virtual void onConnectionStateChange(nertc::NERtcConnectionStateType state, nertc::NERtcReasonConnectionChangedType reason) override;
    virtual void onRejoinChannel(nertc::channel_id_t cid, nertc::uid_t uid, nertc::NERtcErrorCode result, uint64_t elapsed) override;
    virtual void onLeaveChannel(nertc::NERtcErrorCode result) override;
    virtual void onDisconnect(nertc::NERtcErrorCode reason) override;
    virtual void onClientRoleChanged(nertc::NERtcClientRole oldRole, nertc::NERtcClientRole newRole) override;
    virtual void onUserJoined(nertc::uid_t uid, const char * user_name) override;
    virtual void onUserLeft(nertc::uid_t uid, nertc::NERtcSessionLeaveReason reason) override;
    virtual void onUserAudioStart(nertc::uid_t uid) override;
    virtual void onUserAudioStop(nertc::uid_t uid) override;
    virtual void onUserAudioMute(nertc::uid_t uid, bool mute) override;
    virtual void onUserSubStreamAudioStart(nertc::uid_t uid) override;
    virtual void onUserSubStreamAudioStop(nertc::uid_t uid) override;
    virtual void onUserSubStreamAudioMute(nertc::uid_t uid, bool mute) override;
    virtual void onUserVideoStart(nertc::uid_t uid, nertc::NERtcVideoProfileType max_profile) override;
    virtual void onUserVideoStop(nertc::uid_t uid) override;
    virtual void onUserVideoMute(nertc::uid_t uid, bool mute) override;
    virtual void onUserVideoMute(nertc::NERtcVideoStreamType videoStreamType,nertc::uid_t uid, bool mute) override;
    virtual void onUserSubStreamVideoStart(nertc::uid_t uid, nertc::NERtcVideoProfileType max_profile) override;
    virtual void onUserSubStreamVideoStop(nertc::uid_t uid) override;
    virtual void onScreenCaptureStatus(nertc::NERtcScreenCaptureStatus status) override;
    virtual void onFirstAudioDataReceived(nertc::uid_t uid) override;
    virtual void onFirstVideoDataReceived(nertc::uid_t uid) override;
    virtual void onFirstVideoDataReceived(nertc::NERtcVideoStreamType type, nertc::uid_t uid) override;
    virtual void onFirstAudioFrameDecoded(nertc::uid_t uid) override;
    virtual void onFirstVideoFrameDecoded(nertc::uid_t uid, uint32_t width, uint32_t height) override; 
    virtual void onFirstVideoFrameDecoded(nertc::NERtcVideoStreamType type,nertc::uid_t uid, uint32_t width, uint32_t height) override;
    virtual void onLocalAudioVolumeIndication(int volume) override;
    virtual void onLocalAudioVolumeIndication(int volume, bool enable_vad) override;
    virtual void onRemoteAudioVolumeIndication(const nertc::NERtcAudioVolumeInfo *speakers, unsigned int speaker_number, int total_volume) override;
    virtual void onAddLiveStreamTask(const char* task_id, const char* url, int error_code) override;
    virtual void onUpdateLiveStreamTask(const char* task_id, const char* url, int error_code) override;
    virtual void onRemoveLiveStreamTask(const char* task_id, int error_code) override;
    virtual void onLiveStreamState(const char* task_id, const char* url, nertc::NERtcLiveStreamStateCode state) override; 
    virtual void onRecvSEIMsg(nertc::uid_t uid, const char* data, uint32_t dataSize) override;
    virtual void onMediaRelayStateChanged(nertc::NERtcChannelMediaRelayState state, const char* channel_name) override;
    virtual void onMediaRelayEvent(nertc::NERtcChannelMediaRelayEvent event, const char* channel_name, nertc::NERtcErrorCode error) override;
    virtual void onLocalPublishFallbackToAudioOnly(bool is_fallback, nertc::NERtcVideoStreamType stream_type) override;
    virtual void onRemoteSubscribeFallbackToAudioOnly(nertc::uid_t uid, bool is_fallback, nertc::NERtcVideoStreamType stream_type) override;
    virtual void onMediaRightChange(bool is_audio_banned, bool is_video_banned) override;
    virtual void onApiCallExecuted(const char* api_name, nertc::NERtcErrorCode error, const char* message) override;
    virtual void onUserJoined(nertc::uid_t uid, const char* user_name, nertc::NERtcUserJoinExtraInfo join_extra_info) override;
    virtual void onUserLeft(nertc::uid_t uid, nertc::NERtcSessionLeaveReason reason, nertc::NERtcUserJoinExtraInfo leave_extra_info) override;
    virtual void onPermissionKeyWillExpire() override;
    virtual void onUpdatePermissionKey(const char* key, nertc::NERtcErrorCode error, int timeout) override; 

private:
    void Node_onError(int error_code, std::string msg);
    void Node_onWarning(int warn_code, std::string msg);
    void Node_onJoinChannel(nertc::channel_id_t cid, nertc::uid_t uid, nertc::NERtcErrorCode result, uint64_t elapsed);
    void Node_onReconnectingStart(nertc::channel_id_t cid, nertc::uid_t uid);
    void Node_onConnectionStateChange(nertc::NERtcConnectionStateType state, nertc::NERtcReasonConnectionChangedType reason);
    void Node_onRejoinChannel(nertc::channel_id_t cid, nertc::uid_t uid, nertc::NERtcErrorCode result, uint64_t elapsed);
    void Node_onLeaveChannel(nertc::NERtcErrorCode result);
    void Node_onDisconnect(nertc::NERtcErrorCode reason);
    void Node_onClientRoleChanged(nertc::NERtcClientRole oldRole, nertc::NERtcClientRole newRole);
    void Node_onUserJoined(nertc::uid_t uid, std::string user_name);
    void Node_onUserLeft(nertc::uid_t uid, nertc::NERtcSessionLeaveReason reason);
    void Node_onUserAudioStart(nertc::uid_t uid);
    void Node_onUserAudioStop(nertc::uid_t uid);
    void Node_onUserAudioMute(nertc::uid_t uid, bool mute);
    void Node_onUserSubStreamAudioStart(nertc::uid_t uid);
    void Node_onUserSubStreamAudioStop(nertc::uid_t uid);
    void Node_onUserSubStreamAudioMute(nertc::uid_t uid, bool mute);
    void Node_onUserVideoStart(nertc::uid_t uid, nertc::NERtcVideoProfileType max_profile);
    void Node_onUserVideoStop(nertc::uid_t uid);
    void Node_onUserVideoMute(nertc::uid_t uid, bool mute);
    void Node_onUserVideoMuteEx(nertc::NERtcVideoStreamType videoStreamType,nertc::uid_t uid, bool mute);
    void Node_onUserSubStreamVideoStart(nertc::uid_t uid, nertc::NERtcVideoProfileType max_profile);
    void Node_onUserSubStreamVideoStop(nertc::uid_t uid);
    void Node_onScreenCaptureStatus(nertc::NERtcScreenCaptureStatus status);
    void Node_onFirstAudioDataReceived(nertc::uid_t uid);
    void Node_onFirstVideoDataReceived(nertc::uid_t uid);
    void Node_onFirstVideoDataReceived(nertc::NERtcVideoStreamType type, nertc::uid_t uid);
    void Node_onFirstAudioFrameDecoded(nertc::uid_t uid);
    void Node_onFirstVideoFrameDecoded(nertc::uid_t uid, uint32_t width, uint32_t height);
    void Node_onFirstVideoFrameDecoded(nertc::NERtcVideoStreamType type, nertc::uid_t uid, uint32_t width, uint32_t height);
    void Node_onLocalAudioVolumeIndication(int volume);
    void Node_onLocalAudioVolumeIndicationEx(int volume, bool enable_vad);
    void Node_onRemoteAudioVolumeIndication(const nertc::NERtcAudioVolumeInfo *speakers, unsigned int speaker_number, int total_volume);
    void Node_onAddLiveStreamTask(std::string task_id, std::string url, int error_code);
    void Node_onUpdateLiveStreamTask(std::string task_id, std::string url, int error_code);
    void Node_onRemoveLiveStreamTask(std::string task_id, int error_code);
    void Node_onLiveStreamState(std::string task_id, std::string url, nertc::NERtcLiveStreamStateCode state);
    void Node_onRecvSEIMsg(nertc::uid_t uid, const char* data, uint32_t dataSize);
    void Node_onMediaRelayStateChanged(nertc::NERtcChannelMediaRelayState state, std::string channel_name);
    void Node_onMediaRelayEvent(nertc::NERtcChannelMediaRelayEvent event, std::string channel_name, nertc::NERtcErrorCode error);
    void Node_onLocalPublishFallbackToAudioOnly(bool is_fallback, nertc::NERtcVideoStreamType stream_type);
    void Node_onRemoteSubscribeFallbackToAudioOnly(nertc::uid_t uid, bool is_fallback, nertc::NERtcVideoStreamType stream_type);
    void Node_onMediaRightChange(bool is_audio_banned, bool is_video_banned);
    void Node_onApiCallExecuted(std::string api_name, nertc::NERtcErrorCode error, std::string message);
    void Node_onUserJoined(nertc::uid_t uid, std::string user_name, nertc::NERtcUserJoinExtraInfo join_extra_info);
    void Node_onUserLeft(nertc::uid_t uid, nertc::NERtcSessionLeaveReason reason, nertc::NERtcUserJoinExtraInfo leave_extra_info);
    void Node_onPermissionKeyWillExpire();
    void Node_onUpdatePermissionKey(std::string key, nertc::NERtcErrorCode error, int timeout);


};


class NertcChannelRtcMediaStatsHandler : public EventHandler, public nertc::IRtcMediaStatsObserver
{

private:
    /* data */
public:
    NertcChannelRtcMediaStatsHandler(){};
    ~NertcChannelRtcMediaStatsHandler(){};

public:
    virtual void onRtcStats(const nertc::NERtcStats &stats);
    virtual void onLocalAudioStats(const nertc::NERtcAudioSendStats &stats);
    virtual void onRemoteAudioStats(const nertc::NERtcAudioRecvStats *stats, unsigned int user_count);
    virtual void onLocalVideoStats(const nertc::NERtcVideoSendStats &stats) ;
    virtual void onRemoteVideoStats(const nertc::NERtcVideoRecvStats *stats, unsigned int user_count);
    virtual void onNetworkQuality(const nertc::NERtcNetworkQualityInfo *infos, unsigned int user_count);

private:
    void Node_onRtcStats(const nertc::NERtcStats & stats);
    void Node_onLocalAudioStats(const nertc::NERtcAudioSendStats & stats);
    void Node_onRemoteAudioStats(const nertc::NERtcAudioRecvStats *stats, unsigned int user_count);
    void Node_onLocalVideoStats(const nertc::NERtcVideoSendStats & ss);
    void Node_onRemoteVideoStats(const nertc::NERtcVideoRecvStats *ss, unsigned int user_count);
    void Node_onNetworkQuality(const nertc::NERtcNetworkQualityInfo *ss, unsigned int user_count);


};

}
#endif //#ifndef NERTC_NODE_CHANNEL_EVENT_HANDLER_H
