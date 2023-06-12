#include "node_version.h"
#include "nertc_node_channel.h"
#include "../shared/log/logging/logging.h"
#include "../shared/sdk_helper/node_api_helper.h"
#include <string>

using namespace nertc_electron_util;

#define SET_PROTOTYPE(name) \
    InstanceMethod(#name,  &NertcNodeChannel::name)

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
        if(user_data) {
            uint64_t thisAddr = *((nertc::uid_t *)user_data);
		    NodeVideoFrameTransporter * pTransporter = g_channel_transporter_map[thisAddr];
		    if (pTransporter) {
			    pTransporter->deliverFrame_I420(nrt, frame.uid, "", frame, rotate, frame.uid == 0);
		    }
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
        SET_PROTOTYPE(onEvent),
        SET_PROTOTYPE(onStatsObserver),
        SET_PROTOTYPE(onVideoFrame),

        SET_PROTOTYPE(release),
        SET_PROTOTYPE(getChannelName),
        SET_PROTOTYPE(joinChannel),
        SET_PROTOTYPE(leaveChannel),
        SET_PROTOTYPE(enableLocalAudio),
        SET_PROTOTYPE(enableLocalSubStreamAudio),
        SET_PROTOTYPE(muteLocalAudioStream),
        SET_PROTOTYPE(muteLocalSubStreamAudio),
        SET_PROTOTYPE(enableLocalVideo),
        SET_PROTOTYPE(enableLocalVideoEx),
        SET_PROTOTYPE(muteLocalVideoStream),
        SET_PROTOTYPE(muteLocalVideoStreamEx),

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

		SET_PROTOTYPE(setupVideoCanvas),
        SET_PROTOTYPE(setupSubStreamVideoCanvas),
        SET_PROTOTYPE(setLocalVideoMirrorMode),
        SET_PROTOTYPE(setLocalVideoMirrorModeEx),
        SET_PROTOTYPE(setClientRole),
        SET_PROTOTYPE(setLocalMediaPriority),
        SET_PROTOTYPE(getConnectionState),
        SET_PROTOTYPE(setCameraCaptureConfig),
        SET_PROTOTYPE(setCameraCaptureConfigEx),
        SET_PROTOTYPE(setVideoConfig),
        SET_PROTOTYPE(setVideoConfigEx),
        SET_PROTOTYPE(enableDualStreamMode),
        SET_PROTOTYPE(subscribeRemoteAudioStream),
        SET_PROTOTYPE(subscribeRemoteSubStreamAudio),
        SET_PROTOTYPE(subscribeAllRemoteAudioStream),
        SET_PROTOTYPE(setAudioSubscribeOnlyBy),
        SET_PROTOTYPE(subscribeRemoteVideoStream),
        SET_PROTOTYPE(subscribeRemoteVideoSubStream),

        SET_PROTOTYPE(addLiveStreamTask),
        SET_PROTOTYPE(updateLiveStreamTask),
        SET_PROTOTYPE(removeLiveStreamTask),
        SET_PROTOTYPE(sendSEIMsg),
        SET_PROTOTYPE(sendSEIMsgEx),

        SET_PROTOTYPE(adjustUserPlaybackSignalVolume),
        SET_PROTOTYPE(adjustChannelPlaybackSignalVolume),

        SET_PROTOTYPE(startChannelMediaRelay),
        SET_PROTOTYPE(updateChannelMediaRelay),
        SET_PROTOTYPE(stopChannelMediaRelay),

        SET_PROTOTYPE(setLocalPublishFallbackOption),
        SET_PROTOTYPE(setRemoteSubscribeFallbackOption),
        SET_PROTOTYPE(setRemoteHighPriorityAudioStream),
        SET_PROTOTYPE(enableMediaPub),
        SET_PROTOTYPE(enableAudioVolumeIndication),
        SET_PROTOTYPE(updatePermissionKey)

        SET_PROTOTYPE(updateSpatializerAudioRecvRange),
        SET_PROTOTYPE(updateSpatializerSelfPosition),
        SET_PROTOTYPE(enableSpatializerRoomEffects),
        SET_PROTOTYPE(setSpatializerRoomProperty),
        SET_PROTOTYPE(setSpatializerRenderMode),
        SET_PROTOTYPE(enableSpatializer)

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

    _event_handler = std::make_shared<NertcChannelEventHandler>();
    _channel->setChannelEventHandler(_event_handler.get());

    _stats_observer = std::make_shared<NertcChannelRtcMediaStatsHandler>();
    _channel->setStatsObserver(_stats_observer.get());

#ifdef WIN32
    _windows_helper = new WindowsHelpers();
    window_capture_helper_.reset(new WindowCaptureHelper());
    screen_capture_helper_.reset(new ScreenCaptureHelper());
#endif

    uint64_t thisAddr = (uint64_t)this;
    g_channel_transporter_map[thisAddr] = new NodeVideoFrameTransporter();
    LOG_F(INFO, "----create channel, channelName: %s----", channelName);
}

NertcNodeChannel::~NertcNodeChannel() {
     //todo
    if (_channel)
    {
        _channel = nullptr;
    }
    _event_handler->removeAll();
    
#ifdef WIN32
    if (_windows_helper)
    {
        delete _windows_helper;
        _windows_helper = nullptr;
    }
#endif
};

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
            auto sz = _stats_observer->removeEventHandler(eventName);
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

NIM_SDK_NODE_API_DEF(release)
{
    INIT_ENV
    do
    {
        LOG_F(INFO, "----channel release----");
        //todo 停止视频
        _channel->release();
        if (_channel)
        {
            _channel = nullptr;
        }
        _event_handler->removeAll();
    }while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(getChannelName)
{
    INIT_ENV_STR
    ret = _channel->getChannelName();
    return Napi::String::New(env, ret);
}

NIM_SDK_NODE_API_DEF(joinChannel)
{
    INIT_ENV
    do
    {
        std::string token = "";
        napi_get_value_utf8_string(info[0], token);
        ret = _channel->joinChannel(token.c_str());
        LOG_F(INFO, "ret:d%", ret);
    } while(false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(leaveChannel)
{
    INIT_ENV
    do
    {
        ret = _channel->leaveChannel();
        LOG_F(INFO, "ret:d%", ret);
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
        bool mute = false;
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
        bool mute = false;
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

NIM_SDK_NODE_API_DEF(enableLocalVideoEx)
{
    INIT_ENV
    do
    {
        uint32_t type = 0;
        bool enabled = false;
        napi_get_value_uint32(info[0], type);
        napi_get_value_bool(info[1], enabled);
        LOG_F(INFO, "enabled:%d", enabled);
        ret = _channel->enableLocalVideo((nertc::NERtcVideoStreamType)type, enabled);
        LOG_F(INFO, "ret:%d", ret);
    } while (false);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(muteLocalVideoStream)
{
    INIT_ENV
    do
    {
        bool enabled = false;
        napi_get_value_bool(info[0], enabled);
        LOG_F(INFO, "enabled:%d", enabled);
        ret = _channel->muteLocalVideoStream(enabled);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(muteLocalVideoStreamEx)
{
    INIT_ENV
    do
    {
        uint32_t type = 0;
        bool enabled = false;
        napi_get_value_uint32(info[0], type);
        napi_get_value_bool(info[1], enabled);
        LOG_F(INFO, "enabled:%d", enabled);
        ret = _channel->muteLocalVideoStream((nertc::NERtcVideoStreamType)type, enabled);
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
        ret = _channel->startScreenCaptureByScreenRect(screen_rect, region_rect, param);
        if (param.excluded_window_list != nullptr)
        {
            delete[] param.excluded_window_list;
            param.excluded_window_list = nullptr;
        }
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(startScreenCaptureByDisplayId)
{
    INIT_ENV
    do
    {
        int64_t display = 0;
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
                ret = _channel->startScreenCaptureByScreenRect(screen_rect, region_rect, param);
            }
            else
            {
                ret = -100;
            }
#else
            ret = _channel->startScreenCaptureByDisplayId(display, region_rect, param);
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

NIM_SDK_NODE_API_DEF(startScreenCaptureByWindowId)
{
    INIT_ENV
    do
    {
        int32_t windowid = 0;
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
        ret = _channel->startScreenCaptureByWindowId(reinterpret_cast<void *>(windowid), region_rect, param);
#else
        ret = _channel->startScreenCaptureByWindowId(reinterpret_cast<void *>(&windowid), region_rect, param);
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
        ret = _channel->updateScreenCaptureRegion(region_rect);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}


NIM_SDK_NODE_API_DEF(setScreenCaptureMouseCursor)
{
    INIT_ENV
    do
    {
        bool enabled = false;
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

NIM_SDK_NODE_API_DEF(setExcludeWindowList)
{
    INIT_ENV
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
        ret = _channel->setExcludeWindowList((nertc::source_id_t*)wnd_list, index);
        if (wnd_list) {
          delete[] wnd_list;
        }
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
        ret = _channel->updateScreenCaptureParameters(param);
        if (param.excluded_window_list != nullptr)
        {
            delete[] param.excluded_window_list;
            param.excluded_window_list = nullptr;
        }
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(setupVideoCanvas)
{
    INIT_ENV
    do{
        // nertc::uid_t uid = 0;
        uint32_t uid = 0;
        bool lossless = true;
        bool enable = false;
        napi_get_value_uint32(info[0], uid);
        napi_get_value_bool(info[1], enable);
        LOG_F(INFO, "uid:%llu, enable:%d", uid, enable);
        nertc::NERtcVideoCanvas canvas;
        canvas.cb = enable ? ChannelOnFrameDataCallback : nullptr; //NodeVideoFrameTransporter::onFrameDataCallback;
        uint64_t thisAddr = (uint64_t)this;
        canvas.user_data = enable ? (void*)(new nertc::uid_t(thisAddr)): nullptr;//enable ? (void*)(new nertc::uid_t(uid)) : nullptr;
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

NIM_SDK_NODE_API_DEF(setupSubStreamVideoCanvas)
{
    INIT_ENV
    do
    {
        // uint64_t uid;
        unsigned int uid = 0;
        bool enable = false;
        napi_get_value_uint32(info[0], uid);
        napi_get_value_bool(info[1], enable);
        LOG_F(INFO, "uid:%llu, enable:%d", uid, enable);
        nertc::NERtcVideoCanvas canvas;
        canvas.cb = enable ? ChannelOnSubstreamFrameDataCallback : nullptr;
        uint64_t thisAddr = (uint64_t)this;
        canvas.user_data = enable ? (void*)(new nertc::uid_t(thisAddr)) : nullptr;//enable ? (void*)(new nertc::uid_t(uid)) : nullptr;
        canvas.window = nullptr;
        if (uid == 0)
            ret = _channel->setupLocalSubStreamVideoCanvas(&canvas);
        else
            ret = _channel->setupRemoteSubStreamVideoCanvas(uid, &canvas);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(setLocalVideoMirrorMode)
{
    INIT_ENV
    do{
        uint32_t mode = 0;
        napi_get_value_uint32(info[0], mode);
        LOG_F(INFO, "mode:%d", mode);
        uint64_t thisAddr = (uint64_t)this;
        NodeVideoFrameTransporter * pTransporter = g_channel_transporter_map[thisAddr];
		if (pTransporter)
        {
            pTransporter->setLocalVideoMirrorMode((nertc::NERtcVideoMirrorMode)mode);
            ret = 0;
        }
    }while(false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(setLocalVideoMirrorModeEx)
{
    INIT_ENV
    do{
        //todo
        // uint32_t type;
        // uint32_t mode;
        // napi_get_value_uint32(info[0], type);
        // napi_get_value_uint32(info[1], mode);
        // LOG_F(INFO, "mode:%d", mode);
        // uint64_t thisAddr = (uint64_t)this;
        // NodeVideoFrameTransporter * pTransporter = g_channel_transporter_map[thisAddr];
		// if (pTransporter)
        // {
        //     pTransporter->setLocalVideoMirrorMode(type, mode);
        //     ret = 0;
        // }
    }while(false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(setClientRole)
{
    INIT_ENV
    do
    {
        uint32_t role = 0;
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
        int32_t priority = 50;
        bool enable = false;
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

NIM_SDK_NODE_API_DEF(setCameraCaptureConfig)
{
    INIT_ENV
    Napi::Object obj = Napi::Object::New(env);
    do
    {
        LOG_F(INFO, "setCameraCaptureConfig in");
        Napi::Object obj = info[0].As<Napi::Object>();
        nertc::NERtcCameraCaptureConfig config = {};
        nertc_camera_capture_obj_to_struct(env, obj, config);
        ret = _channel->setCameraCaptureConfig(config);

    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);;
}

NIM_SDK_NODE_API_DEF(setCameraCaptureConfigEx)
{
    INIT_ENV
    Napi::Object obj = Napi::Object::New(env);
    do
    {
        LOG_F(INFO, "setCameraCaptureConfig in");
        int type = 0;
        napi_get_value_int32(info[0], type);
        Napi::Object obj = info[1].As<Napi::Object>();
        nertc::NERtcCameraCaptureConfig config = {};
        nertc_camera_capture_obj_to_struct(env, obj, config);
        ret = _channel->setCameraCaptureConfig((nertc::NERtcVideoStreamType)type, config);

    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);;
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
        ret = _channel->setVideoConfig(config);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(setVideoConfigEx)
{
    INIT_ENV
    do
    {
        int type = 0;
        napi_get_value_int32(info[0], type);
        auto status = napi_ok;
        nertc::NERtcVideoConfig config = {};
        status = nertc_video_config_obj_to_struct(env, info[1].As<Napi::Object>(), config);
        if (status != napi_ok)
        {
            break;
        }
        ret = _channel->setVideoConfig((nertc::NERtcVideoStreamType)type, config);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(enableDualStreamMode)
{
    INIT_ENV
    do{
        bool enabled = false;
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
        uint32_t uid = 0;
        bool subscribe = false;
        napi_get_value_uint32(info[0], uid);
        napi_get_value_bool(info[1], subscribe);
        LOG_F(INFO, "uid:%llu subscribe:%d", uid, subscribe);
        ret = _channel->subscribeRemoteAudioStream(uid, subscribe);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(subscribeRemoteSubStreamAudio)
{
    INIT_ENV
    do
    {
        uint32_t uid = 0;
        bool subscribe = false;
        napi_get_value_uint32(info[0], uid);
        napi_get_value_bool(info[1], subscribe);
        LOG_F(INFO, "uid:%llu subscribe:%d", uid, subscribe);
        ret = _channel->subscribeRemoteSubStreamAudio(uid, subscribe);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(subscribeAllRemoteAudioStream)
{
    INIT_ENV
    do
    {
        bool subscribe = false;
        napi_get_value_bool(info[0], subscribe);
        LOG_F(INFO, "subscribe:%d", subscribe);
        ret = _channel->subscribeAllRemoteAudioStream(subscribe);
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
        ret = _channel->setAudioSubscribeOnlyBy(uid_array, size);
        if (uid_array) {
          delete[] uid_array;
        }
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
        unsigned int uid = 0;
        uint32_t type = 0;
        bool sub = false;
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
        bool sub = false;
        // nertc::uid_t uid;
        unsigned int uid = 0;
        napi_get_value_uint32(info[0], uid);
        napi_get_value_bool(info[1], sub);
        LOG_F(INFO, "uid:%llu, sub:%d", uid, sub);
        ret = _channel->subscribeRemoteVideoSubStream(uid, sub);
        LOG_F(INFO, "ret:%d", ret);
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
        memset(info.extraInfo, 0, kNERtcMaxURILength);
        // info.config = {0};
        ret = _channel->addLiveStreamTask(info);
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
        nertc::NERtcLiveStreamTaskInfo info = {};

        nertc_ls_task_info_obj_to_struct(env, obj, info);
        ret = _channel->updateLiveStreamTask(info);
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
        std::string task_id = "";
        napi_get_value_utf8_string(info[0], task_id);
        LOG_F(INFO, "task_id:%s", task_id.c_str());
        ret = _channel->removeLiveStreamTask(task_id.c_str());
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
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
        ret = _channel->sendSEIMsg(static_cast<const char*>(data), len);
        LOG_F(INFO, "ret:%d", ret);
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
        ret = _channel->sendSEIMsg(static_cast<const char*>(data), len, static_cast<nertc::NERtcVideoStreamType>(type));
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

NIM_SDK_NODE_API_DEF(adjustChannelPlaybackSignalVolume)
{
    INIT_ENV
    do
    {
        int volume = 0;
        napi_get_value_int32(info[0], volume);
        LOG_F(INFO, "volume:%d", volume);
        ret = _channel->adjustChannelPlaybackSignalVolume(volume);
        LOG_F(INFO, "ret:%d", ret);
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

        ret = _channel->startChannelMediaRelay(media_relay_config_);

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

        ret = _channel->updateChannelMediaRelay(media_relay_config_);

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
        int opt = 0;
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
        int opt = 0;
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
        bool enabled = false;
        int64_t uid = 0;
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
        bool enabled = false;
        uint32_t type = 0;
        napi_get_value_bool(info[0], enabled);
		napi_get_value_uint32(info[1], type);
        ret = _channel->enableMediaPub(enabled, (nertc::NERtcMediaPubType)type);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}


NIM_SDK_NODE_API_DEF(enableAudioVolumeIndication)
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
        ret = _channel->enableAudioVolumeIndication(enabled, interval, enableVad);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(updatePermissionKey)
{
    INIT_ENV
    do
    {
        std::string key;
        napi_get_value_utf8_string(info[0], key);
        LOG_F(INFO, "key:%s", key);
        ret = _channel->updatePermissionKey(key.c_str());
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
        napi_get_value_bool(info[0], enable);
        LOG_F(INFO, "enable:%d", enable);
        ret = rtc_engine_->enableSpatializer(enable);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(updateSpatializerAudioRecvRange)
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
        ret = rtc_engine_->updateSpatializerAudioRecvRange(audio_distance, conversational_distance, (nertc::NERtcDistanceRolloffModel)roll_off);
    } while (false);
    LOG_F(INFO, "ret:%d", ret);
    return Napi::Number::New(env, ret);
}

NIM_SDK_NODE_API_DEF(updateSpatializerSelfPosition)
{
    INIT_ENV
    uint64_t dur = 0;
    do
    {
        Napi::Object obj = info[0].As<Napi::Object>();
        nertc::NERtcSpatializerPositionInfo info;
        nertc_spatializer_position_to_struct(env, obj, info);
        ret = rtc_engine_->updateSpatializerSelfPosition(info);
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
    uint64_t dur = 0;
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


}