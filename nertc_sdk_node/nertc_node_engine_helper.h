#ifndef NERTC_NODE_SDK_ENGINE_HELPER_H
#define NERTC_NODE_SDK_ENGINE_HELPER_H

#include <napi.h>
#include <stdio.h>
#include "nertc_engine.h"
#include "nertc_engine_defines.h"
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
napi_status nertc_lastmile_probe_obj_to_struct(const Napi::Env& env, const Napi::Object& obj, nertc::NERtcLastmileProbeConfig& config);
napi_status nertc_join_channel_option_to_struct(const Napi::Env& env, const Napi::Object& obj, nertc::NERtcJoinChannelOptions& config);
napi_status nertc_channel_optionex_to_struct(const Napi::Env& env, const Napi::Object& obj, nertc::NERtcJoinChannelOptionsEx& config);
napi_status nertc_recording_option_to_struct(const Napi::Env& env, const Napi::Object& obj, nertc::NERtcAudioRecordingConfiguration& config);
napi_status nertc_virtual_background_option_to_struct(const Napi::Env& env, const Napi::Object& obj, nertc::VirtualBackgroundSource& config);
napi_status nertc_spatializer_room_property_to_struct(const Napi::Env& env, const Napi::Object& obj, nertc::NERtcSpatializerRoomProperty& config);
napi_status nertc_spatializer_position_to_struct(const Napi::Env& env, const Napi::Object& obj, nertc::NERtcPositionInfo& config);
napi_status nertc_position_info_to_struct(const Napi::Env& env, const Napi::Object& obj, nertc::NERtcPositionInfo& info);
// napi_status nertc_screen_capture_source_info_to_struct(const Napi::Env& env, const Napi::Object& obj, nertc::NERtcScreenCaptureSourceInfo& source);
napi_status nertc_screen_size_info_to_struct(const Napi::Env& env, const Napi::Object& obj, nertc::NERtcSize& size);
napi_status nertc_video_water_mark_image_to_struct(const Napi::Env& env, const Napi::Object& obj, nertc::NERtcVideoWatermarkImageConfig& config);
napi_status nertc_video_water_mark_txt_to_struct(const Napi::Env& env, const Napi::Object& obj, nertc::NERtcVideoWatermarkTextConfig& config);
napi_status nertc_video_water_mark_time_to_struct(const Napi::Env& env, const Napi::Object& obj, nertc::NERtcVideoWatermarkTimestampConfig& config);
napi_status nertc_video_water_mark_config_to_struct(const Napi::Env& env, const Napi::Object& obj, nertc::NERtcVideoWatermarkConfig& config);


//set object field
napi_status nertc_stats_to_obj(const Napi::Env env, const nertc::NERtcStats& config,  Napi::Object& obj);
napi_status nertc_audio_send_stats_to_obj(const Napi::Env env, const nertc::NERtcAudioSendStats& config,  Napi::Object& obj);
napi_status nertc_audio_recv_stats_to_obj(const Napi::Env env, const nertc::NERtcAudioRecvStats& config,  Napi::Object& obj);
napi_status nertc_video_send_stats_to_obj(const Napi::Env env, const nertc::NERtcVideoSendStats& config,  Napi::Object& obj);
napi_status nertc_video_recv_stats_to_obj(const Napi::Env env, const nertc::NERtcVideoRecvStats& config,  Napi::Object& obj);
napi_status nertc_network_quality_to_obj(const Napi::Env env, const nertc::NERtcNetworkQualityInfo& config,  Napi::Object& obj);
napi_status nertc_audio_volume_info_to_obj(const Napi::Env env, const nertc::NERtcAudioVolumeInfo& config,  Napi::Object& obj);
napi_status nertc_lastmile_probe_result_to_obj(const Napi::Env env, const nertc::NERtcLastmileProbeResult& config,  Napi::Object& obj);
napi_status nertc_user_join_extra_info_to_obj(const Napi::Env env, const nertc::NERtcUserJoinExtraInfo& join_extra_info,  Napi::Object& obj);
napi_status nertc_screen_capture_source_data_update_to_obj(const Napi::Env env, const nertc::NERtcScreenCaptureSourceData& data,  Napi::Object& obj);


// channel helper
//obj->struct
napi_status nertc_camera_capture_obj_to_struct(const Napi::Env env, Napi::Object& obj, nertc::NERtcCameraCaptureConfig& config);
napi_status nertc_uid_list_to_struct(const Napi::Env& env, const Napi::Object& obj, std::set<uint64_t>& list);
napi_status nertc_virtual_backgroup_source_obj_to_struct(const Napi::Env& env, const Napi::Object& obj, nertc::VirtualBackgroundSource& config);
napi_status nertc_rever_param_obj_to_struct(const Napi::Env& env, const Napi::Object& obj, nertc::NERtcReverbParam& param);

//struct->obj
napi_status nertc_camera_capture_struct_to_obj(const Napi::Env env,  nertc::NERtcCameraCaptureConfig& config,  Napi::Object& obj);
napi_status nertc_audio_format_to_obj(const Napi::Env env, const nertc::NERtcAudioFormat& config,  Napi::Object& obj);
napi_status nertc_audio_frame_to_obj(const Napi::Env env, const nertc::NERtcAudioFrame& config,  Napi::Object& obj);

///////////////////////////
uint8_t *ARGBToBGRA(char *src, int size);

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