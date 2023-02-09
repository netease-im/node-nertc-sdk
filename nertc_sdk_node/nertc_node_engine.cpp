#include <iostream>
#include "node_version.h"
#include "nertc_node_engine.h"
#include "nertc_node_engine_helper.h"
#include "../shared/sdk_helper/node_api_helper.h"
#include "nertc_node_video_frame_provider.h"
#include "../shared/util/logger.h"
#ifdef WIN32
#include "../shared/util/string_util.h"
using namespace nertc_electron_util;
#endif
#include <string>

#define SET_PROTOTYPE(name) InstanceMethod(#name,  &NertcNodeEngine::name)

#define NIM_SDK_NODE_API_DEF(method) \
    Napi::Value NertcNodeEngine::method(const Napi::CallbackInfo& info)

#define INIT_ENV \
    Napi::Env env = info.Env();\
    int ret = -1;

#define INIT_ENV_BOOL \
    Napi::Env env = info.Env();\
    bool ret = false;

#define CHECK_API_FUNC(n)                                                              \
    if (info.Length() != n) {                                                                 \
        Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();  \
    }

namespace nertc_node
{

#if NAPI_VERSION < 6
Napi::FunctionReference NertcNodeEngine::constructor;
#endif

Napi::Object NertcNodeEngine::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function func = DefineClass(env, "NertcNodeEngine", {
        SET_PROTOTYPE(initialize),
        SET_PROTOTYPE(release),
        SET_PROTOTYPE(setChannelProfile),
        SET_PROTOTYPE(joinChannel),
        SET_PROTOTYPE(leaveChannel),
        SET_PROTOTYPE(enableLocalAudio),
        SET_PROTOTYPE(enableLocalVideo),
        SET_PROTOTYPE(subscribeRemoteVideoStream),
        SET_PROTOTYPE(setupVideoCanvas),
        SET_PROTOTYPE(onVideoFrame),
        SET_PROTOTYPE(onEvent),
    
        // 3.9
        SET_PROTOTYPE(setClientRole),
        SET_PROTOTYPE(setupSubStreamVideoCanvas),
        SET_PROTOTYPE(subscribeRemoteVideoSubStream),
        SET_PROTOTYPE(setMixedAudioFrameParameters),
        SET_PROTOTYPE(setExternalAudioSource),
        SET_PROTOTYPE(pushExternalAudioFrame),
        SET_PROTOTYPE(pushExternalVideoFrame),
    
        // 4.0
        SET_PROTOTYPE(sendSEIMsg),
        SET_PROTOTYPE(sendSEIMsgEx),
        SET_PROTOTYPE(setExternalAudioRender),
        SET_PROTOTYPE(pullExternalAudioFrame),
    
        // 4.1.1
        SET_PROTOTYPE(setAudioEffectPreset),
        SET_PROTOTYPE(setVoiceBeautifierPreset),
        SET_PROTOTYPE(setLocalVoicePitch),
        SET_PROTOTYPE(setLocalVoiceEqualization),
        SET_PROTOTYPE(adjustUserPlaybackSignalVolume),
    
        // //4.2.5
        SET_PROTOTYPE(switchChannel),
        SET_PROTOTYPE(setLocalMediaPriority),
        SET_PROTOTYPE(enableLoopbackRecording),
        SET_PROTOTYPE(adjustLoopbackRecordingSignalVolume),
        SET_PROTOTYPE(setExcludeWindowList),
        SET_PROTOTYPE(startAudioRecording),
        SET_PROTOTYPE(stopAudioRecording),
        SET_PROTOTYPE(startChannelMediaRelay),
        SET_PROTOTYPE(updateChannelMediaRelay),
        SET_PROTOTYPE(stopChannelMediaRelay),
        //SET_PROTOTYPE(setLocalPublishFallbackOption),
        //SET_PROTOTYPE(setRemoteSubscribeFallbackOption),
        SET_PROTOTYPE(enableSuperResolution),
        SET_PROTOTYPE(enableEncryption),

        SET_PROTOTYPE(getConnectionState),
        SET_PROTOTYPE(muteLocalAudioStream),
        SET_PROTOTYPE(setAudioProfile),
        SET_PROTOTYPE(subscribeRemoteAudioStream),
        SET_PROTOTYPE(setVideoConfig),
        SET_PROTOTYPE(enableDualStreamMode),
        SET_PROTOTYPE(setLocalVideoMirrorMode),
        SET_PROTOTYPE(startVideoPreview),
        SET_PROTOTYPE(stopVideoPreview),
        SET_PROTOTYPE(muteLocalVideoStream),
        SET_PROTOTYPE(setParameters),
        SET_PROTOTYPE(setRecordingAudioFrameParameters),
        SET_PROTOTYPE(setPlaybackAudioFrameParameters),
        SET_PROTOTYPE(startAudioDump),
        SET_PROTOTYPE(stopAudioDump),
        SET_PROTOTYPE(startAudioMixing),
        SET_PROTOTYPE(stopAudioMixing),
        SET_PROTOTYPE(pauseAudioMixing),
        SET_PROTOTYPE(resumeAudioMixing),
        SET_PROTOTYPE(setAudioMixingSendVolume),
        SET_PROTOTYPE(getAudioMixingSendVolume),
        SET_PROTOTYPE(setAudioMixingPlaybackVolume),
        SET_PROTOTYPE(getAudioMixingPlaybackVolume),
        SET_PROTOTYPE(getAudioMixingDuration),
        SET_PROTOTYPE(getAudioMixingCurrentPosition),
        SET_PROTOTYPE(setAudioMixingPosition),
        SET_PROTOTYPE(playEffect),
        SET_PROTOTYPE(stopEffect),
        SET_PROTOTYPE(stopAllEffects),
        SET_PROTOTYPE(pauseEffect),
        SET_PROTOTYPE(resumeEffect),
        SET_PROTOTYPE(pauseAllEffects),
        SET_PROTOTYPE(resumeAllEffects),
        SET_PROTOTYPE(setEffectSendVolume),
        SET_PROTOTYPE(getEffectSendVolume),
        SET_PROTOTYPE(setEffectPlaybackVolume),
        SET_PROTOTYPE(getEffectPlaybackVolume),
        SET_PROTOTYPE(enableEarback),
        SET_PROTOTYPE(setEarbackVolume),
        SET_PROTOTYPE(onStatsObserver),
        SET_PROTOTYPE(enableAudioVolumeIndication),
        SET_PROTOTYPE(startScreenCaptureByScreenRect),
        SET_PROTOTYPE(startScreenCaptureByDisplayId),
        SET_PROTOTYPE(startScreenCaptureByWindowId),
        SET_PROTOTYPE(updateScreenCaptureRegion),
        SET_PROTOTYPE(stopScreenCapture),
        SET_PROTOTYPE(pauseScreenCapture),
        SET_PROTOTYPE(resumeScreenCapture),
        SET_PROTOTYPE(setExternalVideoSource),
        SET_PROTOTYPE(getVersion),
        SET_PROTOTYPE(getErrorDescription),
        SET_PROTOTYPE(uploadSdkInfo),
        SET_PROTOTYPE(addLiveStreamTask),
        SET_PROTOTYPE(updateLiveStreamTask),
        SET_PROTOTYPE(removeLiveStreamTask),
        SET_PROTOTYPE(enumerateRecordDevices),
        SET_PROTOTYPE(setRecordDevice),
        SET_PROTOTYPE(getRecordDevice),
        SET_PROTOTYPE(enumeratePlayoutDevices),
        SET_PROTOTYPE(setPlayoutDevice),
        SET_PROTOTYPE(getPlayoutDevice),
        SET_PROTOTYPE(setRecordDeviceVolume),
        SET_PROTOTYPE(getRecordDeviceVolume),
        SET_PROTOTYPE(setPlayoutDeviceVolume),
        SET_PROTOTYPE(getPlayoutDeviceVolume),
        SET_PROTOTYPE(setPlayoutDeviceMute),
        SET_PROTOTYPE(getPlayoutDeviceMute),
        SET_PROTOTYPE(setRecordDeviceMute),
        SET_PROTOTYPE(getRecordDeviceMute),
        SET_PROTOTYPE(adjustRecordingSignalVolume),
        SET_PROTOTYPE(adjustPlaybackSignalVolume),
        SET_PROTOTYPE(startRecordDeviceTest),
        SET_PROTOTYPE(stopRecordDeviceTest),
        SET_PROTOTYPE(startPlayoutDeviceTest),
        SET_PROTOTYPE(stopPlayoutDeviceTest),
        SET_PROTOTYPE(startAudioDeviceLoopbackTest),
        SET_PROTOTYPE(stopAudioDeviceLoopbackTest),
        SET_PROTOTYPE(enumerateCaptureDevices),
        SET_PROTOTYPE(setDevice),
        SET_PROTOTYPE(getDevice),
        SET_PROTOTYPE(enumerateScreenCaptureSourceInfo),
        SET_PROTOTYPE(startSystemAudioLoopbackCapture),
        SET_PROTOTYPE(stopSystemAudioLoopbackCapture),
        SET_PROTOTYPE(setSystemAudioLoopbackCaptureVolume)
    });

#if NAPI_VERSION < 6
    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();
#else
    Napi::FunctionReference* constructor = new Napi::FunctionReference();
    *constructor = Napi::Persistent(func);
    env.SetInstanceData(constructor);
#endif
    exports.Set("NertcNodeEngine", func);
    return exports;
}

