#ifndef NERTC_NODE_ENGINE_H
#define NERTC_NODE_ENGINE_H

#include "nertc_engine_ex.h"
#include "nertc_audio_device_manager.h"
#include "nertc_video_device_manager.h"
#include <node.h>
#include <node_object_wrap.h>
#include "../shared/sdk_helper/nim_node_helper.h"
#include "nertc_node_engine_event_handler.h"
#ifdef WIN32
#include "../shared/util/windows_helper.h"
#endif

namespace nertc_node
{

class NertcNodeEngine : public node::ObjectWrap
{
private:
    /* data */
public:
    static void New(const FunctionCallbackInfo<Value> &args);
    // static void InitModule(Local<Object> &module);
    static void InitModule(Local<Object> &exports,
                        Local<Value> &module,
                        Local<Context> &context);

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

    // 4.0
    NIM_SDK_NODE_API(sendSEIMsg);//modify by lyq 修改参数名
    NIM_SDK_NODE_API(sendSEIMsgEx);//modify by lyq
    NIM_SDK_NODE_API(setExternalAudioRender);
    NIM_SDK_NODE_API(pullExternalAudioFrame);

    // 4.1.1
    NIM_SDK_NODE_API(setAudioEffectPreset);
    NIM_SDK_NODE_API(setVoiceBeautifierPreset);
    NIM_SDK_NODE_API(setLocalVoicePitch);
    NIM_SDK_NODE_API(setLocalVoiceEqualization);

    // 4.1.110
    //NIM_SDK_NODE_API(setRemoteHighPriorityAudioStream);//modify by lyq
    //NIM_SDK_NODE_API(subscribeRemoteAudioSubStream);//modify by lyq
    //NIM_SDK_NODE_API(enableLocalAudioStream);//modify by lyq
    //NIM_SDK_NODE_API(enableLoopbackRecording);//modify by lyq
    //NIM_SDK_NODE_API(adjustLoopbackRecordingSignalVolume);//modify by lyq
    NIM_SDK_NODE_API(adjustUserPlaybackSignalVolume);//modify by lyq 删除NERtcAudioStreamType参数

    // // 4.1.112
    // NIM_SDK_NODE_API(checkNECastAudioDriver);//modify by lyq

    //4.2.5
    NIM_SDK_NODE_API(switchChannel);
    NIM_SDK_NODE_API(setLocalRenderMode);
    NIM_SDK_NODE_API(setLocalSubStreamRenderMode);
    NIM_SDK_NODE_API(setRemoteRenderMode); 
    NIM_SDK_NODE_API(setLocalMediaPriority);
    NIM_SDK_NODE_API(setExcludeWindowList);
    NIM_SDK_NODE_API(setLocalCanvasWatermarkConfigs); 
    NIM_SDK_NODE_API(setRemoteCanvasWatermarkConfigs); 
    NIM_SDK_NODE_API(startAudioRecording);
    NIM_SDK_NODE_API(stopAudioRecording);
    //todo
    // NIM_SDK_NODE_API(takeLocalSnapshot); 
    // NIM_SDK_NODE_API(takeRemoteSnapshot); 


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
    NIM_SDK_NODE_API(startScreenCaptureByScreenRect);//modify by lyq modify param 
    NIM_SDK_NODE_API(startScreenCaptureByDisplayId);
    NIM_SDK_NODE_API(startScreenCaptureByWindowId);
    NIM_SDK_NODE_API(updateScreenCaptureRegion);
    NIM_SDK_NODE_API(stopScreenCapture);
    NIM_SDK_NODE_API(pauseScreenCapture);
    NIM_SDK_NODE_API(resumeScreenCapture);
    NIM_SDK_NODE_API(setExternalVideoSource);
    NIM_SDK_NODE_API(pushExternalVideoFrame);
    NIM_SDK_NODE_API(getVersion);
    NIM_SDK_NODE_API(getErrorDescription);
    NIM_SDK_NODE_API(uploadSdkInfo);
    NIM_SDK_NODE_API(addLiveStreamTask);
    NIM_SDK_NODE_API(updateLiveStreamTask);
    NIM_SDK_NODE_API(removeLiveStreamTask);

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
    NIM_SDK_NODE_API(getDevice);

    // CUSTOM
    NIM_SDK_NODE_API(enumerateScreenCaptureSourceInfo);

    // screenshare with audio
    NIM_SDK_NODE_API(startSystemAudioLoopbackCapture);
    NIM_SDK_NODE_API(stopSystemAudioLoopbackCapture);
    NIM_SDK_NODE_API(setSystemAudioLoopbackCaptureVolume);

protected:
    NertcNodeEngine(Isolate *isolate);
    ~NertcNodeEngine();

private:
    DECLARE_CLASS;
    nertc::IRtcEngineEx *rtc_engine_ = nullptr;
    nertc::IAudioDeviceManager *_adm = nullptr;
    nertc::IVideoDeviceManager *_vdm = nullptr;
    Isolate *isolate_;
#ifdef WIN32
    nertc_electron_util::WindowsHelpers *_windows_helper = nullptr;
    std::unique_ptr<nertc_electron_util::WindowCaptureHelper> window_capture_helper_;
    std::unique_ptr<nertc_electron_util::ScreenCaptureHelper> screen_capture_helper_;
#endif
};

#define napi_get_native_this(args, native) \
            native = ObjectWrap::Unwrap<NertcNodeEngine>(args.Holder());

#define CHECK_NATIVE_THIS(engine) \
        if(!engine->rtc_engine_) { \
            break;\
        }

#define CHECK_NATIVE_ADM_THIS(engine) \
        if(!engine->rtc_engine_ || !engine->_adm) { \
            break;\
        }

#define CHECK_NATIVE_VDM_THIS(engine) \
        if(!engine->rtc_engine_ || !engine->_vdm) { \
            break;\
        }
}
#endif //NERTC_NODE_ENGINE_H