#include "node_version.h"
#include "nertc_node_channel.h"
#include "../shared/log/logging/logging.h"
#include "../shared/sdk_helper/node_api_helper.h"
#include <string>

#define SET_PROTOTYPE(name) InstanceMethod(#name,  &NertcNodeChannel::name)

#define NIM_SDK_NODE_API_DEF(method) \
    Napi::Value NertcNodeChannel::method(const Napi::CallbackInfo& info)

#define INIT_ENV \
    Napi::Env env = info.Env();\
    int ret = -1;

#define INIT_ENV_BOOL \
    Napi::Env env = info.Env();\
    bool ret = false;

#define INIT_ENV_STR \
    Napi::Env env = info.Env();\
    std::string ret = "";

namespace nertc_node
{

/****************************************************************************************************************************/
	std::unordered_map<uint64_t, NodeVideoFrameTransporter *> g_channel_transporter_map;
	void ChannelOnFrameDataCallback(nertc::uid_t uid, void *data, uint32_t type, uint32_t width, uint32_t height,
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
		uint64_t thisAddr = *((nertc::uid_t *)user_data);
		NodeVideoFrameTransporter * pTransporter = g_channel_transporter_map[thisAddr];
		if (pTransporter) {
			pTransporter->deliverFrame_I420(nrt, frame.uid, "", frame, rotate, frame.uid == 0);
		}
	}

	void ChannelOnSubstreamFrameDataCallback(
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
		NodeVideoFrameTransporter * pTransporter = g_channel_transporter_map[thisAddr];
		if (pTransporter)
		{
			pTransporter->deliverFrame_I420(nrt, frame.uid, "", frame, rotate, false);
		}
	}
/****************************************************************************************************************************/


#if NAPI_VERSION < 6
Napi::FunctionReference NertcNodeChannel::constructor;
#endif

Napi::Object NertcNodeChannel::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function func = DefineClass(env, "NertcNodeChannel", {
        SET_PROTOTYPE(getChannelName),
        SET_PROTOTYPE(joinChannel),
        SET_PROTOTYPE(leaveChannel),
        SET_PROTOTYPE(enableLocalAudio),
        SET_PROTOTYPE(enableLocalSubStreamAudio),
        SET_PROTOTYPE(muteLocalAudioStream),
        SET_PROTOTYPE(muteLocalSubStreamAudio),
        SET_PROTOTYPE(enableLocalVideo),
        SET_PROTOTYPE(muteLocalVideoStream),
        SET_PROTOTYPE(setScreenCaptureMouseCursor),
        SET_PROTOTYPE(stopScreenCapture),
        SET_PROTOTYPE(pauseScreenCapture),
        SET_PROTOTYPE(resumeScreenCapture),
        SET_PROTOTYPE(onVideoFrame),
		SET_PROTOTYPE(setupVideoCanvas),
        SET_PROTOTYPE(setClientRole),
        SET_PROTOTYPE(setLocalMediaPriority),
        SET_PROTOTYPE(getConnectionState),
        SET_PROTOTYPE(enableDualStreamMode),
        SET_PROTOTYPE(subscribeRemoteAudioStream),
        SET_PROTOTYPE(subscribeRemoteSubStreamAudio),
        SET_PROTOTYPE(subscribeAllRemoteAudioStream),
        SET_PROTOTYPE(subscribeRemoteVideoStream),
        SET_PROTOTYPE(subscribeRemoteVideoSubStream),
        SET_PROTOTYPE(adjustUserPlaybackSignalVolume),
        SET_PROTOTYPE(stopChannelMediaRelay),
        SET_PROTOTYPE(setLocalPublishFallbackOption),
        SET_PROTOTYPE(setRemoteSubscribeFallbackOption),
        SET_PROTOTYPE(setRemoteHighPriorityAudioStream),
        SET_PROTOTYPE(enableMediaPub)

    });

#if NAPI_VERSION < 6
    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();
#else
    Napi::FunctionReference* constructor = new Napi::FunctionReference();
    *constructor = Napi::Persistent(func);
    env.SetInstanceData(constructor);
#endif
    exports.Set("NertcNodeChannel", func);
    return exports;
}

NertcNodeChannel::NertcNodeChannel(const Napi::CallbackInfo& info)
    : Napi::ObjectWrap<NertcNodeChannel>(info) {
    std::string channelName;
    napi_get_value_utf8_string(info[0], channelName);
    nertc::IRtcEngineEx * rtc_engine_ = NertcNodeEngine::getNertcEngine();
    _channel = rtc_engine_->createChannel(channelName.c_str());
    // _channel_event_handler = std::make_shared<NertcChannelEventHandler>();
    // // _channel->setChannelEventHandler(_channel_event_handler.get());
    // _stats_observer = std::make_shared<NertcChannelRtcMediaStatsHandler>();
    // _channel->setStatsObserver(_stats_observer.get());

    uint64_t thisAddr = (uint64_t)this;
    g_channel_transporter_map[thisAddr] = new NodeVideoFrameTransporter();
}