NertcNodeEngine::NertcNodeEngine(const Napi::CallbackInfo& info)
    : Napi::ObjectWrap<NertcNodeEngine>(info) {
    rtc_engine_ = (nertc::IRtcEngineEx *)createNERtcEngine();
    _event_handler = std::make_shared<NertcNodeEventHandler>();
    _stats_observer = std::make_shared<NertcNodeRtcMediaStatsHandler>();
#ifdef WIN32
    _windows_helper = new WindowsHelpers();
    window_capture_helper_.reset(new WindowCaptureHelper());
    screen_capture_helper_.reset(new ScreenCaptureHelper());
#endif
 Logger::Instance()->initPath("/home/liyongqiang01/nertc_node_log.txt");
}

NertcNodeEngine::~NertcNodeEngine() {
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
};

NIM_SDK_NODE_API_DEF(initialize)
{
    INIT_ENV
    do
    {
        Napi::Object obj = info[0].As<Napi::Object>();
        nertc::NERtcEngineContext context = {};
        context.video_use_exnternal_render = true;
        context.video_prefer_hw_decoder = false;
        context.video_prefer_hw_encoder = false;
        context.log_level = nertc::kNERtcLogLevelInfo;
        context.log_file_max_size_KBytes = 20 * 1024;
        nertc_engine_context_obj_to_struct(env, obj, context);
        std::string app_key_;
        std::string log_path_;
        if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"app_key"))))
        {
            app_key_ = obj.Get(static_cast<napi_value>(Napi::String::New(env,"app_key"))).As<Napi::String>().Utf8Value();
            context.app_key = app_key_.c_str();
        }
        // if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"log_dir_path"))))
        // {
        //     log_path_ = obj.Get(static_cast<napi_value>(Napi::String::New(env,"log_dir_path"))).As<Napi::String>().Utf8Value();
        //     context.log_dir_path = log_path_.c_str();
        // }
        std::string logPath = "/tmp";
        context.log_dir_path = logPath.c_str();
        context.event_handler = _event_handler.get();
        ret = rtc_engine_->initialize(context);
        if (ret == 0)
        {
            rtc_engine_->queryInterface(nertc::kNERtcIIDAudioDeviceManager, (void **)&_adm);
            rtc_engine_->queryInterface(nertc::kNERtcIIDVideoDeviceManager, (void **)&_vdm);
        }
        std::string s(context.log_dir_path);
        std::string nodeLogPath = s + "/nertc_node_log.txt";
    }while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(release)
{
     INIT_ENV
    do
    {
        rtc_engine_->release(true);
        if (rtc_engine_)
        {
            destroyNERtcEngine((void *&)rtc_engine_);
            _adm = nullptr;
            _vdm = nullptr;
            rtc_engine_ = nullptr;
        }
        _event_handler->removeAll();
        //NertcNodeRtcMediaStatsHandler::GetInstance()->RemoveAll();
   }while (false);
    return Napi::Number::New(env, ret);
}



