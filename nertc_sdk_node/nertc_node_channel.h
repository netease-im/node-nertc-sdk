#include <napi.h>
#include <memory>
#include "nertc_engine_ex.h"
#include "nertc_node_engine.h"

namespace nertc_node
{

class NertcNodeChannel : public Napi::ObjectWrap<NertcNodeChannel> {

#define NIM_SDK_NODE_API(m) \
Napi::Value (m)(const Napi::CallbackInfo& info)

public:
#if NAPI_VERSION < 6
    static Napi::FunctionReference constructor;
#endif
    NertcNodeChannel(const Napi::CallbackInfo& info);
    virtual ~NertcNodeChannel();

public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);


public:
    //NIM_SDK_NODE_API(release);
    NIM_SDK_NODE_API(getChannelName);
    //NIM_SDK_NODE_API(setChannelEventHandler);
    NIM_SDK_NODE_API(joinChannel);
    // NIM_SDK_NODE_API(joinChannel2); joinChannel(const char* token, NERtcJoinChannelOptions channel_options)
    NIM_SDK_NODE_API(leaveChannel);
    // NIM_SDK_NODE_API(); setStatsObserver
    NIM_SDK_NODE_API(enableLocalAudio);
    NIM_SDK_NODE_API(enableLocalSubStreamAudio);
    NIM_SDK_NODE_API(muteLocalAudioStream);
    NIM_SDK_NODE_API(muteLocalSubStreamAudio);
    NIM_SDK_NODE_API(enableLocalVideo);
    // NIM_SDK_NODE_API(enableLocalVideo);
    NIM_SDK_NODE_API(muteLocalVideoStream);
    // NIM_SDK_NODE_API(muteLocalVideoStream);    virtual int muteLocalVideoStream(NERtcVideoStreamType type, bool mute) = 0;
    NIM_SDK_NODE_API(setScreenCaptureMouseCursor);
    NIM_SDK_NODE_API(stopScreenCapture);
    NIM_SDK_NODE_API(pauseScreenCapture);
    NIM_SDK_NODE_API(resumeScreenCapture);
    // NIM_SDK_NODE_API(); setExcludeWindowList
    // NIM_SDK_NODE_API(); updateScreenCaptureParameters
    // NIM_SDK_NODE_API();setupLocalVideoCanvas
    // NIM_SDK_NODE_API(); setupLocalSubStreamVideoCanvas
    // NIM_SDK_NODE_API(setLocalVideoMirrorMode);
    // NIM_SDK_NODE_API();  int setLocalVideoMirrorMode(NERtcVideoStreamType type, NERtcVideoMirrorMode mirror_mode)
    // NIM_SDK_NODE_API(setupRemoteVideoCanvas);
    // NIM_SDK_NODE_API(setupRemoteSubStreamVideoCanvas);
    NIM_SDK_NODE_API(setClientRole);
    NIM_SDK_NODE_API(setLocalMediaPriority);
    NIM_SDK_NODE_API(getConnectionState);
    // NIM_SDK_NODE_API(setCameraCaptureConfig);
    // NIM_SDK_NODE_API(setCameraCaptureConfig(NERtcVideoStreamType type, const NERtcCameraCaptureConfig& config));
    // NIM_SDK_NODE_API(setVideoConfig);
    // NIM_SDK_NODE_API();int setVideoConfig(NERtcVideoStreamType type, const NERtcVideoConfig& config) 
    NIM_SDK_NODE_API(enableDualStreamMode);
    NIM_SDK_NODE_API(subscribeRemoteAudioStream);
    NIM_SDK_NODE_API(subscribeRemoteSubStreamAudio);
    NIM_SDK_NODE_API(subscribeAllRemoteAudioStream);
    // NIM_SDK_NODE_API(setAudioSubscribeOnlyBy);
    NIM_SDK_NODE_API(subscribeRemoteVideoStream);
    NIM_SDK_NODE_API(subscribeRemoteVideoSubStream);
    // NIM_SDK_NODE_API(addLiveStreamTask);
    // NIM_SDK_NODE_API(updateLiveStreamTask);
    // NIM_SDK_NODE_API(removeLiveStreamTask);
    // NIM_SDK_NODE_API(int sendSEIMsg(const char* data, int length, NERtcVideoStreamType type));
    // NIM_SDK_NODE_API(int sendSEIMsg(const char* data, int length));
    NIM_SDK_NODE_API(adjustUserPlaybackSignalVolume);
    // NIM_SDK_NODE_API(startChannelMediaRelay);
    // NIM_SDK_NODE_API(updateChannelMediaRelay);
    NIM_SDK_NODE_API(stopChannelMediaRelay);
    NIM_SDK_NODE_API(setLocalPublishFallbackOption);
    NIM_SDK_NODE_API(setRemoteSubscribeFallbackOption);
    NIM_SDK_NODE_API(setRemoteHighPriorityAudioStream);
    NIM_SDK_NODE_API(enableMediaPub);


private:
    nertc::IRtcChannel * _channel = nullptr;

};

}