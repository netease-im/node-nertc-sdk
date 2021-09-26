#ifndef NERTC_NODE_SDK_ENGINE_HELPER_H
#define NERTC_NODE_SDK_ENGINE_HELPER_H

#include <node.h>
#include <node_api.h>
#include <list>
#include "nertc_engine.h"

using v8::Context;
using v8::Integer;
using v8::Isolate;
using v8::Local;
using v8::NewStringType;
using v8::Object;
using v8::String;
using v8::Value;
using v8::Exception;
using v8::Array;

namespace nertc_node {

napi_status nertc_engine_context_obj_to_struct(Isolate* isolate, const Local<Object>& obj, nertc::NERtcEngineContext& context);
napi_status nertc_video_config_obj_to_struct(Isolate* isolate, const Local<Object>& obj, nertc::NERtcVideoConfig& config);
napi_status nertc_audio_frame_rf_obj_to_struct(Isolate* isolate, const Local<Object>& obj, nertc::NERtcAudioFrameRequestFormat& format);
napi_status nertc_audio_mixing_option_obj_to_struct(Isolate* isolate, const Local<Object>& obj, nertc::NERtcCreateAudioMixingOption& option);
napi_status nertc_audio_effect_option_obj_to_struct(Isolate* isolate, const Local<Object>& obj, nertc::NERtcCreateAudioEffectOption* option);
napi_status nertc_rectangle_obj_to_struct(Isolate* isolate, const Local<Object>& obj, nertc::NERtcRectangle& rect);
napi_status nertc_video_dimensions_obj_to_struct(Isolate* isolate, const Local<Object>& obj, nertc::NERtcVideoDimensions& dimen);
//napi_status nertc_screen_capture_params_obj_to_struct(Isolate* isolate, const Local<Object>& obj, nertc::NERtcScreenCaptureParameters& params);//modify by lyq 参数不一致，先注释
napi_status nertc_ls_task_info_obj_to_struct(Isolate* isolate, const Local<Object>& obj, nertc::NERtcLiveStreamTaskInfo& info);

napi_status nertc_stats_to_obj(Isolate* isolate, const nertc::NERtcStats& config, Local<Object>& obj);
napi_status nertc_audio_send_stats_to_obj(Isolate* isolate, const nertc::NERtcAudioSendStats& config, Local<Object>& obj);
napi_status nertc_audio_recv_stats_to_obj(Isolate* isolate, const nertc::NERtcAudioRecvStats& config, Local<Object>& obj);
napi_status nertc_video_send_stats_to_obj(Isolate* isolate, const nertc::NERtcVideoSendStats& config, Local<Object>& obj);
napi_status nertc_video_recv_stats_to_obj(Isolate* isolate, const nertc::NERtcVideoRecvStats& config, Local<Object>& obj);
napi_status nertc_network_quality_to_obj(Isolate* isolate, const nertc::NERtcNetworkQualityInfo& config, Local<Object>& obj);
napi_status nertc_audio_volume_info_to_obj(Isolate* isolate, const nertc::NERtcAudioVolumeInfo& config, Local<Object>& obj);

}

#endif //NERTC_NODE_SDK_ENGINE_HELPER_H