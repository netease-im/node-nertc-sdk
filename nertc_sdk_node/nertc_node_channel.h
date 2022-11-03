#include <napi.h>
#include <memory>
#include "nertc_engine_ex.h"
#include "nertc_node_engine.h"
#include "nertc_node_channel_event_handler.h"

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
    NIM_SDK_NODE_API(onEvent);
    NIM_SDK_NODE_API(onStatsObserver);
    NIM_SDK_NODE_API(onVideoFrame);

public:
    NIM_SDK_NODE_API(release);
    NIM_SDK_NODE_API(getChannelName);
    NIM_SDK_NODE_API(joinChannel);
    NIM_SDK_NODE_API(leaveChannel);
    NIM_SDK_NODE_API(enableLocalAudio);
    NIM_SDK_NODE_API(enableLocalSubStreamAudio);
    NIM_SDK_NODE_API(muteLocalAudioStream);
    NIM_SDK_NODE_API(muteLocalSubStreamAudio);
    NIM_SDK_NODE_API(enableLocalVideo);
    NIM_SDK_NODE_API(enableLocalVideoEx);
    NIM_SDK_NODE_API(muteLocalVideoStream);
    NIM_SDK_NODE_API(muteLocalVideoStreamEx);
    
    NIM_SDK_NODE_API(startScreenCaptureByScreenRect);
    NIM_SDK_NODE_API(startScreenCaptureByDisplayId);
    NIM_SDK_NODE_API(startScreenCaptureByWindowId);
    NIM_SDK_NODE_API(updateScreenCaptureRegion);
    NIM_SDK_NODE_API(setScreenCaptureMouseCursor);
    NIM_SDK_NODE_API(stopScreenCapture);
    NIM_SDK_NODE_API(pauseScreenCapture);
    NIM_SDK_NODE_API(resumeScreenCapture);
    NIM_SDK_NODE_API(setExcludeWindowList);
    NIM_SDK_NODE_API(updateScreenCaptureParameters);

    NIM_SDK_NODE_API(setupVideoCanvas);
    NIM_SDK_NODE_API(setupSubStreamVideoCanvas);    
    NIM_SDK_NODE_API(setLocalVideoMirrorMode);
    NIM_SDK_NODE_API(setLocalVideoMirrorModeEx);
    NIM_SDK_NODE_API(setClientRole);
    NIM_SDK_NODE_API(setLocalMediaPriority);
    NIM_SDK_NODE_API(getConnectionState);
    NIM_SDK_NODE_API(setCameraCaptureConfig);
    NIM_SDK_NODE_API(setCameraCaptureConfigEx);
    NIM_SDK_NODE_API(setVideoConfig);
    NIM_SDK_NODE_API(setVideoConfigEx);
    NIM_SDK_NODE_API(enableDualStreamMode);
    NIM_SDK_NODE_API(subscribeRemoteAudioStream);
    NIM_SDK_NODE_API(subscribeRemoteSubStreamAudio);
    NIM_SDK_NODE_API(subscribeAllRemoteAudioStream);
    NIM_SDK_NODE_API(setAudioSubscribeOnlyBy);
    NIM_SDK_NODE_API(subscribeRemoteVideoStream);
    NIM_SDK_NODE_API(subscribeRemoteVideoSubStream);
    
    NIM_SDK_NODE_API(addLiveStreamTask);
    NIM_SDK_NODE_API(updateLiveStreamTask);
    NIM_SDK_NODE_API(removeLiveStreamTask);
    NIM_SDK_NODE_API(sendSEIMsg);
    NIM_SDK_NODE_API(sendSEIMsgEx);

    NIM_SDK_NODE_API(adjustUserPlaybackSignalVolume);
    NIM_SDK_NODE_API(startChannelMediaRelay);
    NIM_SDK_NODE_API(updateChannelMediaRelay);
    NIM_SDK_NODE_API(stopChannelMediaRelay);
    NIM_SDK_NODE_API(setLocalPublishFallbackOption);
    NIM_SDK_NODE_API(setRemoteSubscribeFallbackOption);
    NIM_SDK_NODE_API(setRemoteHighPriorityAudioStream);
    NIM_SDK_NODE_API(enableMediaPub);


private:
    nertc::IRtcChannel * _channel = nullptr;
    std::shared_ptr<NertcChannelEventHandler> _event_handler;
    std::shared_ptr<NertcChannelRtcMediaStatsHandler> _stats_observer;

#ifdef WIN32
    nertc_electron_util::WindowsHelpers *_windows_helper = nullptr;
    std::unique_ptr<nertc_electron_util::WindowCaptureHelper> window_capture_helper_;
    std::unique_ptr<nertc_electron_util::ScreenCaptureHelper> screen_capture_helper_;
#endif

};

}