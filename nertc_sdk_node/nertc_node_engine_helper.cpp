#include "nertc_node_engine_helper.h"
#include "../shared/sdk_helper/nim_node_helper.h"

namespace nertc_node
{

napi_status nertc_engine_context_obj_to_struct(Isolate* isolate, const Local<Object>& obj, nertc::NERtcEngineContext& context)
{
    // UTF8String out, out1;
    uint32_t out_u;
    int32_t out_i;
    // if (nim_napi_get_object_value_utf8string(isolate, obj, "app_key", out) == napi_ok)
    // {
    //     utf8_string o = out.toUtf8String();
    //     context.app_key = o.c_str();
    // }
    // if (nim_napi_get_object_value_utf8string(isolate, obj, "log_dir_path", out1) == napi_ok)
    // {
    //     utf8_string o = out1.toUtf8String();
    //     context.log_dir_path = o.c_str();
    // }
    if (nim_napi_get_object_value_int32(isolate, obj, "log_level", out_i) == napi_ok)
    {
        context.log_level = (nertc::NERtcLogLevel)out_i;
    }
    if (nim_napi_get_object_value_uint32(isolate, obj, "log_file_max_size_KBytes", out_u) == napi_ok)
    {
        context.log_file_max_size_KBytes = out_u;
    }
    return napi_ok;
}

napi_status nertc_video_config_obj_to_struct(Isolate* isolate, const Local<Object>& obj, nertc::NERtcVideoConfig& config)
{
    uint32_t out_u;
    if (nim_napi_get_object_value_uint32(isolate, obj, "max_profile", out_u) == napi_ok)
    {
        config.max_profile = (nertc::NERtcVideoProfileType)out_u;
    }
    if (nim_napi_get_object_value_uint32(isolate, obj, "width", out_u) == napi_ok)
    {
        config.width = out_u;
    }
    if (nim_napi_get_object_value_uint32(isolate, obj, "height", out_u) == napi_ok)
    {
        config.height = out_u;
    }
    if (nim_napi_get_object_value_uint32(isolate, obj, "crop_mode", out_u) == napi_ok)
    {
        config.crop_mode_ = (nertc::NERtcVideoCropMode)out_u;
    }
    if (nim_napi_get_object_value_uint32(isolate, obj, "framerate", out_u) == napi_ok)
    {
        config.framerate = (nertc::NERtcVideoFramerateType)out_u;
    }
    if (nim_napi_get_object_value_uint32(isolate, obj, "min_framerate", out_u) == napi_ok)
    {
        config.min_framerate = (nertc::NERtcVideoFramerateType)out_u;
    }
    if (nim_napi_get_object_value_uint32(isolate, obj, "bitrate", out_u) == napi_ok)
    {
        config.bitrate = out_u;
    }
    if (nim_napi_get_object_value_uint32(isolate, obj, "min_bitrate", out_u) == napi_ok)
    {
        config.min_bitrate = out_u;
    }
    if (nim_napi_get_object_value_uint32(isolate, obj, "degradation_preference", out_u) == napi_ok)
    {
        config.degradation_preference = (nertc::NERtcDegradationPreference)out_u;
    }
    return napi_ok;
}

napi_status nertc_audio_frame_rf_obj_to_struct(Isolate* isolate, const Local<Object>& obj, nertc::NERtcAudioFrameRequestFormat& format)
{
    uint32_t out_u;
    if (nim_napi_get_object_value_uint32(isolate, obj, "channels", out_u) == napi_ok)
    {
        format.channels = out_u;
    }
    if (nim_napi_get_object_value_uint32(isolate, obj, "sample_rate", out_u) == napi_ok)
    {
        format.sample_rate = out_u;
    }    
    return napi_ok; 
}

napi_status nertc_audio_mixing_option_obj_to_struct(Isolate* isolate, const Local<Object>& obj, nertc::NERtcCreateAudioMixingOption& option)
{
    UTF8String out;
    uint32_t out_u;
    int32_t out_i;
    bool out_b;
    if (nim_napi_get_object_value_utf8string(isolate, obj, "path", out) == napi_ok)
    {
        strcpy(option.path, out.toUtf8String().c_str());
    }
    if (nim_napi_get_object_value_int32(isolate, obj, "loop_count", out_i) == napi_ok)
    {
        option.loop_count = out_i;
    }
    if (nim_napi_get_object_value_bool(isolate, obj, "send_enabled", out_b) == napi_ok)
    {
        option.send_enabled = out_b;
    }
    if (nim_napi_get_object_value_uint32(isolate, obj, "send_volume", out_u) == napi_ok)
    {
        option.send_volume = out_u;
    }
    if (nim_napi_get_object_value_bool(isolate, obj, "playback_enabled", out_b) == napi_ok)
    {
        option.playback_enabled = out_b;
    }
    if (nim_napi_get_object_value_uint32(isolate, obj, "playback_volume", out_u) == napi_ok)
    {
        option.playback_volume = out_u;
    }
    return napi_ok;
}

napi_status nertc_audio_effect_option_obj_to_struct(Isolate* isolate, const Local<Object>& obj, nertc::NERtcCreateAudioEffectOption* option)
{
    UTF8String out;
    uint32_t out_u;
    int32_t out_i;
    bool out_b;
    Local<Array> objs = obj.As<Array>();
    for (size_t i = 0; i < objs->Length(); i++)
    {
        auto o = objs->Get(isolate->GetCurrentContext(), i).ToLocalChecked().As<Object>();
        if (nim_napi_get_object_value_utf8string(isolate, o, "path", out) == napi_ok)
        {
            strcpy(option[i].path, out.toUtf8String().c_str());
        }
        if (nim_napi_get_object_value_int32(isolate, o, "loop_count", out_i) == napi_ok)
        {
            option[i].loop_count = out_i;
        }
        if (nim_napi_get_object_value_bool(isolate, o, "send_enabled", out_b) == napi_ok)
        {
            option[i].send_enabled = out_b;
        }
        if (nim_napi_get_object_value_uint32(isolate, o, "send_volume", out_u) == napi_ok)
        {
            option[i].send_volume = out_u;
        }
        if (nim_napi_get_object_value_bool(isolate, o, "playback_enabled", out_b) == napi_ok)
        {
            option[i].playback_enabled = out_b;
        }
        if (nim_napi_get_object_value_uint32(isolate, o, "playback_volume", out_u) == napi_ok)
        {
            option[i].playback_volume = out_u;
        }
    }

    return napi_ok;    
}

napi_status nertc_rectangle_obj_to_struct(Isolate* isolate, const Local<Object>& obj, nertc::NERtcRectangle& rect)
{
    int32_t out_i;
    if (nim_napi_get_object_value_int32(isolate, obj, "x", out_i) == napi_ok)
    {
        rect.x = out_i;
    }
    if (nim_napi_get_object_value_int32(isolate, obj, "y", out_i) == napi_ok)
    {
        rect.y = out_i;
    }
    if (nim_napi_get_object_value_int32(isolate, obj, "width", out_i) == napi_ok)
    {
        rect.width = out_i;
    }
    if (nim_napi_get_object_value_int32(isolate, obj, "height", out_i) == napi_ok)
    {
        rect.height = out_i;
    }
    return napi_ok;    
}

napi_status nertc_video_dimensions_obj_to_struct(Isolate* isolate, const Local<Object>& obj, nertc::NERtcVideoDimensions& dimen)
{
    int32_t out_i;
    if (nim_napi_get_object_value_int32(isolate, obj, "width", out_i) == napi_ok)
    {
        dimen.width = out_i;
    }
    if (nim_napi_get_object_value_int32(isolate, obj, "height", out_i) == napi_ok)
    {
        dimen.height = out_i;
    }
    return napi_ok;        
}

napi_status nertc_screen_capture_params_obj_to_struct(Isolate* isolate, const Local<Object>& obj, nertc::NERtcScreenCaptureParameters& params)
{
    int32_t out_i;
    bool out_b;
    params.excluded_window_list = nullptr;
    if (nim_napi_get_object_value_int32(isolate, obj, "profile", out_i) == napi_ok)
    {
        params.profile = (nertc::NERtcScreenProfileType)out_i;
    }
    Local<Value> so;
    if (nim_napi_get_object_value(isolate, obj, "dimensions", so) == napi_ok)
    {
        if (nim_napi_get_object_value_int32(isolate, so.As<Object>(), "width", out_i) == napi_ok)
        {
            params.dimensions.width = out_i;
        }
        if (nim_napi_get_object_value_int32(isolate, so.As<Object>(), "height", out_i) == napi_ok)
        {
            params.dimensions.height = out_i;
        }
    }
    if (nim_napi_get_object_value_int32(isolate, obj, "frame_rate", out_i) == napi_ok)
    {
        params.frame_rate = out_i;
    }
    if (nim_napi_get_object_value_int32(isolate, obj, "bitrate", out_i) == napi_ok)
    {
        params.bitrate = out_i;
    }
    if (nim_napi_get_object_value_bool(isolate, obj, "capture_mouse_cursor", out_b) == napi_ok)
    {
        params.capture_mouse_cursor = out_b;
    }
    if (nim_napi_get_object_value_bool(isolate, obj, "window_focus", out_b) == napi_ok)
    {
        params.window_focus = out_b;
    }
    if (nim_napi_get_object_value_int32(isolate, obj, "prefer", out_i) == napi_ok)
    {
        params.prefer = (nertc::NERtcSubStreamContentPrefer)out_i;
    }
    if (nim_napi_get_object_value_int32(isolate, obj, "excluded_window_count", out_i) == napi_ok)
    {
        params.excluded_window_count = out_i;
    }
    if (params.excluded_window_count > 0)
    {
        if (nim_napi_get_object_value(isolate, obj, "excluded_window_list", so) == napi_ok)
        {
            Local<Array> wl = so.As<Array>();
            if (wl->IsArray()) {
                intptr_t *wi = new intptr_t[params.excluded_window_count];
                if (wl->Length() == params.excluded_window_count)
                {
                    for (auto i = 0; i < params.excluded_window_count; i++)
                    {
                        wi[i] = wl->Get(isolate->GetCurrentContext(), i).ToLocalChecked()->ToInteger(isolate->GetCurrentContext()).ToLocalChecked()->Value();
                    }
                    params.excluded_window_list = (void *)wi;
                }
                else
                {
                    delete[] wi;
                    wi = nullptr;
                    return napi_invalid_arg;
                }
            } else {
                return napi_invalid_arg;
            }
        }
    }
    return napi_ok;      
}

static napi_status nertc_ls_img_info_obj_to_struct(Isolate* isolate, const Local<Object>& obj, nertc::NERtcLiveStreamImageInfo* info)
{
    UTF8String out;
    int32_t out_i;
    if (nim_napi_get_object_value_utf8string(isolate, obj, "url", out) == napi_ok)
    {
        strcpy(info->url, out.toUtf8String().c_str());
    }
    if (nim_napi_get_object_value_int32(isolate, obj, "x", out_i) == napi_ok)
    {
        info->x = out_i;
    }
    if (nim_napi_get_object_value_int32(isolate, obj, "y", out_i) == napi_ok)
    {
        info->y = out_i;
    }
    if (nim_napi_get_object_value_int32(isolate, obj, "width", out_i) == napi_ok)
    {
        info->width = out_i;
    }
    if (nim_napi_get_object_value_int32(isolate, obj, "height", out_i) == napi_ok)
    {
        info->height = out_i;
    }
    return napi_ok;
}

static napi_status nertc_ls_users_obj_to_struct(Isolate* isolate, const Local<Object>& obj, nertc::NERtcLiveStreamUserTranscoding& user)
{
    uint64_t out_u64;
    bool out_b;
    uint32_t out_u;
    int32_t out_i;
    if (nim_napi_get_object_value_uint64(isolate, obj, "uid", out_u64) == napi_ok)
    {
        user.uid = out_u64;
    }
    if (nim_napi_get_object_value_bool(isolate, obj, "video_push", out_b) == napi_ok)
    {
        user.video_push = out_b;
    }
    if (nim_napi_get_object_value_uint32(isolate, obj, "adaption", out_u) == napi_ok)
    {
        user.adaption = (nertc::NERtcLiveStreamVideoScaleMode)out_u;
    }
    if (nim_napi_get_object_value_int32(isolate, obj, "x", out_i) == napi_ok)
    {
        user.x = out_i;
    }
    if (nim_napi_get_object_value_int32(isolate, obj, "y", out_i) == napi_ok)
    {
        user.y = out_i;
    }
    if (nim_napi_get_object_value_int32(isolate, obj, "width", out_i) == napi_ok)
    {
        user.width = out_i;
    }
    if (nim_napi_get_object_value_int32(isolate, obj, "height", out_i) == napi_ok)
    {
        user.height = out_i;
    }
    if (nim_napi_get_object_value_bool(isolate, obj, "audio_push", out_b) == napi_ok)
    {
        user.audio_push = out_b;
    }
    return napi_ok;
}

static napi_status nertc_ls_layout_obj_to_struct(Isolate* isolate, const Local<Object>& obj, nertc::NERtcLiveStreamLayout& layout)
{
    int32_t out_i;
    uint32_t out_u;
    if (nim_napi_get_object_value_int32(isolate, obj, "width", out_i) == napi_ok)
    {
        layout.width = out_i;
    }
    if (nim_napi_get_object_value_int32(isolate, obj, "height", out_i) == napi_ok)
    {
        layout.height = out_i;
    }
    if (nim_napi_get_object_value_uint32(isolate, obj, "background_color", out_u) == napi_ok)
    {
        layout.background_color = out_u;
    }
    if (nim_napi_get_object_value_uint32(isolate, obj, "user_count", out_u) == napi_ok)
    {
        layout.user_count = out_u;
    }
    layout.users = nullptr;
    layout.bg_image = nullptr;
    if (layout.user_count > 0)
    {
        Local<Value> so, so1;
        if (nim_napi_get_object_value(isolate, obj, "users", so) == napi_ok)
        {
            if (so->IsArray())
            {
                layout.users = new nertc::NERtcLiveStreamUserTranscoding[layout.user_count];
                Local<Array> wl = so.As<Array>();
                if (wl->Length() != layout.user_count)
                {
                    delete[] layout.users;
                    layout.users = nullptr;
                    return napi_invalid_arg;
                }
                for (auto i = 0; i < layout.user_count; i++)
                {
                    nertc_ls_users_obj_to_struct(isolate, wl->Get(isolate->GetCurrentContext(), i).ToLocalChecked().As<Object>(), layout.users[i]);
                }
            }
            else {
                return napi_invalid_arg;
            }
        }
        if (nim_napi_get_object_value(isolate, obj, "bg_image", so1) == napi_ok)
        {
            layout.bg_image = new nertc::NERtcLiveStreamImageInfo;
            nertc_ls_img_info_obj_to_struct(isolate, so1.As<Object>(), layout.bg_image);
        }
    }
    return napi_ok;
}

napi_status nertc_ls_task_info_obj_to_struct(Isolate* isolate, const Local<Object>& obj, nertc::NERtcLiveStreamTaskInfo& info)
{
    UTF8String out;
    int32_t out_i;
    bool out_b;
    if (nim_napi_get_object_value_utf8string(isolate, obj, "task_id", out) == napi_ok)
    {
        strcpy(info.task_id, out.toUtf8String().c_str());
    }
    if (nim_napi_get_object_value_utf8string(isolate, obj, "stream_url", out) == napi_ok)
    {
        strcpy(info.stream_url, out.toUtf8String().c_str());
    }
    if (nim_napi_get_object_value_bool(isolate, obj, "server_record_enabled", out_b) == napi_ok)
    {
        info.server_record_enabled = out_b;
    }
    if (nim_napi_get_object_value_int32(isolate, obj, "ls_mode", out_i) == napi_ok)
    {
        info.ls_mode = (nertc::NERtcLiveStreamMode)out_i;
    }
    Local<Value> so;
    if ((nim_napi_get_object_value(isolate, obj, "layout", so) == napi_ok) && (nertc_ls_layout_obj_to_struct(isolate, so.As<Object>(), info.layout) == napi_ok))
    {
        return napi_ok;
    }
    return napi_invalid_arg;
}

napi_status nertc_stats_to_obj(Isolate* isolate, const nertc::NERtcStats& config, Local<Object>& obj)
{
    nim_napi_set_object_value_uint32(isolate, obj, "cpu_app_usage", config.cpu_app_usage);
    nim_napi_set_object_value_uint32(isolate, obj, "cpu_idle_usage", config.cpu_idle_usage);
    nim_napi_set_object_value_uint32(isolate, obj, "cpu_total_usage", config.cpu_total_usage);
    nim_napi_set_object_value_uint32(isolate, obj, "memory_app_usage", config.memory_app_usage);
    nim_napi_set_object_value_uint32(isolate, obj, "memory_total_usage", config.memory_total_usage);
    nim_napi_set_object_value_uint32(isolate, obj, "memory_app_kbytes", config.memory_app_kbytes);
    nim_napi_set_object_value_int32(isolate, obj, "total_duration", config.total_duration);
    nim_napi_set_object_value_uint64(isolate, obj, "tx_bytes", config.tx_bytes);
    nim_napi_set_object_value_uint64(isolate, obj, "rx_bytes", config.rx_bytes);
    nim_napi_set_object_value_uint64(isolate, obj, "tx_audio_bytes", config.tx_audio_bytes);
    nim_napi_set_object_value_uint64(isolate, obj, "tx_video_bytes", config.tx_video_bytes);
    nim_napi_set_object_value_uint64(isolate, obj, "rx_audio_bytes", config.rx_audio_bytes);
    nim_napi_set_object_value_uint64(isolate, obj, "rx_video_bytes", config.rx_video_bytes);
    nim_napi_set_object_value_int32(isolate, obj, "tx_audio_kbitrate", config.tx_audio_kbitrate);
    nim_napi_set_object_value_int32(isolate, obj, "rx_audio_kbitrate", config.rx_audio_kbitrate);
    nim_napi_set_object_value_int32(isolate, obj, "tx_video_kbitrate", config.tx_video_kbitrate);
    nim_napi_set_object_value_int32(isolate, obj, "rx_video_kbitrate", config.rx_video_kbitrate);
    nim_napi_set_object_value_int32(isolate, obj, "up_rtt", config.up_rtt);
    nim_napi_set_object_value_int32(isolate, obj, "down_rtt", config.down_rtt);
    nim_napi_set_object_value_int32(isolate, obj, "tx_audio_packet_loss_rate", config.tx_audio_packet_loss_rate);
    nim_napi_set_object_value_int32(isolate, obj, "tx_video_packet_loss_rate", config.tx_video_packet_loss_rate);
    nim_napi_set_object_value_int32(isolate, obj, "tx_audio_packet_loss_sum", config.tx_audio_packet_loss_sum);
    nim_napi_set_object_value_int32(isolate, obj, "tx_video_packet_loss_sum", config.tx_video_packet_loss_sum);
    nim_napi_set_object_value_int32(isolate, obj, "tx_audio_jitter", config.tx_audio_jitter);
    nim_napi_set_object_value_int32(isolate, obj, "tx_video_jitter", config.tx_video_jitter);
    nim_napi_set_object_value_int32(isolate, obj, "rx_audio_packet_loss_rate", config.rx_audio_packet_loss_rate);
    nim_napi_set_object_value_int32(isolate, obj, "rx_video_packet_loss_rate", config.rx_video_packet_loss_rate);
    nim_napi_set_object_value_int32(isolate, obj, "rx_audio_packet_loss_sum", config.rx_audio_packet_loss_sum);
    nim_napi_set_object_value_int32(isolate, obj, "rx_video_packet_loss_sum", config.rx_video_packet_loss_sum);
    nim_napi_set_object_value_int32(isolate, obj, "rx_audio_jitter", config.rx_audio_jitter);
    nim_napi_set_object_value_int32(isolate, obj, "rx_video_jitter", config.rx_video_jitter);
    return napi_ok;
}

napi_status nertc_audio_send_stats_to_obj(Isolate* isolate, const nertc::NERtcAudioSendStats& config, Local<Object>& obj)
{
    nim_napi_set_object_value_int32(isolate, obj, "num_channels", config.num_channels);
    nim_napi_set_object_value_int32(isolate, obj, "sent_sample_rate", config.sent_sample_rate);
    nim_napi_set_object_value_int32(isolate, obj, "sent_bitrate", config.sent_bitrate);
    nim_napi_set_object_value_int32(isolate, obj, "audio_loss_rate", config.audio_loss_rate);
    nim_napi_set_object_value_int64(isolate, obj, "rtt", config.rtt);
    nim_napi_set_object_value_uint32(isolate, obj, "volume", config.volume);
    return napi_ok;
}

napi_status nertc_audio_recv_stats_to_obj(Isolate* isolate, const nertc::NERtcAudioRecvStats& config, Local<Object>& obj)
{
    nim_napi_set_object_value_uint64(isolate, obj, "uid", config.uid);
    nim_napi_set_object_value_int32(isolate, obj, "received_bitrate", config.received_bitrate);
    nim_napi_set_object_value_int32(isolate, obj, "total_frozen_time", config.total_frozen_time);
    nim_napi_set_object_value_int32(isolate, obj, "frozen_rate", config.frozen_rate);
    nim_napi_set_object_value_int32(isolate, obj, "audio_loss_rate", config.audio_loss_rate);
    nim_napi_set_object_value_uint32(isolate, obj, "volume", config.volume);
    return napi_ok;
}

napi_status nertc_video_send_stats_to_obj(Isolate *isolate, const nertc::NERtcVideoSendStats &config, Local<Object> &obj)
{
    nim_napi_set_object_value_uint32(isolate, obj, "video_layers_count", config.video_layers_count);
    Local<Array> s = Array::New(isolate);
    for (auto i = 0; i < (int)config.video_layers_count; i++)
    {
        Local<Object> o = Object::New(isolate);
        nim_napi_set_object_value_int32(isolate, o, "layer_type", config.video_layers_list[i].layer_type);
        nim_napi_set_object_value_int32(isolate, o, "width", config.video_layers_list[i].width);
        nim_napi_set_object_value_int32(isolate, o, "height", config.video_layers_list[i].height);
        nim_napi_set_object_value_int32(isolate, o, "capture_frame_rate", config.video_layers_list[i].capture_frame_rate);
        nim_napi_set_object_value_int32(isolate, o, "render_frame_rate", config.video_layers_list[i].render_frame_rate);
        nim_napi_set_object_value_int32(isolate, o, "encoder_frame_rate", config.video_layers_list[i].encoder_frame_rate);
        nim_napi_set_object_value_int32(isolate, o, "sent_frame_rate", config.video_layers_list[i].sent_frame_rate);
        nim_napi_set_object_value_int32(isolate, o, "sent_bitrate", config.video_layers_list[i].sent_bitrate);
        nim_napi_set_object_value_int32(isolate, o, "target_bitrate", config.video_layers_list[i].target_bitrate);
        nim_napi_set_object_value_int32(isolate, o, "encoder_bitrate", config.video_layers_list[i].encoder_bitrate);
        nim_napi_set_object_value_utf8string(isolate, o, "codec_name", config.video_layers_list[i].codec_name);
        s->Set(isolate->GetCurrentContext(), i, o);
    }
    obj->Set(isolate->GetCurrentContext(), nim_napi_new_utf8string(isolate, "video_layers_list"), s);

    return napi_ok;
}

napi_status nertc_video_recv_stats_to_obj(Isolate* isolate, const nertc::NERtcVideoRecvStats& config, Local<Object>& obj)
{
    nim_napi_set_object_value_uint32(isolate, obj, "video_layers_count", config.video_layers_count);
    nim_napi_set_object_value_uint64(isolate, obj, "uid", config.uid);
    Local<Array> s = Array::New(isolate);
    for (int index = 0; index < config.video_layers_count; index++)
    {
        Local<Object> o = Object::New(isolate);
        nim_napi_set_object_value_int32(isolate, o, "layer_type", config.video_layers_list[index].layer_type);
        nim_napi_set_object_value_int32(isolate, o, "width", config.video_layers_list[index].width);
        nim_napi_set_object_value_int32(isolate, o, "height", config.video_layers_list[index].height);
        nim_napi_set_object_value_int32(isolate, o, "received_bitrate", config.video_layers_list[index].received_bitrate);
        nim_napi_set_object_value_int32(isolate, o, "received_frame_rate", config.video_layers_list[index].received_frame_rate);
        nim_napi_set_object_value_int32(isolate, o, "decoder_frame_rate", config.video_layers_list[index].decoder_frame_rate);
        nim_napi_set_object_value_int32(isolate, o, "render_frame_rate", config.video_layers_list[index].render_frame_rate);
        nim_napi_set_object_value_int32(isolate, o, "packet_loss_rate", config.video_layers_list[index].packet_loss_rate);
        nim_napi_set_object_value_int32(isolate, o, "total_frozen_time", config.video_layers_list[index].total_frozen_time);
        nim_napi_set_object_value_int32(isolate, o, "frozen_rate", config.video_layers_list[index].frozen_rate);
        nim_napi_set_object_value_utf8string(isolate, o, "codec_name", config.video_layers_list[index].codec_name);
        s->Set(isolate->GetCurrentContext(), index, o);
    }
    obj->Set(isolate->GetCurrentContext(), nim_napi_new_utf8string(isolate, "video_layers_list"), s);
    return napi_ok;
}

napi_status nertc_network_quality_to_obj(Isolate* isolate, const nertc::NERtcNetworkQualityInfo& config, Local<Object>& obj)
{
    nim_napi_set_object_value_uint64(isolate, obj, "uid", config.uid);
    nim_napi_set_object_value_uint32(isolate, obj, "tx_quality", (uint32_t)config.tx_quality);
    nim_napi_set_object_value_uint32(isolate, obj, "rx_quality", (uint32_t)config.rx_quality);
    return napi_ok;
}

napi_status nertc_audio_volume_info_to_obj(Isolate* isolate, const nertc::NERtcAudioVolumeInfo& config, Local<Object>& obj)
{
    nim_napi_set_object_value_uint64(isolate, obj, "uid", config.uid);
    nim_napi_set_object_value_uint32(isolate, obj, "volume", (uint32_t)config.volume);
    return napi_ok;
}
}