NIM_SDK_NODE_API_DEF(setChannelProfile)
{
    INIT_ENV
    do
    {
        uint32_t profile;
        napi_get_value_uint32(info[0], profile);
        ret = rtc_engine_->setChannelProfile((nertc::NERtcChannelProfileType)profile);
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(joinChannel)
{
    INIT_ENV
    do{
        std::string token = "";
        std::string channel_name = "";
        // nertc::uid_t uid = 0;
        unsigned int uid = 0;
        napi_get_value_utf8_string(info[0], token);
        napi_get_value_utf8_string(info[1], channel_name);
        napi_get_value_uint32(info[2], uid);
        ret = rtc_engine_->joinChannel(token.length() == 0 ? "" : token.c_str(), channel_name.c_str(), uid);
    }while(false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(leaveChannel)
{
    INIT_ENV
    do
    {
        ret = rtc_engine_->leaveChannel();
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(enableLocalAudio)
{
    INIT_ENV
    do
    {
        bool enabled = false;
        napi_get_value_bool(info[0], enabled);
        ret = rtc_engine_->enableLocalAudio(enabled);
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(enableLocalVideo)
{
    INIT_ENV
    do
    {
        bool enabled = false;
        napi_get_value_bool(info[0], enabled);
        ret = rtc_engine_->enableLocalVideo(enabled);
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(subscribeRemoteVideoStream)
{
    INIT_ENV
    do
    {
        // nertc::uid_t uid;
        unsigned int uid;
        uint32_t type;
        bool sub;
        napi_get_value_uint32(info[0], uid);
        napi_get_value_uint32(info[1], type);
        napi_get_value_bool(info[2], sub);
        ret = rtc_engine_->subscribeRemoteVideoStream(uid, (nertc::NERtcRemoteVideoStreamType)type, sub);
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(setupVideoCanvas)
{
    INIT_ENV
    do{
        // nertc::uid_t uid = 0;
        uint32_t uid;
        bool lossless = true;
        bool enable;
        napi_get_value_uint32(info[0], uid);
        // napi_get_value_bigint_uint64(env, info[0], &uid, &lossless);
        napi_get_value_bool(info[1], enable);
        nertc::NERtcVideoCanvas canvas;
        canvas.cb = enable ? NodeVideoFrameTransporter::onFrameDataCallback : nullptr; //NodeVideoFrameTransporter::onFrameDataCallback;
        canvas.user_data = enable ? (void*)(new nertc::uid_t(uid)) : nullptr;
        canvas.window = nullptr;
        if (uid == 0)
        {
            ret = rtc_engine_->setupLocalVideoCanvas(&canvas);
        }else{
            ret = rtc_engine_->setupRemoteVideoCanvas(uid, &canvas);
        }
    }while(false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(onVideoFrame)
{
    INIT_ENV
    do{
        Napi::FunctionReference function;
        napi_get_value_function(info[0], function);
        NodeVideoFrameTransporter *pTransporter = getNodeVideoFrameTransporter();
        if (pTransporter)
        {
            ret = pTransporter->initialize(std::move(function));
        }
    }while(false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(onEvent)
{
    auto env = info.Env();
    std::string event_name = "";
    Napi::FunctionReference function;
    napi_get_value_utf8_string(info[0], event_name);
    napi_get_value_function(info[1], function);
  
    _event_handler->addEvent(event_name, std::move(function));
    auto ret_value = env.Null();
    return ret_value;
}

NIM_SDK_NODE_API_DEF(setClientRole)
{
    INIT_ENV
    do
    {
        uint32_t role;
        napi_get_value_uint32(info[0], role);
        ret = rtc_engine_->setClientRole((nertc::NERtcClientRole)role);
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(setupSubStreamVideoCanvas)
{
    INIT_ENV
    do
    {
        // uint64_t uid;
        unsigned int uid;
        bool enable;
        napi_get_value_uint32(info[0], uid);
        napi_get_value_bool(info[1], enable);
        nertc::NERtcVideoCanvas canvas;
        canvas.cb = enable ? NodeVideoFrameTransporter::onSubstreamFrameDataCallback : nullptr;
        canvas.user_data = enable ? (void*)(new nertc::uid_t(uid)) : nullptr;
        canvas.window = nullptr;
        if (uid == 0)
            ret = rtc_engine_->setupLocalSubStreamVideoCanvas(&canvas);
        else
            ret = rtc_engine_->setupRemoteSubStreamVideoCanvas(uid, &canvas);
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(subscribeRemoteVideoSubStream)
{
    INIT_ENV
    do
    {
        bool sub;
        // nertc::uid_t uid;
        unsigned int uid;
        napi_get_value_uint32(info[0], uid);
        napi_get_value_bool(info[1], sub);
        ret = rtc_engine_->subscribeRemoteVideoSubStream(uid, sub);
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(setMixedAudioFrameParameters)
{
    INIT_ENV
    do
    {
        int samp;
        napi_get_value_int32(info[0], samp);
        ret = rtc_engine_->setMixedAudioFrameParameters(samp);
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(setExternalAudioSource)
{
    INIT_ENV
    do
    {
        bool enabled;
        int samp, chan;
        napi_get_value_bool(info[0], enabled);
        napi_get_value_int32(info[1], samp);
        napi_get_value_int32(info[2], chan);
        ret = rtc_engine_->setExternalAudioSource(enabled, samp, chan);
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(pushExternalAudioFrame)
{
    INIT_ENV
    do
    {
        //todo
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(pushExternalVideoFrame)
{
    INIT_ENV
    do
    {
        //todo
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(sendSEIMsg)
{
    INIT_ENV
    do
    {
        if (!info[0].IsArrayBuffer()) {
            Napi::Error::New(env, "retryErrors must be an arraybuffer").ThrowAsJavaScriptException();
        }
        Napi::ArrayBuffer arrayBuffer = info[0].As<Napi::ArrayBuffer>();
        void * data = arrayBuffer.Data();
        size_t len = arrayBuffer.ByteLength();
        ret = rtc_engine_->sendSEIMsg(static_cast<const char*>(data), len);
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(sendSEIMsgEx)
{
    INIT_ENV
    do
    {
        if (!info[0].IsArrayBuffer()) {
            Napi::Error::New(env, "retryErrors must be an arraybuffer").ThrowAsJavaScriptException();
        }
        Napi::ArrayBuffer arrayBuffer = info[0].As<Napi::ArrayBuffer>();
        void * data = arrayBuffer.Data();
        size_t len = arrayBuffer.ByteLength();
        int32_t type;
        napi_get_value_int32(info[1], type);
        ret = rtc_engine_->sendSEIMsg(static_cast<const char*>(data), len, static_cast<nertc::NERtcVideoStreamType>(type));
    } while (false);
    return Napi::Number::New(env, ret);
}


NIM_SDK_NODE_API_DEF(setExternalAudioRender)
{
    INIT_ENV
    do
    {
        bool enable;
        int sample_rate = 0;
        int channels = 0;
        napi_get_value_bool(info[0], enable);
        napi_get_value_int32(info[1], sample_rate);
        napi_get_value_int32(info[2], channels);
        ret = rtc_engine_->setExternalAudioRender(enable, sample_rate, channels);
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(pullExternalAudioFrame)
{
    INIT_ENV
    do
    {
        int length = 0;
        Napi::FunctionReference function;
        napi_get_value_int32(info[0], length);
        napi_get_value_function(info[1], function);
        auto shared_data = std::make_shared<unsigned char>(length);
        ret = rtc_engine_->pullExternalAudioFrame(shared_data.get(), length);
        _event_handler->onPullExternalAudioFrame(std::move(function), shared_data, length);
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(setAudioEffectPreset)
{
    INIT_ENV
    do
    {
        int32_t type;
        napi_get_value_int32(info[0], type);
        ret = rtc_engine_->setAudioEffectPreset(static_cast<nertc::NERtcVoiceChangerType>(type));
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(setVoiceBeautifierPreset)
{
    INIT_ENV
    do
    {
        int32_t type;
        napi_get_value_int32(info[0], type);
        ret = rtc_engine_->setVoiceBeautifierPreset(static_cast<nertc::NERtcVoiceBeautifierType>(type));
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(setLocalVoicePitch)
{
    INIT_ENV
    do
    {
        double pitch;
        napi_get_value_double(info[0], pitch);
        ret = rtc_engine_->setLocalVoicePitch(pitch);
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(setLocalVoiceEqualization)
{
    INIT_ENV
    do
    {
        int32_t band_frequency;
        int32_t band_gain;
        napi_get_value_int32(info[0], band_frequency);
        napi_get_value_int32(info[1], band_gain);
        ret = rtc_engine_->setLocalVoiceEqualization(
            static_cast<nertc::NERtcVoiceEqualizationBand>(band_frequency), 
            band_gain);
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(adjustUserPlaybackSignalVolume)
{
    INIT_ENV
    do
    {
        uint32_t uid = 0;
        int volume = 0;
        napi_get_value_uint32(info[0], uid);
        napi_get_value_int32(info[1], volume);
        ret = rtc_engine_->adjustUserPlaybackSignalVolume(uid, volume);
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(switchChannel)
{
    INIT_ENV
    do
    {
        std::string token, channel_name;
        napi_get_value_utf8_string(info[0], token);
        napi_get_value_utf8_string(info[1], channel_name);
        if(channel_name.length() == 0){
            break;
        }
        ret = rtc_engine_->switchChannel(token.c_str(), channel_name.c_str());
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(setLocalMediaPriority)
{
    INIT_ENV
    do
    {
        int32_t priority;
        bool enable;
        napi_get_value_int32(info[0], priority);
        napi_get_value_bool(info[1], enable);
        ret = rtc_engine_->setLocalMediaPriority((nertc::NERtcMediaPriorityType)priority, enable);
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(enableLoopbackRecording)
{
    INIT_ENV
    do
    {
        bool enable;
        napi_get_value_bool(info[0], enable);
        std::string deviveName;
        napi_get_value_utf8_string(info[1], deviveName);
        ret = rtc_engine_->enableLoopbackRecording(enable, deviveName.c_str());
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(adjustLoopbackRecordingSignalVolume)
{
    INIT_ENV
    do
    {
        int volume;
        napi_get_value_int32(info[0], volume);
        ret = rtc_engine_->adjustLoopbackRecordingSignalVolume(volume);
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(setExcludeWindowList)
{
    Napi::Env env = info.Env();
    int ret = -1;
    do
    {
        Napi::Object obj = info[0].As<Napi::Object>();
        nertc::source_id_t *window_list = nullptr;
        std::set<int64_t> vsWindowId;
        nertc_window_id_list_to_struct(env, obj, vsWindowId);
        intptr_t* wnd_list = NULL;
        int index = 0;
        if (!vsWindowId.empty()) {
            wnd_list = new intptr_t[vsWindowId.size()];
            for (auto e : vsWindowId) {
                *(wnd_list + index++) = e;
            }
        }
        ret = rtc_engine_->setExcludeWindowList((nertc::source_id_t*)wnd_list, index);
        if (wnd_list) {
          delete[] wnd_list;
        }
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(startAudioRecording)
{
    Napi::Env env = info.Env();
    int ret = -1;
    do
    {
        std::string path;
        uint32_t profile, scenario;
        napi_get_value_utf8_string(info[0], path);
        napi_get_value_uint32(info[1], profile);
        napi_get_value_uint32(info[2], scenario);
        ret = rtc_engine_->startAudioRecording(path.c_str(), profile, (nertc::NERtcAudioRecordingQuality)scenario);
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(stopAudioRecording)
{
    INIT_ENV
    do
    {
        ret = rtc_engine_->stopAudioRecording();
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(startChannelMediaRelay)
{
    INIT_ENV
    do
    {
		nertc::NERtcChannelMediaRelayConfiguration* media_relay_config_ = new nertc::NERtcChannelMediaRelayConfiguration();
        media_relay_config_->src_infos = nullptr;
        media_relay_config_->dest_infos = nullptr;
        media_relay_config_->dest_count = 0;
        
        nertc::NERtcChannelMediaRelayInfo* src_info = (nertc::NERtcChannelMediaRelayInfo*)malloc(sizeof(nertc::NERtcChannelMediaRelayInfo)* 1);
        memset(src_info, 0, sizeof(nertc::NERtcChannelMediaRelayInfo));
        media_relay_config_->src_infos = src_info;

        const Napi::Object obj = info[0].As<Napi::Object>();
        if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"src_infos"))))
        {
            nertc::NERtcChannelMediaRelayInfo src = {};
            Napi::Object o = obj.Get(static_cast<napi_value>(Napi::String::New(env,"src_infos"))).As<Napi::Object>();
            nertc_channel_media_relay_obj_to_struct(env, o, src_info[0]);
        }

        int dest_count = 0;
        if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"dest_count"))))
        {
            dest_count = obj.Get(static_cast<napi_value>(Napi::String::New(env,"dest_count"))).As<Napi::Number>().Int32Value();
            media_relay_config_->dest_count = dest_count;
        }

        if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"dest_infos"))))
        {
            nertc::NERtcChannelMediaRelayInfo* dst_info = (nertc::NERtcChannelMediaRelayInfo*)malloc(sizeof(nertc::NERtcChannelMediaRelayInfo) * (media_relay_config_->dest_count));
            memset(dst_info, 0, sizeof(nertc::NERtcChannelMediaRelayInfo) * (media_relay_config_->dest_count));

            Napi::Object o = obj.Get(static_cast<napi_value>(Napi::String::New(env,"dest_infos"))).As<Napi::Object>();
            Napi::Array objs = o.As<Napi::Array>();
            for (size_t i = 0; i < objs.Length(); i++)
            {
                Napi::Object o2 = objs.Get(i).As<Napi::Object>();
                nertc_channel_media_relay_obj_to_struct(env, o2, dst_info[i]);
            }
            media_relay_config_->dest_infos = dst_info;
        }

        ret = rtc_engine_->startChannelMediaRelay(media_relay_config_);

		if (nullptr != media_relay_config_->dest_infos) {
			delete[]  media_relay_config_->dest_infos;
			media_relay_config_->dest_infos = nullptr;
		}
		if (nullptr != media_relay_config_->src_infos) {
			delete media_relay_config_->src_infos;
			media_relay_config_->src_infos = nullptr;
		}
		if (nullptr != media_relay_config_) {
			delete media_relay_config_;
			media_relay_config_ = nullptr;
		}

        // nertc::NERtcChannelMediaRelayConfiguration* config = new nertc::NERtcChannelMediaRelayConfiguration();
        // int dest_count = 0;
        // const Napi::Object obj = info[0].As<Napi::Object>();
        // nertc::NERtcChannelMediaRelayInfo src = {0};
        // if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"src_infos"))))
        // {
        //     Napi::Object o = obj.Get(static_cast<napi_value>(Napi::String::New(env,"src_infos"))).As<Napi::Object>();
        //     nertc_channel_media_relay_obj_to_struct(env, o, src);
        //     config->src_infos = &src;
        // }
        // if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"dest_count"))))
        // {
        //     dest_count = obj.Get(static_cast<napi_value>(Napi::String::New(env,"dest_count"))).As<Napi::Number>().Int32Value();
        //     config->dest_count = dest_count;
        // }
        // if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"dest_infos"))))
        // {
        //     Napi::Object o = obj.Get(static_cast<napi_value>(Napi::String::New(env,"dest_infos"))).As<Napi::Object>();
        //     Napi::Array objs = o.As<Napi::Array>();
        //     nertc::NERtcChannelMediaRelayInfo * dst = new nertc::NERtcChannelMediaRelayInfo[dest_count];
        //     for (size_t i = 0; i < objs.Length(); i++)
        //     {
        //         Napi::Object o2 = objs.Get(i).As<Napi::Object>();
        //         nertc_channel_media_relay_obj_to_struct(env, o2, dst[i]);
        //     }
        //     config->dest_infos = dst;
        //     ret = rtc_engine_->startChannelMediaRelay(config);
        // }
        
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(updateChannelMediaRelay)
{
    INIT_ENV
    do
    {
		nertc::NERtcChannelMediaRelayConfiguration* media_relay_config_ = new nertc::NERtcChannelMediaRelayConfiguration();
		media_relay_config_->src_infos = nullptr;
		media_relay_config_->dest_infos = nullptr;
		media_relay_config_->dest_count = 0;

		nertc::NERtcChannelMediaRelayInfo* src_info = (nertc::NERtcChannelMediaRelayInfo*)malloc(sizeof(nertc::NERtcChannelMediaRelayInfo) * 1);
		memset(src_info, 0, sizeof(nertc::NERtcChannelMediaRelayInfo));
		media_relay_config_->src_infos = src_info;

		const Napi::Object obj = info[0].As<Napi::Object>();
		if (obj.Has(static_cast<napi_value>(Napi::String::New(env, "src_infos"))))
		{
			nertc::NERtcChannelMediaRelayInfo src = {};
			Napi::Object o = obj.Get(static_cast<napi_value>(Napi::String::New(env, "src_infos"))).As<Napi::Object>();
			nertc_channel_media_relay_obj_to_struct(env, o, src_info[0]);
		}

		int dest_count = 0;
		if (obj.Has(static_cast<napi_value>(Napi::String::New(env, "dest_count"))))
		{
			dest_count = obj.Get(static_cast<napi_value>(Napi::String::New(env, "dest_count"))).As<Napi::Number>().Int32Value();
			media_relay_config_->dest_count = dest_count;
		}

		if (obj.Has(static_cast<napi_value>(Napi::String::New(env, "dest_infos"))))
		{
			nertc::NERtcChannelMediaRelayInfo* dst_info = (nertc::NERtcChannelMediaRelayInfo*)malloc(sizeof(nertc::NERtcChannelMediaRelayInfo) * (media_relay_config_->dest_count));
			memset(dst_info, 0, sizeof(nertc::NERtcChannelMediaRelayInfo) * (media_relay_config_->dest_count));

			Napi::Object o = obj.Get(static_cast<napi_value>(Napi::String::New(env, "dest_infos"))).As<Napi::Object>();
			Napi::Array objs = o.As<Napi::Array>();
			for (size_t i = 0; i < objs.Length(); i++)
			{
				Napi::Object o2 = objs.Get(i).As<Napi::Object>();
				nertc_channel_media_relay_obj_to_struct(env, o2, dst_info[i]);
			}
			media_relay_config_->dest_infos = dst_info;
		}

        ret = rtc_engine_->updateChannelMediaRelay(media_relay_config_);

		if (nullptr != media_relay_config_->dest_infos) {
			delete[]  media_relay_config_->dest_infos;
			media_relay_config_->dest_infos = nullptr;
		}
		if (nullptr != media_relay_config_->src_infos) {
			delete media_relay_config_->src_infos;
			media_relay_config_->src_infos = nullptr;
		}
		if (nullptr != media_relay_config_) {
			delete media_relay_config_;
			media_relay_config_ = nullptr;
		}

    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(stopChannelMediaRelay)
{
    INIT_ENV
    do
    {
        ret = rtc_engine_->stopChannelMediaRelay();
    } while (false);
    return Napi::Number::New(env, ret);
}

#if 0
NIM_SDK_NODE_API_DEF(setLocalPublishFallbackOption)
{
    INIT_ENV
    do
    {
        int opt;
        napi_get_value_int32(info[0], opt);
        ret = rtc_engine_->setLocalPublishFallbackOption(opt);
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(setRemoteSubscribeFallbackOption)
{
    INIT_ENV
    do
    {
        int opt;
        napi_get_value_int32(info[0], opt);
        ret = rtc_engine_->setRemoteSubscribeFallbackOption(opt);
    } while (false);
    return Napi::Number::New(env, ret);
}
#endif

NIM_SDK_NODE_API_DEF(enableSuperResolution)
{
    INIT_ENV
    do
    {
        bool enable;
        napi_get_value_bool(info[0], enable);
        ret = rtc_engine_->enableSuperResolution(enable);
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(enableEncryption)
{
    INIT_ENV
    do
    {
        bool enable;
        nertc::NERtcEncryptionConfig config;
        napi_get_value_bool(info[0], enable);
        Napi::Object obj = info[1].As<Napi::Object>();
        std::string out;
        int32_t out_32;
        if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"mode"))))
        {
            out_32 = obj.Get(static_cast<napi_value>(Napi::String::New(env,"mode"))).As<Napi::Number>().Int32Value();
            config.mode = (nertc::NERtcEncryptionMode)out_32;
        }
        if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"key"))))
        {
            out = obj.Get(static_cast<napi_value>(Napi::String::New(env,"key"))).As<Napi::String>().Utf8Value();
            memset(config.key, 0, kNERtcEncryptByteLength * sizeof(char));
            memcpy(config.key, out.c_str(), out.size() * sizeof(char));
        }
        ret = rtc_engine_->enableEncryption(enable, config);
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(getConnectionState)
{
    INIT_ENV
    do
    {
        ret = rtc_engine_->getConnectionState();
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(muteLocalAudioStream)
{
    INIT_ENV
    do
    {
        bool enabled;
        napi_get_value_bool(info[0], enabled);
        ret = rtc_engine_->muteLocalAudioStream(enabled);
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(setAudioProfile)
{
    INIT_ENV
    do
    {
        uint32_t profile, scenario;
        napi_get_value_uint32(info[0], profile);
        napi_get_value_uint32(info[1], scenario);
        ret = rtc_engine_->setAudioProfile((nertc::NERtcAudioProfileType)profile, (nertc::NERtcAudioScenarioType)scenario);
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(subscribeRemoteAudioStream)
{
    INIT_ENV
    do
    {
        uint32_t uid;
        bool enable;
        napi_get_value_uint32(info[0], uid);
        napi_get_value_bool(info[1], enable);
        ret = rtc_engine_->subscribeRemoteAudioStream(uid, enable);
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(setVideoConfig)
{
    INIT_ENV
    do
    {
        auto status = napi_ok;
        nertc::NERtcVideoConfig config = {};
        status = nertc_video_config_obj_to_struct(env, info[0].As<Napi::Object>(), config);
        if (status != napi_ok)
        {
            break;
        }
        ret = rtc_engine_->setVideoConfig(config);
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(enableDualStreamMode)
{
    INIT_ENV
    do{
        bool enabled;
        napi_get_value_bool(info[0], enabled);
        ret = rtc_engine_->enableDualStreamMode(enabled);
    }while(false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(setLocalVideoMirrorMode)
{
    INIT_ENV
    do{
        uint32_t mode;
        napi_get_value_uint32(info[0], mode);
        NodeVideoFrameTransporter *pTransporter = getNodeVideoFrameTransporter();
        if (pTransporter)
        {
            pTransporter->setLocalVideoMirrorMode(mode);
            ret = 0;
        }
    }while(false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(startVideoPreview)
{
    INIT_ENV
    do{
        ret = rtc_engine_->startVideoPreview();
    }while(false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(stopVideoPreview)
{
    INIT_ENV
    do
    {
        ret = rtc_engine_->stopVideoPreview();
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(muteLocalVideoStream)
{
    INIT_ENV
    do
    {
        bool enabled;
        napi_get_value_bool(info[0], enabled);
        ret = rtc_engine_->muteLocalVideoStream(enabled);
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(setParameters)
{
    INIT_ENV
    do
    {
        std::string para;
        napi_get_value_utf8_string(info[0], para);
        if (para.length() == 0)
        {
            break;
        }
        ret = rtc_engine_->setParameters(para.c_str());
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(setRecordingAudioFrameParameters)
{
    INIT_ENV
    do
    {
        auto status = napi_ok;
        nertc::NERtcAudioFrameRequestFormat config = {};
        status = nertc_audio_frame_rf_obj_to_struct(env, info[0].As<Napi::Object>(), config);
        if (status != napi_ok)
        {
            break;
        }
        ret = rtc_engine_->setRecordingAudioFrameParameters(&config);
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(setPlaybackAudioFrameParameters)
{
    INIT_ENV
    do
    {
        auto status = napi_ok;
        nertc::NERtcAudioFrameRequestFormat config = {};
        status = nertc_audio_frame_rf_obj_to_struct(env, info[0].As<Napi::Object>(), config);
        if (status != napi_ok)
        {
            break;
        }
        ret = rtc_engine_->setPlaybackAudioFrameParameters(&config);
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF( startAudioDump)
{
    INIT_ENV
    do
    {
        ret = rtc_engine_->startAudioDump();
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(stopAudioDump)
{
    INIT_ENV
    do
    {
        ret = rtc_engine_->stopAudioDump();
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(startAudioMixing)
{
    INIT_ENV
    do
    {
        nertc::NERtcCreateAudioMixingOption config = {};
        nertc_audio_mixing_option_obj_to_struct(env, info[0].As<Napi::Object>(), config);
        ret = rtc_engine_->startAudioMixing(&config);
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(stopAudioMixing)
{
    INIT_ENV
    do
    {
        ret = rtc_engine_->stopAudioMixing();
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(pauseAudioMixing)
{
    INIT_ENV
    do
    {
        ret = rtc_engine_->pauseAudioMixing();
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(resumeAudioMixing)
{
    INIT_ENV
    do
    {
        ret = rtc_engine_->resumeAudioMixing();
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(setAudioMixingSendVolume)
{
    INIT_ENV
    do
    {
        uint32_t param;
        napi_get_value_uint32(info[0], param);
        ret = rtc_engine_->setAudioMixingSendVolume(param);
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(getAudioMixingSendVolume)
{
    INIT_ENV
    uint32_t volume = 0;
    do
    {
        ret = rtc_engine_->getAudioMixingSendVolume(&volume);
    } while (false);
    return Napi::Number::New(env, ret == 0 ? volume : ret);
}

NIM_SDK_NODE_API_DEF(setAudioMixingPlaybackVolume)
{
    INIT_ENV
    do
    {
        uint32_t param;
        napi_get_value_uint32(info[0], param);
        ret = rtc_engine_->setAudioMixingPlaybackVolume(param);
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(getAudioMixingPlaybackVolume)
{
    INIT_ENV
    uint32_t volume = 0;
    do
    {
        ret = rtc_engine_->getAudioMixingPlaybackVolume(&volume);
    } while (false);
    return Napi::Number::New(env, ret == 0 ? volume : ret);
}

NIM_SDK_NODE_API_DEF(getAudioMixingDuration)
{
    INIT_ENV
    uint64_t dur = 0;
    do
    {
        ret = rtc_engine_->getAudioMixingDuration(&dur);
    } while (false);
    return Napi::Number::New(env, ret == 0 ? dur : ret);
}

NIM_SDK_NODE_API_DEF(getAudioMixingCurrentPosition)
{
    INIT_ENV
    uint64_t volume = 0;
    do
    {
        ret = rtc_engine_->getAudioMixingCurrentPosition(&volume);
    } while (false);
     return Napi::Number::New(env, ret == 0 ? (uint32_t)volume : ret);
}

NIM_SDK_NODE_API_DEF(setAudioMixingPosition)
{
    INIT_ENV
    do
    {
        uint32_t param;
        napi_get_value_uint32(info[0], param);
        ret = rtc_engine_->setAudioMixingPosition(param);
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(playEffect)
{
    INIT_ENV
    do
    {
        uint32_t effect_id;
        napi_get_value_uint32(info[0], effect_id);

        Napi::Array objs = info[1].As<Napi::Array>();
        nertc::NERtcCreateAudioEffectOption *config = new nertc::NERtcCreateAudioEffectOption[objs.Length()];
        nertc_audio_effect_option_obj_to_struct(env, info[1].As<Napi::Object>(), config);
        ret = rtc_engine_->playEffect(effect_id, config);
        delete[] config;
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(stopEffect)
{
    INIT_ENV
    do
    {
        uint32_t effect_id;
        napi_get_value_uint32(info[0], effect_id);
        ret = rtc_engine_->stopEffect(effect_id);
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(stopAllEffects)
{
    INIT_ENV
    do
    {
        ret = rtc_engine_->stopAllEffects();
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(pauseEffect)
{
    INIT_ENV
    do
    {
        uint32_t effect_id;
        napi_get_value_uint32(info[0], effect_id);
        ret = rtc_engine_->pauseEffect(effect_id);
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(resumeEffect)
{
    INIT_ENV
    do
    {
        uint32_t effect_id;
        napi_get_value_uint32(info[0], effect_id);
        ret = rtc_engine_->resumeEffect(effect_id);
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(pauseAllEffects)
{
    INIT_ENV
    do
    {
        ret = rtc_engine_->pauseAllEffects();
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(resumeAllEffects)
{
    INIT_ENV
    do
    {
        ret = rtc_engine_->resumeAllEffects();
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(setEffectSendVolume)
{
    INIT_ENV
    do
    {
        uint32_t effect_id, volume;
        napi_get_value_uint32(info[0], effect_id);
        napi_get_value_uint32(info[1], volume);
        ret = rtc_engine_->setEffectSendVolume(effect_id, volume);
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(getEffectSendVolume)
{
    INIT_ENV
    uint32_t volume = 0;
    do
    {
        uint32_t effect_id;
        napi_get_value_uint32(info[0], effect_id);
        ret = rtc_engine_->getEffectSendVolume(effect_id, &volume);
    } while (false);
    return Napi::Number::New(env, ret == 0 ? volume : ret);
}

NIM_SDK_NODE_API_DEF(setEffectPlaybackVolume)
{
    INIT_ENV
    do
    {
        uint32_t effect_id, volume;
        napi_get_value_uint32(info[0], effect_id);
        napi_get_value_uint32(info[1], volume);
        ret = rtc_engine_->setEffectPlaybackVolume(effect_id, volume);
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(getEffectPlaybackVolume)
{
    INIT_ENV
    uint32_t vol = 0;
    do
    {
        uint32_t effect_id;
        napi_get_value_uint32(info[0], effect_id);
        ret = rtc_engine_->getEffectPlaybackVolume(effect_id, &vol);
    } while (false);
    return Napi::Number::New(env, ret == 0 ? vol : ret);
}

NIM_SDK_NODE_API_DEF(enableEarback)
{
    INIT_ENV
    do
    {
        bool enabled;
        uint32_t volume;
        napi_get_value_bool(info[0], enabled);
        napi_get_value_uint32(info[1], volume);
        ret = rtc_engine_->enableEarback(enabled, volume);
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(setEarbackVolume)
{
    INIT_ENV
    do
    {
        uint32_t volume;
        napi_get_value_uint32(info[0], volume);
        ret = rtc_engine_->setEarbackVolume(volume);
    } while (false);
    return Napi::Number::New(env, ret);
}

// NIM_SDK_NODE_API_DEF(onAudioFrameObserver)
// {
//     Napi::Env env = info.Env();
//     int ret = -1;
//     do
//     {
//       //todo
//     } while (false);
//     return Napi::Number::New(env, ret);
// }

NIM_SDK_NODE_API_DEF(onStatsObserver)
{
    INIT_ENV
    do
    {
        rtc_engine_->setStatsObserver(_stats_observer.get());
        std::string eventName;
        bool enable;
        napi_get_value_utf8_string(info[0], eventName);
        napi_get_value_bool(info[1], enable);
        if (eventName.length() == 0)
        {
            break;
        }
        if (!enable)
        {
            auto sz = _stats_observer->removeEventHandler(eventName);
            if (sz == 0)
            {
                rtc_engine_->setStatsObserver(nullptr);
            }
        }
        else
        {
            Napi::FunctionReference function;
            napi_get_value_function(info[2], function);
            _stats_observer->addEvent(eventName, std::move(function));
        }
    } while (false);
	return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(enableAudioVolumeIndication)
{
    INIT_ENV
    do
    {
        bool enabled;
        uint32_t interval;
        napi_get_value_bool(info[0], enabled);
        napi_get_value_uint32(info[1], interval);
        ret = rtc_engine_->enableAudioVolumeIndication(enabled, interval);
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(startScreenCaptureByScreenRect)
{
    INIT_ENV
    do
    {
        nertc::NERtcRectangle screen_rect = {}, region_rect = {};
        nertc::NERtcScreenCaptureParameters param = {};
        nertc_rectangle_obj_to_struct(env, info[0].As<Napi::Object>(), screen_rect);
        nertc_rectangle_obj_to_struct(env, info[1].As<Napi::Object>(), region_rect);
        std::set<int64_t> vsWindowId;
        nertc_screen_capture_params_obj_to_struct(env, info[2].As<Napi::Object>(), param, vsWindowId);
        intptr_t* wnd_list = nullptr;
        int index = 0;
        if (!vsWindowId.empty()) {
            wnd_list = new intptr_t[vsWindowId.size()];
            for (auto e : vsWindowId) {
                *(wnd_list + index++) = e;
            }
        }
        param.excluded_window_list = (nertc::source_id_t*)wnd_list;
        ret = rtc_engine_->startScreenCaptureByScreenRect(screen_rect, region_rect, param);
        if (param.excluded_window_list != nullptr)
        {
            delete[] param.excluded_window_list;
            param.excluded_window_list = nullptr;
        }
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(startScreenCaptureByWindowId)
{
    INIT_ENV
    do
    {
        int32_t windowid;
        nertc::NERtcRectangle region_rect = {};
        nertc::NERtcScreenCaptureParameters param = {};
        napi_get_value_int32(info[0], windowid);
        nertc_rectangle_obj_to_struct(env, info[1].As<Napi::Object>(), region_rect);
        std::set<int64_t> vsWindowId;
        nertc_screen_capture_params_obj_to_struct(env, info[2].As<Napi::Object>(), param, vsWindowId);
        intptr_t* wnd_list = nullptr;
        int index = 0;
        if (!vsWindowId.empty()) {
            wnd_list = new intptr_t[vsWindowId.size()];
            for (auto e : vsWindowId) {
                *(wnd_list + index++) = e;
            }
        }
        param.excluded_window_list = (nertc::source_id_t*)wnd_list;
#ifdef WIN32
        ret = rtc_engine_->startScreenCaptureByWindowId(reinterpret_cast<void *>(windowid), region_rect, param);
#else
        ret = rtc_engine_->startScreenCaptureByWindowId(reinterpret_cast<void *>(&windowid), region_rect, param);
#endif
        if (param.excluded_window_list != nullptr)
        {
            delete[] param.excluded_window_list;
            param.excluded_window_list = nullptr;
        }
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(updateScreenCaptureRegion)
{
    INIT_ENV
    do
    {
        auto status = napi_ok;
        nertc::NERtcRectangle region_rect = {};
        nertc_rectangle_obj_to_struct(env, info[0].As<Napi::Object>(), region_rect);

        if (status != napi_ok)
        {
            break;
        }
        ret = rtc_engine_->updateScreenCaptureRegion(region_rect);
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(startScreenCaptureByDisplayId)
{
    INIT_ENV
    do
    {
        int64_t display;
        nertc::NERtcRectangle region_rect = {};
        nertc::NERtcScreenCaptureParameters param = {};
        napi_get_value_int64(info[0], display);
        nertc_rectangle_obj_to_struct(env, info[1].As<Napi::Object>(), region_rect);
        std::set<int64_t> vsWindowId;
        nertc_screen_capture_params_obj_to_struct(env, info[2].As<Napi::Object>(), param, vsWindowId);
        intptr_t* wnd_list = nullptr;
        int index = 0;
        if (!vsWindowId.empty()) {
            wnd_list = new intptr_t[vsWindowId.size()];
            for (auto e : vsWindowId) {
                *(wnd_list + index++) = e;
            }
        }
        param.excluded_window_list = (nertc::source_id_t*)wnd_list;
#ifdef WIN32
            RECT rc = _windows_helper->getCachedRect(display);
            if (rc.bottom == 0 && rc.left == 0 && rc.right == 0 && rc.top == 0)
            {
                WindowsHelpers::CaptureTargetInfoList list;
                _windows_helper->getCaptureWindowList(&list, 1);
                for (auto w : list)
                {
                    if (std::to_string(display) == w.display_id)
                    {
                        rc = w.rc;
                        _windows_helper->updateCachedInfos(display, rc);
                        break;
                    }
                }
            }
            if (rc.bottom != 0 || rc.left != 0 || rc.right != 0 || rc.top != 0)
            {
                nertc::NERtcRectangle screen_rect = {};
                screen_rect.x = rc.left;
                screen_rect.y = rc.top;
                screen_rect.width = rc.right - rc.left;
                screen_rect.height = rc.bottom - rc.top;
                ret = rtc_engine_->startScreenCaptureByScreenRect(screen_rect, region_rect, param);
            }
            else
            {
                ret = -100;
            }
#else
            ret = rtc_engine_->startScreenCaptureByDisplayId(display, region_rect, param);
#endif
            if (param.excluded_window_list != nullptr)
            {
                delete[] param.excluded_window_list;
                param.excluded_window_list = nullptr;
            }

    } while (false);
    return Napi::Number::New(env, ret);
}


NIM_SDK_NODE_API_DEF(stopScreenCapture)
{
    INIT_ENV
    do
    {
        ret = rtc_engine_->stopScreenCapture();
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(pauseScreenCapture)
{
    INIT_ENV
    do
    {
        ret = rtc_engine_->pauseScreenCapture();
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(resumeScreenCapture)
{
    INIT_ENV
    do
    {
        ret = rtc_engine_->resumeScreenCapture();
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(setExternalVideoSource)
{
    INIT_ENV
    do
    {
        bool enabled;
        napi_get_value_bool(info[0], enabled);
        ret = rtc_engine_->setExternalVideoSource(enabled);
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(getVersion)
{
    Napi::Env env = info.Env();
    std::string ret;
    do
    {
        int32_t build;
        ret = (std::string)rtc_engine_->getVersion(&build);
    } while (false);
    return Napi::String::New(env, ret);
}

NIM_SDK_NODE_API_DEF(getErrorDescription)
{
    Napi::Env env = info.Env();
    std::string ret;
    do
    {
        int32_t error;
        napi_get_value_int32(info[0], error);
        ret = (std::string)rtc_engine_->getErrorDescription(error);
    } while (false);
    return Napi::String::New(env, ret);
}

NIM_SDK_NODE_API_DEF(uploadSdkInfo)
{
    INIT_ENV
    do
    {
        rtc_engine_->uploadSdkInfo();
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(addLiveStreamTask)
{
    INIT_ENV
    do
    {
        Napi::Object obj = info[0].As<Napi::Object>();
        nertc::NERtcLiveStreamTaskInfo info = {};
        nertc_ls_task_info_obj_to_struct(env, obj, info);
        memset(info.extraInfo, 0, kNERtcEncryptByteLength);
        // info.config = {0};
        ret = rtc_engine_->addLiveStreamTask(info);
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
    } while (false);
    return Napi::Number::New(env, ret);
}


NIM_SDK_NODE_API_DEF(updateLiveStreamTask)
{
    INIT_ENV
    do
    {
        Napi::Object obj = info[0].As<Napi::Object>();
        nertc::NERtcLiveStreamTaskInfo info = {};

        nertc_ls_task_info_obj_to_struct(env, obj, info);
        ret = rtc_engine_->updateLiveStreamTask(info);
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
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(removeLiveStreamTask)
{
    INIT_ENV
    do
    {
        std::string task_id;
        napi_get_value_utf8_string(info[0], task_id);
        ret = rtc_engine_->removeLiveStreamTask(task_id.c_str());
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(enumerateRecordDevices)
{
    INIT_ENV
    Napi::Array arr = Napi::Array::New(env);
    do
    {
        auto devices = _adm->enumerateRecordDevices();
        if (devices != nullptr)
        {
            size_t count = devices->getCount();
            for (size_t i = 0; i < count; i++)
            {
                nertc::NERtcDeviceInfo info;
                Napi::Object obj = Napi::Object::New(env);
                if (devices->getDeviceInfo(i, &info) == 0)
                {
                    obj.Set(Napi::String::New(env, "device_id"), Napi::String::New(env, (char *)info.device_id));
                    obj.Set(Napi::String::New(env, "device_name"), Napi::String::New(env, (char *)info.device_name));
                    obj.Set(Napi::String::New(env, "transport_type"), Napi::Number::New(env, info.transport_type));
                    obj.Set(Napi::String::New(env, "suspected_unavailable"), Napi::Boolean::New(env, info.suspected_unavailable));
                    obj.Set(Napi::String::New(env, "system_default_device"), Napi::Boolean::New(env, info.system_default_device));
                    arr[i] = obj;
                }
            }
            devices->destroy();
        }
    } while (false);
    return arr;
}

NIM_SDK_NODE_API_DEF(setRecordDevice)
{
    INIT_ENV
    do
    {
        std::string device;
        napi_get_value_utf8_string(info[0], device);
        if (device.length() == 0)
        {
            break;
        }
        ret = _adm->setRecordDevice(device.c_str());
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(getRecordDevice)
{
    INIT_ENV
    char id[256];
    do
    {
        ret = _adm->getRecordDevice(id);
    } while (false);
    return Napi::String::New(env, ret== 0 ? (char*)id : "");
}

NIM_SDK_NODE_API_DEF(enumeratePlayoutDevices)
{
    INIT_ENV
    Napi::Array arr = Napi::Array::New(env);
    do
    {
        auto devices = _adm->enumeratePlayoutDevices();
        if (devices != nullptr)
        {
            size_t count = devices->getCount();
            for (size_t i = 0; i < count; i++)
            {
                nertc::NERtcDeviceInfo info;
                Napi::Object obj = Napi::Object::New(env);
                if (devices->getDeviceInfo(i, &info) == 0)
                {
                    obj.Set(Napi::String::New(env, "device_id"), Napi::String::New(env, (char *)info.device_id));
                    obj.Set(Napi::String::New(env, "device_name"), Napi::String::New(env, (char *)info.device_name));
                    obj.Set(Napi::String::New(env, "transport_type"), Napi::Number::New(env, info.transport_type));
                    obj.Set(Napi::String::New(env, "suspected_unavailable"), Napi::Boolean::New(env, info.suspected_unavailable));
                    obj.Set(Napi::String::New(env, "system_default_device"), Napi::Boolean::New(env, info.system_default_device));
                    arr[i] = obj;
                }
            }
            devices->destroy();
        }

    } while (false);
    return arr;
}

NIM_SDK_NODE_API_DEF(setPlayoutDevice)
{
    INIT_ENV
    do
    {
        std::string device;
        napi_get_value_utf8_string(info[0], device);
        if (device.length() == 0)
        {
            break;
        }
        ret = _adm->setPlayoutDevice(device.c_str());
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(getPlayoutDevice)
{
    INIT_ENV
    char id[256]; 
    do
    {
        ret = _adm->getPlayoutDevice(id);
    } while (false);
    return Napi::String::New(env, ret== 0 ? (char*)id : "");
}

NIM_SDK_NODE_API_DEF(setRecordDeviceVolume)
{
    INIT_ENV
    do
    {
        uint32_t volume;
        napi_get_value_uint32(info[0], volume);
        ret = _adm->setRecordDeviceVolume(volume);
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(getRecordDeviceVolume)
{
    INIT_ENV
    uint32_t volume = 0;
    do
    {
        ret = _adm->getRecordDeviceVolume(&volume);
    } while (false);
    return Napi::Number::New(env, ret == 0 ? volume : ret);
}

NIM_SDK_NODE_API_DEF(setPlayoutDeviceVolume)
{
    INIT_ENV
    do
    {
        uint32_t volume;
        napi_get_value_uint32(info[0], volume);
        ret = _adm->setPlayoutDeviceVolume(volume);
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(getPlayoutDeviceVolume)
{
    INIT_ENV
    uint32_t volume = 0;
    do
    {
        ret = _adm->getPlayoutDeviceVolume(&volume);
    } while (false);
    return Napi::Number::New(env, ret == 0 ? volume : ret);
}


NIM_SDK_NODE_API_DEF(setPlayoutDeviceMute)
{
    INIT_ENV
    do
    {
        bool mute;
        napi_get_value_bool(info[0], mute);
        ret = _adm->setPlayoutDeviceMute(mute);
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(getPlayoutDeviceMute)
{
    INIT_ENV_BOOL
    do
    {
        _adm->getPlayoutDeviceMute(&ret);
    } while (false);
    return Napi::Boolean ::New(env, ret);
}

NIM_SDK_NODE_API_DEF(setRecordDeviceMute)
{
    INIT_ENV
    do
    {
        bool mute;
        napi_get_value_bool(info[0], mute);
        ret = _adm->setRecordDeviceMute(mute);
    } while (false);
    return Napi::Number ::New(env, ret);
}

NIM_SDK_NODE_API_DEF(getRecordDeviceMute)
{
    INIT_ENV_BOOL
    do
    {
        _adm->getRecordDeviceMute(&ret);
    } while (false);
    return Napi::Boolean ::New(env, ret);
}

NIM_SDK_NODE_API_DEF(adjustRecordingSignalVolume)
{
    INIT_ENV
    do
    {
        uint32_t volume;
        napi_get_value_uint32(info[0], volume);
        ret = _adm->adjustRecordingSignalVolume(volume);
    } while (false);
    return Napi::Number ::New(env, ret);
}

NIM_SDK_NODE_API_DEF(adjustPlaybackSignalVolume)
{
    INIT_ENV
    do
    {
        uint32_t volume;
        napi_get_value_uint32(info[0], volume);
        ret = _adm->adjustPlaybackSignalVolume(volume);
    } while (false);
    return Napi::Number ::New(env, ret);
}


NIM_SDK_NODE_API_DEF(startRecordDeviceTest)
{
    INIT_ENV
    do
    { 
        uint32_t interval; // todo uint64
        napi_get_value_uint32(info[0], interval);
        ret = _adm->startRecordDeviceTest(interval);
    } while (false);
    return Napi::Number ::New(env, ret);
}

NIM_SDK_NODE_API_DEF(stopRecordDeviceTest)
{
    INIT_ENV
    do
    {
        ret = _adm->stopRecordDeviceTest();
    } while (false);
    return Napi::Number ::New(env, ret);
}

NIM_SDK_NODE_API_DEF(startPlayoutDeviceTest)
{
    INIT_ENV
    do
    {
        std::string path;
        napi_get_value_utf8_string(info[0], path);
        if (path.length() == 0)
        {
            break;
        }
        ret = _adm->startPlayoutDeviceTest(path.c_str());
    } while (false);
    return Napi::Number ::New(env, ret);
}

NIM_SDK_NODE_API_DEF(stopPlayoutDeviceTest)
{
    INIT_ENV
    do
    {
        ret = _adm->stopPlayoutDeviceTest();
    } while (false);
    return Napi::Number ::New(env, ret);
}

NIM_SDK_NODE_API_DEF(startAudioDeviceLoopbackTest)
{
    INIT_ENV
    do
    {
        uint32_t interval;  // todo uint64
        napi_get_value_uint32(info[0], interval);
        ret = _adm->startAudioDeviceLoopbackTest(interval);
    } while (false);
    return Napi::Number ::New(env, ret);
}

NIM_SDK_NODE_API_DEF(stopAudioDeviceLoopbackTest)
{
    INIT_ENV
    do
    {
        ret = _adm->stopAudioDeviceLoopbackTest();
    } while (false);
    return Napi::Number ::New(env, ret);
}

NIM_SDK_NODE_API_DEF(enumerateCaptureDevices)
{
    INIT_ENV
    Napi::Array arr = Napi::Array::New(env);
    do
    {
        auto devices = _vdm->enumerateCaptureDevices();
        if (devices != nullptr)
        {
            size_t count = devices->getCount();
            for (size_t i = 0; i < count; i++)
            {
                nertc::NERtcDeviceInfo info;
                if (devices->getDeviceInfo(i, &info) == 0)
                {
                    Napi::Object obj = Napi::Object::New(env);
                    obj.Set(Napi::String::New(env, "device_id"), Napi::String::New(env, (const char*)info.device_id));
                    obj.Set(Napi::String::New(env, "device_name"), Napi::String::New(env, (const char*)info.device_name));
                    obj.Set(Napi::String::New(env, "transport_type"), Napi::Number::New(env, info.transport_type));
                    obj.Set(Napi::String::New(env, "suspected_unavailable"), Napi::Boolean::New(env, info.suspected_unavailable));
                    obj.Set(Napi::String::New(env, "system_default_device"), Napi::Boolean::New(env, info.system_default_device));
                    arr[i] = obj;        
                }
            }
            devices->destroy();
        }
    } while (false);
    return arr;
}

NIM_SDK_NODE_API_DEF(setDevice)
{
    INIT_ENV
    do
    {
        std::string device;
        napi_get_value_utf8_string(info[0], device);
        if (device.length() == 0)
        {
            break;
        }
        ret = _vdm->setDevice(device.c_str());
    } while (false);
    return Napi::Number ::New(env, ret);
}

NIM_SDK_NODE_API_DEF(getDevice)
{
    INIT_ENV
    char id[256];
    do
    {
        ret = _vdm->getDevice(id);
    } while (false);
    return Napi::String ::New(env,  ret== 0 ? (char*)id : "");
}

NIM_SDK_NODE_API_DEF(enumerateScreenCaptureSourceInfo)
{
    INIT_ENV
    Napi::Array arr = Napi::Array::New(env);
#ifdef WIN32
    do
    {
        int thumbWidth, thumbHeight, iconWidth, iconHeight;
        napi_get_value_int32(info[0], thumbWidth);
        napi_get_value_int32(info[1], thumbHeight);
        napi_get_value_int32(info[2], iconWidth);
        napi_get_value_int32(info[3], iconHeight);
        WindowsHelpers::CaptureTargetInfoList list; 
        _windows_helper->getCaptureWindowList(&list);

        // Logger::Instance()->debug("enumerateScreenCaptureSourceInfo::[list]" + Logger::int32ToStr(list.size()));
        uint32_t i = 0;
        bool captureScreenRet = screen_capture_helper_->InitScreen();
        if (captureScreenRet)
        {
            uint32_t vx = GetSystemMetrics(SM_XVIRTUALSCREEN);
            uint32_t vy = GetSystemMetrics(SM_YVIRTUALSCREEN);
            for (auto w : list)
            {
                if (w.type != 1)
                {
                    continue;
                }
                Napi::Object obj = Napi::Object::New(env);
                int32_t sid = reinterpret_cast<int32_t>(w.id);
                obj.Set(Napi::String::New(env, "sourceId"), Napi::Number::New(env, sid));
                obj.Set(Napi::String::New(env, "sourceName"), Napi::String::New(env, UTF16ToUTF8(w.title)));
                obj.Set(Napi::String::New(env, "type"), Napi::Number::New(env, w.type));
                obj.Set(Napi::String::New(env, "left"), Napi::Number::New(env, w.rc.left));
                obj.Set(Napi::String::New(env, "top"), Napi::Number::New(env, w.rc.top));
                obj.Set(Napi::String::New(env, "right"), Napi::Number::New(env, w.rc.right));
                obj.Set(Napi::String::New(env, "bottom"), Napi::Number::New(env, w.rc.bottom));
                obj.Set(Napi::String::New(env, "displayId"), Napi::String::New(env, w.display_id));
                _windows_helper->updateCachedInfos(sid, w.rc);
                int left = w.rc.left - vx;
                int top = w.rc.top - vy;
                int sWidth = w.rc.right - w.rc.left;
                int sHeight = w.rc.bottom - w.rc.top;
                if (screen_capture_helper_->CaptureScreenRect(left, top, sWidth, sHeight, thumbWidth, thumbHeight))
                {
                    int size = GetRGBASize(thumbWidth, thumbHeight);
                    uint8_t *data = RGBAToBGRA(screen_capture_helper_->GetData(), size);
                    Napi::Object thumb = Napi::Object::New(env);
                    thumb.Set(Napi::String::New(env, "length"), Napi::Number::New(env, size));
                    thumb.Set(Napi::String::New(env, "width"), Napi::Number::New(env, thumbWidth));
                    thumb.Set(Napi::String::New(env, "height"), Napi::Number::New(env, thumbHeight));
                    Napi::ArrayBuffer buff = Napi::ArrayBuffer::New(env, data, size);
                    Napi::Uint8Array dataarray = Napi::TypedArrayOf<uint8_t>::New(env, buff.ByteLength(), buff, 0, napi_uint8_array);
                    thumb.Set(Napi::String::New(env, "buffer"), dataarray);
                    obj.Set(Napi::String::New(env, "thumbBGRA"), thumb);
                }
                arr[i++] = obj;
            }
            screen_capture_helper_->UnintScreen();
        }

        for (auto w : list)
        {
            if (w.type != 2) continue;
            Napi::Object obj = Napi::Object::New(env);
            obj.Set(Napi::String::New(env, "sourceId"), Napi::Number::New(env, reinterpret_cast<int32_t>(w.id)));
            obj.Set(Napi::String::New(env, "sourceName"), Napi::String::New(env, UTF16ToUTF8(w.title)));
            obj.Set(Napi::String::New(env, "type"), Napi::Number::New(env, w.type));
            obj.Set(Napi::String::New(env, "isMinimizeWindow"), Napi::Boolean::New(env, w.isMinimizeWindow));
            if (!w.isMinimizeWindow)
            {
                if (window_capture_helper_->CaptureWindow(w.id) && window_capture_helper_->Zoom(thumbWidth, thumbHeight))
                {
                    int size = GetRGBASize(thumbWidth, thumbHeight);
                    uint8_t *data = RGBAToBGRA(window_capture_helper_->GetData(), size);
                    Napi::Object thumb = Napi::Object::New(env);
                    thumb.Set(Napi::String::New(env, "length"), Napi::Number::New(env, size));
                    thumb.Set(Napi::String::New(env, "width"), Napi::Number::New(env, thumbWidth));
                    thumb.Set(Napi::String::New(env, "height"), Napi::Number::New(env, thumbHeight));
                    Napi::ArrayBuffer buff = Napi::ArrayBuffer::New(env, data, size);
                    Napi::Uint8Array dataarray = Napi::TypedArrayOf<uint8_t>::New(env, buff.ByteLength(), buff, 0, napi_uint8_array);
                    thumb.Set(Napi::String::New(env, "buffer"), dataarray);
                    obj.Set(Napi::String::New(env, "thumbBGRA"), thumb);
                } else {
                    int iconSize = 0;
                    uint8_t *rgba = GetWindowsIconRGBA(w.id, &thumbWidth, &thumbHeight, &iconSize);
                    if (rgba != NULL)
                    {
                        uint8_t *data = RGBAToBGRA((void *)rgba, iconSize);
                        free(rgba);
                        rgba = nullptr;
                        Napi::Object icon = Napi::Object::New(env);
                        icon.Set(Napi::String::New(env, "length"), Napi::Number::New(env, iconSize));
                        icon.Set(Napi::String::New(env, "width"), Napi::Number::New(env, thumbWidth));
                        icon.Set(Napi::String::New(env, "height"), Napi::Number::New(env, thumbHeight));
                        Napi::ArrayBuffer buff = Napi::ArrayBuffer::New(env, data, iconSize);
                        Napi::Uint8Array dataarray = Napi::TypedArrayOf<uint8_t>::New(env, buff.ByteLength(), buff, 0, napi_uint8_array);
                        icon.Set(Napi::String::New(env, "buffer"), dataarray);
                        obj.Set(Napi::String::New(env, "thumbBGRA"), icon);
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
                Napi::Object icon = Napi::Object::New(env);
                icon.Set(Napi::String::New(env, "length"), Napi::Number::New(env, iconSize));
                icon.Set(Napi::String::New(env, "width"), Napi::Number::New(env, iconWidth));
                icon.Set(Napi::String::New(env, "height"), Napi::Number::New(env, iconHeight));
                Napi::ArrayBuffer buff = Napi::ArrayBuffer::New(env, data, iconSize);
                Napi::Uint8Array dataarray = Napi::TypedArrayOf<uint8_t>::New(env, buff.ByteLength(), buff, 0, napi_uint8_array);
                icon.Set(Napi::String::New(env, "buffer"), dataarray);
                obj.Set(Napi::String::New(env, "iconBGRA"), icon);
            }
            arr[i++] = obj;
        }
        window_capture_helper_->Cleanup();
    } while (false);
#endif
    return arr;
}


NIM_SDK_NODE_API_DEF(startSystemAudioLoopbackCapture)
{
    INIT_ENV
    do
    {
#ifdef WIN32
        // ret = rtc_engine_->startSystemAudioLoopbackCapture();
#endif
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(stopSystemAudioLoopbackCapture)
{
    INIT_ENV
    do
    {
#ifdef WIN32
        // ret = rtc_engine_->stopSystemAudioLoopbackCapture();
#endif
    } while (false);
    return Napi::Number ::New(env, ret);
}

NIM_SDK_NODE_API_DEF(setSystemAudioLoopbackCaptureVolume)
{
    INIT_ENV
    do
    {
        uint32_t vol;
        napi_get_value_uint32(info[0], vol);
#ifdef WIN32
        // ret = rtc_engine_->setSystemAudioLoopbackCaptureVolume(vol);
#endif
    } while (false);
    return Napi::Number ::New(env, ret);
}

    //请勿删除，开发调试参数使用
    // Logger::Instance()->initPath("D:/nertc_node_log.txt");
    // Logger::Instance()->debug("joinChannel" + Logger::int32ToStr(1990));

}