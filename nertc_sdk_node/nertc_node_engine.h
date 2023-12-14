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
#include "nertc_node_video_frame_provider.h"

namespace nertc_node
{

class NertcNodeEngine : public Napi::ObjectWrap<NertcNodeEngine> {

#define NIM_SDK_NODE_API(m) \
    Napi::Value (m)(const Napi::CallbackInfo& info)


public:
#if NAPI_VERSION < 6
    static Napi::FunctionReference constructor;
#endif
    
    NertcNodeEngine(const Napi::CallbackInfo& info);
    virtual ~NertcNodeEngine();

public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    static nertc::IRtcEngineEx * getNertcEngine();

public:
    NIM_SDK_NODE_API(initialize);
    NIM_SDK_NODE_API(release);
    NIM_SDK_NODE_API(setClientRole);
    NIM_SDK_NODE_API(setChannelProfile);
    NIM_SDK_NODE_API(joinChannel);
    NIM_SDK_NODE_API(joinChannelWithOptions);
    NIM_SDK_NODE_API(switchChannel);
    NIM_SDK_NODE_API(switchChannelWithOptions);
    NIM_SDK_NODE_API(switchChannelWithOptionsEx);
    NIM_SDK_NODE_API(leaveChannel);
    NIM_SDK_NODE_API(enableLocalAudio);
    NIM_SDK_NODE_API(setupVideoCanvas);
    NIM_SDK_NODE_API(enableLocalVideo);
    NIM_SDK_NODE_API(enableLocalVideoWithType);
    NIM_SDK_NODE_API(subscribeRemoteVideoStream);
    NIM_SDK_NODE_API(setLocalVideoWatermarkConfigs);


    NIM_SDK_NODE_API(onVideoFrame);
    NIM_SDK_NODE_API(onEvent);
    // NIM_SDK_NODE_API(onAudioFrameEvent);
    NIM_SDK_NODE_API(onStatsObserver);
    NIM_SDK_NODE_API(onQsObserver);


