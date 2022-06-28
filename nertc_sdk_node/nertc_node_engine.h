/**
 * @file meeting-engine.cpp
 * @brief nertc-electron-sdk导出类
 * @copyright Copyright (c) 2021 NetEase, Inc.  All rights reserved.\n
 *            Use of this source code is governed by a MIT license that can be found in the LICENSE file.
 * @author qyl
 * @date 2021/10/13
 */

#ifndef NERTC_NODE_ENGINE_H
#define NERTC_NODE_ENGINE_H

#include <napi.h>
#include <memory>
#include "nertc_engine_ex.h"
#include "nertc_node_engine_event_handler.h"
#include "nertc_audio_device_manager.h"
#include "nertc_video_device_manager.h"
#ifdef WIN32
#include "../shared/util/windows_helper.h"
#endif

namespace nertc_node
{

#define NIM_SDK_NODE_API(m) \
    Napi::Value (m)(const Napi::CallbackInfo& info)

class NertcNodeEngine : public Napi::ObjectWrap<NertcNodeEngine> {

public:
#if NAPI_VERSION < 6
    static Napi::FunctionReference constructor;
#endif
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    NertcNodeEngine(const Napi::CallbackInfo& info);
    virtual ~NertcNodeEngine();

public:
    NIM_SDK_NODE_API(initialize);
    NIM_SDK_NODE_API(release);
    NIM_SDK_NODE_API(setChannelProfile);
    NIM_SDK_NODE_API(joinChannel);
    NIM_SDK_NODE_API(leaveChannel);
    NIM_SDK_NODE_API(enableLocalAudio);
    NIM_SDK_NODE_API(enableLocalVideo);
    NIM_SDK_NODE_API(subscribeRemoteVideoStream);
    NIM_SDK_NODE_API(setupVideoCanvas);
    NIM_SDK_NODE_API(onVideoFrame);
    NIM_SDK_NODE_API(onEvent);

    // 3.9
    NIM_SDK_NODE_API(setClientRole);
    NIM_SDK_NODE_API(setupSubStreamVideoCanvas);
    NIM_SDK_NODE_API(subscribeRemoteVideoSubStream);
    NIM_SDK_NODE_API(setMixedAudioFrameParameters);
    NIM_SDK_NODE_API(setExternalAudioSource);
    NIM_SDK_NODE_API(pushExternalAudioFrame);
    NIM_SDK_NODE_API(pushExternalVideoFrame);
    
    // 4.0
    NIM_SDK_NODE_API(sendSEIMsg);
    NIM_SDK_NODE_API(sendSEIMsgEx);
    NIM_SDK_NODE_API(setExternalAudioRender);
    NIM_SDK_NODE_API(pullExternalAudioFrame); //todo

    // 4.1.1
    NIM_SDK_NODE_API(setAudioEffectPreset);
    NIM_SDK_NODE_API(setVoiceBeautifierPreset);
    NIM_SDK_NODE_API(setLocalVoicePitch);
    NIM_SDK_NODE_API(setLocalVoiceEqualization);

    // 4.1.110(timing custom)
    NIM_SDK_NODE_API(adjustUserPlaybackSignalVolume);

    // 4.4.8
    NIM_SDK_NODE_API(switchChannel);
    NIM_SDK_NODE_API(setLocalMediaPriority);
    NIM_SDK_NODE_API(enableLoopbackRecording);
    NIM_SDK_NODE_API(adjustLoopbackRecordingSignalVolume);
    NIM_SDK_NODE_API(setExcludeWindowList);
    NIM_SDK_NODE_API(startAudioRecording);
    NIM_SDK_NODE_API(stopAudioRecording);
    NIM_SDK_NODE_API(startChannelMediaRelay);
    NIM_SDK_NODE_API(updateChannelMediaRelay);
    NIM_SDK_NODE_API(stopChannelMediaRelay);
    //NIM_SDK_NODE_API(setLocalPublishFallbackOption);
    //NIM_SDK_NODE_API(setRemoteSubscribeFallbackOption);
    NIM_SDK_NODE_API(enableSuperResolution);
    NIM_SDK_NODE_API(enableEncryption);


