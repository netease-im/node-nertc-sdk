#include <iostream>
#include "node_version.h"
#include "nertc_node_engine.h"
#include "nertc_node_engine_helper.h"
#include "../shared/sdk_helper/node_api_helper.h"
#include "nertc_node_video_frame_provider.h"
#include "../shared/log/logging/logging.h"
#ifdef WIN32
#include "../shared/util/string_util.h"
using namespace nertc_electron_util;
#endif
#include <string>
#include <unordered_map>

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

/****************************************************************************************************************************/
    // support mutile engine
	std::unordered_map<uint64_t, NodeVideoFrameTransporter *> g_transporter_map;
	void EngineOnFrameDataCallback(nertc::uid_t uid, void *data, uint32_t type, uint32_t width, uint32_t height,
		uint32_t count, uint32_t offset[4], uint32_t stride[4], uint32_t rotation, void *user_data)
	{
		int rotate = 0;
		switch (rotation)
		{
		case nertc::kNERtcVideoRotation_0:
		{
		}
		break;
		case nertc::kNERtcVideoRotation_90:
		{
			rotate = 90;
		}
		break;
		case nertc::kNERtcVideoRotation_180:
		{
			rotate = 180;
		}
		break;
		case nertc::kNERtcVideoRotation_270:
		{
			rotate = 270;
		}
		break;
		}

		IVideoFrame frame;
		frame.data = reinterpret_cast<uint8_t *>(data);
		frame.rotation = rotate;
		frame.count = count;

		for (auto i = 0; i < count; i++)
		{
			frame.offset[i] = offset[i];
			frame.stride[i] = stride[i];
		}

		frame.uid = uid;//*((nertc::uid_t *)user_data);
		frame.width = width;
		frame.height = height;

		NodeRenderType nrt = frame.uid == 0 ? NODE_RENDER_TYPE_LOCAL : NODE_RENDER_TYPE_REMOTE;
		/*auto *pTransporter = getNodeVideoFrameTransporter();
		if (pTransporter)
		{
			pTransporter->deliverFrame_I420(nrt, frame.uid, "", frame, rotate, frame.uid == 0);
		}*/
        if(user_data) {
            uint64_t thisAddr = *((nertc::uid_t *)user_data);
		    NodeVideoFrameTransporter * pTransporter = g_transporter_map[thisAddr];
		    if (pTransporter) {
			    pTransporter->deliverFrame_I420(nrt, frame.uid, "", frame, rotate, frame.uid == 0, (nertc::NERtcVideoStreamType)type);
		    }
        }
	}

    void NodeOnSubstreamFrameDataCallback(
    nertc::uid_t uid,
    void *data,
    uint32_t type,
    uint32_t width,
    uint32_t height,
    uint32_t count,
    uint32_t offset[4],
    uint32_t stride[4],
    uint32_t rotation,
    void *user_data)
    {
        int rotate = 0;
        switch (rotation)
        {
        case nertc::kNERtcVideoRotation_0:
        {
        }
        break;
        case nertc::kNERtcVideoRotation_90:
        {
            rotate = 90;
        }
        break;
        case nertc::kNERtcVideoRotation_180:
        {
            rotate = 180;
        }
        break;
        case nertc::kNERtcVideoRotation_270:
        {
            rotate = 270;
        }
        break;
        }
    
        IVideoFrame frame;
        frame.data = reinterpret_cast<uint8_t *>(data);
        frame.rotation = rotate;
        frame.count = count;
    
        for (auto i = 0; i < count; i++)
        {
            frame.offset[i] = offset[i];
            frame.stride[i] = stride[i];
        }
    
        frame.uid = uid;//*((nertc::uid_t *)user_data);
        frame.width = width;
        frame.height = height;
    
        NodeRenderType nrt = frame.uid == 0 ? NODE_RENDER_TYPE_LOCAL_SUBSTREAM : NODE_RENDER_TYPE_REMOTE_SUBSTREAM;
        //auto *pTransporter = getNodeVideoFrameTransporter();
        uint64_t thisAddr = *((nertc::uid_t *)user_data);
		NodeVideoFrameTransporter * pTransporter = g_transporter_map[thisAddr];
        if (pTransporter)
        {
            pTransporter->deliverFrame_I420(nrt, frame.uid, "", frame, rotate, false, (nertc::NERtcVideoStreamType)type);
        }
    }
/****************************************************************************************************************************/



nertc::IRtcEngineEx *rtc_engine_ = nullptr;
nertc::IRtcEngineEx * NertcNodeEngine::getNertcEngine() {
    return rtc_engine_;
}

#if NAPI_VERSION < 6
Napi::FunctionReference NertcNodeEngine::constructor;
#endif