    NIM_SDK_NODE_API(getConnectionState);
    NIM_SDK_NODE_API(muteLocalAudioStream);
    NIM_SDK_NODE_API(enableLocalSubStreamAudio);
    NIM_SDK_NODE_API(muteLocalSubStreamAudio);
    NIM_SDK_NODE_API(setAudioProfile);
    NIM_SDK_NODE_API(setAudioEffectPreset);
    NIM_SDK_NODE_API(setVoiceBeautifierPreset);
    NIM_SDK_NODE_API(setLocalVoicePitch);
    NIM_SDK_NODE_API(setLocalVoiceEqualization);
    NIM_SDK_NODE_API(subscribeRemoteAudioStream);
    NIM_SDK_NODE_API(subscribeRemoteSubStreamAudio);
    NIM_SDK_NODE_API(subscribeAllRemoteAudioStream);
    NIM_SDK_NODE_API(setAudioSubscribeOnlyBy);
    NIM_SDK_NODE_API(setSubscribeAudioAllowlist);
    NIM_SDK_NODE_API(setSubscribeAudioBlocklist);
    NIM_SDK_NODE_API(setStreamAlignmentProperty);
    NIM_SDK_NODE_API(getNtpTimeOffset);
    NIM_SDK_NODE_API(setCameraCaptureConfig);
    NIM_SDK_NODE_API(setCameraCaptureConfigWithType);
    NIM_SDK_NODE_API(setVideoConfig);
    NIM_SDK_NODE_API(setVideoConfigWithType);
    NIM_SDK_NODE_API(enableDualStreamMode);
    NIM_SDK_NODE_API(setupSubStreamVideoCanvas);
    NIM_SDK_NODE_API(setLocalVideoMirrorMode);
    NIM_SDK_NODE_API(setLocalVideoMirrorModeWithType);
    NIM_SDK_NODE_API(subscribeRemoteVideoSubStream);
    NIM_SDK_NODE_API(startVideoPreview);
    NIM_SDK_NODE_API(startVideoPreviewWithType);
    NIM_SDK_NODE_API(stopVideoPreview);
    NIM_SDK_NODE_API(stopVideoPreviewWithType);
    NIM_SDK_NODE_API(muteLocalVideoStream);
    NIM_SDK_NODE_API(muteLocalVideoStreamWithType);
    NIM_SDK_NODE_API(setLocalMediaPriority);
    NIM_SDK_NODE_API(setParameters);
    NIM_SDK_NODE_API(getParameters);
    NIM_SDK_NODE_API(setRecordingAudioFrameParameters);
    NIM_SDK_NODE_API(setPlaybackAudioFrameParameters);
    NIM_SDK_NODE_API(setMixedAudioFrameParameters);
    NIM_SDK_NODE_API(startAudioDump);
    NIM_SDK_NODE_API(startAudioDumpWithType);
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
    NIM_SDK_NODE_API(setAudioMixingPitch);
    NIM_SDK_NODE_API(getAudioMixingPitch);
    NIM_SDK_NODE_API(setEffectPitch);
    NIM_SDK_NODE_API(getEffectPitch);
    NIM_SDK_NODE_API(enableLoopbackRecording);
    NIM_SDK_NODE_API(adjustLoopbackRecordingSignalVolume);
    NIM_SDK_NODE_API(enableEarback);
    NIM_SDK_NODE_API(setEarbackVolume);
    NIM_SDK_NODE_API(enableAudioVolumeIndication);
    NIM_SDK_NODE_API(enableAudioVolumeIndicationEx);
    NIM_SDK_NODE_API(getScreenCaptureSources);
    NIM_SDK_NODE_API(setScreenCaptureSource);
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
    //todo
    // NIM_SDK_NODE_API(setExternalVideoSource);
    // NIM_SDK_NODE_API(setExternalVideoSourceEx);
    // NIM_SDK_NODE_API(pushExternalVideoFrame);
    // NIM_SDK_NODE_API(setExternalAudioSource);
    // NIM_SDK_NODE_API(pushExternalAudioFrame);
    // NIM_SDK_NODE_API(setExternalAudioRender);
    // NIM_SDK_NODE_API(pullExternalAudioFrame); //todo
    NIM_SDK_NODE_API(getVersion);
    NIM_SDK_NODE_API(getErrorDescription);
    NIM_SDK_NODE_API(uploadSdkInfo);
    NIM_SDK_NODE_API(addLiveStreamTask);
    NIM_SDK_NODE_API(updateLiveStreamTask);
    NIM_SDK_NODE_API(removeLiveStreamTask);
    NIM_SDK_NODE_API(sendSEIMsg);
    NIM_SDK_NODE_API(sendSEIMsgWithType);
    NIM_SDK_NODE_API(startAudioRecording);
    NIM_SDK_NODE_API(startAudioRecordingWithConfig);
    NIM_SDK_NODE_API(stopAudioRecording);
    NIM_SDK_NODE_API(adjustUserPlaybackSignalVolume);
    NIM_SDK_NODE_API(adjustChannelPlaybackSignalVolume);
    NIM_SDK_NODE_API(startChannelMediaRelay);
    NIM_SDK_NODE_API(updateChannelMediaRelay);
    NIM_SDK_NODE_API(stopChannelMediaRelay);
    NIM_SDK_NODE_API(setLocalPublishFallbackOption);
    NIM_SDK_NODE_API(setRemoteSubscribeFallbackOption);
    NIM_SDK_NODE_API(enableSuperResolution);
    NIM_SDK_NODE_API(enableEncryption);
    NIM_SDK_NODE_API(startLastmileProbeTest);
    NIM_SDK_NODE_API(stopLastmileProbeTest);
    NIM_SDK_NODE_API(setRemoteHighPriorityAudioStream);
    NIM_SDK_NODE_API(checkNECastAudioDriver);
    NIM_SDK_NODE_API(enableVirtualBackground);
    NIM_SDK_NODE_API(isFeatureSupported);
    NIM_SDK_NODE_API(setCloudProxy);
    NIM_SDK_NODE_API(enableLocalData);
    NIM_SDK_NODE_API(subscribeRemoteData);
    NIM_SDK_NODE_API(sendData);
    NIM_SDK_NODE_API(startBeauty);
    NIM_SDK_NODE_API(stopBeauty);
    NIM_SDK_NODE_API(enableBeauty);
    // NIM_SDK_NODE_API(enableBeautyMirrorMode); // 该方法已废弃
    NIM_SDK_NODE_API(getBeautyEffect);
    NIM_SDK_NODE_API(setBeautyEffect);
    NIM_SDK_NODE_API(addBeautyFilter);
    NIM_SDK_NODE_API(removeBeautyFilter);
    NIM_SDK_NODE_API(setBeautyFilterLevel);
    NIM_SDK_NODE_API(addBeautySticker);
    NIM_SDK_NODE_API(removeBeautySticker);
    NIM_SDK_NODE_API(addBeautyMakeup);
    NIM_SDK_NODE_API(removeBeautyMakeup);
    NIM_SDK_NODE_API(setLocalVoiceReverbParam);
    NIM_SDK_NODE_API(enableMediaPub);
    NIM_SDK_NODE_API(updatePermissionKey);
    NIM_SDK_NODE_API(setEffectPosition);
    NIM_SDK_NODE_API(getEffectCurrentPosition);
    NIM_SDK_NODE_API(getEffectDuration);
    NIM_SDK_NODE_API(setRangeAudioMode);
    NIM_SDK_NODE_API(setRangeAudioTeamID);
    NIM_SDK_NODE_API(setAudioRecvRange);
    NIM_SDK_NODE_API(updateSelfPosition);
    NIM_SDK_NODE_API(enableSpatializerRoomEffects);
    NIM_SDK_NODE_API(setSpatializerRoomProperty);
    NIM_SDK_NODE_API(setSpatializerRenderMode);
    NIM_SDK_NODE_API(initSpatializer);
    NIM_SDK_NODE_API(enableSpatializer);


    // adm
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
    NIM_SDK_NODE_API(setDeviceWithType);
    NIM_SDK_NODE_API(getDevice);
    NIM_SDK_NODE_API(getDeviceWithType);

    // CUSTOM
    NIM_SDK_NODE_API(enumerateScreenCaptureSourceInfo);
    


private:
    //nertc::IRtcEngineEx *rtc_engine_ = nullptr;
    nertc::IAudioDeviceManager *_adm = nullptr;
    nertc::IVideoDeviceManager *_vdm = nullptr;
    std::shared_ptr<NertcNodeEventHandler> _event_handler;
    std::shared_ptr<NertcNodeRtcMediaStatsHandler> _stats_observer;
    std::shared_ptr<NertcNodeVideoEncoderQosObserver> _qs_handler;
    std::shared_ptr<NertcNodeAudioFrameObserverHandler> _audio_observer;
    std::shared_ptr<NertcNodePacketObserver> _packet_observer;
    bool is_qosObserver_init_ = false;

#ifdef WIN32
    nertc_electron_util::WindowsHelpers *_windows_helper = nullptr;
    std::unique_ptr<nertc_electron_util::WindowCaptureHelper> window_capture_helper_;
    std::unique_ptr<nertc_electron_util::ScreenCaptureHelper> screen_capture_helper_;
#endif
};

}
#endif//NERTC_NODE_ENGINE_H