    // ex
    NIM_SDK_NODE_API(getConnectionState);
    NIM_SDK_NODE_API(muteLocalAudioStream);
    NIM_SDK_NODE_API(setAudioProfile);
    NIM_SDK_NODE_API(subscribeRemoteAudioStream);
    NIM_SDK_NODE_API(setVideoConfig);
    NIM_SDK_NODE_API(enableDualStreamMode);
    NIM_SDK_NODE_API(setLocalVideoMirrorMode);
    NIM_SDK_NODE_API(startVideoPreview);
    NIM_SDK_NODE_API(stopVideoPreview);
    NIM_SDK_NODE_API(muteLocalVideoStream);
    NIM_SDK_NODE_API(setParameters);
    NIM_SDK_NODE_API(setRecordingAudioFrameParameters);
    NIM_SDK_NODE_API(setPlaybackAudioFrameParameters);
    NIM_SDK_NODE_API(startAudioDump);
    NIM_SDK_NODE_API(stopAudioDump);
    NIM_SDK_NODE_API(startAudioMixing);
    NIM_SDK_NODE_API(stopAudioMixing);
    NIM_SDK_NODE_API(pauseAudioMixing);
    NIM_SDK_NODE_API(resumeAudioMixing);
    NIM_SDK_NODE_API(setAudioMixingSendVolume);
    NIM_SDK_NODE_API(getAudioMixingSendVolume);
    NIM_SDK_NODE_API(setAudioMixingPlaybackVolume);
    NIM_SDK_NODE_API(getAudioMixingPlaybackVolume);
    NIM_SDK_NODE_API(getAudioMixingDuration);
    NIM_SDK_NODE_API(getAudioMixingCurrentPosition);
    NIM_SDK_NODE_API(setAudioMixingPosition);
    NIM_SDK_NODE_API(playEffect);
    NIM_SDK_NODE_API(stopEffect);
    NIM_SDK_NODE_API(stopAllEffects);
    NIM_SDK_NODE_API(pauseEffect);
    NIM_SDK_NODE_API(resumeEffect);
    NIM_SDK_NODE_API(pauseAllEffects);
    NIM_SDK_NODE_API(resumeAllEffects);
    NIM_SDK_NODE_API(setEffectSendVolume);
    NIM_SDK_NODE_API(getEffectSendVolume);
    NIM_SDK_NODE_API(setEffectPlaybackVolume);
    NIM_SDK_NODE_API(getEffectPlaybackVolume);
    NIM_SDK_NODE_API(enableEarback);
    NIM_SDK_NODE_API(setEarbackVolume);
    NIM_SDK_NODE_API(onStatsObserver);
    NIM_SDK_NODE_API(enableAudioVolumeIndication);

    NIM_SDK_NODE_API(startScreenCaptureByScreenRect);
    NIM_SDK_NODE_API(startScreenCaptureByDisplayId);
    NIM_SDK_NODE_API(startScreenCaptureByWindowId);
    NIM_SDK_NODE_API(updateScreenCaptureRegion);
    NIM_SDK_NODE_API(stopScreenCapture);
    NIM_SDK_NODE_API(pauseScreenCapture);
    NIM_SDK_NODE_API(resumeScreenCapture);
    NIM_SDK_NODE_API(setExternalVideoSource);
    NIM_SDK_NODE_API(getVersion);
    NIM_SDK_NODE_API(getErrorDescription);
    NIM_SDK_NODE_API(uploadSdkInfo);
    NIM_SDK_NODE_API(addLiveStreamTask);
    NIM_SDK_NODE_API(updateLiveStreamTask);
    NIM_SDK_NODE_API(removeLiveStreamTask);

    // // adm
    NIM_SDK_NODE_API(enumerateRecordDevices);
    NIM_SDK_NODE_API(setRecordDevice);
    NIM_SDK_NODE_API(getRecordDevice);
    NIM_SDK_NODE_API(enumeratePlayoutDevices);
    NIM_SDK_NODE_API(setPlayoutDevice);
    NIM_SDK_NODE_API(getPlayoutDevice);
    NIM_SDK_NODE_API(setRecordDeviceVolume);
    NIM_SDK_NODE_API(getRecordDeviceVolume);
    NIM_SDK_NODE_API(setPlayoutDeviceVolume);
    NIM_SDK_NODE_API(getPlayoutDeviceVolume);
    NIM_SDK_NODE_API(setPlayoutDeviceMute);
    NIM_SDK_NODE_API(getPlayoutDeviceMute);
    NIM_SDK_NODE_API(setRecordDeviceMute);
    NIM_SDK_NODE_API(getRecordDeviceMute);
    NIM_SDK_NODE_API(adjustRecordingSignalVolume);
    NIM_SDK_NODE_API(adjustPlaybackSignalVolume);
    NIM_SDK_NODE_API(startRecordDeviceTest);
    NIM_SDK_NODE_API(stopRecordDeviceTest);
    NIM_SDK_NODE_API(startPlayoutDeviceTest);
    NIM_SDK_NODE_API(stopPlayoutDeviceTest);
    NIM_SDK_NODE_API(startAudioDeviceLoopbackTest);
    NIM_SDK_NODE_API(stopAudioDeviceLoopbackTest);

    // vdm
    NIM_SDK_NODE_API(enumerateCaptureDevices);
    NIM_SDK_NODE_API(setDevice);
    NIM_SDK_NODE_API(getDevice);

    // CUSTOM
    NIM_SDK_NODE_API(enumerateScreenCaptureSourceInfo);

    // screenshare with audio
    NIM_SDK_NODE_API(startSystemAudioLoopbackCapture);
    NIM_SDK_NODE_API(stopSystemAudioLoopbackCapture);
    NIM_SDK_NODE_API(setSystemAudioLoopbackCaptureVolume);

private:
    nertc::IRtcEngineEx *rtc_engine_ = nullptr;
    nertc::IAudioDeviceManager *_adm = nullptr;
    nertc::IVideoDeviceManager *_vdm = nullptr;
    std::shared_ptr<NertcNodeEventHandler> _event_handler;
    std::shared_ptr<NertcNodeRtcMediaStatsHandler> _stats_observer;

#ifdef WIN32
    nertc_electron_util::WindowsHelpers *_windows_helper = nullptr;
    std::unique_ptr<nertc_electron_util::WindowCaptureHelper> window_capture_helper_;
    std::unique_ptr<nertc_electron_util::ScreenCaptureHelper> screen_capture_helper_;
#endif

};

}
#endif//NERTC_NODE_ENGINE_H