NertcNodeChannel::~NertcNodeChannel() {
     //todo
};

// NIM_SDK_NODE_API_DEF(onEvent)
// {
//     auto env = info.Env();
//     std::string event_name = "";
//     Napi::FunctionReference function;
//     napi_get_value_utf8_string(info[0], event_name);
//     napi_get_value_function(info[1], function);
  
//     _channel_event_handler->addEvent(event_name, std::move(function));
//     auto ret_value = env.Null();
//     return ret_value;
// }

// NIM_SDK_NODE_API_DEF(onStatsObserver)
// {
//     INIT_ENV
//     do
//     {
//         std::string eventName;
//         bool enable;
//         napi_get_value_utf8_string(info[0], eventName);
//         napi_get_value_bool(info[1], enable);
//         if (eventName.length() == 0)
//         {
//             break;
//         }
//         if (!enable)
//         {
//             auto sz = _stats_observer->removeEventHandler(eventName);
//             if (sz == 0)
//             {
//                 _channel->setStatsObserver(nullptr);
//             }
//         }
//         else
//         {
//             Napi::FunctionReference function;
//             napi_get_value_function(info[2], function);
//             _stats_observer->addEvent(eventName, std::move(function));
//         }
//     } while (false);
// 	return Napi::Number::New(env, ret);
// }

NIM_SDK_NODE_API_DEF(getChannelName)
{
    INIT_ENV_STR
    ret = _channel->getChannelName();
    return Napi::String::New(env, ret);
}

