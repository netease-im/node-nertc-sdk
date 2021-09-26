#include "nertc_node_engine.h"
#include "nertc_node_engine_helper.h"
#include "nertc_node_video_frame_provider.h"
#ifdef WIN32
#include "../shared/util/string_util.h"
using namespace nertc_electron_util;
#endif

namespace nertc_node
{
DEFINE_CLASS(NertcNodeEngine);

NertcNodeEngine::NertcNodeEngine(Isolate *isolate)
{
    isolate_ = isolate;
    rtc_engine_ = (nertc::IRtcEngineEx *)createNERtcEngine();
#ifdef WIN32
    _windows_helper = new WindowsHelpers();
    window_capture_helper_.reset(new WindowCaptureHelper());
    screen_capture_helper_.reset(new ScreenCaptureHelper());
#endif
}
NertcNodeEngine::~NertcNodeEngine()
{
    if (rtc_engine_)
    {
        destroyNERtcEngine((void *&)rtc_engine_);
        _adm = nullptr;
        _vdm = nullptr;
        rtc_engine_ = nullptr;
    }
#ifdef WIN32
    if (_windows_helper)
    {
        delete _windows_helper;
        _windows_helper = nullptr;
    }
#endif
}
// void NertcNodeEngine::InitModule(Local<Object> &module)
void NertcNodeEngine::InitModule(Local<Object> &exports,
                        Local<Value> &module,
                        Local<Context> &context)
{
    BEGIN_OBJECT_INIT_EX(NertcNodeEngine, New, 5)
    SET_PROTOTYPE(initialize)
    SET_PROTOTYPE(release)
    SET_PROTOTYPE(setChannelProfile)
    SET_PROTOTYPE(joinChannel)
    SET_PROTOTYPE(leaveChannel)
    SET_PROTOTYPE(enableLocalAudio)
    SET_PROTOTYPE(enableLocalVideo)
    SET_PROTOTYPE(subscribeRemoteVideoStream)
    SET_PROTOTYPE(setupVideoCanvas)
    SET_PROTOTYPE(onVideoFrame)
    SET_PROTOTYPE(onEvent)

    // 3.9
    SET_PROTOTYPE(setClientRole)
    SET_PROTOTYPE(setupSubStreamVideoCanvas)
    SET_PROTOTYPE(subscribeRemoteVideoSubStream)
    SET_PROTOTYPE(setMixedAudioFrameParameters)
    SET_PROTOTYPE(setExternalAudioSource)
    SET_PROTOTYPE(pushExternalAudioFrame)

    // 4.0
    SET_PROTOTYPE(sendSEIMsg)
    SET_PROTOTYPE(sendSEIMsgEx)
    SET_PROTOTYPE(setExternalAudioRender)
    SET_PROTOTYPE(pullExternalAudioFrame)

    // 4.1.1
    SET_PROTOTYPE(setAudioEffectPreset)
    SET_PROTOTYPE(setVoiceBeautifierPreset)
    SET_PROTOTYPE(setLocalVoicePitch)
    SET_PROTOTYPE(setLocalVoiceEqualization)

    // 4.1.110
    //SET_PROTOTYPE(setRemoteHighPriorityAudioStream);
    //SET_PROTOTYPE(subscribeRemoteAudioSubStream);
    //SET_PROTOTYPE(enableLocalAudioStream);
    //SET_PROTOTYPE(enableLoopbackRecording);
    //SET_PROTOTYPE(adjustLoopbackRecordingSignalVolume);
    SET_PROTOTYPE(adjustUserPlaybackSignalVolume);

    // // 4.1.112
    // SET_PROTOTYPE(checkNECastAudioDriver);

    SET_PROTOTYPE(getConnectionState)
    SET_PROTOTYPE(muteLocalAudioStream)
    SET_PROTOTYPE(setAudioProfile)
    SET_PROTOTYPE(subscribeRemoteAudioStream)
    SET_PROTOTYPE(setVideoConfig)
    SET_PROTOTYPE(enableDualStreamMode)
    SET_PROTOTYPE(setLocalVideoMirrorMode)
    SET_PROTOTYPE(startVideoPreview)
    SET_PROTOTYPE(stopVideoPreview)
    SET_PROTOTYPE(muteLocalVideoStream)
    SET_PROTOTYPE(setParameters)
    SET_PROTOTYPE(setRecordingAudioFrameParameters)
    SET_PROTOTYPE(setPlaybackAudioFrameParameters)
    SET_PROTOTYPE(startAudioDump)
    SET_PROTOTYPE(stopAudioDump)
    SET_PROTOTYPE(startAudioMixing)
    SET_PROTOTYPE(stopAudioMixing)
    SET_PROTOTYPE(pauseAudioMixing)
    SET_PROTOTYPE(resumeAudioMixing)
    SET_PROTOTYPE(setAudioMixingSendVolume)
    SET_PROTOTYPE(getAudioMixingSendVolume)
    SET_PROTOTYPE(setAudioMixingPlaybackVolume)
    SET_PROTOTYPE(getAudioMixingPlaybackVolume)
    SET_PROTOTYPE(getAudioMixingDuration)
    SET_PROTOTYPE(getAudioMixingCurrentPosition)
    SET_PROTOTYPE(setAudioMixingPosition)
    SET_PROTOTYPE(playEffect)
    SET_PROTOTYPE(stopEffect)
    SET_PROTOTYPE(stopAllEffects)
    SET_PROTOTYPE(pauseEffect)
    SET_PROTOTYPE(resumeEffect)
    SET_PROTOTYPE(pauseAllEffects)
    SET_PROTOTYPE(resumeAllEffects)
    SET_PROTOTYPE(setEffectSendVolume)
    SET_PROTOTYPE(getEffectSendVolume)
    SET_PROTOTYPE(setEffectPlaybackVolume)
    SET_PROTOTYPE(getEffectPlaybackVolume)
    SET_PROTOTYPE(enableEarback)
    SET_PROTOTYPE(setEarbackVolume)
    SET_PROTOTYPE(onStatsObserver)
    SET_PROTOTYPE(enableAudioVolumeIndication)
    //SET_PROTOTYPE(startScreenCaptureByScreenRect)
    // SET_PROTOTYPE(startScreenCaptureByDisplayId)
    // SET_PROTOTYPE(startScreenCaptureByWindowId)
    SET_PROTOTYPE(updateScreenCaptureRegion)
    SET_PROTOTYPE(stopScreenCapture)
    SET_PROTOTYPE(pauseScreenCapture)
    SET_PROTOTYPE(resumeScreenCapture)
    SET_PROTOTYPE(setExternalVideoSource)
    SET_PROTOTYPE(pushExternalVideoFrame)
    SET_PROTOTYPE(getVersion)
    SET_PROTOTYPE(getErrorDescription)
    SET_PROTOTYPE(uploadSdkInfo)
    SET_PROTOTYPE(addLiveStreamTask)
    SET_PROTOTYPE(updateLiveStreamTask)
    SET_PROTOTYPE(removeLiveStreamTask)

    SET_PROTOTYPE(enumerateRecordDevices)
    SET_PROTOTYPE(setRecordDevice)
    SET_PROTOTYPE(getRecordDevice)
    SET_PROTOTYPE(enumeratePlayoutDevices)
    SET_PROTOTYPE(setPlayoutDevice)
    SET_PROTOTYPE(getPlayoutDevice)
    SET_PROTOTYPE(setRecordDeviceVolume)
    SET_PROTOTYPE(getRecordDeviceVolume)
    SET_PROTOTYPE(setPlayoutDeviceVolume)
    SET_PROTOTYPE(getPlayoutDeviceVolume)
    SET_PROTOTYPE(setPlayoutDeviceMute)
    SET_PROTOTYPE(getPlayoutDeviceMute)
    SET_PROTOTYPE(setRecordDeviceMute)
    SET_PROTOTYPE(getRecordDeviceMute)
    SET_PROTOTYPE(adjustRecordingSignalVolume)
    SET_PROTOTYPE(adjustPlaybackSignalVolume)
    SET_PROTOTYPE(startRecordDeviceTest)
    SET_PROTOTYPE(stopRecordDeviceTest)
    SET_PROTOTYPE(startPlayoutDeviceTest)
    SET_PROTOTYPE(stopPlayoutDeviceTest)
    SET_PROTOTYPE(startAudioDeviceLoopbackTest)
    SET_PROTOTYPE(stopAudioDeviceLoopbackTest)

    SET_PROTOTYPE(enumerateCaptureDevices)
    SET_PROTOTYPE(setDevice)
    SET_PROTOTYPE(getDevice)

    SET_PROTOTYPE(enumerateScreenCaptureSourceInfo)

    SET_PROTOTYPE(startSystemAudioLoopbackCapture)
    SET_PROTOTYPE(stopSystemAudioLoopbackCapture)
    SET_PROTOTYPE(setSystemAudioLoopbackCaptureVolume)

    END_OBJECT_INIT_EX(NertcNodeEngine)
}

void NertcNodeEngine::New(const FunctionCallbackInfo<Value> &args)
{
    Isolate *isolate = args.GetIsolate();
    if (args.IsConstructCall())
    {
        NertcNodeEngine *engine = new NertcNodeEngine(isolate);
        engine->Wrap(args.This());
        args.GetReturnValue().Set(args.This());
    }
    else
    {
        Local<Function> cons = Local<Function>::New(isolate, constructor);
        Local<Context> context = isolate->GetCurrentContext();
        Local<Object> instance = cons->NewInstance(context).ToLocalChecked();
        args.GetReturnValue().Set(instance);
    }
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, initialize)
{
    CHECK_API_FUNC(NertcNodeEngine, 1)
    int ret = -1; bool log_ret = false;
    do
    {
        CHECK_NATIVE_THIS(instance);
        auto status = napi_ok;
        nertc::NERtcEngineContext context;
        context.video_use_exnternal_render = true;
        context.video_prefer_hw_decoder = false;
        context.video_prefer_hw_encoder = false;
        context.log_level = nertc::kNERtcLogLevelInfo;
        context.log_file_max_size_KBytes = 20 * 1024;
        context.event_handler = NertcNodeEventHandler::GetInstance();
        UTF8String app_key, log_dir_path;
        if (nim_napi_get_object_value_utf8string(isolate, args[0]->ToObject(isolate->GetCurrentContext()).ToLocalChecked(), "app_key", app_key) == napi_ok)
        {
            context.app_key = (const char *)app_key.get();
        }
        if (nim_napi_get_object_value_utf8string(isolate, args[0]->ToObject(isolate->GetCurrentContext()).ToLocalChecked(), "log_dir_path", log_dir_path) == napi_ok)
        {
            context.log_dir_path = (const char *)log_dir_path.get();
        }
        uint32_t logLevel = 3, log_file_max_size_KBytes = 20 * 1024;
        if (nim_napi_get_object_value_uint32(isolate, args[0]->ToObject(isolate->GetCurrentContext()).ToLocalChecked(), "log_level", logLevel) == napi_ok)
        {
            context.log_level = (nertc::NERtcLogLevel)logLevel;
        }
        if (nim_napi_get_object_value_uint32(isolate, args[0]->ToObject(isolate->GetCurrentContext()).ToLocalChecked(), "log_file_max_size_KBytes", log_file_max_size_KBytes) == napi_ok)
        {
            context.log_file_max_size_KBytes = log_file_max_size_KBytes;
        }
        ret = instance->rtc_engine_->initialize(context);
        if (ret == 0)
        {
            instance->rtc_engine_->queryInterface(nertc::kNERtcIIDAudioDeviceManager, (void **)&instance->_adm);
            instance->rtc_engine_->queryInterface(nertc::kNERtcIIDVideoDeviceManager, (void **)&instance->_vdm);
        }
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, release)
{
    CHECK_API_FUNC(NertcNodeEngine, 0)
    do
    {
        CHECK_NATIVE_THIS(instance);
        instance->rtc_engine_->release(true);
        if (instance->rtc_engine_)
        {
            destroyNERtcEngine((void *&)instance->rtc_engine_);
            instance->_adm = nullptr;
            instance->_vdm = nullptr;
            instance->rtc_engine_ = nullptr;
        }
        NertcNodeEventHandler::GetInstance()->RemoveAll();
        NertcNodeRtcMediaStatsHandler::GetInstance()->RemoveAll();
    } while (false);
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, setChannelProfile)
{
    CHECK_API_FUNC(NertcNodeEngine, 1)
    int ret = -1;
    do
    {
        CHECK_NATIVE_THIS(instance);
        auto status = napi_ok;
        uint32_t profile;
        GET_ARGS_VALUE(isolate, 0, uint32, profile)
        if (status != napi_ok)
        {
            break;
        }
        ret = instance->rtc_engine_->setChannelProfile((nertc::NERtcChannelProfileType)profile);
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, joinChannel)
{
    CHECK_API_FUNC(NertcNodeEngine, 3)
    int ret = -1;
    do
    {
        CHECK_NATIVE_THIS(instance);
        auto status = napi_ok;
        uint64_t uid;
        UTF8String token, channel_name;
        GET_ARGS_VALUE(isolate, 0, utf8string, token)
        GET_ARGS_VALUE(isolate, 1, utf8string, channel_name)
        GET_ARGS_VALUE(isolate, 2, uint64, uid)
        if (status != napi_ok || channel_name.length() == 0)
        {
            break;
        }
        ret = instance->rtc_engine_->joinChannel(token.length() == 0 ? "" : token.get(), channel_name.get(), uid);
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, leaveChannel)
{
    CHECK_API_FUNC(NertcNodeEngine, 0)
    int ret = -1;
    do
    {
        CHECK_NATIVE_THIS(instance);
        ret = instance->rtc_engine_->leaveChannel();
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, enableLocalAudio)
{
    CHECK_API_FUNC(NertcNodeEngine, 1)
    int ret = -1;
    do
    {
        CHECK_NATIVE_THIS(instance);
        auto status = napi_ok;
        bool enabled;
        GET_ARGS_VALUE(isolate, 0, bool, enabled)
        if (status != napi_ok)
        {
            break;
        }
        ret = instance->rtc_engine_->enableLocalAudio(enabled);
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, enableLocalVideo)
{
    CHECK_API_FUNC(NertcNodeEngine, 1)
    int ret = -1;
    do
    {
        CHECK_NATIVE_THIS(instance);
        auto status = napi_ok;
        bool enabled;
        GET_ARGS_VALUE(isolate, 0, bool, enabled)
        if (status != napi_ok)
        {
            break;
        }
        ret = instance->rtc_engine_->enableLocalVideo(enabled);
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, subscribeRemoteVideoStream)
{
    CHECK_API_FUNC(NertcNodeEngine, 3)
    int ret = -1;
    do
    {
        CHECK_NATIVE_THIS(instance);
        auto status = napi_ok;
        uint64_t uid;
        bool sub;
        uint32_t type;
        GET_ARGS_VALUE(isolate, 0, uint64, uid)
        GET_ARGS_VALUE(isolate, 1, uint32, type)
        GET_ARGS_VALUE(isolate, 2, bool, sub)
        if (status != napi_ok)
        {
            break;
        }
        ret = instance->rtc_engine_->subscribeRemoteVideoStream(uid, (nertc::NERtcRemoteVideoStreamType)type, sub);
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, setLocalVideoMirrorMode)
{
    CHECK_API_FUNC(NertcNodeEngine, 1)
    int ret = -1;
    do
    {
        CHECK_NATIVE_THIS(instance);
        auto status = napi_ok;
        uint32_t mode;
        GET_ARGS_VALUE(isolate, 0, uint32, mode)
        if (status != napi_ok)
        {
            break;
        }
        NodeVideoFrameTransporter *pTransporter = getNodeVideoFrameTransporter();
        if (pTransporter)
        {
            pTransporter->setLocalVideoMirrorMode(mode);
            ret = 0;
        }
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, setupVideoCanvas)
{
    CHECK_API_FUNC(NertcNodeEngine, 2)
    int ret = -1;
    do
    {
        CHECK_NATIVE_THIS(instance);
        auto status = napi_ok;
        uint64_t uid;
        bool enable;
        GET_ARGS_VALUE(isolate, 0, uint64, uid)
        GET_ARGS_VALUE(isolate, 1, bool, enable)
        if (status != napi_ok)
        {
            break;
        }
        nertc::NERtcVideoCanvas canvas;
        canvas.cb = enable ? NodeVideoFrameTransporter::onFrameDataCallback : nullptr;
        canvas.user_data = enable ? (void*)(new nertc::uid_t(uid)) : nullptr;
        canvas.window = nullptr;
        if (uid == 0)
            ret = instance->rtc_engine_->setupLocalVideoCanvas(&canvas);
        else
            ret = instance->rtc_engine_->setupRemoteVideoCanvas(uid, &canvas);
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, onVideoFrame)
{
    CHECK_API_FUNC(NertcNodeEngine, 1)
    int ret = -1;
    do
    {
        CHECK_NATIVE_THIS(instance);
        NodeVideoFrameTransporter *pTransporter = getNodeVideoFrameTransporter();
        if (pTransporter)
        {
            ret = pTransporter->initialize(isolate, args);
        }
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, onEvent)
{
    CHECK_API_FUNC(NertcNodeEngine, 2)
    do
    {
        CHECK_NATIVE_THIS(instance);
        auto status = napi_ok;
        UTF8String eventName;
        GET_ARGS_VALUE(isolate, 0, utf8string, eventName)
        if (status != napi_ok || eventName.length() == 0)
        {
            break;
        }
        ASSEMBLE_REG_CALLBACK(1, NertcNodeEventHandler, eventName.toUtf8String())
    } while (false);
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, getConnectionState)
{
    CHECK_API_FUNC(NertcNodeEngine, 0)
    int ret = -1;
    do
    {
        CHECK_NATIVE_THIS(instance);
        ret = instance->rtc_engine_->getConnectionState();
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), (int)ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, muteLocalAudioStream)
{
    CHECK_API_FUNC(NertcNodeEngine, 1)
    int ret = -1;
    do
    {
        CHECK_NATIVE_THIS(instance);
        auto status = napi_ok;
        bool enabled;
        GET_ARGS_VALUE(isolate, 0, bool, enabled)
        if (status != napi_ok)
        {
            break;
        }
        ret = instance->rtc_engine_->muteLocalAudioStream(enabled);
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, setAudioProfile)
{
    CHECK_API_FUNC(NertcNodeEngine, 2)
    int ret = -1;
    do
    {
        CHECK_NATIVE_THIS(instance);
        auto status = napi_ok;
        uint32_t profile, scenario;
        GET_ARGS_VALUE(isolate, 0, uint32, profile)
        GET_ARGS_VALUE(isolate, 1, uint32, scenario)
        if (status != napi_ok)
        {
            break;
        }
        ret = instance->rtc_engine_->setAudioProfile((nertc::NERtcAudioProfileType)profile, (nertc::NERtcAudioScenarioType)scenario);
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, subscribeRemoteAudioStream)
{
    CHECK_API_FUNC(NertcNodeEngine, 2)
    int ret = -1;
    do
    {
        CHECK_NATIVE_THIS(instance);
        auto status = napi_ok;
        uint64_t uid;
        bool enable;
        GET_ARGS_VALUE(isolate, 0, uint64, uid)
        GET_ARGS_VALUE(isolate, 1, bool, enable)
        if (status != napi_ok)
        {
            break;
        }
        ret = instance->rtc_engine_->subscribeRemoteAudioStream(uid, enable);
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, setVideoConfig)
{
    CHECK_API_FUNC(NertcNodeEngine, 1)
    int ret = -1;
    do
    {
        CHECK_NATIVE_THIS(instance);
        auto status = napi_ok;
        nertc::NERtcVideoConfig config = {};
        status = nertc_video_config_obj_to_struct(isolate, args[0]->ToObject(isolate->GetCurrentContext()).ToLocalChecked(), config);
        if (status != napi_ok)
        {
            break;
        }
        ret = instance->rtc_engine_->setVideoConfig(config);
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, enableDualStreamMode)
{
    CHECK_API_FUNC(NertcNodeEngine, 1)
    int ret = -1;
    do
    {
        CHECK_NATIVE_THIS(instance);
        auto status = napi_ok;
        bool enable;
        GET_ARGS_VALUE(isolate, 0, bool, enable)
        if (status != napi_ok)
        {
            break;
        }
        ret = instance->rtc_engine_->enableDualStreamMode(enable);
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, startVideoPreview)
{
    CHECK_API_FUNC(NertcNodeEngine, 0)
    int ret = -1;
    do
    {
        CHECK_NATIVE_THIS(instance);
        ret = instance->rtc_engine_->startVideoPreview();
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, stopVideoPreview)
{
    CHECK_API_FUNC(NertcNodeEngine, 0)
    int ret = -1;
    do
    {
        CHECK_NATIVE_THIS(instance);
        ret = instance->rtc_engine_->stopVideoPreview();
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, muteLocalVideoStream)
{
    CHECK_API_FUNC(NertcNodeEngine, 1)
    int ret = -1;
    do
    {
        CHECK_NATIVE_THIS(instance);
        auto status = napi_ok;
        bool enabled;
        GET_ARGS_VALUE(isolate, 0, bool, enabled)
        if (status != napi_ok)
        {
            break;
        }
        ret = instance->rtc_engine_->muteLocalVideoStream(enabled);
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, setParameters)
{
    CHECK_API_FUNC(NertcNodeEngine, 1)
    int ret = -1;
    do
    {
        CHECK_NATIVE_THIS(instance);
        auto status = napi_ok;
        UTF8String para;
        GET_ARGS_VALUE(isolate, 0, utf8string, para)
        if (status != napi_ok || para.length() == 0)
        {
            break;
        }
        ret = instance->rtc_engine_->setParameters(para.get());
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, setRecordingAudioFrameParameters)
{
    CHECK_API_FUNC(NertcNodeEngine, 1)
    int ret = -1;
    do
    {
        CHECK_NATIVE_THIS(instance);
        auto status = napi_ok;
        nertc::NERtcAudioFrameRequestFormat config = {};
        status = nertc_audio_frame_rf_obj_to_struct(isolate, args[0]->ToObject(isolate->GetCurrentContext()).ToLocalChecked(), config);
        if (status != napi_ok)
        {
            break;
        }
        ret = instance->rtc_engine_->setRecordingAudioFrameParameters(&config);
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, setPlaybackAudioFrameParameters)
{
    CHECK_API_FUNC(NertcNodeEngine, 1)
    int ret = -1;
    do
    {
        CHECK_NATIVE_THIS(instance);
        auto status = napi_ok;
        nertc::NERtcAudioFrameRequestFormat config = {};
        status = nertc_audio_frame_rf_obj_to_struct(isolate, args[0]->ToObject(isolate->GetCurrentContext()).ToLocalChecked(), config);
        if (status != napi_ok)
        {
            break;
        }
        ret = instance->rtc_engine_->setPlaybackAudioFrameParameters(&config);
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, startAudioDump)
{
    CHECK_API_FUNC(NertcNodeEngine, 0)
    int ret = -1;
    do
    {
        CHECK_NATIVE_THIS(instance);
        ret = instance->rtc_engine_->startAudioDump();
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, stopAudioDump)
{
    CHECK_API_FUNC(NertcNodeEngine, 0)
    int ret = -1;
    do
    {
        CHECK_NATIVE_THIS(instance);
        ret = instance->rtc_engine_->stopAudioDump();
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, startAudioMixing)
{
    CHECK_API_FUNC(NertcNodeEngine, 1)
    int ret = -1;
    do
    {
        CHECK_NATIVE_THIS(instance);
        auto status = napi_ok;
        nertc::NERtcCreateAudioMixingOption config = {};
        status = nertc_audio_mixing_option_obj_to_struct(isolate, args[0]->ToObject(isolate->GetCurrentContext()).ToLocalChecked(), config);
        if (status != napi_ok)
        {
            break;
        }
        ret = instance->rtc_engine_->startAudioMixing(&config);
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, stopAudioMixing)
{
    CHECK_API_FUNC(NertcNodeEngine, 0)
    int ret = -1;
    do
    {
        CHECK_NATIVE_THIS(instance);
        ret = instance->rtc_engine_->stopAudioMixing();
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, pauseAudioMixing)
{
    CHECK_API_FUNC(NertcNodeEngine, 0)
    int ret = -1;
    do
    {
        CHECK_NATIVE_THIS(instance);
        ret = instance->rtc_engine_->pauseAudioMixing();
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, resumeAudioMixing)
{
    CHECK_API_FUNC(NertcNodeEngine, 0)
    int ret = -1;
    do
    {
        CHECK_NATIVE_THIS(instance);
        ret = instance->rtc_engine_->resumeAudioMixing();
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, setAudioMixingSendVolume)
{
    CHECK_API_FUNC(NertcNodeEngine, 1)
    int ret = -1;
    do
    {
        CHECK_NATIVE_THIS(instance);
        auto status = napi_ok;
        uint32_t param;
        GET_ARGS_VALUE(isolate, 0, uint32, param)
        if (status != napi_ok)
        {
            break;
        }
        ret = instance->rtc_engine_->setAudioMixingSendVolume(param);
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, getAudioMixingSendVolume)
{
    CHECK_API_FUNC(NertcNodeEngine, 0)
    uint32_t volume = 0;
    int ret = -1;
    do
    {
        CHECK_NATIVE_THIS(instance);
        ret = instance->rtc_engine_->getAudioMixingSendVolume(&volume);
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret == 0 ? volume : ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, setAudioMixingPlaybackVolume)
{
    CHECK_API_FUNC(NertcNodeEngine, 1)
    int ret = -1;
    do
    {
        CHECK_NATIVE_THIS(instance);
        auto status = napi_ok;
        uint32_t param;
        GET_ARGS_VALUE(isolate, 0, uint32, param)
        if (status != napi_ok)
        {
            break;
        }
        ret = instance->rtc_engine_->setAudioMixingPlaybackVolume(param);
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, getAudioMixingPlaybackVolume)
{
    CHECK_API_FUNC(NertcNodeEngine, 0)
    uint32_t volume = 0;
    int ret = -1;
    do
    {
        CHECK_NATIVE_THIS(instance);
        ret = instance->rtc_engine_->getAudioMixingPlaybackVolume(&volume);
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret == 0 ? volume : ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, getAudioMixingDuration)
{
    CHECK_API_FUNC(NertcNodeEngine, 0)
    uint64_t dur = 0;
    int ret = 0;
    do
    {
        CHECK_NATIVE_THIS(instance);
        ret = instance->rtc_engine_->getAudioMixingDuration(&dur);
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret == 0 ? dur : ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, getAudioMixingCurrentPosition)
{
    CHECK_API_FUNC(NertcNodeEngine, 0)
    uint64_t volume = 0;
    int ret = -1;
    do
    {
        CHECK_NATIVE_THIS(instance);
        ret = instance->rtc_engine_->getAudioMixingCurrentPosition(&volume);
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret == 0 ? volume : ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, setAudioMixingPosition)
{
    CHECK_API_FUNC(NertcNodeEngine, 1)
    int ret = -1;
    do
    {
        CHECK_NATIVE_THIS(instance);
        auto status = napi_ok;
        uint64_t param;
        GET_ARGS_VALUE(isolate, 0, uint64, param)
        if (status != napi_ok)
        {
            break;
        }
        ret = instance->rtc_engine_->setAudioMixingPosition(param);
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, playEffect)
{
    CHECK_API_FUNC(NertcNodeEngine, 2)
    int ret = -1;
    do
    {
        CHECK_NATIVE_THIS(instance);
        auto status = napi_ok;
        uint32_t effect_id;
        GET_ARGS_VALUE(isolate, 0, uint32, effect_id)
        auto objs = args[1]->ToObject(isolate->GetCurrentContext()).ToLocalChecked().As<Array>();
        nertc::NERtcCreateAudioEffectOption *config = new nertc::NERtcCreateAudioEffectOption[objs->Length()];
        status = nertc_audio_effect_option_obj_to_struct(isolate, args[1]->ToObject(isolate->GetCurrentContext()).ToLocalChecked(), config);

        if (status != napi_ok)
        {
            break;
        }
        ret = instance->rtc_engine_->playEffect(effect_id, config);
        delete[] config;
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, stopEffect)
{
    CHECK_API_FUNC(NertcNodeEngine, 1)
    int ret = -1;
    do
    {
        CHECK_NATIVE_THIS(instance);
        auto status = napi_ok;
        uint32_t effect_id;
        GET_ARGS_VALUE(isolate, 0, uint32, effect_id)
        if (status != napi_ok)
        {
            break;
        }
        ret = instance->rtc_engine_->stopEffect(effect_id);
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, stopAllEffects)
{
    CHECK_API_FUNC(NertcNodeEngine, 0)
    int ret = -1;
    do
    {
        CHECK_NATIVE_THIS(instance);
        ret = instance->rtc_engine_->stopAllEffects();
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, pauseEffect)
{
    CHECK_API_FUNC(NertcNodeEngine, 1)
    int ret = -1;
    do
    {
        CHECK_NATIVE_THIS(instance);
        auto status = napi_ok;
        uint32_t effect_id;
        GET_ARGS_VALUE(isolate, 0, uint32, effect_id)
        if (status != napi_ok)
        {
            break;
        }
        ret = instance->rtc_engine_->pauseEffect(effect_id);
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, resumeEffect)
{
    CHECK_API_FUNC(NertcNodeEngine, 1)
    int ret = -1;
    do
    {
        CHECK_NATIVE_THIS(instance);
        auto status = napi_ok;
        uint32_t effect_id;
        GET_ARGS_VALUE(isolate, 0, uint32, effect_id)
        if (status != napi_ok)
        {
            break;
        }
        ret = instance->rtc_engine_->resumeEffect(effect_id);
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, pauseAllEffects)
{
    CHECK_API_FUNC(NertcNodeEngine, 0)
    int ret = -1;
    do
    {
        CHECK_NATIVE_THIS(instance);
        ret = instance->rtc_engine_->pauseAllEffects();
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, resumeAllEffects)
{
    CHECK_API_FUNC(NertcNodeEngine, 0)
    int ret = -1;
    do
    {
        CHECK_NATIVE_THIS(instance);
        ret = instance->rtc_engine_->resumeAllEffects();
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, setEffectSendVolume)
{
    CHECK_API_FUNC(NertcNodeEngine, 2)
    int ret = -1;
    do
    {
        CHECK_NATIVE_THIS(instance);
        auto status = napi_ok;
        uint32_t effect_id, volume;
        GET_ARGS_VALUE(isolate, 0, uint32, effect_id)
        GET_ARGS_VALUE(isolate, 1, uint32, volume)
        if (status != napi_ok)
        {
            break;
        }
        ret = instance->rtc_engine_->setEffectSendVolume(effect_id, volume);
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, getEffectSendVolume)
{
    CHECK_API_FUNC(NertcNodeEngine, 1)
    uint32_t volume = 0;
    int ret = -1;
    do
    {
        CHECK_NATIVE_THIS(instance);
        auto status = napi_ok;
        uint32_t effect_id;
        GET_ARGS_VALUE(isolate, 0, uint32, effect_id)
        if (status != napi_ok)
        {
            break;
        }
        ret = instance->rtc_engine_->getEffectSendVolume(effect_id, &volume);
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret == 0 ? volume : ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, setEffectPlaybackVolume)
{
    CHECK_API_FUNC(NertcNodeEngine, 2)
    int ret = -1;
    do
    {
        CHECK_NATIVE_THIS(instance);
        auto status = napi_ok;
        uint32_t effect_id, volume;
        GET_ARGS_VALUE(isolate, 0, uint32, effect_id)
        GET_ARGS_VALUE(isolate, 1, uint32, volume)
        if (status != napi_ok)
        {
            break;
        }
        ret = instance->rtc_engine_->setEffectPlaybackVolume(effect_id, volume);
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, getEffectPlaybackVolume)
{
    CHECK_API_FUNC(NertcNodeEngine, 1)
    uint32_t ret = 0, vol = 0;
    do
    {
        CHECK_NATIVE_THIS(instance);
        auto status = napi_ok;
        uint32_t effect_id;
        GET_ARGS_VALUE(isolate, 0, uint32, effect_id)
        if (status != napi_ok)
        {
            break;
        }
        ret = instance->rtc_engine_->getEffectPlaybackVolume(effect_id, &vol);
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret == 0 ? vol : ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, enableEarback)
{
    CHECK_API_FUNC(NertcNodeEngine, 2)
    int ret = -1;
    do
    {
        CHECK_NATIVE_THIS(instance);
        auto status = napi_ok;
        uint32_t volume;
        bool enabled;
        GET_ARGS_VALUE(isolate, 0, bool, enabled)
        GET_ARGS_VALUE(isolate, 1, uint32, volume)
        if (status != napi_ok)
        {
            break;
        }
        ret = instance->rtc_engine_->enableEarback(enabled, volume);
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, setEarbackVolume)
{
    CHECK_API_FUNC(NertcNodeEngine, 1)
    int ret = -1;
    do
    {
        CHECK_NATIVE_THIS(instance);
        auto status = napi_ok;
        uint32_t volume;
        GET_ARGS_VALUE(isolate, 0, uint32, volume)
        if (status != napi_ok)
        {
            break;
        }
        ret = instance->rtc_engine_->setEarbackVolume(volume);
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, onStatsObserver)
{
    CHECK_API_FUNC(NertcNodeEngine, 3)
    do
    {
        CHECK_NATIVE_THIS(instance);
        instance->rtc_engine_->setStatsObserver(NertcNodeRtcMediaStatsHandler::GetInstance());
        auto status = napi_ok;
        UTF8String eventName;
        bool enable;
        GET_ARGS_VALUE(isolate, 0, utf8string, eventName)
        GET_ARGS_VALUE(isolate, 1, bool, enable)
        if (status != napi_ok || eventName.length() == 0)
        {
            break;
        }
        if (!enable)
        {
            auto sz = NertcNodeRtcMediaStatsHandler::GetInstance()->RemoveEventHandler(eventName.toUtf8String());
            if (sz == 0)
            {
                instance->rtc_engine_->setStatsObserver(nullptr);
            }
        }
        else
        {
            ASSEMBLE_REG_CALLBACK(2, NertcNodeRtcMediaStatsHandler, eventName.toUtf8String())
        }
    } while (false);
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, enableAudioVolumeIndication)
{
    CHECK_API_FUNC(NertcNodeEngine, 2)
    int ret = -1;
    do
    {
        CHECK_NATIVE_THIS(instance);
        auto status = napi_ok;
        uint64_t interval;
        bool enabled;
        GET_ARGS_VALUE(isolate, 0, bool, enabled)
        GET_ARGS_VALUE(isolate, 1, uint64, interval)
        if (status != napi_ok)
        {
            break;
        }
        ret = instance->rtc_engine_->enableAudioVolumeIndication(enabled, interval);
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

// NIM_SDK_NODE_API_DEF(NertcNodeEngine, startScreenCaptureByScreenRect)
// {
//     CHECK_API_FUNC(NertcNodeEngine, 3)
//     int ret = -1;
//     do
//     {
//         CHECK_NATIVE_THIS(instance);
//         auto status = napi_ok;
//         nertc::NERtcRectangle screen_rect = {}, region_rect = {};
//         nertc::NERtcScreenCaptureParameters param = {};
//         status = nertc_rectangle_obj_to_struct(isolate, args[0]->ToObject(isolate->GetCurrentContext()).ToLocalChecked(), screen_rect);
//         if (status != napi_ok) break;
//         status = nertc_rectangle_obj_to_struct(isolate, args[1]->ToObject(isolate->GetCurrentContext()).ToLocalChecked(), region_rect);
//         if (status != napi_ok) break;
//         status = nertc_screen_capture_params_obj_to_struct(isolate, args[2]->ToObject(isolate->GetCurrentContext()).ToLocalChecked(), param);

//         if (status == napi_ok)
//         {
//             ret = instance->rtc_engine_->startScreenCaptureByScreenRect(screen_rect, region_rect, param);
//             if (param.excluded_window_list != nullptr)
//             {
//                 delete[] param.excluded_window_list;
//                 param.excluded_window_list = nullptr;
//             }
//         }
//     } while (false);
//     args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
// }

// NIM_SDK_NODE_API_DEF(NertcNodeEngine, startScreenCaptureByDisplayId)
// {
//     CHECK_API_FUNC(NertcNodeEngine, 3)
//     int ret = -1;
//     do
//     {
//         CHECK_NATIVE_THIS(instance);
//         auto status = napi_ok;
//         int64_t display;
//         nertc::NERtcRectangle region_rect = {};
//         nertc::NERtcScreenCaptureParameters param = {};
//         GET_ARGS_VALUE(isolate, 0, int64, display)
//         status = nertc_rectangle_obj_to_struct(isolate, args[1]->ToObject(isolate->GetCurrentContext()).ToLocalChecked(), region_rect);
//         if (status != napi_ok) break;
//         status = nertc_screen_capture_params_obj_to_struct(isolate, args[2]->ToObject(isolate->GetCurrentContext()).ToLocalChecked(), param);

//         if (status == napi_ok)
//         {
// #ifdef WIN32
//             RECT rc = instance->_windows_helper->getCachedRect(display);
//             if (rc.bottom == 0 && rc.left == 0 && rc.right == 0 && rc.top == 0)
//             {
//                 WindowsHelpers::CaptureTargetInfoList list;
//                 instance->_windows_helper->getCaptureWindowList(&list, 1);
//                 for (auto w : list)
//                 {
//                     if (std::to_string(display) == w.display_id)
//                     {
//                         rc = w.rc;
//                         instance->_windows_helper->updateCachedInfos(display, rc);
//                         break;
//                     }
//                 }
//             }
//             if (rc.bottom != 0 || rc.left != 0 || rc.right != 0 || rc.top != 0)
//             {
//                 nertc::NERtcRectangle screen_rect = {};
//                 screen_rect.x = rc.left;
//                 screen_rect.y = rc.top;
//                 screen_rect.width = rc.right - rc.left;
//                 screen_rect.height = rc.bottom - rc.top;
//                 ret = instance->rtc_engine_->startScreenCaptureByScreenRect(screen_rect, region_rect, param);
//             }
//             else
//             {
//                 ret = -100;
//             }
// #else
//             ret = instance->rtc_engine_->startScreenCaptureByDisplayId(display, region_rect, param);
// #endif
//             if (param.excluded_window_list != nullptr)
//             {
//                 delete[] param.excluded_window_list;
//                 param.excluded_window_list = nullptr;
//             }
//         }

//     } while (false);
//     args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
// }

// NIM_SDK_NODE_API_DEF(NertcNodeEngine, startScreenCaptureByWindowId)
// {
//     CHECK_API_FUNC(NertcNodeEngine, 3)
//     int ret = -1;
//     do
//     {
//         CHECK_NATIVE_THIS(instance);
//         auto status = napi_ok;
//         int32_t windowid;
//         nertc::NERtcRectangle region_rect = {};
//         nertc::NERtcScreenCaptureParameters param = {};
//         GET_ARGS_VALUE(isolate, 0, int32, windowid)
//         status = nertc_rectangle_obj_to_struct(isolate, args[1]->ToObject(isolate->GetCurrentContext()).ToLocalChecked(), region_rect);
//         if (status != napi_ok) break;
//         status = nertc_screen_capture_params_obj_to_struct(isolate, args[2]->ToObject(isolate->GetCurrentContext()).ToLocalChecked(), param);

//         if (status == napi_ok)
//         {
// #ifdef WIN32
//             ret = instance->rtc_engine_->startScreenCaptureByWindowId(reinterpret_cast<void *>(windowid), region_rect, param);
// #else
//             ret = instance->rtc_engine_->startScreenCaptureByWindowId(reinterpret_cast<void *>(&windowid), region_rect, param);
// #endif
//             if (param.excluded_window_list != nullptr)
//             {
//                 delete[] param.excluded_window_list;
//                 param.excluded_window_list = nullptr;
//             }
//         }

//     } while (false);
//     args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
// }

NIM_SDK_NODE_API_DEF(NertcNodeEngine, updateScreenCaptureRegion)
{
    CHECK_API_FUNC(NertcNodeEngine, 1)
    int ret = -1;
    do
    {
        CHECK_NATIVE_THIS(instance);
        auto status = napi_ok;
        nertc::NERtcRectangle region_rect = {};
        status = nertc_rectangle_obj_to_struct(isolate, args[0]->ToObject(isolate->GetCurrentContext()).ToLocalChecked(), region_rect);

        if (status != napi_ok)
        {
            break;
        }
        ret = instance->rtc_engine_->updateScreenCaptureRegion(region_rect);
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, stopScreenCapture)
{
    CHECK_API_FUNC(NertcNodeEngine, 0)
    int ret = -1;
    do
    {
        CHECK_NATIVE_THIS(instance);
        ret = instance->rtc_engine_->stopScreenCapture();
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, pauseScreenCapture)
{
    CHECK_API_FUNC(NertcNodeEngine, 0)
    int ret = -1;
    do
    {
        CHECK_NATIVE_THIS(instance);
        ret = instance->rtc_engine_->pauseScreenCapture();
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, resumeScreenCapture)
{
    CHECK_API_FUNC(NertcNodeEngine, 0)
    int ret = -1;
    do
    {
        CHECK_NATIVE_THIS(instance);
        ret = instance->rtc_engine_->resumeScreenCapture();
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, setExternalVideoSource)
{
    CHECK_API_FUNC(NertcNodeEngine, 1)
    int ret = -1;
    do
    {
        CHECK_NATIVE_THIS(instance);
        auto status = napi_ok;
        bool enabled;
        GET_ARGS_VALUE(isolate, 0, bool, enabled)
        if (status != napi_ok)
        {
            break;
        }
        ret = instance->rtc_engine_->setExternalVideoSource(enabled);
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, pushExternalVideoFrame)
{
    //TODO(litianyi)
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, getVersion)
{
    CHECK_API_FUNC(NertcNodeEngine, 0)
    std::string ret;
    do
    {
        CHECK_NATIVE_THIS(instance);
        int32_t build;
        ret = (std::string)instance->rtc_engine_->getVersion(&build);
    } while (false);
    args.GetReturnValue().Set(String::NewFromUtf8(args.GetIsolate(), ret.c_str(), NewStringType::kNormal).ToLocalChecked());
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, getErrorDescription)
{
    CHECK_API_FUNC(NertcNodeEngine, 1)
    std::string ret;
    do
    {
        CHECK_NATIVE_THIS(instance);
        auto status = napi_ok;
        int32_t error;
        GET_ARGS_VALUE(isolate, 0, int32, error)
        ret = (std::string)instance->rtc_engine_->getErrorDescription(error);
    } while (false);
    args.GetReturnValue().Set(String::NewFromUtf8(args.GetIsolate(), ret.c_str(), NewStringType::kNormal).ToLocalChecked());
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, uploadSdkInfo)
{
    CHECK_API_FUNC(NertcNodeEngine, 0)
    do
    {
        CHECK_NATIVE_THIS(instance);
        instance->rtc_engine_->uploadSdkInfo();
    } while (false);
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, addLiveStreamTask)
{
    CHECK_API_FUNC(NertcNodeEngine, 1)
    int ret = -1;
    do
    {
        CHECK_NATIVE_THIS(instance);
        auto status = napi_ok;
        nertc::NERtcLiveStreamTaskInfo info = {0};
        status = nertc_ls_task_info_obj_to_struct(isolate, args[0]->ToObject(isolate->GetCurrentContext()).ToLocalChecked(), info);
        if (status == napi_ok)
        {
            memset(info.extraInfo, 0, kNERtcMacSEIBufferLength);
            // info.config = {0};
            ret = instance->rtc_engine_->addLiveStreamTask(info);
            if (info.layout.users)
            {
                delete[] info.layout.users;
                info.layout.users = nullptr;
            };
            if (info.layout.bg_image)
            {
                delete info.layout.bg_image;
                info.layout.bg_image = nullptr;
            }
        }
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, updateLiveStreamTask)
{
    CHECK_API_FUNC(NertcNodeEngine, 1)
    int ret = -1;
    do
    {
        CHECK_NATIVE_THIS(instance);
        auto status = napi_ok;
        nertc::NERtcLiveStreamTaskInfo info = {};
        status = nertc_ls_task_info_obj_to_struct(isolate, args[0]->ToObject(isolate->GetCurrentContext()).ToLocalChecked(), info);
        if (status == napi_ok)
        {
            ret = instance->rtc_engine_->updateLiveStreamTask(info);
            if (info.layout.users)
            {
                delete[] info.layout.users;
                info.layout.users = nullptr;
            };
            if (info.layout.bg_image)
            {
                delete info.layout.bg_image;
                info.layout.bg_image = nullptr;
            }
        }
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, removeLiveStreamTask)
{
    CHECK_API_FUNC(NertcNodeEngine, 1)
    int ret = -1;
    do
    {
        CHECK_NATIVE_THIS(instance);
        auto status = napi_ok;
        UTF8String task_id;
        GET_ARGS_VALUE(isolate, 0, utf8string, task_id)
        if (status != napi_ok || task_id.length() == 0)
        {
            break;
        }
        ret = instance->rtc_engine_->removeLiveStreamTask(task_id.get());
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, enumerateRecordDevices)
{
    CHECK_API_FUNC(NertcNodeEngine, 0)
    Local<Array> arr = Array::New(isolate);;
    do
    {
        CHECK_NATIVE_ADM_THIS(instance);
        auto devices = instance->_adm->enumerateRecordDevices();
        if (devices != nullptr)
        {
            size_t count = devices->getCount();
            for (size_t i = 0; i < count; i++)
            {
                nertc::NERtcDeviceInfo info;
                Local<Object> obj = Object::New(isolate);;
                if (devices->getDeviceInfo(i, &info) == 0)
                {
                    obj->Set(isolate->GetCurrentContext(),  nim_napi_new_utf8string(isolate, "device_id"), nim_napi_new_utf8string(isolate, (char *)info.device_id));
                    obj->Set(isolate->GetCurrentContext(),  nim_napi_new_utf8string(isolate, "device_name"), nim_napi_new_utf8string(isolate, (char *)info.device_name));
                    obj->Set(isolate->GetCurrentContext(),  nim_napi_new_utf8string(isolate, "transport_type"), nim_napi_new_uint32(isolate, info.transport_type));
                    obj->Set(isolate->GetCurrentContext(),  nim_napi_new_utf8string(isolate, "suspected_unavailable"), nim_napi_new_bool(isolate, info.suspected_unavailable));
                    obj->Set(isolate->GetCurrentContext(),  nim_napi_new_utf8string(isolate, "system_default_device"), nim_napi_new_bool(isolate, info.system_default_device));
                    arr->Set(isolate->GetCurrentContext(), i, obj);
                }
            }
            devices->destroy();
        }

    } while (false);
    args.GetReturnValue().Set(arr);
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, setRecordDevice)
{
    CHECK_API_FUNC(NertcNodeEngine, 1)
    int ret = -1;
    do
    {
        CHECK_NATIVE_ADM_THIS(instance);
        auto status = napi_ok;
        UTF8String device;
        GET_ARGS_VALUE(isolate, 0, utf8string, device)
        if (status != napi_ok || device.length() == 0)
        {
            break;
        }
        ret = instance->_adm->setRecordDevice(device.get());
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, getRecordDevice)
{
    CHECK_API_FUNC(NertcNodeEngine, 0)
    char id[256]; int ret = -1;
    do
    {
        CHECK_NATIVE_ADM_THIS(instance);
        ret = instance->_adm->getRecordDevice(id);
    } while (false);
    args.GetReturnValue().Set(String::NewFromUtf8(args.GetIsolate(), ret== 0 ? (char*)id : "", NewStringType::kNormal).ToLocalChecked());
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, enumeratePlayoutDevices)
{
    CHECK_API_FUNC(NertcNodeEngine, 0)
    Local<Array> arr = Array::New(isolate);
    do
    {
        CHECK_NATIVE_ADM_THIS(instance);
        auto devices = instance->_adm->enumeratePlayoutDevices();
        if (devices != nullptr)
        {
            size_t count = devices->getCount();
            for (size_t i = 0; i < count; i++)
            {
                nertc::NERtcDeviceInfo info;
                Local<Object> obj = Object::New(isolate);;
                if (devices->getDeviceInfo(i, &info) == 0)
                {
                    obj->Set(isolate->GetCurrentContext(),  nim_napi_new_utf8string(isolate, "device_id"), nim_napi_new_utf8string(isolate, (char *)info.device_id));
                    obj->Set(isolate->GetCurrentContext(),  nim_napi_new_utf8string(isolate, "device_name"), nim_napi_new_utf8string(isolate, (char *)info.device_name));
                    obj->Set(isolate->GetCurrentContext(),  nim_napi_new_utf8string(isolate, "transport_type"), nim_napi_new_uint32(isolate, info.transport_type));
                    obj->Set(isolate->GetCurrentContext(),  nim_napi_new_utf8string(isolate, "suspected_unavailable"), nim_napi_new_bool(isolate, info.suspected_unavailable));
                    obj->Set(isolate->GetCurrentContext(),  nim_napi_new_utf8string(isolate, "system_default_device"), nim_napi_new_bool(isolate, info.system_default_device));
                    arr->Set(isolate->GetCurrentContext(), i, obj);
                }
            }
            devices->destroy();
        }

    } while (false);
    args.GetReturnValue().Set(arr);
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, setPlayoutDevice)
{
    CHECK_API_FUNC(NertcNodeEngine, 1)
    int ret = -1;
    do
    {
        CHECK_NATIVE_ADM_THIS(instance);
        auto status = napi_ok;
        UTF8String device;
        GET_ARGS_VALUE(isolate, 0, utf8string, device)
        if (status != napi_ok || device.length() == 0)
        {
            break;
        }
        ret = instance->_adm->setPlayoutDevice(device.get());
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, getPlayoutDevice)
{
    CHECK_API_FUNC(NertcNodeEngine, 0)
    char id[256]; int ret = -1;
    do
    {
        CHECK_NATIVE_ADM_THIS(instance);
        ret = instance->_adm->getPlayoutDevice(id);
    } while (false);
    args.GetReturnValue().Set(String::NewFromUtf8(args.GetIsolate(), ret== 0 ? (char*)id : "", NewStringType::kNormal).ToLocalChecked());
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, setRecordDeviceVolume)
{
    CHECK_API_FUNC(NertcNodeEngine, 1)
    int ret = -1;
    do
    {
        CHECK_NATIVE_ADM_THIS(instance);
        auto status = napi_ok;
        uint32_t volume;
        GET_ARGS_VALUE(isolate, 0, uint32, volume)
        if (status != napi_ok)
        {
            break;
        }
        ret = instance->_adm->setRecordDeviceVolume(volume);
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, getRecordDeviceVolume)
{
    CHECK_API_FUNC(NertcNodeEngine, 0)
    int ret = -1;
    uint32_t volume = 0;
    do
    {
        CHECK_NATIVE_ADM_THIS(instance);
        auto status = napi_ok;
        ret = instance->_adm->getRecordDeviceVolume(&volume);
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret == 0 ? volume : ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, setPlayoutDeviceVolume)
{
    CHECK_API_FUNC(NertcNodeEngine, 1)
    int ret = -1;
    do
    {
        CHECK_NATIVE_ADM_THIS(instance);
        auto status = napi_ok;
        uint32_t volume;
        GET_ARGS_VALUE(isolate, 0, uint32, volume)
        if (status != napi_ok)
        {
            break;
        }
        ret = instance->_adm->setPlayoutDeviceVolume(volume);
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, getPlayoutDeviceVolume)
{
    CHECK_API_FUNC(NertcNodeEngine, 0)
    int ret = -1;
    uint32_t volume = 0;
    do
    {
        CHECK_NATIVE_ADM_THIS(instance);
        auto status = napi_ok;
        ret = instance->_adm->getPlayoutDeviceVolume(&volume);
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret == 0 ? volume : ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, setPlayoutDeviceMute)
{
    CHECK_API_FUNC(NertcNodeEngine, 1)
    int ret = -1;
    do
    {
        CHECK_NATIVE_ADM_THIS(instance);
        auto status = napi_ok;
        bool mute;
        GET_ARGS_VALUE(isolate, 0, bool, mute)
        if (status != napi_ok)
        {
            break;
        }
        ret = instance->_adm->setPlayoutDeviceMute(mute);
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, getPlayoutDeviceMute)
{
    CHECK_API_FUNC(NertcNodeEngine, 0)
    bool ret = false;
    do
    {
        CHECK_NATIVE_ADM_THIS(instance);
        instance->_adm->getPlayoutDeviceMute(&ret);
    } while (false);
    args.GetReturnValue().Set(Boolean::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, setRecordDeviceMute)
{
    CHECK_API_FUNC(NertcNodeEngine, 1)
    int ret = -1;
    do
    {
        CHECK_NATIVE_ADM_THIS(instance);
        auto status = napi_ok;
        bool mute;
        GET_ARGS_VALUE(isolate, 0, bool, mute)
        if (status != napi_ok)
        {
            break;
        }
        ret = instance->_adm->setRecordDeviceMute(mute);
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, getRecordDeviceMute)
{
    CHECK_API_FUNC(NertcNodeEngine, 0)
    bool ret = false;
    do
    {
        CHECK_NATIVE_ADM_THIS(instance);
        instance->_adm->getRecordDeviceMute(&ret);
    } while (false);
    args.GetReturnValue().Set(Boolean::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, adjustRecordingSignalVolume)
{
    CHECK_API_FUNC(NertcNodeEngine, 1)
    int ret = -1;
    do
    {
        CHECK_NATIVE_ADM_THIS(instance);
        auto status = napi_ok;
        uint32_t volume;
        GET_ARGS_VALUE(isolate, 0, uint32, volume)
        if (status != napi_ok)
        {
            break;
        }
        ret = instance->_adm->adjustRecordingSignalVolume(volume);
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, adjustPlaybackSignalVolume)
{
    CHECK_API_FUNC(NertcNodeEngine, 1)
    int ret = -1;
    do
    {
        CHECK_NATIVE_ADM_THIS(instance);
        auto status = napi_ok;
        uint32_t volume;
        GET_ARGS_VALUE(isolate, 0, uint32, volume)
        if (status != napi_ok)
        {
            break;
        }
        ret = instance->_adm->adjustPlaybackSignalVolume(volume);
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, startRecordDeviceTest)
{
    CHECK_API_FUNC(NertcNodeEngine, 1)
    int ret = -1;
    do
    {
        CHECK_NATIVE_ADM_THIS(instance);
        auto status = napi_ok;
        uint64_t interval;
        GET_ARGS_VALUE(isolate, 0, uint64, interval)
        if (status != napi_ok)
        {
            break;
        }
        ret = instance->_adm->startRecordDeviceTest(interval);
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, stopRecordDeviceTest)
{
    CHECK_API_FUNC(NertcNodeEngine, 0)
    int ret = -1;
    do
    {
        CHECK_NATIVE_ADM_THIS(instance);
        ret = instance->_adm->stopRecordDeviceTest();
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, startPlayoutDeviceTest)
{
    CHECK_API_FUNC(NertcNodeEngine, 1)
    int ret = -1;
    do
    {
        CHECK_NATIVE_ADM_THIS(instance);
        auto status = napi_ok;
        UTF8String path;
        GET_ARGS_VALUE(isolate, 0, utf8string, path)
        if (status != napi_ok || path.length() == 0)
        {
            break;
        }
        ret = instance->_adm->startPlayoutDeviceTest(path.get());
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, stopPlayoutDeviceTest)
{
    CHECK_API_FUNC(NertcNodeEngine, 0)
    int ret = -1;
    do
    {
        CHECK_NATIVE_ADM_THIS(instance);
        ret = instance->_adm->stopPlayoutDeviceTest();
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, startAudioDeviceLoopbackTest)
{
    CHECK_API_FUNC(NertcNodeEngine, 1)
    int ret = -1;
    do
    {
        CHECK_NATIVE_ADM_THIS(instance);
        auto status = napi_ok;
        uint64_t interval;
        GET_ARGS_VALUE(isolate, 0, uint64, interval)
        if (status != napi_ok)
        {
            break;
        }
        ret = instance->_adm->startAudioDeviceLoopbackTest(interval);
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, stopAudioDeviceLoopbackTest)
{
    CHECK_API_FUNC(NertcNodeEngine, 0)
    int ret = -1;
    do
    {
        CHECK_NATIVE_ADM_THIS(instance);
        ret = instance->_adm->stopAudioDeviceLoopbackTest();
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, enumerateCaptureDevices)
{
    CHECK_API_FUNC(NertcNodeEngine, 0)
    Local<Array> arr = Array::New(isolate);
    do
    {
        CHECK_NATIVE_VDM_THIS(instance);
        auto devices = instance->_vdm->enumerateCaptureDevices();
        if (devices != nullptr)
        {
            size_t count = devices->getCount();
            for (size_t i = 0; i < count; i++)
            {
                nertc::NERtcDeviceInfo info;
                if (devices->getDeviceInfo(i, &info) == 0)
                {
                    Local<Object> obj = Object::New(isolate);
                    obj->Set(isolate->GetCurrentContext(),  nim_napi_new_utf8string(isolate, "device_id"), nim_napi_new_utf8string(isolate, (const char*)info.device_id));
                    obj->Set(isolate->GetCurrentContext(),  nim_napi_new_utf8string(isolate, "device_name"), nim_napi_new_utf8string(isolate, (const char*)info.device_name));
                    obj->Set(isolate->GetCurrentContext(),  nim_napi_new_utf8string(isolate, "transport_type"), nim_napi_new_uint32(isolate, info.transport_type));
                    obj->Set(isolate->GetCurrentContext(),  nim_napi_new_utf8string(isolate, "suspected_unavailable"), nim_napi_new_bool(isolate, info.suspected_unavailable));
                    obj->Set(isolate->GetCurrentContext(),  nim_napi_new_utf8string(isolate, "system_default_device"), nim_napi_new_bool(isolate, info.system_default_device));
                    arr->Set(isolate->GetCurrentContext(), i, obj);
                }
            }
            devices->destroy();
        }

    } while (false);
    args.GetReturnValue().Set(arr);
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, setDevice)
{
    CHECK_API_FUNC(NertcNodeEngine, 1)
    int ret = -1;
    do
    {
        CHECK_NATIVE_VDM_THIS(instance);
        auto status = napi_ok;
        UTF8String device;
        GET_ARGS_VALUE(isolate, 0, utf8string, device)
        if (status != napi_ok || device.length() == 0)
        {
            break;
        }
        ret = instance->_vdm->setDevice(device.get());
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, getDevice)
{
    CHECK_API_FUNC(NertcNodeEngine, 0)
    char id[256]; int ret = -1;
    do
    {
        CHECK_NATIVE_VDM_THIS(instance);
        ret = instance->_vdm->getDevice(id);
    } while (false);
    args.GetReturnValue().Set(String::NewFromUtf8(args.GetIsolate(), ret== 0 ? (char*)id : "", NewStringType::kNormal).ToLocalChecked());
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, setClientRole)
{
    CHECK_API_FUNC(NertcNodeEngine, 1)
    int ret = -1;
    do
    {
        CHECK_NATIVE_THIS(instance);
        auto status = napi_ok;
        uint32_t role;
        GET_ARGS_VALUE(isolate, 0, uint32, role)
        if (status != napi_ok)
        {
            break;
        }
        ret = instance->rtc_engine_->setClientRole((nertc::NERtcClientRole)role);
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, setupSubStreamVideoCanvas)
{
    CHECK_API_FUNC(NertcNodeEngine, 2)
    int ret = -1;
    do
    {
        CHECK_NATIVE_THIS(instance);
        auto status = napi_ok;
        uint64_t uid;
        bool enable;
        GET_ARGS_VALUE(isolate, 0, uint64, uid)
        GET_ARGS_VALUE(isolate, 1, bool, enable)
        if (status != napi_ok)
        {
            break;
        }
        nertc::NERtcVideoCanvas canvas;
        canvas.cb = enable ? NodeVideoFrameTransporter::onSubstreamFrameDataCallback : nullptr;
        canvas.user_data = enable ? (void*)(new nertc::uid_t(uid)) : nullptr;
        canvas.window = nullptr;
        if (uid == 0)
            ret = instance->rtc_engine_->setupLocalSubStreamVideoCanvas(&canvas);
        else
            ret = instance->rtc_engine_->setupRemoteSubStreamVideoCanvas(uid, &canvas);
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, subscribeRemoteVideoSubStream)
{
    CHECK_API_FUNC(NertcNodeEngine, 2)
    int ret = -1;
    do
    {
        CHECK_NATIVE_VDM_THIS(instance);
        auto status = napi_ok;
        bool sub;
        nertc::uid_t uid;
        GET_ARGS_VALUE(isolate, 0, uint64, uid)
        GET_ARGS_VALUE(isolate, 1, bool, sub)
        if (status != napi_ok)
        {
            break;
        }
        ret = instance->rtc_engine_->subscribeRemoteVideoSubStream(uid, sub);
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, setMixedAudioFrameParameters)
{
    CHECK_API_FUNC(NertcNodeEngine, 1)
    int ret = -1;
    do
    {
        CHECK_NATIVE_VDM_THIS(instance);
        auto status = napi_ok;
        int samp;
        GET_ARGS_VALUE(isolate, 0, int32, samp)
        if (status != napi_ok)
        {
            break;
        }
        ret = instance->rtc_engine_->setMixedAudioFrameParameters(samp);
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, setExternalAudioSource)
{
    CHECK_API_FUNC(NertcNodeEngine, 3)
    int ret = -1;
    do
    {
        CHECK_NATIVE_VDM_THIS(instance);
        auto status = napi_ok;
        bool enabled;
        int samp, chan;
        GET_ARGS_VALUE(isolate, 0, bool, enabled)
        GET_ARGS_VALUE(isolate, 1, int32, samp)
        GET_ARGS_VALUE(isolate, 2, int32, chan)
        if (status != napi_ok)
        {
            break;
        }
        ret = instance->rtc_engine_->setExternalAudioSource(enabled, samp, chan);
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, pushExternalAudioFrame)
{
    CHECK_API_FUNC(NertcNodeEngine, 1)
    int ret = -1;
    do
    {
        CHECK_NATIVE_VDM_THIS(instance);
        // TODO(litianyi)
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, sendSEIMsg)
{
    CHECK_API_FUNC(NertcNodeEngine, 1)
    int ret = -1;
    do
    {
        CHECK_NATIVE_ADM_THIS(instance);
        if (!args[0]->IsArrayBuffer())
            break;
        auto buffer = args[0].As<ArrayBuffer>();
        ret = instance->rtc_engine_->sendSEIMsg(
            static_cast<const char*>(buffer->GetContents().Data()),
            buffer->GetContents().ByteLength());
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, sendSEIMsgEx)
{
    CHECK_API_FUNC(NertcNodeEngine, 2)
    int ret = -1;
    do
    {
        CHECK_NATIVE_ADM_THIS(instance);
        auto status = napi_ok;
        if (!args[0]->IsArrayBuffer())
            break;
        auto buffer = args[0].As<ArrayBuffer>();

        int32_t type;
        GET_ARGS_VALUE(isolate, 1, int32, type)

        ret = instance->rtc_engine_->sendSEIMsg(
            static_cast<const char*>(buffer->GetContents().Data()), 
            buffer->GetContents().ByteLength(),
            static_cast<nertc::NERtcVideoStreamType>(type));
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, setExternalAudioRender)
{
    CHECK_API_FUNC(NertcNodeEngine, 3)
    int ret = -1;
    do
    {
        CHECK_NATIVE_THIS(instance);
        auto status = napi_ok;
        bool enable;
        int sample_rate = 0;
        int channels = 0;
        GET_ARGS_VALUE(isolate, 0, bool, enable)
        GET_ARGS_VALUE(isolate, 1, int32, sample_rate)
        if (status != napi_ok)
            break;
        GET_ARGS_VALUE(isolate, 2, int32, channels)
        if (status != napi_ok)
            break;
        ret = instance->rtc_engine_->setExternalAudioRender(enable, sample_rate, channels);
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, pullExternalAudioFrame)
{
    CHECK_API_FUNC(NertcNodeEngine, 2)
    int ret = -1;
    do
    {
        CHECK_NATIVE_ADM_THIS(instance);
        auto status = napi_ok;
        int length = 0;
        GET_ARGS_VALUE(isolate, 0, int32, length)
        if (status != napi_ok)
            break;
        ASSEMBLE_BASE_CALLBACK(1);
        auto shared_data = std::make_shared<unsigned char>(length);
        ret = instance->rtc_engine_->pullExternalAudioFrame(shared_data.get(), length);
        NertcNodeEventHandler::GetInstance()->onPullExternalAudioFrame(bcb, shared_data, length);
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, setAudioEffectPreset)
{
    CHECK_API_FUNC(NertcNodeEngine, 1)
    int ret = -1;
    do
    {
        CHECK_NATIVE_THIS(instance);
        auto status = napi_ok;
        int32_t type;
        GET_ARGS_VALUE(isolate, 0, int32, type)
        if (status != napi_ok)
        {
            break;
        }
        ret = instance->rtc_engine_->setAudioEffectPreset(static_cast<nertc::NERtcVoiceChangerType>(type));
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, setVoiceBeautifierPreset)
{
    CHECK_API_FUNC(NertcNodeEngine, 1)
    int ret = -1;
    do
    {
        CHECK_NATIVE_THIS(instance);
        auto status = napi_ok;
        int32_t type;
        GET_ARGS_VALUE(isolate, 0, int32, type)
        if (status != napi_ok)
        {
            break;
        }
        ret = instance->rtc_engine_->setVoiceBeautifierPreset(static_cast<nertc::NERtcVoiceBeautifierType>(type));
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, setLocalVoicePitch)
{
    CHECK_API_FUNC(NertcNodeEngine, 1)
    int ret = -1;
    do
    {
        CHECK_NATIVE_THIS(instance);
        auto status = napi_ok;
        double pitch;
        GET_ARGS_VALUE(isolate, 0, double, pitch)
        if (status != napi_ok)
        {
            break;
        }
        ret = instance->rtc_engine_->setLocalVoicePitch(pitch);
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, setLocalVoiceEqualization)
{
    CHECK_API_FUNC(NertcNodeEngine, 2)
    int ret = -1;
    do
    {
        CHECK_NATIVE_THIS(instance);
        auto status = napi_ok;
        int32_t band_frequency;
        int32_t band_gain;
        GET_ARGS_VALUE(isolate, 0, int32, band_frequency)
        if (status != napi_ok)
            break;
        GET_ARGS_VALUE(isolate, 1, int32, band_gain)
        if (status != napi_ok)
            break;
        ret = instance->rtc_engine_->setLocalVoiceEqualization(
            static_cast<nertc::NERtcVoiceEqualizationBand>(band_frequency), 
            band_gain);
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

// NIM_SDK_NODE_API_DEF(NertcNodeEngine, setRemoteHighPriorityAudioStream)
// {
//     CHECK_API_FUNC(NertcNodeEngine, 3)
//     int ret = -1;
//     do
//     {
//         CHECK_NATIVE_THIS(instance);
//         auto status = napi_ok;
//         bool enable = false;
//         uint64_t uid = 0;
//         int32_t stream_type;
//         GET_ARGS_VALUE(isolate, 0, bool, enable)
//         if (status != napi_ok)
//             break;
//         GET_ARGS_VALUE(isolate, 1, uint64, uid)
//         if (status != napi_ok)
//             break;
//         GET_ARGS_VALUE(isolate, 2, int32, stream_type)
//         if (status != napi_ok)
//             break;
//         ret = instance->rtc_engine_->setRemoteHighPriorityAudioStream(enable, uid, 
//             static_cast<nertc::NERtcAudioStreamType>(stream_type));
//     } while (false);
//     args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
// }

// NIM_SDK_NODE_API_DEF(NertcNodeEngine, subscribeRemoteAudioSubStream)
// {
//     CHECK_API_FUNC(NertcNodeEngine, 2)
//     int ret = -1;
//     do
//     {
//         CHECK_NATIVE_ADM_THIS(instance);
//         auto status = napi_ok;
//         uint64_t uid = 0;
//         bool subscribe = false;
//         GET_ARGS_VALUE(isolate, 0, uint64, uid)
//         if (status != napi_ok)
//             break;
//         GET_ARGS_VALUE(isolate, 1, bool, subscribe)
//         if (status != napi_ok)
//             break;
//         ret = instance->rtc_engine_->subscribeRemoteAudioSubStream(uid, subscribe);
//     } while (false);
//     args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
// }

// NIM_SDK_NODE_API_DEF(NertcNodeEngine, enableLocalAudioStream)
// {
//     CHECK_API_FUNC(NertcNodeEngine, 2)
//     int ret = -1;
//     do
//     {
//         CHECK_NATIVE_THIS(instance);
//         auto status = napi_ok;
//         bool enable = false;
//         int32_t stream_type;
//         GET_ARGS_VALUE(isolate, 0, bool, enable)
//         if (status != napi_ok)
//             break;
//         GET_ARGS_VALUE(isolate, 1, int32, stream_type)
//         if (status != napi_ok)
//             break;
//         ret = instance->rtc_engine_->enableLocalAudioStream(enable, static_cast<nertc::NERtcAudioStreamType>(stream_type));
//     } while (false);
//     args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
// }

// NIM_SDK_NODE_API_DEF(NertcNodeEngine, enableLoopbackRecording)
// {
//     CHECK_API_FUNC(NertcNodeEngine, 2)
//     int ret = -1;
//     do
//     {
//         CHECK_NATIVE_THIS(instance);
//         auto status = napi_ok;
//         bool enable = false;
//         UTF8String device_name;
//         GET_ARGS_VALUE(isolate, 0, bool, enable)
//         if (status != napi_ok)
//             break;
//         GET_ARGS_VALUE(isolate, 1, utf8string, device_name)
//         if (status != napi_ok)
//             break;
//         ret = instance->rtc_engine_->enableLoopbackRecording(enable, 
//             device_name.length() > 0 ? device_name.toUtf8String().c_str() : nullptr
//         );
//     } while (false);
//     args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
// }

// NIM_SDK_NODE_API_DEF(NertcNodeEngine, adjustLoopbackRecordingSignalVolume)
// {
//     CHECK_API_FUNC(NertcNodeEngine, 1)
//     int ret = -1;
//     do
//     {
//         CHECK_NATIVE_THIS(instance);
//         auto status = napi_ok;
//         uint32_t volume = 0;
//         GET_ARGS_VALUE(isolate, 0, uint32, volume)
//         if (status != napi_ok)
//             break;
//         ret = instance->rtc_engine_->adjustLoopbackRecordingSignalVolume(volume);
//     } while (false);
//     args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
// }

NIM_SDK_NODE_API_DEF(NertcNodeEngine, adjustUserPlaybackSignalVolume)
{
    CHECK_API_FUNC(NertcNodeEngine, 2)
    int ret = -1;
    do
    {
        CHECK_NATIVE_THIS(instance);
        auto status = napi_ok;
        bool enable = false;
        uint32_t volume = 0;
        uint64_t uid = 0;
        int32_t stream_type;
        GET_ARGS_VALUE(isolate, 0, uint64, uid)
        if (status != napi_ok)
            break;
        GET_ARGS_VALUE(isolate, 1, uint32, volume)
        if (status != napi_ok)
            break;
        // GET_ARGS_VALUE(isolate, 2, int32, stream_type)
        // if (status != napi_ok)
        //     break;
        ret = instance->rtc_engine_->adjustUserPlaybackSignalVolume(uid, volume//,static_cast<nertc::NERtcAudioStreamType>(stream_type)
            );
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

// NIM_SDK_NODE_API_DEF(NertcNodeEngine, checkNECastAudioDriver)
// {
//     CHECK_API_FUNC(NertcNodeEngine, 0)
//     int ret = -1;
//     do
//     {
//         CHECK_NATIVE_THIS(instance);
//         ret = instance->rtc_engine_->checkNECastAudioDriver();
//     } while (false);
//     args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
// }

NIM_SDK_NODE_API_DEF(NertcNodeEngine, enumerateScreenCaptureSourceInfo)
{
    CHECK_API_FUNC(NertcNodeEngine, 4)
    Local<Array> arr = Array::New(isolate);
#ifdef WIN32
    do
    {
        CHECK_NATIVE_THIS(instance);
        auto status = napi_ok;
        int thumbWidth, thumbHeight, iconWidth, iconHeight;
        GET_ARGS_VALUE(isolate, 0, int32, thumbWidth)
        GET_ARGS_VALUE(isolate, 1, int32, thumbHeight)
        GET_ARGS_VALUE(isolate, 2, int32, iconWidth)
        GET_ARGS_VALUE(isolate, 3, int32, iconHeight)
        if (status != napi_ok)
        {
            break;
        }
        WindowsHelpers::CaptureTargetInfoList list; 
        instance->_windows_helper->getCaptureWindowList(&list);

        uint32_t i = 0;
        bool captureScreenRet = instance->screen_capture_helper_->InitScreen();
        if (captureScreenRet)
        {
            uint32_t vx = GetSystemMetrics(SM_XVIRTUALSCREEN);
            uint32_t vy = GetSystemMetrics(SM_YVIRTUALSCREEN);
            for (auto w : list)
            {
                if (w.type != 1)
                    continue;
                Local<Object> obj = Object::New(isolate);
                int32_t sid = reinterpret_cast<int32_t>(w.id);
                nim_napi_set_object_value_int32(isolate, obj, "sourceId", sid);
                nim_napi_set_object_value_utf8string(isolate, obj, "sourceName", UTF16ToUTF8(w.title));
                nim_napi_set_object_value_int32(isolate, obj, "type", w.type);
                nim_napi_set_object_value_int32(isolate, obj, "left", w.rc.left);
                nim_napi_set_object_value_int32(isolate, obj, "top", w.rc.top);
                nim_napi_set_object_value_int32(isolate, obj, "right", w.rc.right);
                nim_napi_set_object_value_int32(isolate, obj, "bottom", w.rc.bottom);
                nim_napi_set_object_value_utf8string(isolate, obj, "displayId", w.display_id);
                instance->_windows_helper->updateCachedInfos(sid, w.rc);
                int left = w.rc.left - vx;
                int top = w.rc.top - vy;
                int sWidth = w.rc.right - w.rc.left;
                int sHeight = w.rc.bottom - w.rc.top;
                if (instance->screen_capture_helper_->CaptureScreenRect(left, top, sWidth, sHeight, thumbWidth, thumbHeight))
                {
                    int size = GetRGBASize(thumbWidth, thumbHeight);
                    uint8_t *data = RGBAToBGRA(instance->screen_capture_helper_->GetData(), size);
                    Local<v8::Object> thumb = Object::New(isolate);
                    nim_napi_set_object_value_int32(isolate, thumb, "length", size);
                    nim_napi_set_object_value_int32(isolate, thumb, "width", thumbWidth);
                    nim_napi_set_object_value_int32(isolate, thumb, "height", thumbHeight);
                    Local<v8::ArrayBuffer> buff = v8::ArrayBuffer::New(isolate, data, size);
                    Local<v8::Uint8Array> dataarray = v8::Uint8Array::New(buff, 0, size);
                    Local<Value> propName = String::NewFromUtf8(isolate, "buffer", NewStringType::kInternalized).ToLocalChecked();
                    thumb->Set(isolate->GetCurrentContext(), propName, dataarray);
                    Local<Value> thumbKey = String::NewFromUtf8(isolate, "thumbBGRA", NewStringType::kInternalized).ToLocalChecked();
                    obj->Set(isolate->GetCurrentContext(), thumbKey, thumb);
                }
                arr->Set(isolate->GetCurrentContext(), i++, obj);
            }
            instance->screen_capture_helper_->UnintScreen();
        }

        for (auto w : list)
        {
            if (w.type != 2) continue;
            Local<Object> obj = Object::New(isolate);
            nim_napi_set_object_value_int32(isolate, obj, "sourceId", reinterpret_cast<int32_t>(w.id));
            nim_napi_set_object_value_utf8string(isolate, obj, "sourceName", UTF16ToUTF8(w.title));
            nim_napi_set_object_value_int32(isolate, obj, "type", w.type);
            nim_napi_set_object_value_bool(isolate, obj, "isMinimizeWindow", w.isMinimizeWindow);
            if (!w.isMinimizeWindow)
            {
                if (instance->window_capture_helper_->CaptureWindow(w.id) && instance->window_capture_helper_->Zoom(thumbWidth, thumbHeight))
                {
                    int size = GetRGBASize(thumbWidth, thumbHeight);
                    uint8_t *data = RGBAToBGRA(instance->window_capture_helper_->GetData(), size);
                    Local<v8::Object> thumb = Object::New(isolate);
                    nim_napi_set_object_value_int32(isolate, thumb, "length", size);
                    nim_napi_set_object_value_int32(isolate, thumb, "width", thumbWidth);
                    nim_napi_set_object_value_int32(isolate, thumb, "height", thumbHeight);
                    Local<v8::ArrayBuffer> buff = v8::ArrayBuffer::New(isolate, data, size);
                    Local<v8::Uint8Array> dataarray = v8::Uint8Array::New(buff, 0, size);
                    Local<Value> propName = String::NewFromUtf8(isolate, "buffer", NewStringType::kInternalized).ToLocalChecked();
                    thumb->Set(isolate->GetCurrentContext(), propName, dataarray);
                    Local<Value> thumbKey = String::NewFromUtf8(isolate, "thumbBGRA", NewStringType::kInternalized).ToLocalChecked();
                    obj->Set(isolate->GetCurrentContext(), thumbKey, thumb);
                } else {
                    int iconSize = 0;
                    uint8_t *rgba = GetWindowsIconRGBA(w.id, &thumbWidth, &thumbHeight, &iconSize);
                    if (rgba != NULL)
                    {
                        uint8_t *data = RGBAToBGRA((void *)rgba, iconSize);
                        free(rgba);
                        rgba = nullptr;
                        Local<v8::Object> icon = Object::New(isolate);
                        nim_napi_set_object_value_int32(isolate, icon, "length", iconSize);
                        nim_napi_set_object_value_int32(isolate, icon, "width", thumbWidth);
                        nim_napi_set_object_value_int32(isolate, icon, "height", thumbHeight);
                        Local<v8::ArrayBuffer> buff = v8::ArrayBuffer::New(isolate, data, iconSize);
                        Local<v8::Uint8Array> dataarray = v8::Uint8Array::New(buff, 0, iconSize);
                        Local<Value> propName = String::NewFromUtf8(isolate, "buffer", NewStringType::kInternalized).ToLocalChecked();
                        icon->Set(isolate->GetCurrentContext(), propName, dataarray);
                        Local<Value> thumbKey = String::NewFromUtf8(isolate, "thumbBGRA", NewStringType::kInternalized).ToLocalChecked();
                        obj->Set(isolate->GetCurrentContext(), thumbKey, icon);
                    }
                }
            }
            int iconSize = 0;
            uint8_t *rgba = GetWindowsIconRGBA(w.id, &iconWidth, &iconHeight, &iconSize);
            if (rgba != NULL)
            {
                uint8_t *data = RGBAToBGRA((void *)rgba, iconSize);
                free(rgba);
                rgba = nullptr;
                Local<v8::Object> icon = Object::New(isolate);
                nim_napi_set_object_value_int32(isolate, icon, "length", iconSize);
                nim_napi_set_object_value_int32(isolate, icon, "width", iconWidth);
                nim_napi_set_object_value_int32(isolate, icon, "height", iconHeight);
                Local<v8::ArrayBuffer> buff = v8::ArrayBuffer::New(isolate, data, iconSize);
                Local<v8::Uint8Array> dataarray = v8::Uint8Array::New(buff, 0, iconSize);
                Local<Value> propName = String::NewFromUtf8(isolate, "buffer", NewStringType::kInternalized).ToLocalChecked();
                icon->Set(isolate->GetCurrentContext(), propName, dataarray);
                Local<Value> thumbKey = String::NewFromUtf8(isolate, "iconBGRA", NewStringType::kInternalized).ToLocalChecked();
                obj->Set(isolate->GetCurrentContext(), thumbKey, icon);
            }
            arr->Set(isolate->GetCurrentContext(), i++, obj);
        }
        instance->window_capture_helper_->Cleanup();
    } while (false);
#endif
    args.GetReturnValue().Set(arr);
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, startSystemAudioLoopbackCapture)
{
    CHECK_API_FUNC(NertcNodeEngine, 0)
    int ret = -1;
    do
    {
        CHECK_NATIVE_THIS(instance);
#ifdef WIN32
        // ret = instance->rtc_engine_->startSystemAudioLoopbackCapture();
#endif
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, stopSystemAudioLoopbackCapture)
{
    CHECK_API_FUNC(NertcNodeEngine, 0)
    int ret = -1;
    do
    {
        CHECK_NATIVE_THIS(instance);
#ifdef WIN32
        // ret = instance->rtc_engine_->stopSystemAudioLoopbackCapture();
#endif
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

NIM_SDK_NODE_API_DEF(NertcNodeEngine, setSystemAudioLoopbackCaptureVolume)
{
    CHECK_API_FUNC(NertcNodeEngine, 1)
    int ret = -1;
    do
    {
        auto status = napi_ok;
        uint32_t vol;
        GET_ARGS_VALUE(isolate, 0, uint32, vol)
        if (status != napi_ok)
        {
            break;
        }
#ifdef WIN32
        // ret = instance->rtc_engine_->setSystemAudioLoopbackCaptureVolume(vol);
#endif
    } while (false);
    args.GetReturnValue().Set(Integer::New(args.GetIsolate(), ret));
}

}