Napi::Object NertcNodeEngine::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function func = DefineClass(env, "NertcNodeEngine", {
        SET_PROTOTYPE(initialize),
        SET_PROTOTYPE(release),
        SET_PROTOTYPE(setClientRole),
        SET_PROTOTYPE(setChannelProfile),
        SET_PROTOTYPE(joinChannel),
        SET_PROTOTYPE(joinChannelWithOptions),
        SET_PROTOTYPE(switchChannel),
        SET_PROTOTYPE(switchChannelWithOptions),
        SET_PROTOTYPE(switchChannelWithOptionsEx),
        SET_PROTOTYPE(leaveChannel),
        SET_PROTOTYPE(enableLocalAudio),
        SET_PROTOTYPE(setupVideoCanvas),
        SET_PROTOTYPE(enableLocalVideo),
        SET_PROTOTYPE(enableLocalVideoWithType),
        SET_PROTOTYPE(subscribeRemoteVideoStream),
        SET_PROTOTYPE(setLocalVideoWatermarkConfigs),

    
    
        SET_PROTOTYPE(onVideoFrame),
        SET_PROTOTYPE(onEvent),
        // SET_PROTOTYPE(onAudioFrameEvent),
        SET_PROTOTYPE(onStatsObserver),
        SET_PROTOTYPE(onQsObserver),
    
    
        SET_PROTOTYPE(getConnectionState),
        SET_PROTOTYPE(muteLocalAudioStream),
        SET_PROTOTYPE(enableLocalSubStreamAudio),
        SET_PROTOTYPE(muteLocalSubStreamAudio),
        SET_PROTOTYPE(setAudioProfile),
        SET_PROTOTYPE(setAudioEffectPreset),
        SET_PROTOTYPE(setVoiceBeautifierPreset),
        SET_PROTOTYPE(setLocalVoicePitch),
        SET_PROTOTYPE(setLocalVoiceEqualization),
        SET_PROTOTYPE(subscribeRemoteAudioStream),
        SET_PROTOTYPE(subscribeRemoteSubStreamAudio),
        SET_PROTOTYPE(subscribeAllRemoteAudioStream),
        SET_PROTOTYPE(setAudioSubscribeOnlyBy),
        SET_PROTOTYPE(setSubscribeAudioAllowlist),
        SET_PROTOTYPE(setSubscribeAudioBlocklist),
        SET_PROTOTYPE(setStreamAlignmentProperty),
        SET_PROTOTYPE(getNtpTimeOffset),
        SET_PROTOTYPE(setCameraCaptureConfig),
        SET_PROTOTYPE(setCameraCaptureConfigWithType),
        SET_PROTOTYPE(setVideoConfig),
        SET_PROTOTYPE(setVideoConfigWithType),
        SET_PROTOTYPE(enableDualStreamMode),
        SET_PROTOTYPE(setupSubStreamVideoCanvas),
        SET_PROTOTYPE(setLocalVideoMirrorMode),
        SET_PROTOTYPE(setLocalVideoMirrorModeWithType),
        SET_PROTOTYPE(subscribeRemoteVideoSubStream),
        SET_PROTOTYPE(startVideoPreview),
        SET_PROTOTYPE(startVideoPreviewWithType),
        SET_PROTOTYPE(stopVideoPreview),
        SET_PROTOTYPE(stopVideoPreviewWithType),
        SET_PROTOTYPE(muteLocalVideoStream),
        SET_PROTOTYPE(muteLocalVideoStreamWithType),
        SET_PROTOTYPE(setLocalMediaPriority),
        SET_PROTOTYPE(setParameters),
        SET_PROTOTYPE(getParameters),
        SET_PROTOTYPE(setRecordingAudioFrameParameters),
        SET_PROTOTYPE(setPlaybackAudioFrameParameters),
        SET_PROTOTYPE(setMixedAudioFrameParameters),
        SET_PROTOTYPE(startAudioDump),
        SET_PROTOTYPE(startAudioDumpWithType),
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
        SET_PROTOTYPE(setAudioMixingPitch),
        SET_PROTOTYPE(getAudioMixingPitch),
        SET_PROTOTYPE(setEffectPitch),
        SET_PROTOTYPE(getEffectPitch),
        SET_PROTOTYPE(enableLoopbackRecording),
        SET_PROTOTYPE(adjustLoopbackRecordingSignalVolume),
        SET_PROTOTYPE(enableEarback),
        SET_PROTOTYPE(setEarbackVolume),
        SET_PROTOTYPE(enableAudioVolumeIndication),
        SET_PROTOTYPE(enableAudioVolumeIndicationEx),
        SET_PROTOTYPE(getScreenCaptureSources),
        SET_PROTOTYPE(setScreenCaptureSource),
        SET_PROTOTYPE(startScreenCaptureByScreenRect),
        SET_PROTOTYPE(startScreenCaptureByDisplayId),
        SET_PROTOTYPE(startScreenCaptureByWindowId),
        SET_PROTOTYPE(updateScreenCaptureRegion),
        SET_PROTOTYPE(setScreenCaptureMouseCursor),
        SET_PROTOTYPE(stopScreenCapture),
        SET_PROTOTYPE(pauseScreenCapture),
        SET_PROTOTYPE(resumeScreenCapture),
        SET_PROTOTYPE(setExcludeWindowList),
        SET_PROTOTYPE(updateScreenCaptureParameters),
        // SET_PROTOTYPE(setExternalVideoSource),
        // SET_PROTOTYPE(setExternalVideoSourceEx),
        // SET_PROTOTYPE(pushExternalVideoFrame),
        // SET_PROTOTYPE(setExternalAudioSource),
        // SET_PROTOTYPE(pushExternalAudioFrame),
        // SET_PROTOTYPE(setExternalAudioRender),
        // SET_PROTOTYPE(pullExternalAudioFrame), //todo
        SET_PROTOTYPE(getVersion),
        SET_PROTOTYPE(getErrorDescription),
        SET_PROTOTYPE(uploadSdkInfo),
        SET_PROTOTYPE(addLiveStreamTask),
        SET_PROTOTYPE(updateLiveStreamTask),
        SET_PROTOTYPE(removeLiveStreamTask),
        SET_PROTOTYPE(sendSEIMsg),
        SET_PROTOTYPE(sendSEIMsgWithType),
        SET_PROTOTYPE(startAudioRecording),
        SET_PROTOTYPE(startAudioRecordingWithConfig),
        SET_PROTOTYPE(stopAudioRecording),
        SET_PROTOTYPE(adjustUserPlaybackSignalVolume),
        SET_PROTOTYPE(adjustChannelPlaybackSignalVolume),
        SET_PROTOTYPE(startChannelMediaRelay),
        SET_PROTOTYPE(updateChannelMediaRelay),
        SET_PROTOTYPE(stopChannelMediaRelay),
        SET_PROTOTYPE(setLocalPublishFallbackOption),
        SET_PROTOTYPE(setRemoteSubscribeFallbackOption),
        SET_PROTOTYPE(enableSuperResolution),
        SET_PROTOTYPE(enableEncryption),
        SET_PROTOTYPE(startLastmileProbeTest),
        SET_PROTOTYPE(stopLastmileProbeTest),
        SET_PROTOTYPE(setRemoteHighPriorityAudioStream),
        SET_PROTOTYPE(checkNECastAudioDriver),
        SET_PROTOTYPE(enableVirtualBackground),
        SET_PROTOTYPE(isFeatureSupported),
        SET_PROTOTYPE(setCloudProxy),
        SET_PROTOTYPE(enableLocalData),
        SET_PROTOTYPE(subscribeRemoteData),
        SET_PROTOTYPE(sendData),
        SET_PROTOTYPE(startBeauty),
        SET_PROTOTYPE(stopBeauty),
        SET_PROTOTYPE(enableBeauty),
        // SET_PROTOTYPE(enableBeautyMirrorMode),
        SET_PROTOTYPE(getBeautyEffect),
        SET_PROTOTYPE(setBeautyEffect),
        SET_PROTOTYPE(addBeautyFilter),
        SET_PROTOTYPE(removeBeautyFilter),
        SET_PROTOTYPE(setBeautyFilterLevel),
        SET_PROTOTYPE(addBeautySticker),
        SET_PROTOTYPE(removeBeautySticker),
        SET_PROTOTYPE(addBeautyMakeup),
        SET_PROTOTYPE(removeBeautyMakeup),
        SET_PROTOTYPE(setLocalVoiceReverbParam),
        SET_PROTOTYPE(enableMediaPub),
        SET_PROTOTYPE(updatePermissionKey),
        SET_PROTOTYPE(setEffectPosition),
        SET_PROTOTYPE(getEffectCurrentPosition),
        SET_PROTOTYPE(getEffectDuration),
        SET_PROTOTYPE(setRangeAudioMode),
        SET_PROTOTYPE(setRangeAudioTeamID),
        SET_PROTOTYPE(setAudioRecvRange),
        SET_PROTOTYPE(updateSelfPosition),
        SET_PROTOTYPE(enableSpatializerRoomEffects),
        SET_PROTOTYPE(setSpatializerRoomProperty),
        SET_PROTOTYPE(setSpatializerRenderMode),
        SET_PROTOTYPE(initSpatializer),
        SET_PROTOTYPE(enableSpatializer),
    
    
        // adm
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
    
        // vdm
        SET_PROTOTYPE(enumerateCaptureDevices),
        SET_PROTOTYPE(setDevice),
        SET_PROTOTYPE(setDeviceWithType),
        SET_PROTOTYPE(getDevice),
        SET_PROTOTYPE(getDeviceWithType),
    
        // CUSTOM
		SET_PROTOTYPE(enumerateScreenCaptureSourceInfo),

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
    _qs_handler = std::make_shared<NertcNodeVideoEncoderQosObserver>();
    _audio_observer = std::make_shared<NertcNodeAudioFrameObserverHandler>();
    _packet_observer = std::make_shared<NertcNodePacketObserver>();
#ifdef WIN32
    _windows_helper = new WindowsHelpers();
    window_capture_helper_.reset(new WindowCaptureHelper());
    screen_capture_helper_.reset(new ScreenCaptureHelper());
#endif
    uint64_t thisAddr = (uint64_t)this;
    g_transporter_map[thisAddr] = new NodeVideoFrameTransporter();
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
        //CHECK_API_FUNC(1);
        Napi::Object obj = info[0].As<Napi::Object>();
        nertc::NERtcEngineContext context;
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
        if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"log_dir_path"))))
        {
            log_path_ = obj.Get(static_cast<napi_value>(Napi::String::New(env,"log_dir_path"))).As<Napi::String>().Utf8Value();
            context.log_dir_path = log_path_.c_str();
        }
        context.event_handler = _event_handler.get();
        std::string para = "{\"sdk.business.scenario.type\": 6,  \"video.h265.decoder.type\": false , \"video.h265.encoder.type\": false}";
        ret = rtc_engine_->setParameters(para.c_str());
        ret = rtc_engine_->initialize(context);
        if (ret == 0)
        {
            rtc_engine_->queryInterface(nertc::kNERtcIIDAudioDeviceManager, (void **)&_adm);
            rtc_engine_->queryInterface(nertc::kNERtcIIDVideoDeviceManager, (void **)&_vdm);
            rtc_engine_->setAudioFrameObserver(_audio_observer.get());
        }
        rtc_engine_->setStatsObserver(_stats_observer.get());
        std::string log_directory(context.log_dir_path);
        auto error_code = nelog::InitailizeLogFileStream(
                          log_directory.c_str(),
                          "addon_log",
                           false
                           );
        nelog::SetMinLoggingSeverity(nelog::LS_INFO);
        //LOG_F(INFO, "%s:%d", "test", 1);
        LOG_F(INFO, "-------------initialize ret:%d-------------", ret);
    }while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(release)
{
    INIT_ENV
    do
    {
        LOG_F(INFO, "-------------sdk release-------------");
        rtc_engine_->setAudioFrameObserver(nullptr);
        _audio_observer->removeAll();
        ret = rtc_engine_->stopVideoPreview();
		// NodeVideoFrameTransporter *pTransporter = getNodeVideoFrameTransporter();
		// pTransporter->stopFlushVideo();
        rtc_engine_->release(true);
        if (rtc_engine_)
        {
            destroyNERtcEngine((void *&)rtc_engine_);
            _adm = nullptr;
            _vdm = nullptr;
            rtc_engine_ = nullptr;
        }
        _event_handler->removeAll();
        _packet_observer.reset();

        //todo
        // NertcNodeRtcMediaStatsHandler::GetInstance()->RemoveAll();
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
        LOG_F(INFO, "profile:%d", profile);
        ret = rtc_engine_->setChannelProfile((nertc::NERtcChannelProfileType)profile);
        LOG_F(INFO, "ret:%d", ret);
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
        int64_t uid = 0;
        napi_get_value_utf8_string(info[0], token);
        napi_get_value_utf8_string(info[1], channel_name);
        napi_get_value_int64(info[2], uid);
        LOG_F(INFO, "channel_name:%s, uid:%llu", channel_name.c_str(), uid);
        ret = rtc_engine_->joinChannel(token.length() == 0 ? "" : token.c_str(), channel_name.c_str(), uid);
        // NodeVideoFrameTransporter *pTransporter = getNodeVideoFrameTransporter();
		// pTransporter->startFlushVideo();
        LOG_F(INFO, "ret:%d", ret);
    }while(false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(joinChannelWithOptions)
{
    INIT_ENV
    do{
        std::string token = "";
        std::string channel_name = "";
        nertc::NERtcJoinChannelOptions option;
        int64_t uid = 0;
        napi_get_value_utf8_string(info[0], token);
        napi_get_value_utf8_string(info[1], channel_name);
        napi_get_value_int64(info[2], uid);
		Napi::Object obj = info[3].As<Napi::Object>();
        nertc_join_channel_option_to_struct(env, obj, option);
        LOG_F(INFO, "joinChannelwithoption channel_name:%s, uid:%llu", channel_name.c_str(), uid);
        ret = rtc_engine_->joinChannel(token.length() == 0 ? "" : token.c_str(), channel_name.c_str(), uid, option);
		if (option.permission_key != nullptr) {
			delete[] option.permission_key;
			option.permission_key = nullptr;
		}
        LOG_F(INFO, "ret:%d", ret);
    }while(false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(leaveChannel)
{
    INIT_ENV
    do
    {
		ret = rtc_engine_->stopVideoPreview();
		// NodeVideoFrameTransporter *pTransporter = getNodeVideoFrameTransporter();
		// pTransporter->stopFlushVideo();
        ret = rtc_engine_->leaveChannel();
        LOG_F(INFO, "ret:%d", ret);
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
        LOG_F(INFO, "enabled:%d", enabled);
        ret = rtc_engine_->enableLocalAudio(enabled);
        LOG_F(INFO, "ret:%d", ret);
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
        LOG_F(INFO, "enabled:%d", enabled);
        ret = rtc_engine_->enableLocalVideo(enabled);
        LOG_F(INFO, "ret:%d", ret);
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(enableLocalVideoWithType)
{
    INIT_ENV
    do
    {
        int type = 0;
        bool enabled = false;
        napi_get_value_int32(info[0], type);
        napi_get_value_bool(info[1], enabled);
        LOG_F(INFO, "type:%d enabled:%d", type, enabled);
        ret = rtc_engine_->enableLocalVideo((nertc::NERtcVideoStreamType)type, enabled);
        LOG_F(INFO, "ret:%d", ret);
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
        LOG_F(INFO, "uid:%llu, type:%d, sub:%d", uid, type, sub);
        ret = rtc_engine_->subscribeRemoteVideoStream(uid, (nertc::NERtcRemoteVideoStreamType)type, sub);
        LOG_F(INFO, "ret:%d", ret);
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(setLocalVideoWatermarkConfigs)
{
    INIT_ENV
    do
    {
        bool enbale;
        napi_get_value_bool(info[0], enbale);
        int32_t type;
        napi_get_value_int32(info[1], type);
        LOG_F(INFO, "enbale:%d type:%d", enbale, type);
        nertc::NERtcVideoWatermarkConfig wateark_info;
        memset(&wateark_info, 0, sizeof(nertc::NERtcVideoWatermarkConfig));
        Napi::Object obj = info[2].As<Napi::Object>();
        nertc_video_water_mark_config_to_struct(env, obj, wateark_info);
        ret = rtc_engine_->setLocalVideoWatermarkConfigs(enbale, (nertc::NERtcVideoStreamType)type, wateark_info);
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(setupVideoCanvas)
{
    INIT_ENV
    do{
        // nertc::uid_t uid = 0;
        int64_t uid;
        bool lossless = true;
        bool enable;
        napi_get_value_int64(info[0], uid);
        // napi_get_value_bigint_uint64(env, info[0], &uid, &lossless);
        napi_get_value_bool(info[1], enable);
        LOG_F(INFO, "uid:%llu, enable:%d", uid, enable);
        nertc::NERtcVideoCanvas canvas;
        canvas.cb = enable ? EngineOnFrameDataCallback : nullptr; //NodeVideoFrameTransporter::onFrameDataCallback;
        uint64_t thisAddr = (uint64_t)this;
        canvas.user_data = enable ? (void*)(new nertc::uid_t(thisAddr)) : nullptr;//enable ? (void*)(new nertc::uid_t(uid)) : nullptr;
        canvas.window = nullptr;
        if (uid == 0)
        {
            ret = rtc_engine_->setupLocalVideoCanvas(&canvas);
        }else{
            ret = rtc_engine_->setupRemoteVideoCanvas(uid, &canvas);
        }
        LOG_F(INFO, "ret:%d", ret);
    }while(false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(onVideoFrame)
{
    INIT_ENV
    do{
        Napi::FunctionReference function;
        napi_get_value_function(info[0], function);
        uint64_t thisAddr = (uint64_t)this;
        NodeVideoFrameTransporter * pTransporter = g_transporter_map[thisAddr];
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

// NIM_SDK_NODE_API_DEF(onAudioFrameEvent)
// {
//     auto env = info.Env();
//     std::string event_name = "";
//     bool enable;
//     Napi::FunctionReference function;
//     napi_get_value_utf8_string(info[0], event_name);
//     napi_get_value_function(info[1], function);
//     napi_get_value_bool(info[2], enable);
//     if(enable) {
//         _audio_observer->addEvent(event_name, std::move(function));
//     } else {
//         _audio_observer->removeEventHandler(event_name);
//     }
//     auto ret_value = env.Null();
//     return ret_value;
// }

NIM_SDK_NODE_API_DEF(setClientRole)
{
    INIT_ENV
    do
    {
        uint32_t role;
        napi_get_value_uint32(info[0], role);
        LOG_F(INFO, "role:%d", role);
        ret = rtc_engine_->setClientRole((nertc::NERtcClientRole)role);
        LOG_F(INFO, "ret:%d", ret);
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(setupSubStreamVideoCanvas)
{
    INIT_ENV
    do
    {
        // uint64_t uid;
        int64_t uid;
        bool enable;
        napi_get_value_int64(info[0], uid);
        napi_get_value_bool(info[1], enable);
        LOG_F(INFO, "uid:%llu, enable:%d", uid, enable);
        nertc::NERtcVideoCanvas canvas;
        canvas.cb = enable ? NodeOnSubstreamFrameDataCallback : nullptr;
        uint64_t thisAddr = (uint64_t)this;
        canvas.user_data = enable ? (void*)(new nertc::uid_t(thisAddr)) : nullptr;//enable ? (void*)(new nertc::uid_t(uid)) : nullptr;
        canvas.window = nullptr;
        if (uid == 0)
            ret = rtc_engine_->setupLocalSubStreamVideoCanvas(&canvas);
        else
            ret = rtc_engine_->setupRemoteSubStreamVideoCanvas(uid, &canvas);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
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
        LOG_F(INFO, "uid:%llu, sub:%d", uid, sub);
        ret = rtc_engine_->subscribeRemoteVideoSubStream(uid, sub);
        LOG_F(INFO, "ret:%d", ret);
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
        LOG_F(INFO, "samp:%d", samp);
        ret = rtc_engine_->setMixedAudioFrameParameters(samp);
        LOG_F(INFO, "ret:%d", ret);
    } while (false);
    return Napi::Number::New(env, ret);
}

// NIM_SDK_NODE_API_DEF(setExternalAudioSource)
// {
//     INIT_ENV
//     do
//     {
//         // Not Supported External Audio
//         // bool enabled;
//         // int samp, chan;
//         // napi_get_value_bool(info[0], enabled);
//         // napi_get_value_int32(info[1], samp);
//         // napi_get_value_int32(info[2], chan);
//         // LOG_F(INFO, "enabled:%d samp:%d chan:%d", enabled, samp, chan);
//         // ret = rtc_engine_->setExternalAudioSource(enabled, samp, chan);
//         // LOG_F(INFO, "ret:%d", ret);
//     } while (false);
//     return Napi::Number::New(env, ret);
// }

// NIM_SDK_NODE_API_DEF(pushExternalAudioFrame)
// {
//     INIT_ENV
//     do
//     {
//         //todo
//     } while (false);
//     return Napi::Number::New(env, ret);
// }

// NIM_SDK_NODE_API_DEF(pushExternalVideoFrame)
// {
//     INIT_ENV
//     do
//     {
//         //todo
//     } while (false);
//     return Napi::Number::New(env, ret);
// }

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
        LOG_F(INFO, "ret:%d", ret);
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(sendSEIMsgWithType)
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
        LOG_F(INFO, "ret:%d", ret);
    } while (false);
    return Napi::Number::New(env, ret);
}


// NIM_SDK_NODE_API_DEF(setExternalAudioRender)
// {
//     INIT_ENV
//     do
//     {
//         bool enable;
//         int sample_rate = 0;
//         int channels = 0;
//         napi_get_value_bool(info[0], enable);
//         napi_get_value_int32(info[1], sample_rate);
//         napi_get_value_int32(info[2], channels);
//         LOG_F(INFO, "enable:%d sample_rate:%d channels:%d", enable, sample_rate, channels);
//         ret = rtc_engine_->setExternalAudioRender(enable, sample_rate, channels);
//         LOG_F(INFO, "ret:%d", ret);
//     } while (false);
//     return Napi::Number::New(env, ret);
// }

// NIM_SDK_NODE_API_DEF(pullExternalAudioFrame)
// {
//     INIT_ENV
//     do
//     {
//         int length = 0;
//         Napi::FunctionReference function;
//         napi_get_value_int32(info[0], length);
//         napi_get_value_function(info[1], function);
//         auto shared_data = std::make_shared<unsigned char>(length);
//         ret = rtc_engine_->pullExternalAudioFrame(shared_data.get(), length);
//         _event_handler->onPullExternalAudioFrame(std::move(function), shared_data, length);
//     } while (false);
//     LOG_F(INFO, "ret:%d", ret);
//     return Napi::Number::New(env, ret);
// }

NIM_SDK_NODE_API_DEF(setAudioEffectPreset)
{
    INIT_ENV
    do
    {
        int32_t type;
        napi_get_value_int32(info[0], type);
        LOG_F(INFO, "type:%d", type);
        ret = rtc_engine_->setAudioEffectPreset(static_cast<nertc::NERtcVoiceChangerType>(type));
        LOG_F(INFO, "ret:%d", ret);
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
        LOG_F(INFO, "type:%d", type);
        ret = rtc_engine_->setVoiceBeautifierPreset(static_cast<nertc::NERtcVoiceBeautifierType>(type));
        LOG_F(INFO, "ret:%d", ret);
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
        LOG_F(INFO, "pitch:%f", pitch);
        ret = rtc_engine_->setLocalVoicePitch(pitch);
        LOG_F(INFO, "ret:%d", ret);
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
        LOG_F(INFO, "band_frequency:%d band_gain:%d", band_frequency, band_gain);
        ret = rtc_engine_->setLocalVoiceEqualization(
            (nertc::NERtcVoiceEqualizationBand)(band_frequency), 
            band_gain);
        LOG_F(INFO, "ret:%d", ret);
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(adjustUserPlaybackSignalVolume)
{
    INIT_ENV
    do
    {
        int64_t uid = 0;
        int volume = 0;
        napi_get_value_int64(info[0], uid);
        napi_get_value_int32(info[1], volume);
        LOG_F(INFO, "uid:%llu volume:%d", uid, volume);
        ret = rtc_engine_->adjustUserPlaybackSignalVolume(uid, volume);
        LOG_F(INFO, "ret:%d", ret);
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(adjustChannelPlaybackSignalVolume)
{
    INIT_ENV
    do
    {
        int volume = 0;
        napi_get_value_int32(info[0], volume);
        LOG_F(INFO, "volume:%d", volume);
        ret = rtc_engine_->adjustChannelPlaybackSignalVolume(volume);
        LOG_F(INFO, "ret:%d", ret);
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
        LOG_F(INFO, "channel_name:%s", channel_name.c_str());
        ret = rtc_engine_->switchChannel(token.c_str(), channel_name.c_str());
        LOG_F(INFO, "ret:%d", ret);
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(switchChannelWithOptions)
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
        LOG_F(INFO, "channel_name:%s", channel_name.c_str());
        nertc::NERtcJoinChannelOptions option;
        Napi::Object obj = info[2].As<Napi::Object>();
        nertc_join_channel_option_to_struct(env, obj, option);
        LOG_F(INFO, "switchChannelwithoption channel_name:%s", channel_name.c_str());
        ret = rtc_engine_->switchChannel(token.c_str(), channel_name.c_str(), option);
        LOG_F(INFO, "ret:%d", ret);
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(switchChannelWithOptionsEx)
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
        LOG_F(INFO, "channel_name:%s", channel_name.c_str());
        nertc::NERtcJoinChannelOptionsEx option;
        Napi::Object obj = info[2].As<Napi::Object>();
        nertc_channel_optionex_to_struct(env, obj, option);
        LOG_F(INFO, "switchChannelWithOptionsEx channel_name:%s", channel_name.c_str());
        ret = rtc_engine_->switchChannelEx(token.c_str(), channel_name.c_str(), option);
        LOG_F(INFO, "ret:%d", ret);
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
        LOG_F(INFO, "priority:%d enable:%d", priority, enable);
        ret = rtc_engine_->setLocalMediaPriority((nertc::NERtcMediaPriorityType)priority, enable);
        LOG_F(INFO, "ret:%d", ret);
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
        LOG_F(INFO, "enable:%d deviveName:%s", enable, deviveName.c_str());
        ret = rtc_engine_->enableLoopbackRecording(enable, deviveName.c_str());
        LOG_F(INFO, "ret:%d", ret);
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
        LOG_F(INFO, "volume:%d", volume);
        ret = rtc_engine_->adjustLoopbackRecordingSignalVolume(volume);
        LOG_F(INFO, "ret:%d", ret);
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
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(startAudioRecording)
{
    Napi::Env env = info.Env();
    int ret = -1;
    do
    {
        std::string path;
        uint32_t profile, quality;
        napi_get_value_utf8_string(info[0], path);
        napi_get_value_uint32(info[1], profile);
        napi_get_value_uint32(info[2], quality);
        LOG_F(INFO, "path:%s profile:%d quality:%d", path.c_str(), profile, quality);
        ret = rtc_engine_->startAudioRecording(path.c_str(), profile, (nertc::NERtcAudioRecordingQuality)quality);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(startAudioRecordingWithConfig)
{
    Napi::Env env = info.Env();
    int ret = -1;
    do
    {
        nertc::NERtcAudioRecordingConfiguration config;
        Napi::Object obj = info[0].As<Napi::Object>();
        nertc_recording_option_to_struct(env, obj, config);
        ret = rtc_engine_->startAudioRecordingWithConfig(config);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(stopAudioRecording)
{
    INIT_ENV
    do
    {
        LOG_F(INFO, "stopAudioRecording in");
        ret = rtc_engine_->stopAudioRecording();
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
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
            nertc::NERtcChannelMediaRelayInfo src;
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
    LOG_F(INFO, "ret:%d", ret);
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
			nertc::NERtcChannelMediaRelayInfo src;
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
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(stopChannelMediaRelay)
{
    INIT_ENV
    do
    {
        LOG_F(INFO, "stopChannelMediaRelay in");
        ret = rtc_engine_->stopChannelMediaRelay();
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(setLocalPublishFallbackOption)
{
    INIT_ENV
    do
    {
        int opt;
        napi_get_value_int32(info[0], opt);
        LOG_F(INFO, "opt:%d", opt);
        ret = rtc_engine_->setLocalPublishFallbackOption((nertc::NERtcStreamFallbackOption)opt);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(setRemoteSubscribeFallbackOption)
{
    INIT_ENV
    do
    {
        int opt;
        napi_get_value_int32(info[0], opt);
        LOG_F(INFO, "opt:%d", opt);
        ret = rtc_engine_->setRemoteSubscribeFallbackOption((nertc::NERtcStreamFallbackOption)opt);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(enableSuperResolution)
{
    INIT_ENV
    do
    {
        bool enable;
        napi_get_value_bool(info[0], enable);
        LOG_F(INFO, "enable:%d", enable);
        ret = rtc_engine_->enableSuperResolution(enable);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
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
        if (nertc::kNERtcGMCryptoSM4ECB == config.mode) {
            if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"key"))))
            {
                out = obj.Get(static_cast<napi_value>(Napi::String::New(env,"key"))).As<Napi::String>().Utf8Value();
                memset(config.key, 0, kNERtcEncryptByteLength * sizeof(char));
                memcpy(config.key, out.c_str(), out.size() * sizeof(char));
            }
            ret = rtc_engine_->enableEncryption(enable, config);
        } else if (nertc::NERtcEncryptionModeCustom == config.mode) {
            config.observer = enable ? _packet_observer.get() : nullptr;
            ret = rtc_engine_->enableEncryption(enable, config);
        }
        
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(startLastmileProbeTest)
{
    INIT_ENV
    do
    {
        Napi::Object obj = info[0].As<Napi::Object>();
        nertc::NERtcLastmileProbeConfig config;
        nertc_lastmile_probe_obj_to_struct(env, obj, config);
        ret = rtc_engine_->startLastmileProbeTest(config);
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(stopLastmileProbeTest)
{
    INIT_ENV
    do
    {
        ret = rtc_engine_->stopLastmileProbeTest();
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(getConnectionState)
{
    INIT_ENV
    do
    {
        LOG_F(INFO, "getConnectionState in");
        ret = rtc_engine_->getConnectionState();
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(muteLocalAudioStream)
{
    INIT_ENV
    do
    {
        bool enabled;
        napi_get_value_bool(info[0], enabled);
        LOG_F(INFO, "enabled:%d", enabled);
        ret = rtc_engine_->muteLocalAudioStream(enabled);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
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
        LOG_F(INFO, "profile:%d scenario:%d", profile, scenario);
        ret = rtc_engine_->setAudioProfile((nertc::NERtcAudioProfileType)profile, (nertc::NERtcAudioScenarioType)scenario);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
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
        LOG_F(INFO, "uid:%llu enable:%d", uid, enable);
        ret = rtc_engine_->subscribeRemoteAudioStream(uid, enable);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
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
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(setVideoConfigWithType)
{
    INIT_ENV
    do
    {
        int type;
        napi_get_value_int32(info[0], type);
        auto status = napi_ok;
        nertc::NERtcVideoConfig config = {};
        status = nertc_video_config_obj_to_struct(env, info[1].As<Napi::Object>(), config);
        if (status != napi_ok)
        {
            break;
        }
        ret = rtc_engine_->setVideoConfig((nertc::NERtcVideoStreamType)type, config);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(enableDualStreamMode)
{
    INIT_ENV
    do{
        bool enabled;
        napi_get_value_bool(info[0], enabled);
        LOG_F(INFO, "enabled:%d", enabled);
        ret = rtc_engine_->enableDualStreamMode(enabled);
    }while(false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(setLocalVideoMirrorMode)
{
    INIT_ENV
    do{
        uint32_t mode;
        napi_get_value_uint32(info[0], mode);
        LOG_F(INFO, "mode:%d", mode);
        uint64_t thisAddr = (uint64_t)this;
        NodeVideoFrameTransporter * pTransporter = g_transporter_map[thisAddr];
		if (pTransporter)
        {
            pTransporter->setLocalVideoMirrorMode(mode);
            ret = 0;
        }
    }while(false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(setLocalVideoMirrorModeWithType)
{
    INIT_ENV
    do{
        uint32_t type;
        uint32_t mode;
        napi_get_value_uint32(info[0], type);
        napi_get_value_uint32(info[1], mode);
        LOG_F(INFO, "type:%d mode:%d", type, mode);
        uint64_t thisAddr = (uint64_t)this;
        NodeVideoFrameTransporter * pTransporter = g_transporter_map[thisAddr];
		if (pTransporter)
        {
            pTransporter->setLocalVideoMirrorModeWithType((nertc::NERtcVideoStreamType)type, mode);
            ret = 0;
        }
    }while(false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(startVideoPreview)
{
    INIT_ENV
    do{
        LOG_F(INFO, "startVideoPreview in");
		// NodeVideoFrameTransporter *pTransporter = getNodeVideoFrameTransporter();
		// pTransporter->startFlushVideo();
        ret = rtc_engine_->startVideoPreview();
    }while(false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(stopVideoPreview)
{
    INIT_ENV
    do
    {
        LOG_F(INFO, "stopVideoPreview in");
        ret = rtc_engine_->stopVideoPreview();
		// NodeVideoFrameTransporter *pTransporter = getNodeVideoFrameTransporter();
		// pTransporter->stopFlushVideo();
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(muteLocalVideoStream)
{
    INIT_ENV
    do
    {
        bool enabled;
        napi_get_value_bool(info[0], enabled);
        LOG_F(INFO, "enabled:%d", enabled);
        ret = rtc_engine_->muteLocalVideoStream(enabled);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
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
        LOG_F(INFO, "para:%s", para.c_str());
        ret = rtc_engine_->setParameters(para.c_str());
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(getParameters)
{
    Napi::Env env = info.Env();
    std::string ret = "";
    do
    {
        std::string para;
        napi_get_value_utf8_string(info[0], para);
        if (para.length() == 0)
        {
            break;
        }
        std::string extra_info;
        napi_get_value_utf8_string(info[1], extra_info);
        if (extra_info.length() == 0)
        {
            break;
        }
        LOG_F(INFO, "para:%s, extra_info:%s", para.c_str(), extra_info.c_str());
        ret  = rtc_engine_->getParameters(para.c_str(), extra_info.c_str());
    } while (false);
    LOG_F(INFO, "ret:%s", ret.c_str());
    return Napi::String::New(env, ret);
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
    LOG_F(INFO, "ret:%d", ret);
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
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF( startAudioDump)
{
    INIT_ENV
    do
    {
        LOG_F(INFO, "startAudioDump in");
        ret = rtc_engine_->startAudioDump();
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(stopAudioDump)
{
    INIT_ENV
    do
    {
        LOG_F(INFO, "stopAudioDump in");
        ret = rtc_engine_->stopAudioDump();
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(startAudioMixing)
{
    INIT_ENV
    do
    {
        nertc::NERtcCreateAudioMixingOption config;
        nertc_audio_mixing_option_obj_to_struct(env, info[0].As<Napi::Object>(), config);
        ret = rtc_engine_->startAudioMixing(&config);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(stopAudioMixing)
{
    INIT_ENV
    do
    {
        LOG_F(INFO, "stopAudioMixing in");
        ret = rtc_engine_->stopAudioMixing();
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(pauseAudioMixing)
{
    INIT_ENV
    do
    {
        LOG_F(INFO, "pauseAudioMixing in");
        ret = rtc_engine_->pauseAudioMixing();
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(resumeAudioMixing)
{
    INIT_ENV
    do
    {
        LOG_F(INFO, "resumeAudioMixing in");
        ret = rtc_engine_->resumeAudioMixing();
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(setAudioMixingSendVolume)
{
    INIT_ENV
    do
    {
        uint32_t param;
        napi_get_value_uint32(info[0], param);
        LOG_F(INFO, "param:%d", param);
        ret = rtc_engine_->setAudioMixingSendVolume(param);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
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
    LOG_F(INFO, "volume:%d ret:%d", volume, ret);
    return Napi::Number::New(env, ret == 0 ? volume : ret);
}

NIM_SDK_NODE_API_DEF(setAudioMixingPlaybackVolume)
{
    INIT_ENV
    do
    {
        uint32_t param;
        napi_get_value_uint32(info[0], param);
        LOG_F(INFO, "param:%d", param);
        ret = rtc_engine_->setAudioMixingPlaybackVolume(param);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(getAudioMixingPlaybackVolume)
{
    INIT_ENV
    uint32_t volume = 0;
    do
    {
        LOG_F(INFO, "getAudioMixingPlaybackVolume in");
        ret = rtc_engine_->getAudioMixingPlaybackVolume(&volume);
    } while (false);
    LOG_F(INFO, "volume:%d ret:%d", volume, ret);
    return Napi::Number::New(env, ret == 0 ? volume : ret);
}

NIM_SDK_NODE_API_DEF(getAudioMixingDuration)
{
    INIT_ENV
    uint64_t dur = 0;
    do
    {
        LOG_F(INFO, "getAudioMixingDuration in");
        ret = rtc_engine_->getAudioMixingDuration(&dur);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
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
    LOG_F(INFO, "volume:%llu ret:%d", volume, ret);
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
        LOG_F(INFO, "param:%d ret:%d", param, ret);
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(setAudioMixingPitch)
{
    INIT_ENV
    do
    {
        uint32_t param;
        napi_get_value_uint32(info[0], param);
        LOG_F(INFO, "param:%d", param);
        ret = rtc_engine_->setAudioMixingPitch(param);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(getAudioMixingPitch)
{
    INIT_ENV
    int32_t pitch = 0;
    do
    {
        ret = rtc_engine_->getAudioMixingPitch(&pitch);
    } while (false);
    LOG_F(INFO, "ret:%d pitch:%d", ret, pitch);
    return Napi::Number::New(env, ret == 0 ? pitch : ret);
}

NIM_SDK_NODE_API_DEF(playEffect)
{
    INIT_ENV
    do
    {
        uint32_t effect_id;
        napi_get_value_uint32(info[0], effect_id);
        LOG_F(INFO, "effect_id:%llu", effect_id);
        Napi::Array objs = info[1].As<Napi::Array>();
        nertc::NERtcCreateAudioEffectOption *config = new nertc::NERtcCreateAudioEffectOption[objs.Length()];
        nertc_audio_effect_option_obj_to_struct(env, info[1].As<Napi::Object>(), config);
        ret = rtc_engine_->playEffect(effect_id, config);
        delete[] config;
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(stopEffect)
{
    INIT_ENV
    do
    {
        uint32_t effect_id;
        napi_get_value_uint32(info[0], effect_id);
        LOG_F(INFO, "effect_id:%llu", effect_id);
        ret = rtc_engine_->stopEffect(effect_id);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(setEffectPitch)
{
    INIT_ENV
    do
    {
        uint32_t effect_id;
        napi_get_value_uint32(info[0], effect_id);
        int32_t pitch;
        napi_get_value_int32(info[1], pitch);
        LOG_F(INFO, "effect_id:%d pitch:%d", effect_id, pitch);
        ret = rtc_engine_->setEffectPitch(effect_id, pitch);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(getEffectPitch)
{
    INIT_ENV
    int32_t pitch = 0;
    do
    {
        uint32_t effect_id;
        napi_get_value_uint32(info[0], effect_id);
        LOG_F(INFO, "effect_id:%d", effect_id);
        ret = rtc_engine_->getEffectPitch(effect_id, &pitch);
    } while (false);
    LOG_F(INFO, "ret:%d pitch:%d", ret, pitch);
    return Napi::Number::New(env, ret == 0 ? pitch : ret);
}

NIM_SDK_NODE_API_DEF(stopAllEffects)
{
    INIT_ENV
    do
    {
        LOG_F(INFO, "stopAllEffects in");
        ret = rtc_engine_->stopAllEffects();
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(pauseEffect)
{
    INIT_ENV
    do
    {
        uint32_t effect_id;
        napi_get_value_uint32(info[0], effect_id);
		LOG_F(INFO, "effect_id:%llu", effect_id);
        ret = rtc_engine_->pauseEffect(effect_id);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(resumeEffect)
{
    INIT_ENV
    do
    {
        uint32_t effect_id;
        napi_get_value_uint32(info[0], effect_id);
        LOG_F(INFO, "effect_id:%llu", effect_id);
        ret = rtc_engine_->resumeEffect(effect_id);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(pauseAllEffects)
{
    INIT_ENV
    do
    {
        LOG_F(INFO, "pauseAllEffects in");
        ret = rtc_engine_->pauseAllEffects();
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(resumeAllEffects)
{
    INIT_ENV
    do
    {
        LOG_F(INFO, "resumeAllEffects in");
        ret = rtc_engine_->resumeAllEffects();
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
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
        LOG_F(INFO, "effect_id:%llu volume:%d", effect_id, volume);
        ret = rtc_engine_->setEffectSendVolume(effect_id, volume);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
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
        LOG_F(INFO, "effect_id:%llu", effect_id);
        ret = rtc_engine_->getEffectSendVolume(effect_id, &volume);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
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
        LOG_F(INFO, "effect_id:%llu volume:%d", effect_id, volume);
        ret = rtc_engine_->setEffectPlaybackVolume(effect_id, volume);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
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
        LOG_F(INFO, "effect_id:%llu", effect_id);
        ret = rtc_engine_->getEffectPlaybackVolume(effect_id, &vol);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
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
        LOG_F(INFO, "enabled:%d volume:%d", enabled, volume);
        ret = rtc_engine_->enableEarback(enabled, volume);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(setEarbackVolume)
{
    INIT_ENV
    do
    {
        uint32_t volume;
        napi_get_value_uint32(info[0], volume);
        LOG_F(INFO, "volume:%d", volume);
        ret = rtc_engine_->setEarbackVolume(volume);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
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
            _stats_observer->removeEventHandler(eventName);
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


NIM_SDK_NODE_API_DEF(onQsObserver)
{
    INIT_ENV
    do
    {
        if(!is_qosObserver_init_) {
            rtc_engine_->setVideoEncoderQosObserver(_qs_handler.get());
            is_qosObserver_init_ = true;
        }
        
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
            auto sz = _qs_handler->removeEventHandler(eventName);
            if (sz == 0)
            {
                rtc_engine_->setVideoEncoderQosObserver(nullptr);
            }
        }
        else
        {
            Napi::FunctionReference function;
            napi_get_value_function(info[2], function);
            _qs_handler->addEvent(eventName, std::move(function));
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
        LOG_F(INFO, "enabled:%d interval:%d", enabled, interval);
        ret = rtc_engine_->enableAudioVolumeIndication(enabled, interval);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(enableAudioVolumeIndicationEx)
{
    INIT_ENV
    do
    {
        bool enabled;
        uint32_t interval;
        bool enableVad;
        napi_get_value_bool(info[0], enabled);
        napi_get_value_uint32(info[1], interval);
        napi_get_value_bool(info[2], enableVad);
        LOG_F(INFO, "enabled:%d interval:%d enableVad:%d", enabled, interval, enableVad);
        ret = rtc_engine_->enableAudioVolumeIndication(enabled, interval, enableVad);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

#define NODE_SET_OBJ_PROP_DATA(obj, name, it) \
    { \
        Napi::ArrayBuffer arrayBuffer1 = Napi::ArrayBuffer::New(env, (it)->length);  \
        memcpy(arrayBuffer1.Data(), (it)->buffer, (it)->length); \
        Napi::Uint8Array buff = Napi::TypedArrayOf<uint8_t>::New(env, arrayBuffer1.ByteLength(), arrayBuffer1, 0, napi_uint8_array);\
        obj.Set(Napi::String::New(env, name), buff); \
    }

NIM_SDK_NODE_API_DEF(getScreenCaptureSources)
{
    INIT_ENV
    Napi::Array infos = Napi::Array::New(env);
    do
    {
        nertc::NERtcSize thumbSize = {};
        nertc_screen_size_info_to_struct(env, info[0].As<Napi::Object>(), thumbSize);
        nertc::NERtcSize iconSize = {};
        nertc_screen_size_info_to_struct(env, info[1].As<Napi::Object>(), iconSize);
        bool includeScreen = info[2].As<Napi::Boolean>().Value();
        int count = 0;
        auto source_list = rtc_engine_->getScreenCaptureSources(thumbSize, iconSize, includeScreen);
        if (source_list) {
            count = source_list->getCount();
        }

        for (int index = 0; index < count; index++) {

            nertc::NERtcScreenCaptureSourceInfo info = source_list->getSourceInfo(index);
            
            Napi::Object obj = Napi::Object::New(env);
            obj.Set(Napi::String::New(env, "type"), Napi::Number::New(env, info.type));
            
            int64_t source_id_ = reinterpret_cast<int64_t>(info.source_id);
            obj.Set(Napi::String::New(env, "source_id"), Napi::Number::New(env, source_id_));
            
            std::string source_name_ = info.source_name;
            obj.Set(Napi::String::New(env, "source_name"), Napi::String::New(env, source_name_));

            { //thumb_image
                Napi::Object thumb = Napi::Object::New(env);
                thumb.Set(Napi::String::New(env, "length"), Napi::Number::New(env, info.thumb_image.length));
                thumb.Set(Napi::String::New(env, "width"), Napi::Number::New(env, info.thumb_image.width));
                thumb.Set(Napi::String::New(env, "height"), Napi::Number::New(env, info.thumb_image.height));
                uint8_t *data = ARGBToBGRA(const_cast<char*>(info.thumb_image.buffer), info.thumb_image.length);
                Napi::ArrayBuffer buff = Napi::ArrayBuffer::New(env, info.thumb_image.length);
                memcpy(buff.Data(), data, info.thumb_image.length);
                Napi::Uint8Array dataarray = Napi::TypedArrayOf<uint8_t>::New(env, buff.ByteLength(), buff, 0, napi_uint8_array);
                thumb.Set(Napi::String::New(env, "buffer"), dataarray);
                obj.Set(Napi::String::New(env, "thumb_image"), thumb);
            }

            {  //icon_image
                Napi::Object icon = Napi::Object::New(env);
                icon.Set(Napi::String::New(env, "length"), Napi::Number::New(env, info.icon_image.length));
                icon.Set(Napi::String::New(env, "width"), Napi::Number::New(env, info.icon_image.width));
                icon.Set(Napi::String::New(env, "height"), Napi::Number::New(env, info.icon_image.height));
                uint8_t *data1 = ARGBToBGRA(const_cast<char*>(info.icon_image.buffer), info.icon_image.length);
                Napi::ArrayBuffer buff1 = Napi::ArrayBuffer::New(env, info.icon_image.length);
                memcpy(buff1.Data(), data1, info.icon_image.length);
                Napi::Uint8Array dataarray = Napi::TypedArrayOf<uint8_t>::New(env, buff1.ByteLength(), buff1, 0, napi_uint8_array);
                icon.Set(Napi::String::New(env, "buffer"), dataarray);
                obj.Set(Napi::String::New(env, "icon_image"), icon);
            }

            std::string process_path_ = info.process_path;
            obj.Set(Napi::String::New(env, "process_path"), Napi::String::New(env, process_path_));

            std::string source_title_ = info.source_title;
            obj.Set(Napi::String::New(env, "source_title"), Napi::String::New(env, source_title_));
            
            obj.Set(Napi::String::New(env, "primary_monitor"), Napi::Boolean::New(env, info.primaryMonitor));

            infos.Set(static_cast<napi_value>(Napi::Number::New(env, index)),  obj);
        }

        if (source_list) {
            source_list->release();
        }
       
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return infos;
}

NIM_SDK_NODE_API_DEF(setScreenCaptureSource)
{
    INIT_ENV
    do
    {
        int32_t out_i = 0;
        int64_t out_64 = 0;
        nertc::NERtcScreenCaptureSourceInfo source = {};
        Napi::Object obj = info[0].As<Napi::Object>();
        if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"type"))))
        {
            out_i = obj.Get(static_cast<napi_value>(Napi::String::New(env,"type"))).As<Napi::Number>().Int32Value();
            source.type = (nertc::NERtcScreenCaptureSourceType)out_i;
        }
        if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"source_id"))))
        {
            out_64 = obj.Get(static_cast<napi_value>(Napi::String::New(env,"source_id"))).As<Napi::Number>().Int64Value();
            source.source_id = (nertc::source_id_t)out_64;
        }
       

        nertc::NERtcRectangle region_rect = {};
        nertc::NERtcScreenCaptureParameters capture_params = {};
        nertc_rectangle_obj_to_struct(env, info[1].As<Napi::Object>(), region_rect);
        std::set<int64_t> vsWindowId;
        nertc_screen_capture_params_obj_to_struct(env, info[2].As<Napi::Object>(), capture_params, vsWindowId);
        intptr_t* wnd_list = nullptr;
        int index = 0;
        if (!vsWindowId.empty()) {
            wnd_list = new intptr_t[vsWindowId.size()];
            for (auto e : vsWindowId) {
                *(wnd_list + index++) = e;
            }
        }
        capture_params.excluded_window_list = (nertc::source_id_t*)wnd_list;
        ret = rtc_engine_->setScreenCaptureSource(source, region_rect, capture_params);
        if (capture_params.excluded_window_list != nullptr)
        {
            delete[] capture_params.excluded_window_list;
            capture_params.excluded_window_list = nullptr;
        }
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
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
    LOG_F(INFO, "ret:%d", ret);
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
        ret = rtc_engine_->startScreenCaptureByWindowId(reinterpret_cast<void *>(windowid), region_rect, param);
#endif
        if (param.excluded_window_list != nullptr)
        {
            delete[] param.excluded_window_list;
            param.excluded_window_list = nullptr;
        }
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
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
    LOG_F(INFO, "ret:%d", ret);
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
            // RECT rc = _windows_helper->getCachedRect(display);
            // if (rc.bottom == 0 && rc.left == 0 && rc.right == 0 && rc.top == 0)
            // {
            //     WindowsHelpers::CaptureTargetInfoList list;
            //     _windows_helper->getCaptureWindowList(&list, 1);
            //     for (auto w : list)
            //     {
            //         if (std::to_string(display) == w.display_id)
            //         {
            //             rc = w.rc;
            //             _windows_helper->updateCachedInfos(display, rc);
            //             break;
            //         }
            //     }
            // }
            // if (rc.bottom != 0 || rc.left != 0 || rc.right != 0 || rc.top != 0)
            // {
            //     nertc::NERtcRectangle screen_rect = {};
            //     screen_rect.x = rc.left;
            //     screen_rect.y = rc.top;
            //     screen_rect.width = rc.right - rc.left;
            //     screen_rect.height = rc.bottom - rc.top;
            //     ret = rtc_engine_->startScreenCaptureByScreenRect(screen_rect, region_rect, param);
            // }
            // else
            // {
            //     ret = -100;
            // }
            ret = rtc_engine_->startScreenCaptureByDisplayId(reinterpret_cast<void *>(display), region_rect, param);
#else
            ret = rtc_engine_->startScreenCaptureByDisplayId(reinterpret_cast<void *>(display), region_rect, param);
#endif
            if (param.excluded_window_list != nullptr)
            {
                delete[] param.excluded_window_list;
                param.excluded_window_list = nullptr;
            }

    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}


NIM_SDK_NODE_API_DEF(stopScreenCapture)
{
    INIT_ENV
    do
    {
        ret = rtc_engine_->stopScreenCapture();
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(pauseScreenCapture)
{
    INIT_ENV
    do
    {
        ret = rtc_engine_->pauseScreenCapture();
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(resumeScreenCapture)
{
    INIT_ENV
    do
    {
        ret = rtc_engine_->resumeScreenCapture();
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

// NIM_SDK_NODE_API_DEF(setExternalVideoSource)
// {
//     INIT_ENV
//     do
//     {
//         bool enabled;
//         napi_get_value_bool(info[0], enabled);
//         LOG_F(INFO, "enabled:%d", enabled);
//         ret = rtc_engine_->setExternalVideoSource(enabled);
//     } while (false);
//     LOG_F(INFO, "ret:%d", ret);
//     return Napi::Number::New(env, ret);
// }

NIM_SDK_NODE_API_DEF(getVersion)
{
    Napi::Env env = info.Env();
    std::string ret;
    do
    {
        int32_t build;
        ret = (std::string)rtc_engine_->getVersion(&build);
        LOG_F(INFO, "build:%d, ret:%s", build, ret.c_str());
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
        LOG_F(INFO, "error:%d", error);
        ret = (std::string)rtc_engine_->getErrorDescription(error);
        LOG_F(INFO, "ret:%s", ret.c_str());
    } while (false);
    return Napi::String::New(env, ret);
}

NIM_SDK_NODE_API_DEF(uploadSdkInfo)
{
    INIT_ENV
    do
    {
        LOG_F(INFO, "uploadSdkInfo in");
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
        nertc::NERtcLiveStreamTaskInfo info;
        nertc_ls_task_info_obj_to_struct(env, obj, info);
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
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}


NIM_SDK_NODE_API_DEF(updateLiveStreamTask)
{
    INIT_ENV
    do
    {
        Napi::Object obj = info[0].As<Napi::Object>();
        nertc::NERtcLiveStreamTaskInfo info;

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
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(removeLiveStreamTask)
{
    INIT_ENV
    do
    {
        std::string task_id;
        napi_get_value_utf8_string(info[0], task_id);
        LOG_F(INFO, "task_id:%s", task_id.c_str());
        ret = rtc_engine_->removeLiveStreamTask(task_id.c_str());
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
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
        LOG_F(INFO, "device:%s", device.c_str());
        ret = _adm->setRecordDevice(device.c_str());
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
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
    LOG_F(INFO, "ret:%d", ret);
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
    LOG_F(INFO, "ret:%d", ret);
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
        LOG_F(INFO, "volume:%d", volume);
        ret = _adm->setRecordDeviceVolume(volume);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
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
    LOG_F(INFO, "volume:%d ret:%d", volume, ret);
    return Napi::Number::New(env, ret == 0 ? volume : ret);
}

NIM_SDK_NODE_API_DEF(setPlayoutDeviceVolume)
{
    INIT_ENV
    do
    {
        uint32_t volume;
        napi_get_value_uint32(info[0], volume);
        LOG_F(INFO, "volume:%d", volume);
        ret = _adm->setPlayoutDeviceVolume(volume);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
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
    LOG_F(INFO, "volume:%d ret:%d", volume, ret);
    return Napi::Number::New(env, ret == 0 ? volume : ret);
}


NIM_SDK_NODE_API_DEF(setPlayoutDeviceMute)
{
    INIT_ENV
    do
    {
        bool mute;
        napi_get_value_bool(info[0], mute);
        LOG_F(INFO, "mute:%d", mute);
        ret = _adm->setPlayoutDeviceMute(mute);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(getPlayoutDeviceMute)
{
    INIT_ENV_BOOL
    do
    {
        _adm->getPlayoutDeviceMute(&ret);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Boolean::New(env, ret);
}

NIM_SDK_NODE_API_DEF(setRecordDeviceMute)
{
    INIT_ENV
    do
    {
        bool mute;
        napi_get_value_bool(info[0], mute);
        LOG_F(INFO, "mute:%d", mute);
        ret = _adm->setRecordDeviceMute(mute);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number ::New(env, ret);
}

NIM_SDK_NODE_API_DEF(getRecordDeviceMute)
{
    INIT_ENV_BOOL
    do
    {
        _adm->getRecordDeviceMute(&ret);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Boolean::New(env, ret);
}

NIM_SDK_NODE_API_DEF(adjustRecordingSignalVolume)
{
    INIT_ENV
    do
    {
        uint32_t volume;
        napi_get_value_uint32(info[0], volume);
        LOG_F(INFO, "volume:%d", volume);
        ret = _adm->adjustRecordingSignalVolume(volume);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number ::New(env, ret);
}

NIM_SDK_NODE_API_DEF(adjustPlaybackSignalVolume)
{
    INIT_ENV
    do
    {
        uint32_t volume;
        napi_get_value_uint32(info[0], volume);
        LOG_F(INFO, "volume:%d", volume);
        ret = _adm->adjustPlaybackSignalVolume(volume);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number ::New(env, ret);
}


NIM_SDK_NODE_API_DEF(startRecordDeviceTest)
{
    INIT_ENV
    do
    { 
        uint32_t interval; // todo uint64
        napi_get_value_uint32(info[0], interval);
        LOG_F(INFO, "interval:%d", interval);
        ret = _adm->startRecordDeviceTest(interval);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number ::New(env, ret);
}

NIM_SDK_NODE_API_DEF(stopRecordDeviceTest)
{
    INIT_ENV
    do
    {
        ret = _adm->stopRecordDeviceTest();
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
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
        LOG_F(INFO, "path:%s", path.c_str());
        ret = _adm->startPlayoutDeviceTest(path.c_str());
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number ::New(env, ret);
}

NIM_SDK_NODE_API_DEF(stopPlayoutDeviceTest)
{
    INIT_ENV
    do
    {
        ret = _adm->stopPlayoutDeviceTest();
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number ::New(env, ret);
}

NIM_SDK_NODE_API_DEF(startAudioDeviceLoopbackTest)
{
    INIT_ENV
    do
    {
        uint32_t interval;  // todo uint64
        napi_get_value_uint32(info[0], interval);
        LOG_F(INFO, "interval:%d", interval);
        ret = _adm->startAudioDeviceLoopbackTest(interval);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number ::New(env, ret);
}

NIM_SDK_NODE_API_DEF(stopAudioDeviceLoopbackTest)
{
    INIT_ENV
    do
    {
        ret = _adm->stopAudioDeviceLoopbackTest();
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
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
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number ::New(env, ret);
}

NIM_SDK_NODE_API_DEF(setDeviceWithType)
{
    INIT_ENV
    do
    {
        std::string device;
        int type;
        napi_get_value_utf8_string(info[0], device);
        napi_get_value_int32(info[1], type);
        if (device.length() == 0)
        {
            break;
        }
        ret = _vdm->setDevice(device.c_str(), (nertc::NERtcVideoStreamType)type);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
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
    LOG_F(INFO, "ret:%d", ret);
    return Napi::String ::New(env,  ret== 0 ? (char*)id : "");
}

NIM_SDK_NODE_API_DEF(getDeviceWithType)
{
    INIT_ENV
    int type;
    napi_get_value_int32(info[0], type);
    char id[256];
    do
    {
        ret = _vdm->getDevice(id, (nertc::NERtcVideoStreamType)type);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
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
                    Napi::ArrayBuffer buff = Napi::ArrayBuffer::New(env, size);
                    memcpy(buff.Data(), data, size);
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
                    Napi::ArrayBuffer buff = Napi::ArrayBuffer::New(env, size);
                    memcpy(buff.Data(), data, size);
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
                        Napi::ArrayBuffer buff = Napi::ArrayBuffer::New(env, iconSize);
                        memcpy(buff.Data(), data, iconSize);
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
                Napi::ArrayBuffer buff = Napi::ArrayBuffer::New(env, iconSize);
                memcpy(buff.Data(), data, iconSize);
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

NIM_SDK_NODE_API_DEF(enableLocalSubStreamAudio)
{
    INIT_ENV
    do
    {
        bool enabled = false;
        napi_get_value_bool(info[0], enabled);
        LOG_F(INFO, "enabled:%d", enabled);
        ret = rtc_engine_->enableLocalSubStreamAudio(enabled);
        LOG_F(INFO, "ret:%d", ret);
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(muteLocalSubStreamAudio)
{
    INIT_ENV
    do
    {
        bool mute;
        napi_get_value_bool(info[0], mute);
        LOG_F(INFO, "mute:%d", mute);
        ret = rtc_engine_->muteLocalSubStreamAudio(mute);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(subscribeRemoteSubStreamAudio)
{
    INIT_ENV
    do
    {
        uint32_t uid;
        bool subscribe;
        napi_get_value_uint32(info[0], uid);
        napi_get_value_bool(info[1], subscribe);
        LOG_F(INFO, "uid:%llu subscribe:%d", uid, subscribe);
        ret = rtc_engine_->subscribeRemoteSubStreamAudio(uid, subscribe);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(subscribeAllRemoteAudioStream)
{
    INIT_ENV
    do
    {
        bool subscribe;
        napi_get_value_bool(info[0], subscribe);
        LOG_F(INFO, "enable:%d", subscribe);
        ret = rtc_engine_->subscribeAllRemoteAudioStream(subscribe);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(setAudioSubscribeOnlyBy)
{
    INIT_ENV
    do
    {
        std::set<uint64_t> set_uids;
        Napi::Object obj = info[0].As<Napi::Object>();
        nertc_uid_list_to_struct(env, obj, set_uids);
        nertc::uid_t* uid_array = NULL;
        int index = 0;
        if (!set_uids.empty()) {
            uid_array = new nertc::uid_t[set_uids.size()];
            for (auto e : set_uids) {
                *(uid_array + index++) = e;
            }
        }
        int size;
        napi_get_value_int32(info[1], size);
        LOG_F(INFO, "size:%d", size);
        ret = rtc_engine_->setAudioSubscribeOnlyBy(uid_array, size);
        if (rtc_engine_) {
          delete[] uid_array;
        }
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(setSubscribeAudioAllowlist)
{
    INIT_ENV
    do
    {
        std::set<uint64_t> set_uids;
        Napi::Object obj = info[0].As<Napi::Object>();
        nertc_uid_list_to_struct(env, obj, set_uids);
        nertc::uid_t* uid_array = NULL;
        int index = 0;
        if (!set_uids.empty()) {
            uid_array = new nertc::uid_t[set_uids.size()];
            for (auto e : set_uids) {
                *(uid_array + index++) = e;
            }
        }
        int size;
        napi_get_value_int32(info[1], size);
        LOG_F(INFO, "size:%d", size);
        ret = rtc_engine_->setSubscribeAudioAllowlist(uid_array, size);
        if (uid_array) {
          delete[] uid_array;
        }
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(setSubscribeAudioBlocklist)
{
    INIT_ENV
    do
    {
        int type;
        napi_get_value_int32(info[0], type);
        std::set<uint64_t> set_uids;
        Napi::Object obj = info[1].As<Napi::Object>();
        nertc_uid_list_to_struct(env, obj, set_uids);
        nertc::uid_t* uid_array = NULL;
        int index = 0;
        if (!set_uids.empty()) {
            uid_array = new nertc::uid_t[set_uids.size()];
            for (auto e : set_uids) {
                *(uid_array + index++) = e;
            }
        }
        int size;
        napi_get_value_int32(info[2], size);
        LOG_F(INFO, "size:%d", size);
        ret = rtc_engine_->setSubscribeAudioBlocklist((nertc::NERtcAudioStreamType)type, uid_array, size);
        if (uid_array) {
          delete[] uid_array;
        }
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(setStreamAlignmentProperty)
{
    INIT_ENV
    do
    {
        bool enable;
        napi_get_value_bool(info[0], enable);
        LOG_F(INFO, "enable:%d", enable);
        rtc_engine_->setStreamAlignmentProperty(enable);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(getNtpTimeOffset)
{
    INIT_ENV
    do
    {
        ret = rtc_engine_->getNtpTimeOffset();
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(setCameraCaptureConfig)
{
    INIT_ENV
    Napi::Object obj = Napi::Object::New(env);
    do
    {
        LOG_F(INFO, "setCameraCaptureConfig in");
        Napi::Object obj = info[0].As<Napi::Object>();
        nertc::NERtcCameraCaptureConfig config;
        nertc_camera_capture_obj_to_struct(env,obj,config);
        ret = rtc_engine_->setCameraCaptureConfig(config);

    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);;
}

NIM_SDK_NODE_API_DEF(setCameraCaptureConfigWithType)
{
    INIT_ENV
    Napi::Object obj = Napi::Object::New(env);
    do
    {
        LOG_F(INFO, "setCameraCaptureConfig in");
        int type;
        napi_get_value_int32(info[0], type);
        Napi::Object obj = info[1].As<Napi::Object>();
        nertc::NERtcCameraCaptureConfig config;
        nertc_camera_capture_obj_to_struct(env,obj,config);
        ret = rtc_engine_->setCameraCaptureConfig((nertc::NERtcVideoStreamType)type, config);

    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);;
}


NIM_SDK_NODE_API_DEF(startVideoPreviewWithType)
{
    INIT_ENV
    do
    {
        int type;
        napi_get_value_int32(info[0], type);
        ret = rtc_engine_->startVideoPreview((nertc::NERtcVideoStreamType)type);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(stopVideoPreviewWithType)
{
    INIT_ENV
    do
    {
        int type;
        napi_get_value_int32(info[0], type);
        ret = rtc_engine_->stopVideoPreview((nertc::NERtcVideoStreamType)type);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(muteLocalVideoStreamWithType)
{
    INIT_ENV
    do
    {
        uint32_t type;
        bool enabled = false;
        napi_get_value_uint32(info[0], type);
        napi_get_value_bool(info[1], enabled);
        LOG_F(INFO, "enabled:%d", enabled);
        ret = rtc_engine_->muteLocalVideoStream((nertc::NERtcVideoStreamType)type, enabled);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(startAudioDumpWithType)
{
    INIT_ENV
    do
    {
        uint32_t type;
        napi_get_value_uint32(info[0], type);
        LOG_F(INFO, "type:%d", type);
        ret = rtc_engine_->startAudioDump((nertc::NERtcAudioDumpType)type);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(setScreenCaptureMouseCursor)
{
    INIT_ENV
    do
    {
        bool capture_cursor = false;
        napi_get_value_bool(info[0], capture_cursor);
        LOG_F(INFO, "capture_cursor:%d", capture_cursor);
        ret = rtc_engine_->setScreenCaptureMouseCursor(capture_cursor);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(updateScreenCaptureParameters)
{
    Napi::Env env = info.Env();
    int ret = -1;
    do
    {
        nertc::NERtcScreenCaptureParameters param = {};
        std::set<int64_t> vsWindowId;
        nertc_screen_capture_params_obj_to_struct(env, info[0].As<Napi::Object>(), param, vsWindowId);
        intptr_t* wnd_list = nullptr;
        int index = 0;
        if (!vsWindowId.empty()) {
            wnd_list = new intptr_t[vsWindowId.size()];
            for (auto e : vsWindowId) {
                *(wnd_list + index++) = e;
            }
        }
        param.excluded_window_list = (nertc::source_id_t*)wnd_list;
        ret = rtc_engine_->updateScreenCaptureParameters(param);
        if (param.excluded_window_list != nullptr)
        {
            delete[] param.excluded_window_list;
            param.excluded_window_list = nullptr;
        }
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

// NIM_SDK_NODE_API_DEF(setExternalVideoSourceEx)
// {
//     INIT_ENV
//     do
//     {
//         int type = 0;
//         bool enabled;
//         napi_get_value_int32(info[0], type);
//         napi_get_value_bool(info[1], enabled);
//         LOG_F(INFO, "type:%d enabled:%d", type, enabled);
//         ret = rtc_engine_->setExternalVideoSource((nertc::NERtcVideoStreamType)type, enabled);
//     } while (false);
//     LOG_F(INFO, "ret:%d", ret);
//     return Napi::Number::New(env, ret);
// }

NIM_SDK_NODE_API_DEF(setRemoteHighPriorityAudioStream)
{
    INIT_ENV
    do
    {
        bool enabled;
        napi_get_value_bool(info[0], enabled);
        int64_t uid = 0;
        napi_get_value_int64(info[1], uid);
        LOG_F(INFO, "enabled:%d uid:%d", enabled, uid);
        ret = rtc_engine_->setRemoteHighPriorityAudioStream(enabled, uid);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(checkNECastAudioDriver)
{
    INIT_ENV
    do
    {
        ret = rtc_engine_->checkNECastAudioDriver();
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

// NIM_SDK_NODE_API_DEF(enableVirtualBackground)
// {
//     INIT_ENV
//     do
//     {
//         bool enabled = false;
//         napi_get_value_bool(info[0], enabled);
//         LOG_F(INFO, "enabled:%d", enabled);

//         nertc::VirtualBackgroundSource param = {};
//         nertc_virtual_backgroup_source_obj_to_struct(env, info[1].As<Napi::Object>(), param);
//         Napi::Object obj = info[1].As<Napi::Object>();
//         if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"source"))))
//         {
//             std::string strSource = obj.Get(static_cast<napi_value>(Napi::String::New(env,"source"))).As<Napi::String>().Utf8Value();
//             param.source = const_cast<char*>(strSource.c_str());
//         }
//         ret = rtc_engine_->enableVirtualBackground(enabled, param);
//     } while (false);
//     LOG_F(INFO, "ret:%d", ret);
//     return Napi::Number::New(env, ret);
// }

NIM_SDK_NODE_API_DEF(setCloudProxy)
{
    INIT_ENV
    do
    {
        int type = false;
        napi_get_value_int32(info[0], type);
        LOG_F(INFO, "type:%d", type);
        ret = rtc_engine_->setCloudProxy(type);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(startBeauty)
{
    INIT_ENV
    do
    {
#ifdef WIN32
        std::string path;
        napi_get_value_utf8_string(info[0], path);
        if (path.length() == 0)
        {
            break;
        }
        // std::string str = UTF16ToUTF8(path);
        LOG_F(INFO, "path:%s", path);
        ret = rtc_engine_->startBeauty(path.c_str());
#else

#endif
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(stopBeauty)
{
    INIT_ENV
    do
    {
        ret = 0;
        rtc_engine_->stopBeauty();
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(enableBeauty)
{
    INIT_ENV
    do
    {
        bool enable = false;
        napi_get_value_bool(info[0], enable);
        LOG_F(INFO, "enable:%d", enable);
        rtc_engine_->enableBeauty(enable);
        ret = 0;
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

// NIM_SDK_NODE_API_DEF(enableBeautyMirrorMode)
// {
//     INIT_ENV
//     do
//     {
//         bool enable = false;
//         napi_get_value_bool(info[0], enable);
//         LOG_F(INFO, "enable:%d", enable);
//         rtc_engine_->enableBeautyMirrorMode(enable);
//         ret = 0;
//     } while (false);
//     LOG_F(INFO, "ret:%d", ret);
//     return Napi::Number::New(env, ret);
// }

NIM_SDK_NODE_API_DEF(getBeautyEffect)
{
    INIT_ENV
    do
    {
        int type;
        napi_get_value_int32(info[0], type);
        LOG_F(INFO, "type:%d", type);
        float level = rtc_engine_->getBeautyEffect((nertc::NERtcBeautyEffectType)type);
        LOG_F(INFO, "level:%f", level);
        ret = (int)(level * 100);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(setBeautyEffect)
{
    INIT_ENV
    do
    {
        int type;
        napi_get_value_int32(info[0], type);
        int level;
        napi_get_value_int32(info[1], level);
        float flevel = level/100.0;
        LOG_F(INFO, "type:%d level:%f", type, flevel);
        ret = rtc_engine_->setBeautyEffect((nertc::NERtcBeautyEffectType)2, flevel);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(addBeautyFilter)
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
        LOG_F(INFO, "path:%s", path.c_str());
        ret = rtc_engine_->addBeautyFilter(path.c_str());
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(removeBeautyFilter)
{
    INIT_ENV
    do
    {
        ret = rtc_engine_->removeBeautyFilter();
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(setBeautyFilterLevel)
{
    INIT_ENV
    do
    {
        int level;
        napi_get_value_int32(info[0], level);
        LOG_F(INFO, "level:%d", level);
        float flevel = level/100.0;
        ret = rtc_engine_->setBeautyFilterLevel(flevel);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(addBeautySticker)
{
    INIT_ENV
    do
    {
        std::string path;
        napi_get_value_utf8_string(info[0], path);
        LOG_F(INFO, "path:%s", path.c_str());
        ret = rtc_engine_->addBeautySticker(path.c_str());
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(removeBeautySticker)
{
    INIT_ENV
    do
    {
        ret = rtc_engine_->removeBeautySticker();
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(addBeautyMakeup)
{
    INIT_ENV
    do
    {
        std::string path;
        napi_get_value_utf8_string(info[0], path);
        LOG_F(INFO, "path:%s", path.c_str());
        ret = rtc_engine_->addBeautyMakeup(path.c_str());
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(removeBeautyMakeup)
{
    INIT_ENV
    do
    {
        ret = rtc_engine_->removeBeautyMakeup();
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(setLocalVoiceReverbParam)
{
    INIT_ENV
    do
    {
        //todo
        nertc::NERtcReverbParam param;
        nertc_rever_param_obj_to_struct(env, info[0].As<Napi::Object>(), param);
        ret = rtc_engine_->setLocalVoiceReverbParam(param);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(enableMediaPub)
{
    INIT_ENV
    do
    {
        bool enable = false;
        napi_get_value_bool(info[0], enable);
        int type;
        napi_get_value_int32(info[1], type);
        LOG_F(INFO, "enable:%d type:%d", enable, type);
        ret = rtc_engine_->enableMediaPub(enable, (nertc::NERtcMediaPubType)type);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(enableVirtualBackground)
{
    INIT_ENV
    do
    {
        bool enable = false;
        napi_get_value_bool(info[0], enable);
        nertc::VirtualBackgroundSource option;
        nertc_virtual_background_option_to_struct(env, info[1].As<Napi::Object>(), option);
        ret = rtc_engine_->enableVirtualBackground(enable, option);
        if (option.source != nullptr) {
			delete[] option.source;
			option.source = nullptr;
		}
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(isFeatureSupported)
{
    INIT_ENV_BOOL
    do
    {
        uint32_t type = 0;
        napi_get_value_uint32(info[0], type);
        rtc_engine_->isFeatureSupported((nertc::NERtcFeatureType)type, &ret);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Boolean::New(env, ret);
}

NIM_SDK_NODE_API_DEF(enableLocalData)
{
    INIT_ENV
    do
    {
        bool enable = false;
        napi_get_value_bool(info[0], enable);
        ret = rtc_engine_->enableLocalData(enable);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(subscribeRemoteData)
{
    INIT_ENV
    do
    {
        unsigned int uid;
        bool sub = false;
        napi_get_value_uint32(info[0], uid);
        napi_get_value_bool(info[1], sub);
        ret = rtc_engine_->subscribeRemoteData(uid, sub);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(sendData)
{
    INIT_ENV
    do
    {
        if (!info[0].IsArrayBuffer()) {
            Napi::Error::New(env, "retryErrors must be an arraybuffer").ThrowAsJavaScriptException();
        } else {
            Napi::ArrayBuffer arrayBuffer = info[0].As<Napi::ArrayBuffer>();
            void * data = arrayBuffer.Data();
            size_t len = arrayBuffer.ByteLength();
            ret = rtc_engine_->sendData(data, len);
        }
        LOG_F(INFO, "ret:%d", ret);
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(updatePermissionKey)
{
    INIT_ENV
    do
    {
        std::string key;
        napi_get_value_utf8_string(info[0], key);
        LOG_F(INFO, "key:%s", key.c_str());
        ret = rtc_engine_->updatePermissionKey(key.c_str());
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(setEffectPosition)
{
    INIT_ENV
    do
    {
        uint32_t effect_id;
        uint32_t pos;
        napi_get_value_uint32(info[0], effect_id);
        napi_get_value_uint32(info[1], pos);
        LOG_F(INFO, "effect_id:%llu pos:%d", effect_id, pos);
        ret = rtc_engine_->setEffectPosition(effect_id, pos);
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(getEffectCurrentPosition)
{
    INIT_ENV
    uint64_t pos = 0;
    do
    {
        uint32_t effect_id;
        napi_get_value_uint32(info[0], effect_id);
        LOG_F(INFO, "effect_id:%llu", effect_id);
        ret = rtc_engine_->getEffectCurrentPosition(effect_id, &pos);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret == 0 ? pos : ret);
}

NIM_SDK_NODE_API_DEF(getEffectDuration)
{
    INIT_ENV
    uint64_t dur = 0;
    do
    {
        uint32_t effect_id;
        napi_get_value_uint32(info[0], effect_id);
        LOG_F(INFO, "effect_id:%llu", effect_id);
        ret = rtc_engine_->getEffectDuration(effect_id, &dur);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret == 0 ? dur : ret);
}

NIM_SDK_NODE_API_DEF(initSpatializer)
{
    INIT_ENV
    uint64_t dur = 0;
    do
    {
        LOG_F(INFO, "initSpatializer");
        ret = rtc_engine_->initSpatializer();
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(enableSpatializer)
{
    INIT_ENV
    uint64_t dur = 0;
    do
    {
        bool enable;
        bool apply_to_team;
        napi_get_value_bool(info[0], enable);
        napi_get_value_bool(info[1], apply_to_team);
        LOG_F(INFO, "enable:%d, apply_to_team:%d", enable, apply_to_team);
        ret = rtc_engine_->enableSpatializer(enable, apply_to_team);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}


NIM_SDK_NODE_API_DEF(setRangeAudioMode)
{
    INIT_ENV
    do
    {
        int32_t mode;
        napi_get_value_int32(info[0], mode);
        LOG_F(INFO, "mode:%d", mode);
        ret = rtc_engine_->setRangeAudioMode((nertc::NERtcRangeAudioMode)mode);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(setRangeAudioTeamID)
{
    INIT_ENV
    do
    {
        int32_t team_id;
        napi_get_value_int32(info[0], team_id);
        LOG_F(INFO, "team_id:%d", team_id);
        ret = rtc_engine_->setRangeAudioTeamID(team_id);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(setAudioRecvRange)
{
    INIT_ENV
    uint64_t dur = 0;
    do
    {
        int32_t audio_distance, conversational_distance, roll_off;
        napi_get_value_int32(info[0], audio_distance);
        napi_get_value_int32(info[1], conversational_distance);
        napi_get_value_int32(info[2], roll_off);
        LOG_F(INFO, "audible_distance:%d, conversational_distance:%d, roll_off:%d", audio_distance, conversational_distance, roll_off);
        ret = rtc_engine_->setAudioRecvRange(audio_distance, conversational_distance, (nertc::NERtcDistanceRolloffModel)roll_off);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(updateSelfPosition)
{
    INIT_ENV
    uint64_t dur = 0;
    do
    {
        Napi::Object obj = info[0].As<Napi::Object>();
        nertc::NERtcPositionInfo info;
        nertc_spatializer_position_to_struct(env, obj, info);
        ret = rtc_engine_->updateSelfPosition(info);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(enableSpatializerRoomEffects)
{
    INIT_ENV
    uint64_t dur = 0;
    do
    {
        bool enable;
        napi_get_value_bool(info[0], enable);
        LOG_F(INFO, "enable:%d", enable);
        ret = rtc_engine_->enableSpatializerRoomEffects(enable);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}


NIM_SDK_NODE_API_DEF(setSpatializerRoomProperty)
{
    INIT_ENV
    uint64_t dur = 0;
    do
    {
        nertc::NERtcSpatializerRoomProperty config;
        Napi::Object obj = info[0].As<Napi::Object>();
        nertc_spatializer_room_property_to_struct(env, obj, config);
        ret = rtc_engine_->setSpatializerRoomProperty(config);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(setSpatializerRenderMode)
{
    INIT_ENV
    do
    {
        int32_t mode;
        napi_get_value_int32(info[0], mode);
        LOG_F(INFO, "mode:%d", mode);
        ret = rtc_engine_->setSpatializerRenderMode((nertc::NERtcSpatializerRenderMode)mode);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

// NIM_SDK_NODE_API_DEF(setLocalVideoWatermarkConfigs)
// {
//     INIT_ENV
//     do
//     {
//         bool enbale;
//         napi_get_value_bool(info[0], enbale);
//         int32_t type;
//         napi_get_value_int32(info[1], type);
//         LOG_F(INFO, "enbale:%d type:%d", enbale, type);
//         nertc::NERtcVideoWatermarkConfig config;
//         Napi::Object obj = info[2].As<Napi::Object>();
//         nertc_video_water_mark_config_to_struct(env, obj, config);
//         ret = rtc_engine_->setLocalVideoWatermarkConfigs(enbale, (nertc::NERtcVideoStreamType)type, config);
//     } while (false);
//     LOG_F(INFO, "ret:%d", ret);
//     return Napi::Number::New(env, ret);
// }




} //namespace nertc_node