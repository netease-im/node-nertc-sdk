﻿#include <napi.h>
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
    NIM_SDK_NODE_API(joinChannelWithUid);
    NIM_SDK_NODE_API(joinChannelWithOptions);
    NIM_SDK_NODE_API(leaveChannel);
    NIM_SDK_NODE_API(enableLocalAudio);
    NIM_SDK_NODE_API(enableLocalSubStreamAudio);
    NIM_SDK_NODE_API(muteLocalAudioStream);
    NIM_SDK_NODE_API(muteLocalSubStreamAudio);
    NIM_SDK_NODE_API(enableLocalVideo);
    NIM_SDK_NODE_API(enableLocalVideoWithType);
    NIM_SDK_NODE_API(muteLocalVideoStream);
    NIM_SDK_NODE_API(muteLocalVideoStreamWithType);
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
    NIM_SDK_NODE_API(setupVideoCanvas); // todo 需要注册的事件回调中进行调用
    NIM_SDK_NODE_API(setupSubStreamVideoCanvas);  // setupRemoteSubStreamVideoCanvas todo 
    NIM_SDK_NODE_API(setLocalVideoMirrorMode); 
    NIM_SDK_NODE_API(setLocalVideoMirrorModeWithType); //todo  NodeVideoFrameTransporter
    NIM_SDK_NODE_API(setClientRole);
    NIM_SDK_NODE_API(setLocalMediaPriority);
    NIM_SDK_NODE_API(getConnectionState);
    NIM_SDK_NODE_API(setCameraCaptureConfig); // demo未实现
    NIM_SDK_NODE_API(setCameraCaptureConfigWithType); // demo未实现
    NIM_SDK_NODE_API(setVideoConfig);
    NIM_SDK_NODE_API(setVideoConfigWithType);// demo未实现
    NIM_SDK_NODE_API(enableDualStreamMode);
    NIM_SDK_NODE_API(subscribeRemoteAudioStream);
    NIM_SDK_NODE_API(subscribeRemoteSubStreamAudio); // demo未实现
    NIM_SDK_NODE_API(subscribeAllRemoteAudioStream); // demo未实现
    NIM_SDK_NODE_API(setAudioSubscribeOnlyBy); // demo未实现
    NIM_SDK_NODE_API(setSubscribeAudioAllowlist);
    NIM_SDK_NODE_API(setSubscribeAudioBlocklist);
    NIM_SDK_NODE_API(subscribeRemoteVideoStream); // 通过事件调用
    NIM_SDK_NODE_API(subscribeRemoteVideoSubStream); // 通过事件调用
    NIM_SDK_NODE_API(addLiveStreamTask); //待优化 参数输入栏各个房间公用
    NIM_SDK_NODE_API(updateLiveStreamTask);
    NIM_SDK_NODE_API(removeLiveStreamTask);
    NIM_SDK_NODE_API(sendSEIMsg);
    NIM_SDK_NODE_API(sendSEIMsgWithType);
    NIM_SDK_NODE_API(adjustUserPlaybackSignalVolume);
    NIM_SDK_NODE_API(adjustChannelPlaybackSignalVolume);
    NIM_SDK_NODE_API(startChannelMediaRelay);
    NIM_SDK_NODE_API(updateChannelMediaRelay);
    NIM_SDK_NODE_API(stopChannelMediaRelay);
    NIM_SDK_NODE_API(setLocalPublishFallbackOption);
    NIM_SDK_NODE_API(setRemoteSubscribeFallbackOption);
    NIM_SDK_NODE_API(setRemoteHighPriorityAudioStream);
    NIM_SDK_NODE_API(enableMediaPub); // demo 没有入口
    NIM_SDK_NODE_API(updatePermissionKey);
    NIM_SDK_NODE_API(enableAudioVolumeIndication);
    NIM_SDK_NODE_API(setRangeAudioMode);
    NIM_SDK_NODE_API(setRangeAudioTeamID);
    NIM_SDK_NODE_API(setAudioRecvRange);
    NIM_SDK_NODE_API(updateSelfPosition);
    NIM_SDK_NODE_API(enableSpatializerRoomEffects);
    NIM_SDK_NODE_API(setSpatializerRoomProperty);
    NIM_SDK_NODE_API(setSpatializerRenderMode);
    NIM_SDK_NODE_API(initSpatializer);
    NIM_SDK_NODE_API(enableSpatializer);

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