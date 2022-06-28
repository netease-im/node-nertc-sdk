#ifndef NERTC_NODE_SDK_ENGINE_HELPER_H
#define NERTC_NODE_SDK_ENGINE_HELPER_H

#include <napi.h>
#include <stdio.h>
#include "nertc_engine.h"
#include <list>
#include <set>

namespace nertc_node
{
//get object field
napi_status nertc_engine_context_obj_to_struct(const Napi::Env& env, const Napi::Object& obj, nertc::NERtcEngineContext& context);
napi_status nertc_window_id_list_to_struct(const Napi::Env& env, const Napi::Object& obj, std::set<int64_t>& list);
napi_status nertc_screen_capture_params_obj_to_struct(const Napi::Env& env, const Napi::Object& obj, nertc::NERtcScreenCaptureParameters& params, std::set<int64_t>& list);
napi_status nertc_rectangle_obj_to_struct(const Napi::Env& env, const Napi::Object& obj, nertc::NERtcRectangle& rect);
napi_status nertc_ls_task_info_obj_to_struct(const Napi::Env& env, const Napi::Object& obj, nertc::NERtcLiveStreamTaskInfo& info);
napi_status nertc_audio_effect_option_obj_to_struct(const Napi::Env& env, const Napi::Object& obj, nertc::NERtcCreateAudioEffectOption* option);
napi_status nertc_audio_mixing_option_obj_to_struct(const Napi::Env& env, const Napi::Object& obj, nertc::NERtcCreateAudioMixingOption& option);
napi_status nertc_audio_frame_rf_obj_to_struct(const Napi::Env& env, const Napi::Object& obj, nertc::NERtcAudioFrameRequestFormat& format);
napi_status nertc_video_config_obj_to_struct(const Napi::Env& env, const Napi::Object& obj, nertc::NERtcVideoConfig& config);
napi_status nertc_media_relay_obj_to_struct(const Napi::Env& env, const Napi::Object& obj, nertc::NERtcChannelMediaRelayConfiguration* config);
napi_status nertc_encry_obj_to_struct(const Napi::Env& env, const Napi::Object& obj, nertc::NERtcEncryptionConfig& config);
napi_status nertc_channel_media_relay_obj_to_struct(const Napi::Env& env, const Napi::Object& obj, nertc::NERtcChannelMediaRelayInfo& config);

//set object field
napi_status nertc_stats_to_obj(const Napi::Env env, const nertc::NERtcStats& config,  Napi::Object& obj);
//napi_status nertc_audio_send_stats_to_obj(const Napi::Env env, const nertc::NERtcAudioSendStats& config,  Napi::Object& obj);
//napi_status nertc_audio_recv_stats_to_obj(const Napi::Env env, const nertc::NERtcAudioRecvStats& config,  Napi::Object& obj);
napi_status nertc_video_send_stats_to_obj(const Napi::Env env, const nertc::NERtcVideoSendStats& config,  Napi::Object& obj);
napi_status nertc_video_recv_stats_to_obj(const Napi::Env env, const nertc::NERtcVideoRecvStats& config,  Napi::Object& obj);
napi_status nertc_network_quality_to_obj(const Napi::Env env, const nertc::NERtcNetworkQualityInfo& config,  Napi::Object& obj);
napi_status nertc_audio_volume_info_to_obj(const Napi::Env env, const nertc::NERtcAudioVolumeInfo& config,  Napi::Object& obj);
}

//饿汉模式
#define SINGLETON_DEFINE(TypeName)				\
static TypeName* GetInstance()					\
{												\
	static TypeName type_instance;				\
	return &type_instance;						\
}												\
												\
TypeName(const TypeName&) = delete;				\
TypeName& operator=(const TypeName&) = delete
    
#endif