NIM_SDK_NODE_API_DEF(joinChannel)
{
    INIT_ENV
    std::string token;
    napi_get_value_utf8_string(info[0], token);
    ret = _channel->joinChannel(token.c_str());
    LOG_F(INFO, "ret:d%", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(leaveChannel)
{
    INIT_ENV
    ret = _channel->leaveChannel();
    LOG_F(INFO, "ret:d%", ret);
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
        ret = _channel->enableLocalAudio(enabled);
        LOG_F(INFO, "ret:%d", ret);
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(enableLocalSubStreamAudio)
{
    INIT_ENV
    do
    {
        bool enabled = false;
        napi_get_value_bool(info[0], enabled);
        LOG_F(INFO, "enabled:%d", enabled);
        ret = _channel->enableLocalSubStreamAudio(enabled);
        LOG_F(INFO, "ret:%d", ret);
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(muteLocalAudioStream)
{
    INIT_ENV
    do
    {
        bool mute;
        napi_get_value_bool(info[0], mute);
        LOG_F(INFO, "mute:%d", mute);
        ret = _channel->muteLocalAudioStream(mute);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
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
        ret = _channel->muteLocalSubStreamAudio(mute);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
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
        ret = _channel->enableLocalVideo(enabled);
        LOG_F(INFO, "ret:%d", ret);
    } while (false);
    return Napi::Number::New(env, ret);
}

// NIM_SDK_NODE_API_DEF(enableLocalVideo)
// {
//     INIT_ENV
//     do
//     {
//         uint32_t type;
//         bool enabled = false;
//         napi_get_value_uint32(info[0], type);
//         napi_get_value_bool(info[1], enabled);
//         LOG_F(INFO, "enabled:%d", enabled);
//         ret = _channel->enableLocalVideo((nertc::NERtcVideoStreamType)type, enabled);
//         LOG_F(INFO, "ret:%d", ret);
//     } while (false);
//     return Napi::Number::New(env, ret);
// }

NIM_SDK_NODE_API_DEF(muteLocalVideoStream)
{
    INIT_ENV
    do
    {
        bool enabled;
        napi_get_value_bool(info[0], enabled);
        LOG_F(INFO, "enabled:%d", enabled);
        ret = _channel->muteLocalVideoStream(enabled);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(setScreenCaptureMouseCursor)
{
    INIT_ENV
    do
    {
        bool enabled;
        napi_get_value_bool(info[0], enabled);
        LOG_F(INFO, "enabled:%d", enabled);
        ret = _channel->setScreenCaptureMouseCursor(enabled);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(stopScreenCapture)
{
    INIT_ENV
    do
    {
        ret = _channel->stopScreenCapture();
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(pauseScreenCapture)
{
    INIT_ENV
    do
    {
        ret = _channel->pauseScreenCapture();
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(resumeScreenCapture)
{
    INIT_ENV
    do
    {
        ret = _channel->resumeScreenCapture();
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(onVideoFrame)
{
    INIT_ENV
    do{
        Napi::FunctionReference function;
        napi_get_value_function(info[0], function);
        uint64_t thisAddr = (uint64_t)this;
        NodeVideoFrameTransporter * pTransporter = g_channel_transporter_map[thisAddr];
		if (pTransporter)
        {
            ret = pTransporter->initialize(std::move(function));
        }
    }while(false);
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
        LOG_F(INFO, "uid:%llu, enable:%d", uid, enable);
        nertc::NERtcVideoCanvas canvas;
        canvas.cb = enable ? ChannelOnFrameDataCallback : nullptr; //NodeVideoFrameTransporter::onFrameDataCallback;
        uint64_t thisAddr = (uint64_t)this;
        canvas.user_data = (void*)(new nertc::uid_t(thisAddr));//enable ? (void*)(new nertc::uid_t(uid)) : nullptr;
        canvas.window = nullptr;
        if (uid == 0)
        {
            ret = _channel->setupLocalVideoCanvas(&canvas);
        }else{
            ret = _channel->setupRemoteVideoCanvas(uid, &canvas);
        }
        LOG_F(INFO, "ret:%d", ret);
    }while(false);
    return Napi::Number::New(env, ret);
}


NIM_SDK_NODE_API_DEF(setClientRole)
{
    INIT_ENV
    do
    {
        uint32_t role;
        napi_get_value_uint32(info[0], role);
        LOG_F(INFO, "role:%d", role);
        ret = _channel->setClientRole((nertc::NERtcClientRole)role);
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
        ret = _channel->setLocalMediaPriority((nertc::NERtcMediaPriorityType)priority, enable);
        LOG_F(INFO, "ret:%d", ret);
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(getConnectionState)
{
    INIT_ENV
    do
    {
        LOG_F(INFO, "getConnectionState in");
        ret = _channel->getConnectionState();
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

// NIM_SDK_NODE_API_DEF(setVideoConfig)
// {
//     INIT_ENV
//     do
//     {
//         auto status = napi_ok;
//         nertc::NERtcVideoConfig config = {};
//         status = nertc_video_config_obj_to_struct(env, info[0].As<Napi::Object>(), config);
//         if (status != napi_ok)
//         {
//             break;
//         }
//         ret = _channel->setVideoConfig(config);
//     } while (false);
//     LOG_F(INFO, "ret:%d", ret);
//     return Napi::Number::New(env, ret);
// }

NIM_SDK_NODE_API_DEF(enableDualStreamMode)
{
    INIT_ENV
    do{
        bool enabled;
        napi_get_value_bool(info[0], enabled);
        LOG_F(INFO, "enabled:%d", enabled);
        ret = _channel->enableDualStreamMode(enabled);
    }while(false);
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
        ret = _channel->subscribeRemoteAudioStream(uid, enable);
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
        bool enable;
        napi_get_value_uint32(info[0], uid);
        napi_get_value_bool(info[1], enable);
        LOG_F(INFO, "uid:%llu enable:%d", uid, enable);
        ret = _channel->subscribeRemoteSubStreamAudio(uid, enable);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(subscribeAllRemoteAudioStream)
{
    INIT_ENV
    do
    {
        bool enable;
        napi_get_value_bool(info[0], enable);
        LOG_F(INFO, "enable:%d", enable);
        ret = _channel->subscribeAllRemoteAudioStream(enable);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
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
        ret = _channel->subscribeRemoteVideoStream(uid, (nertc::NERtcRemoteVideoStreamType)type, sub);
        LOG_F(INFO, "ret:%d", ret);
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
        LOG_F(INFO, "uid:%llu, sub:%d", uid, sub);
        ret = _channel->subscribeRemoteVideoSubStream(uid, sub);
        LOG_F(INFO, "ret:%d", ret);
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
        LOG_F(INFO, "uid:%llu volume:%d", uid, volume);
        ret = _channel->adjustUserPlaybackSignalVolume(uid, volume);
        LOG_F(INFO, "ret:%d", ret);
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(stopChannelMediaRelay)
{
    INIT_ENV
    do
    {
        LOG_F(INFO, "stopChannelMediaRelay in");
        ret = _channel->stopChannelMediaRelay();
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
        ret = _channel->setLocalPublishFallbackOption((nertc::NERtcStreamFallbackOption)opt);
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
        ret = _channel->setRemoteSubscribeFallbackOption((nertc::NERtcStreamFallbackOption)opt);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}


NIM_SDK_NODE_API_DEF(setRemoteHighPriorityAudioStream)
{
    INIT_ENV
    do
    {
        bool enabled;
        int64_t uid;
        napi_get_value_bool(info[0], enabled);
        napi_get_value_int64(info[1], uid);
        ret = _channel->setRemoteHighPriorityAudioStream(enabled, uid);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(enableMediaPub)
{
    INIT_ENV
    do
    {
        bool enabled;
        uint32_t type;
        napi_get_value_bool(info[0], enabled);
		napi_get_value_uint32(info[1], type);
        ret = _channel->enableMediaPub(enabled, (nertc::NERtcMediaPubType)type);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}




}