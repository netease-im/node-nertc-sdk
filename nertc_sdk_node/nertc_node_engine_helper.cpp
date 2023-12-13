#include "nertc_node_engine_helper.h"
#include "./util/js-convert.h"
#include "./util/json/include/json.h"
#include "nertc_engine_ex.h"
#include "../shared/sdk_helper/node_api_helper.h"

using namespace nemeeting_util;
namespace nertc_node
{
napi_status nertc_engine_context_obj_to_struct(const Napi::Env& env, const Napi::Object& obj, nertc::NERtcEngineContext& context)
{
    bool out_b;
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"log_level"))))
    {
        int log_level_ = obj.Get(static_cast<napi_value>(Napi::String::New(env,"log_level"))).As<Napi::Number>().Int32Value();
        context.log_level =  (nertc::NERtcLogLevel)log_level_;
    }
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"log_file_max_size_KBytes"))))
    {
        uint32_t log_max_size_ = obj.Get(static_cast<napi_value>(Napi::String::New(env,"log_file_max_size_KBytes"))).As<Napi::Number>().Uint32Value();
        context.log_file_max_size_KBytes = log_max_size_;
    }
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"server_config"))))
    {
        std::string out;
        bool out_b;
        Napi::Object o = obj.Get(static_cast<napi_value>(Napi::String::New(env,"server_config"))).As<Napi::Object>();
        if(o.Has(static_cast<napi_value>(Napi::String::New(env,"channel_server"))))
        {
            out = o.Get(static_cast<napi_value>(Napi::String::New(env,"channel_server"))).As<Napi::String>().Utf8Value();
            memset(context.server_config.channel_server, 0, kNERtcMaxURILength);
            strncpy(context.server_config.channel_server, out.c_str(), kNERtcMaxURILength);
        }
        if(o.Has(static_cast<napi_value>(Napi::String::New(env,"statistics_server"))))
        {
            out = o.Get(static_cast<napi_value>(Napi::String::New(env,"statistics_server"))).As<Napi::String>().Utf8Value();
            memset(context.server_config.statistics_server, 0, kNERtcMaxURILength);
            strncpy(context.server_config.statistics_server, out.c_str(), kNERtcMaxURILength);
        }
        if(o.Has(static_cast<napi_value>(Napi::String::New(env,"statistics_dispatch_server"))))
        {
            out = o.Get(static_cast<napi_value>(Napi::String::New(env,"statistics_dispatch_server"))).As<Napi::String>().Utf8Value();
            memset(context.server_config.statistics_dispatch_server, 0, kNERtcMaxURILength);
            strncpy(context.server_config.statistics_dispatch_server, out.c_str(), kNERtcMaxURILength);
        }
        if(o.Has(static_cast<napi_value>(Napi::String::New(env,"statistics_backup_server"))))
        {
            out = o.Get(static_cast<napi_value>(Napi::String::New(env,"statistics_backup_server"))).As<Napi::String>().Utf8Value();
            memset(context.server_config.statistics_backup_server, 0, kNERtcMaxURILength);
            strncpy(context.server_config.statistics_backup_server, out.c_str(), kNERtcMaxURILength);
        }
        if(o.Has(static_cast<napi_value>(Napi::String::New(env,"room_server"))))
        {
            out = o.Get(static_cast<napi_value>(Napi::String::New(env,"room_server"))).As<Napi::String>().Utf8Value();
            memset(context.server_config.room_server, 0, kNERtcMaxURILength);
            strncpy(context.server_config.room_server, out.c_str(), kNERtcMaxURILength);
        }
        if(o.Has(static_cast<napi_value>(Napi::String::New(env,"compat_server"))))
        {
            out = o.Get(static_cast<napi_value>(Napi::String::New(env,"compat_server"))).As<Napi::String>().Utf8Value();
            memset(context.server_config.compat_server, 0, kNERtcMaxURILength);
            strncpy(context.server_config.compat_server, out.c_str(), kNERtcMaxURILength);
        }
        if(o.Has(static_cast<napi_value>(Napi::String::New(env,"nos_lbs_server"))))
        {
            out = o.Get(static_cast<napi_value>(Napi::String::New(env,"nos_lbs_server"))).As<Napi::String>().Utf8Value();
            memset(context.server_config.nos_lbs_server, 0, kNERtcMaxURILength);
            strncpy(context.server_config.nos_lbs_server, out.c_str(), kNERtcMaxURILength);
        }
        if(o.Has(static_cast<napi_value>(Napi::String::New(env,"nos_upload_sever"))))
        {
            out = o.Get(static_cast<napi_value>(Napi::String::New(env,"nos_upload_sever"))).As<Napi::String>().Utf8Value();
            memset(context.server_config.nos_upload_sever, 0, kNERtcMaxURILength);
            strncpy(context.server_config.nos_upload_sever, out.c_str(), kNERtcMaxURILength);
        }
        if(o.Has(static_cast<napi_value>(Napi::String::New(env,"nos_token_server"))))
        {
            out = o.Get(static_cast<napi_value>(Napi::String::New(env,"nos_token_server"))).As<Napi::String>().Utf8Value();
            memset(context.server_config.nos_token_server, 0, kNERtcMaxURILength);
            strncpy(context.server_config.nos_token_server, out.c_str(), kNERtcMaxURILength);
        }
        if(o.Has(static_cast<napi_value>(Napi::String::New(env,"cloud_proxy_server"))))
        {
            out = o.Get(static_cast<napi_value>(Napi::String::New(env,"cloud_proxy_server"))).As<Napi::String>().Utf8Value();
            memset(context.server_config.cloud_proxy_server, 0, kNERtcMaxURILength);
            strncpy(context.server_config.cloud_proxy_server, out.c_str(), kNERtcMaxURILength);
        }
        if(o.Has(static_cast<napi_value>(Napi::String::New(env,"websocket_proxy_server"))))
        {
            out = o.Get(static_cast<napi_value>(Napi::String::New(env,"websocket_proxy_server"))).As<Napi::String>().Utf8Value();
            memset(context.server_config.websocket_proxy_server, 0, kNERtcMaxURILength);
            strncpy(context.server_config.websocket_proxy_server, out.c_str(), kNERtcMaxURILength);
        }
        if(o.Has(static_cast<napi_value>(Napi::String::New(env,"quic_proxy_server"))))
        {
            out = o.Get(static_cast<napi_value>(Napi::String::New(env,"quic_proxy_server"))).As<Napi::String>().Utf8Value();
            memset(context.server_config.quic_proxy_server, 0, kNERtcMaxURILength);
            strncpy(context.server_config.quic_proxy_server, out.c_str(), kNERtcMaxURILength);
        }
        if(o.Has(static_cast<napi_value>(Napi::String::New(env,"media_proxy_server"))))
        {
            out = o.Get(static_cast<napi_value>(Napi::String::New(env,"media_proxy_server"))).As<Napi::String>().Utf8Value();
            memset(context.server_config.media_proxy_server, 0, kNERtcMaxURILength);
            strncpy(context.server_config.media_proxy_server, out.c_str(), kNERtcMaxURILength);
        }
        if(o.Has(static_cast<napi_value>(Napi::String::New(env,"use_ipv6"))))
        {
            out_b = o.Get(static_cast<napi_value>(Napi::String::New(env,"use_ipv6"))).As<Napi::Boolean>().Value();
            context.server_config.use_ipv6 = out_b;
        }
    }
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"area_code_type_"))))
    {
        int area_code_type_ = obj.Get(static_cast<napi_value>(Napi::String::New(env,"area_code_type"))).As<Napi::Number>().Int32Value();
        context.area_code_type = area_code_type_;
    }
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"video_prefer_hw_decoder"))))
    {
        out_b = obj.Get(static_cast<napi_value>(Napi::String::New(env,"video_prefer_hw_decoder"))).As<Napi::Boolean>().Value();
        context.video_prefer_hw_decoder =  out_b;
    }
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"video_prefer_hw_encoder"))))
    {
        out_b = obj.Get(static_cast<napi_value>(Napi::String::New(env,"video_prefer_hw_encoder"))).As<Napi::Boolean>().Value();
        context.video_prefer_hw_encoder =  out_b;
    }
    return napi_ok;
}

napi_status nertc_window_id_list_to_struct(const Napi::Env& env, const Napi::Object& obj, std::set<int64_t>& list)
{
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"excluded_window_list"))))
    {
        Napi::Object so = obj.Get(static_cast<napi_value>(Napi::String::New(env,"excluded_window_list"))).As<Napi::Object>();
        if(so.IsArray())
        {
            Napi::Array wl = so.As<Napi::Array>();
            for (auto i = 0; i < wl.Length(); i++)
            {
				int64_t id  = wl.Get(i).As<Napi::Number>().Int64Value();
				list.insert(id);
            }
        }
    }
    return napi_ok;      
}

napi_status nertc_rectangle_obj_to_struct(const Napi::Env& env, const Napi::Object& obj, nertc::NERtcRectangle& rect)
{
    int32_t out_i;
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"x"))))
    {
        out_i = obj.Get(static_cast<napi_value>(Napi::String::New(env,"x"))).As<Napi::Number>().Int32Value();
        rect.x = out_i;
    }
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"y"))))
    {
        out_i = obj.Get(static_cast<napi_value>(Napi::String::New(env,"y"))).As<Napi::Number>().Int32Value();
        rect.y = out_i;
    }
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"width"))))
    {
        out_i = obj.Get(static_cast<napi_value>(Napi::String::New(env,"width"))).As<Napi::Number>().Int32Value();
        rect.width = out_i;
    }
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"height"))))
    {
        out_i = obj.Get(static_cast<napi_value>(Napi::String::New(env,"height"))).As<Napi::Number>().Int32Value();
        rect.height = out_i;
    }
    return napi_ok;
}

napi_status nertc_screen_capture_params_obj_to_struct(const Napi::Env& env, const Napi::Object& obj, nertc::NERtcScreenCaptureParameters& params, std::set<int64_t>& list)
{
    int32_t out_i;
    bool out_b;
    params.excluded_window_list = nullptr;
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"profile"))))
    {
        out_i = obj.Get(static_cast<napi_value>(Napi::String::New(env,"profile"))).As<Napi::Number>().Int32Value();
        params.profile = (nertc::NERtcScreenProfileType)out_i;
    }  
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"dimensions"))))
    {
        Napi::Object so = obj.Get(static_cast<napi_value>(Napi::String::New(env,"dimensions"))).As<Napi::Object>();
        if(so.Has(static_cast<napi_value>(Napi::String::New(env,"width"))))
        {
            out_i = so.Get(static_cast<napi_value>(Napi::String::New(env,"width"))).As<Napi::Number>().Int32Value();
            params.dimensions.width = out_i;
        } 
        if(so.Has(static_cast<napi_value>(Napi::String::New(env,"height"))))
        {
            out_i = so.Get(static_cast<napi_value>(Napi::String::New(env,"height"))).As<Napi::Number>().Int32Value();
            params.dimensions.height = out_i;
        }
    }
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"frame_rate"))))
    {
        out_i = obj.Get(static_cast<napi_value>(Napi::String::New(env,"frame_rate"))).As<Napi::Number>().Int32Value();
        params.frame_rate = out_i;
    }
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"min_framerate"))))
    {
        out_i = obj.Get(static_cast<napi_value>(Napi::String::New(env,"min_framerate"))).As<Napi::Number>().Int32Value();
        params.min_framerate = out_i;
    }
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"bitrate"))))
    {
        out_i = obj.Get(static_cast<napi_value>(Napi::String::New(env,"bitrate"))).As<Napi::Number>().Int32Value();
        params.bitrate = out_i;
    }
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"min_bitrate"))))
    {
        out_i = obj.Get(static_cast<napi_value>(Napi::String::New(env,"min_bitrate"))).As<Napi::Number>().Int32Value();
        params.min_bitrate = out_i;
    }
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"capture_mouse_cursor"))))
    {
        out_b = obj.Get(static_cast<napi_value>(Napi::String::New(env,"capture_mouse_cursor"))).As<Napi::Boolean>().Value();
        params.capture_mouse_cursor = out_b;
    }
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"window_focus"))))
    {
        out_b = obj.Get(static_cast<napi_value>(Napi::String::New(env,"window_focus"))).As<Napi::Boolean>().Value();
        params.window_focus = out_b;
    }
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"prefer"))))
    {
        out_i = obj.Get(static_cast<napi_value>(Napi::String::New(env,"prefer"))).As<Napi::Number>().Int32Value();
        params.prefer = (nertc::NERtcSubStreamContentPrefer)out_i;
    }
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"degradation_preference"))))
    {
        out_i = obj.Get(static_cast<napi_value>(Napi::String::New(env,"degradation_preference"))).As<Napi::Number>().Int32Value();
        params.degradation_preference = (nertc::NERtcDegradationPreference)out_i;
    }
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"excluded_window_count"))))
    {
        out_i = obj.Get(static_cast<napi_value>(Napi::String::New(env,"excluded_window_count"))).As<Napi::Number>().Int32Value();
        params.excluded_window_count = out_i;
    }
    if (params.excluded_window_count > 0)
    {
        if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"excluded_window_list"))))
        {
            Napi::Object so = obj.Get(static_cast<napi_value>(Napi::String::New(env,"excluded_window_list"))).As<Napi::Object>();
            if(so.IsArray()){
                Napi::Array wl = so.As<Napi::Array>();
                intptr_t *wi = new intptr_t[params.excluded_window_count];
                if (wl.Length() == params.excluded_window_count)
                {
                    for (auto i = 0; i < params.excluded_window_count; i++)
                    {
						int64_t id  = wl.Get(i).As<Napi::Number>().Int64Value();
						list.insert(id);

                    }
                    // params.excluded_window_list = (void *)wi;
                }
                else
                {
                    delete[] wi;
                    wi = nullptr;
                    return napi_invalid_arg;
                }
            }
        }
    }
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"enable_high_performance"))))
    {
        out_b = obj.Get(static_cast<napi_value>(Napi::String::New(env,"enable_high_performance"))).As<Napi::Boolean>().Value();
        params.enable_high_performance = out_b;
    }
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"enable_high_light"))))
    {
        out_b = obj.Get(static_cast<napi_value>(Napi::String::New(env,"enable_high_light"))).As<Napi::Boolean>().Value();
        params.enable_high_light = out_b;
    }
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"high_light_width"))))
    {
        out_i = obj.Get(static_cast<napi_value>(Napi::String::New(env,"high_light_width"))).As<Napi::Number>().Int32Value();
        params.high_light_width = out_i;
    }
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"high_light_color"))))
    {
        out_i = obj.Get(static_cast<napi_value>(Napi::String::New(env,"high_light_color"))).As<Napi::Number>().Int32Value();
        params.high_light_color = out_i;
    }
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"high_light_length"))))
    {
        out_i = obj.Get(static_cast<napi_value>(Napi::String::New(env,"high_light_length"))).As<Napi::Number>().Int32Value();
        params.high_light_length = out_i;
    }
    return napi_ok;
}

napi_status nertc_ls_users_obj_to_struct(const Napi::Env& env, const Napi::Object& obj, nertc::NERtcLiveStreamUserTranscoding& user)
{
    int64_t out_64;
    bool out_b;
    uint32_t out_u;
    int32_t out_i;

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"uid"))))
    {
        out_64 = obj.Get(static_cast<napi_value>(Napi::String::New(env,"uid"))).As<Napi::Number>().Int64Value();
        user.uid = out_64;
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"video_push"))))
    {
        out_b = obj.Get(static_cast<napi_value>(Napi::String::New(env,"video_push"))).As<Napi::Boolean>().Value();
        user.video_push = out_b;
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"adaption"))))
    {
        out_u = obj.Get(static_cast<napi_value>(Napi::String::New(env,"adaption"))).As<Napi::Number>().Uint32Value();
        user.adaption = (nertc::NERtcLiveStreamVideoScaleMode)out_u;
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"x"))))
    {
        out_i = obj.Get(static_cast<napi_value>(Napi::String::New(env,"x"))).As<Napi::Number>().Int32Value();
        user.x = out_i;
    }
    
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"y"))))
    {
        out_i = obj.Get(static_cast<napi_value>(Napi::String::New(env,"y"))).As<Napi::Number>().Int32Value();
        user.y = out_i;
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"width"))))
    {
        out_i = obj.Get(static_cast<napi_value>(Napi::String::New(env,"width"))).As<Napi::Number>().Int32Value();
        user.width = out_i;
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"height"))))
    {
        out_i = obj.Get(static_cast<napi_value>(Napi::String::New(env,"height"))).As<Napi::Number>().Int32Value();
        user.height = out_i;
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"audio_push"))))
    {
        out_b = obj.Get(static_cast<napi_value>(Napi::String::New(env,"audio_push"))).As<Napi::Boolean>().Value();
        user.audio_push = out_b;
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"z_order"))))
    {
        out_i = obj.Get(static_cast<napi_value>(Napi::String::New(env,"z_order"))).As<Napi::Number>().Int32Value();
        user.z_order = out_i;
    }

    return napi_ok;  
}

napi_status nertc_ls_img_info_obj_to_struct(const Napi::Env& env, const Napi::Object& obj, nertc::NERtcLiveStreamImageInfo* info)
{
    std::string out;
    int32_t out_i;

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"url"))))
    {
        out = obj.Get(static_cast<napi_value>(Napi::String::New(env,"url"))).As<Napi::String>().Utf8Value();
        memset(info->url, 0, kNERtcMaxURILength);
        strncpy(info->url, out.c_str(), kNERtcMaxURILength);
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"x"))))
    {
        out_i = obj.Get(static_cast<napi_value>(Napi::String::New(env,"x"))).As<Napi::Number>().Int32Value();
        info->x = out_i;
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"y"))))
    {
        out_i = obj.Get(static_cast<napi_value>(Napi::String::New(env,"y"))).As<Napi::Number>().Int32Value();
        info->y = out_i;
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"width"))))
    {
        out_i = obj.Get(static_cast<napi_value>(Napi::String::New(env,"width"))).As<Napi::Number>().Int32Value();
        info->width = out_i;
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"height"))))
    {
        out_i = obj.Get(static_cast<napi_value>(Napi::String::New(env,"height"))).As<Napi::Number>().Int32Value();
        info->height = out_i;
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"z_order"))))
    {
        out_i = obj.Get(static_cast<napi_value>(Napi::String::New(env,"z_order"))).As<Napi::Number>().Int32Value();
        info->z_order = out_i;
    }

    return napi_ok;
}

napi_status nertc_task_config_obj_to_struct(const Napi::Env& env, const Napi::Object& obj, nertc::NERtcLiveConfig& config)
{
    int32_t out_i;
    uint32_t out_u;
    bool out_b;
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"single_video_passthrough")))){
        out_b = obj.Get(static_cast<napi_value>(Napi::String::New(env,"single_video_passthrough"))).As<Napi::Boolean>().Value();
        config.single_video_passthrough = out_b;
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"audio_bitrate"))))
    {
        out_i = obj.Get(static_cast<napi_value>(Napi::String::New(env,"audio_bitrate"))).As<Napi::Number>().Int32Value();
        config.audio_bitrate = out_i;
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"sampleRate"))))
    {
        out_i = obj.Get(static_cast<napi_value>(Napi::String::New(env,"sampleRate"))).As<Napi::Number>().Int32Value();
        config.sampleRate = (nertc::NERtcLiveStreamAudioSampleRate)out_i;
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"channels"))))
    {
        out_i = obj.Get(static_cast<napi_value>(Napi::String::New(env,"channels"))).As<Napi::Number>().Int32Value();
        config.channels = out_i;
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"audioCodecProfile"))))
    {
        out_i = obj.Get(static_cast<napi_value>(Napi::String::New(env,"audioCodecProfile"))).As<Napi::Number>().Int32Value();
        config.audioCodecProfile = (nertc::NERtcLiveStreamAudioCodecProfile)out_i;
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"interrupted_place_image")))){
        out_b = obj.Get(static_cast<napi_value>(Napi::String::New(env,"interrupted_place_image"))).As<Napi::Boolean>().Value();
        config.interrupted_place_image = out_b;
    }
    
    return napi_ok;
}

napi_status nertc_ls_layout_obj_to_struct(const Napi::Env& env, const Napi::Object& obj, nertc::NERtcLiveStreamLayout& layout)
{
    int32_t out_i;
    uint32_t out_u;
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"width"))))
    {
        out_i = obj.Get(static_cast<napi_value>(Napi::String::New(env,"width"))).As<Napi::Number>().Int32Value();
        layout.width = out_i;
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"height"))))
    {
        out_i = obj.Get(static_cast<napi_value>(Napi::String::New(env,"height"))).As<Napi::Number>().Int32Value();
        layout.height = out_i;
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"background_color"))))
    {
        out_u = obj.Get(static_cast<napi_value>(Napi::String::New(env,"background_color"))).As<Napi::Number>().Uint32Value();
        layout.background_color = out_u;
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"user_count"))))
    {
        out_u = obj.Get(static_cast<napi_value>(Napi::String::New(env,"user_count"))).As<Napi::Number>().Uint32Value();
        layout.user_count = out_u;
    }

    layout.users = nullptr;
    layout.bg_image = nullptr;
    if (layout.user_count > 0)
    {
        if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"users")))){
            Napi::Object so = obj.Get(static_cast<napi_value>(Napi::String::New(env,"users"))).As<Napi::Object>();
            if(so.IsArray()){
                layout.users = new nertc::NERtcLiveStreamUserTranscoding[layout.user_count];
                Napi::Array wl = so.As<Napi::Array>();
                if (wl.Length() != layout.user_count)
                {
                    delete[] layout.users;
                    layout.users = nullptr;
                    return napi_invalid_arg;
                }                
                for (auto i = 0; i < layout.user_count; i++)
                {
                     Napi::Object obj_i = wl.Get(i).As<Napi::Object>(); //todo
                    nertc_ls_users_obj_to_struct(env, obj_i, layout.users[i]);
                }

            }
        }

        if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"bg_image")))){
            Napi::Object so1 = obj.Get(static_cast<napi_value>(Napi::String::New(env,"bg_image"))).As<Napi::Object>();
            layout.bg_image = new nertc::NERtcLiveStreamImageInfo;
            nertc_ls_img_info_obj_to_struct(env, so1, layout.bg_image);

        }

        if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"bg_image_count"))))
        {
            out_i = obj.Get(static_cast<napi_value>(Napi::String::New(env,"bg_image_count"))).As<Napi::Number>().Int32Value();
            layout.bg_image_count = out_i;
        }
    }
    return napi_ok;
}

napi_status nertc_ls_task_info_obj_to_struct(const Napi::Env& env, const Napi::Object& obj, nertc::NERtcLiveStreamTaskInfo& info)
{
    std::string out;
    int32_t out_i;
    bool out_b;

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"task_id"))))
    {
        out = obj.Get(static_cast<napi_value>(Napi::String::New(env,"task_id"))).As<Napi::String>().Utf8Value();
        memset(info.task_id, 0, kNERtcMaxTaskIDLength);
        strncpy(info.task_id, out.c_str(), kNERtcMaxTaskIDLength);
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"stream_url")))){
        out = obj.Get(static_cast<napi_value>(Napi::String::New(env,"stream_url"))).As<Napi::String>().Utf8Value();
        memset(info.stream_url, 0, kNERtcMaxURILength);
        strncpy(info.stream_url, out.c_str(), kNERtcMaxURILength);
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"server_record_enabled")))){
        out_b = obj.Get(static_cast<napi_value>(Napi::String::New(env,"server_record_enabled"))).As<Napi::Boolean>().Value();
        info.server_record_enabled = out_b;
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"ls_mode")))){
        out_i = obj.Get(static_cast<napi_value>(Napi::String::New(env,"ls_mode"))).As<Napi::Number>().Int32Value();
        info.ls_mode = (nertc::NERtcLiveStreamMode)out_i;
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"layout")))){
        Napi::Object layout_obj = obj.Get(static_cast<napi_value>(Napi::String::New(env,"layout"))).As<Napi::Object>();
        nertc_ls_layout_obj_to_struct(env, layout_obj, info.layout);
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"config")))){
        Napi::Object config_obj = obj.Get(static_cast<napi_value>(Napi::String::New(env,"config"))).As<Napi::Object>();
        nertc_task_config_obj_to_struct(env, config_obj, info.config);
    }
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"extraInfo")))){
        out = obj.Get(static_cast<napi_value>(Napi::String::New(env,"extraInfo"))).As<Napi::String>().Utf8Value();
        memset(info.extraInfo, 0, kNERtcMaxSEIBufferLength);
        strncpy(info.extraInfo, out.c_str(), kNERtcMaxSEIBufferLength);
    }
    return napi_ok;
}


napi_status nertc_stats_to_obj(const Napi::Env env, const nertc::NERtcStats& config,  Napi::Object& obj)
{
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"cpu_app_usage")), config.cpu_app_usage);
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"cpu_idle_usage")), config.cpu_idle_usage);
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"cpu_total_usage")), config.cpu_total_usage);
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"memory_app_usage")), config.memory_app_usage);
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"memory_total_usage")), config.memory_total_usage);
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"memory_app_kbytes")), config.memory_app_kbytes);
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"total_duration")), config.total_duration);
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"tx_bytes")), config.tx_bytes);
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"rx_bytes")), config.rx_bytes);
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"tx_audio_bytes")), config.tx_audio_bytes);
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"tx_video_bytes")), config.tx_video_bytes);
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"rx_audio_bytes")), config.rx_audio_bytes);
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"rx_video_bytes")), config.rx_video_bytes);
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"tx_audio_kbitrate")), config.tx_audio_kbitrate);
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"rx_audio_kbitrate")), config.rx_audio_kbitrate);
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"tx_video_kbitrate")), config.tx_video_kbitrate);
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"rx_video_kbitrate")), config.rx_video_kbitrate);
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"up_rtt")), config.up_rtt);
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"down_rtt")), config.down_rtt);
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"tx_audio_packet_loss_rate")), config.tx_audio_packet_loss_rate);
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"tx_video_packet_loss_rate")), config.tx_video_packet_loss_rate);
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"tx_audio_packet_loss_sum")), config.tx_audio_packet_loss_sum);
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"tx_video_packet_loss_sum")), config.tx_video_packet_loss_sum);
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"tx_audio_jitter")), config.tx_audio_jitter);
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"tx_video_jitter")), config.tx_video_jitter);
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"rx_audio_packet_loss_rate")), config.rx_audio_packet_loss_rate);
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"rx_video_packet_loss_rate")), config.rx_video_packet_loss_rate);
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"rx_audio_packet_loss_sum")), config.rx_audio_packet_loss_sum);
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"rx_video_packet_loss_sum")), config.rx_video_packet_loss_sum);
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"rx_audio_jitter")), config.rx_audio_jitter);
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"rx_video_jitter")), config.rx_video_jitter);
    return napi_ok;
}

napi_status nertc_audio_send_stats_to_obj(const Napi::Env env, const nertc::NERtcAudioSendStats& config,  Napi::Object& obj)
{
    /*obj.Set(static_cast<napi_value>(Napi::String::New(env,"num_channels")), config.num_channels);
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"sent_sample_rate")), config.sent_sample_rate);
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"sent_bitrate")), config.sent_bitrate);
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"audio_loss_rate")), config.audio_loss_rate);
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"rtt")), config.rtt);
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"volume")), config.volume);*/
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"audio_layers_count")), config.audio_layers_count);
    Napi::Array s = Napi::Array::New(env);
	for (auto i = 0; i < (int)config.audio_layers_count; i++) {
		Napi::Object o = Napi::Object::New(env);
		int stream_type_temp = config.audio_layers_list[i].stream_type;
		o.Set(static_cast<napi_value>(Napi::String::New(env, "stream_type")), stream_type_temp);
		int num_channels_temp = config.audio_layers_list[i].num_channels;
		o.Set(static_cast<napi_value>(Napi::String::New(env, "num_channels")), num_channels_temp);
		int sent_sample_rate_temp = config.audio_layers_list[i].sent_sample_rate;
		o.Set(static_cast<napi_value>(Napi::String::New(env, "sent_sample_rate")), sent_sample_rate_temp);
		int sent_bitrate_temp = config.audio_layers_list[i].sent_bitrate;
		o.Set(static_cast<napi_value>(Napi::String::New(env, "sent_bitrate")), sent_bitrate_temp);
		int audio_loss_rate_temp = config.audio_layers_list[i].audio_loss_rate;
		o.Set(static_cast<napi_value>(Napi::String::New(env, "audio_loss_rate")), audio_loss_rate_temp);
		int rtt_temp = config.audio_layers_list[i].rtt;
		o.Set(static_cast<napi_value>(Napi::String::New(env, "rtt")), rtt_temp);
		int volume_temp = config.audio_layers_list[i].volume;
		o.Set(static_cast<napi_value>(Napi::String::New(env, "volume")), volume_temp);
		int cap_volume_temp = config.audio_layers_list[i].cap_volume;
		o.Set(static_cast<napi_value>(Napi::String::New(env, "cap_volume")), cap_volume_temp);
		s.Set(static_cast<napi_value>(Napi::Number::New(env, i)), o);
	}
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"audio_layers_list")), s);
    return napi_ok;
}

napi_status nertc_audio_recv_stats_to_obj(const Napi::Env env, const  nertc::NERtcAudioRecvStats& config,  Napi::Object& obj)
{
   /* obj.Set(static_cast<napi_value>(Napi::String::New(env,"uid")), config.uid);
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"received_bitrate")), config.received_bitrate);
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"total_frozen_time")), config.total_frozen_time);
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"frozen_rate")), config.frozen_rate);
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"audio_loss_rate")), config.audio_loss_rate);
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"volume")), config.volume);*/
	obj.Set(static_cast<napi_value>(Napi::String::New(env, "uid")), config.uid);
	obj.Set(static_cast<napi_value>(Napi::String::New(env, "audio_layers_count")), config.audio_layers_count);
	Napi::Array s = Napi::Array::New(env);
	for (auto i = 0; i < (int)config.audio_layers_count; i++) {
		Napi::Object o = Napi::Object::New(env);
		int stream_type_temp = config.audio_layers_list[i].stream_type;
		o.Set(static_cast<napi_value>(Napi::String::New(env, "stream_type")), stream_type_temp);
		int received_bitrate_temp = config.audio_layers_list[i].received_bitrate;
		o.Set(static_cast<napi_value>(Napi::String::New(env, "received_bitrate")), received_bitrate_temp);
		int total_frozen_time_temp = config.audio_layers_list[i].total_frozen_time;
		o.Set(static_cast<napi_value>(Napi::String::New(env, "total_frozen_time")), total_frozen_time_temp);
		int frozen_rate_temp = config.audio_layers_list[i].frozen_rate;
		o.Set(static_cast<napi_value>(Napi::String::New(env, "frozen_rate")), frozen_rate_temp);
		int audio_loss_rate_temp = config.audio_layers_list[i].audio_loss_rate;
		o.Set(static_cast<napi_value>(Napi::String::New(env, "audio_loss_rate")), audio_loss_rate_temp);
		int volume_temp = config.audio_layers_list[i].volume;
		o.Set(static_cast<napi_value>(Napi::String::New(env, "volume")), volume_temp);
        int av_timestamp_diff = config.audio_layers_list[i].av_timestamp_diff;
		o.Set(static_cast<napi_value>(Napi::String::New(env, "av_timestamp_diff")), av_timestamp_diff);
        int peer_to_peer_delay = config.audio_layers_list[i].peer_to_peer_delay;
		o.Set(static_cast<napi_value>(Napi::String::New(env, "peer_to_peer_delay")), peer_to_peer_delay);
		s.Set(static_cast<napi_value>(Napi::Number::New(env, i)), o);
	}
	obj.Set(static_cast<napi_value>(Napi::String::New(env, "audio_layers_list")), s);
    return napi_ok;
}

napi_status nertc_video_send_stats_to_obj(const Napi::Env env, const nertc::NERtcVideoSendStats& config,  Napi::Object& obj)
{
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"video_layers_count")), config.video_layers_count);
    Napi::Array s = Napi::Array::New(env);
    for (auto i = 0; i < (int)config.video_layers_count; i++)
    {
        Napi::Object o = Napi::Object::New(env);
        o.Set(static_cast<napi_value>(Napi::String::New(env,"layer_type")), config.video_layers_list[i].layer_type);
        o.Set(static_cast<napi_value>(Napi::String::New(env,"width")), config.video_layers_list[i].width);
        o.Set(static_cast<napi_value>(Napi::String::New(env,"height")), config.video_layers_list[i].height);
        o.Set(static_cast<napi_value>(Napi::String::New(env,"capture_width")), config.video_layers_list[i].capture_width);
        o.Set(static_cast<napi_value>(Napi::String::New(env,"capture_height")), config.video_layers_list[i].capture_height);
        o.Set(static_cast<napi_value>(Napi::String::New(env,"capture_frame_rate")), config.video_layers_list[i].capture_frame_rate);
        o.Set(static_cast<napi_value>(Napi::String::New(env,"render_frame_rate")), config.video_layers_list[i].render_frame_rate);
        o.Set(static_cast<napi_value>(Napi::String::New(env,"encoder_frame_rate")), config.video_layers_list[i].encoder_frame_rate);
        o.Set(static_cast<napi_value>(Napi::String::New(env,"sent_frame_rate")), config.video_layers_list[i].sent_frame_rate);
        o.Set(static_cast<napi_value>(Napi::String::New(env,"sent_bitrate")), config.video_layers_list[i].sent_bitrate);
        o.Set(static_cast<napi_value>(Napi::String::New(env,"target_bitrate")), config.video_layers_list[i].target_bitrate);
        o.Set(static_cast<napi_value>(Napi::String::New(env,"encoder_bitrate")), config.video_layers_list[i].encoder_bitrate);
        o.Set(static_cast<napi_value>(Napi::String::New(env,"codec_name")), config.video_layers_list[i].codec_name);
        o.Set(static_cast<napi_value>(Napi::String::New(env,"drop_bandwidth_strategy_enabled")), config.video_layers_list[i].drop_bandwidth_strategy_enabled);
        s.Set(static_cast<napi_value>(Napi::Number::New(env, i)),  o);
    }
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"video_layers_list")), s);
    return napi_ok;
}

napi_status nertc_audio_effect_option_obj_to_struct(const Napi::Env& env, const Napi::Object& obj, nertc::NERtcCreateAudioEffectOption* option)
{
    std::string out;
    uint32_t out_u;
    int32_t out_i;
    bool out_b;
    Napi::Array objs = obj.As<Napi::Array>();
    for (size_t i = 0; i < objs.Length(); i++)
    {
        Napi::Object obj = objs.Get(i).As<Napi::Object>();
        if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"path"))))
        {
            out = obj.Get(static_cast<napi_value>(Napi::String::New(env,"path"))).As<Napi::String>().Utf8Value();
            // 实际内容长度超出了可以容纳的缓冲区长度，缓冲区要包含 \0 结尾字符串，实际内容需要小于缓冲区大小
            if (out.length() >= kNERtcMaxURILength)
                return napi_invalid_arg;
            memset(option[i].path, 0, kNERtcMaxURILength);
            strncpy(option[i].path, out.c_str(), kNERtcMaxURILength);
        }
        if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"loop_count"))))
        {
            out_i = obj.Get(static_cast<napi_value>(Napi::String::New(env,"loop_count"))).As<Napi::Number>().Int32Value();
            option[i].loop_count = out_i;
        }
        if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"send_enabled"))))
        {
            out_b = obj.Get(static_cast<napi_value>(Napi::String::New(env,"send_enabled"))).As<Napi::Boolean>().Value();
            option[i].send_enabled = out_b;
        }
        if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"send_volume"))))
        {
            out_u = obj.Get(static_cast<napi_value>(Napi::String::New(env,"send_volume"))).As<Napi::Number>().Uint32Value();
            option[i].send_volume = out_u;
        }
        if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"playback_enabled"))))
        {
            out_b = obj.Get(static_cast<napi_value>(Napi::String::New(env,"playback_enabled"))).As<Napi::Boolean>().Value();
            option[i].playback_enabled = out_b;
        }
        if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"playback_volume"))))
        {
            out_u = obj.Get(static_cast<napi_value>(Napi::String::New(env,"playback_volume"))).As<Napi::Number>().Uint32Value();
            option[i].playback_volume = out_u;
        }
        if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"start_timestamp"))))
        {
            out_i = obj.Get(static_cast<napi_value>(Napi::String::New(env,"start_timestamp"))).As<Napi::Number>().Int32Value();
            option[i].start_timestamp = out_i;
        }
        if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"send_with_audio_type"))))
        {
            out_i = obj.Get(static_cast<napi_value>(Napi::String::New(env,"send_with_audio_type"))).As<Napi::Number>().Int32Value();
            option[i].send_with_audio_type = (nertc::NERtcAudioStreamType)out_i;
        }
        if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"progress_interval"))))
        {
            out_i = obj.Get(static_cast<napi_value>(Napi::String::New(env,"progress_interval"))).As<Napi::Number>().Int32Value();
            option[i].progress_interval = out_i;
        }
    }
    return napi_ok;
}

napi_status nertc_audio_mixing_option_obj_to_struct(const Napi::Env& env, const Napi::Object& obj, nertc::NERtcCreateAudioMixingOption& option)
{
    std::string out;
    int64_t out64;
    uint32_t out_u;
    int32_t out_i;
    bool out_b;
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"path"))))
    {
        out = obj.Get(static_cast<napi_value>(Napi::String::New(env,"path"))).As<Napi::String>().Utf8Value();
        // 实际内容长度超出了可以容纳的缓冲区长度，缓冲区要包含 \0 结尾字符串，实际内容需要小于缓冲区大小
        if (out.length() >= kNERtcMaxURILength)
            return napi_invalid_arg;
        memset(option.path, 0, kNERtcMaxURILength);
        strncpy(option.path, out.c_str(), kNERtcMaxURILength);
    }
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"loop_count"))))
    {
        out_i = obj.Get(static_cast<napi_value>(Napi::String::New(env,"loop_count"))).As<Napi::Number>().Int32Value();
        option.loop_count = out_i;
    }
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"send_enabled"))))
    {
        out_b = obj.Get(static_cast<napi_value>(Napi::String::New(env,"send_enabled"))).As<Napi::Boolean>().Value();
        option.send_enabled = out_b;
    }  
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"send_volume"))))
    {
        out_u = obj.Get(static_cast<napi_value>(Napi::String::New(env,"send_volume"))).As<Napi::Number>().Uint32Value();
        option.send_volume = out_u;
    }
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"playback_enabled"))))
    {
        out_b = obj.Get(static_cast<napi_value>(Napi::String::New(env,"playback_enabled"))).As<Napi::Boolean>().Value();
        option.playback_enabled = out_b;
    }
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"playback_volume"))))
    {
        out_u = obj.Get(static_cast<napi_value>(Napi::String::New(env,"playback_volume"))).As<Napi::Number>().Uint32Value();
        option.playback_volume = out_u;
    }
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"start_timestamp"))))
    {
        out64 = obj.Get(static_cast<napi_value>(Napi::String::New(env,"start_timestamp"))).As<Napi::Number>().Int64Value();
        option.start_timestamp = out64;
    }
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"send_with_audio_type"))))
    {
        out_u = obj.Get(static_cast<napi_value>(Napi::String::New(env,"send_with_audio_type"))).As<Napi::Number>().Int64Value();
        option.send_with_audio_type = (nertc::NERtcAudioStreamType)out_u;
    }
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"progress_interval"))))
    {
        out_u = obj.Get(static_cast<napi_value>(Napi::String::New(env,"progress_interval"))).As<Napi::Number>().Int64Value();
        option.progress_interval = out_u;
    }
    return napi_ok;
}

napi_status nertc_audio_frame_rf_obj_to_struct(const Napi::Env& env, const Napi::Object& obj, nertc::NERtcAudioFrameRequestFormat& format)
{
    uint32_t out_u;
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"channels"))))
    {
        out_u = obj.Get(static_cast<napi_value>(Napi::String::New(env,"channels"))).As<Napi::Number>().Uint32Value();
        format.channels = out_u;
    }
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"sample_rate"))))
    {
        out_u = obj.Get(static_cast<napi_value>(Napi::String::New(env,"sample_rate"))).As<Napi::Number>().Uint32Value();
        format.sample_rate = out_u;
    } 
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"mode"))))
    {
        out_u = obj.Get(static_cast<napi_value>(Napi::String::New(env,"mode"))).As<Napi::Number>().Uint32Value();
        format.mode = (nertc::NERtcRawAudioFrameOpModeType)out_u;
    } 
    return napi_ok; 
}

napi_status nertc_video_config_obj_to_struct(const Napi::Env& env, const Napi::Object& obj, nertc::NERtcVideoConfig& config)
{
    uint32_t out_u;
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"max_profile"))))
    {
        out_u = obj.Get(static_cast<napi_value>(Napi::String::New(env,"max_profile"))).As<Napi::Number>().Uint32Value();
        config.max_profile = (nertc::NERtcVideoProfileType)out_u;
    }
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"width"))))
    {
        out_u = obj.Get(static_cast<napi_value>(Napi::String::New(env,"width"))).As<Napi::Number>().Uint32Value();
        config.width = out_u;
    }
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"height"))))
    {
        out_u = obj.Get(static_cast<napi_value>(Napi::String::New(env,"height"))).As<Napi::Number>().Uint32Value();
        config.height = out_u;
    }
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"crop_mode"))))
    {
        out_u = obj.Get(static_cast<napi_value>(Napi::String::New(env,"crop_mode"))).As<Napi::Number>().Uint32Value();
        config.crop_mode_ = (nertc::NERtcVideoCropMode)out_u;
    }
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"framerate"))))
    {
        out_u = obj.Get(static_cast<napi_value>(Napi::String::New(env,"framerate"))).As<Napi::Number>().Uint32Value();
        config.framerate = (nertc::NERtcVideoFramerateType)out_u;
    }
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"min_framerate"))))
    {
        out_u = obj.Get(static_cast<napi_value>(Napi::String::New(env,"min_framerate"))).As<Napi::Number>().Uint32Value();
        config.min_framerate = (nertc::NERtcVideoFramerateType)out_u;
    }
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"bitrate"))))
    {
        out_u = obj.Get(static_cast<napi_value>(Napi::String::New(env,"bitrate"))).As<Napi::Number>().Uint32Value();
        config.bitrate = out_u;
    }
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"min_bitrate"))))
    {
        out_u = obj.Get(static_cast<napi_value>(Napi::String::New(env,"min_bitrate"))).As<Napi::Number>().Uint32Value();
        config.min_bitrate = out_u;
    }
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"degradation_preference"))))
    {
        out_u = obj.Get(static_cast<napi_value>(Napi::String::New(env,"degradation_preference"))).As<Napi::Number>().Uint32Value();
        config.degradation_preference = (nertc::NERtcDegradationPreference)out_u;
    }
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"mirror_mode"))))
    {
        out_u = obj.Get(static_cast<napi_value>(Napi::String::New(env,"mirror_mode"))).As<Napi::Number>().Uint32Value();
        config.mirror_mode = (nertc::NERtcVideoMirrorMode)out_u;
    }
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"orientation_mode"))))
    {
        out_u = obj.Get(static_cast<napi_value>(Napi::String::New(env,"orientation_mode"))).As<Napi::Number>().Uint32Value();
        config.orientation_mode = (nertc::NERtcVideoOutputOrientationMode)out_u;
    }
    return napi_ok;
}

napi_status nertc_channel_media_relay_obj_to_struct(const Napi::Env& env, const Napi::Object& obj, nertc::NERtcChannelMediaRelayInfo& config)
{
    std::string out;
    int64_t out_64;
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"channel_name"))))
    {
        out = obj.Get(static_cast<napi_value>(Napi::String::New(env,"channel_name"))).As<Napi::String>().Utf8Value();
        memset((void *)(config.channel_name), 0, kNERtcMaxChannelNameLength);
        strncpy((char *)(config.channel_name), out.c_str(), kNERtcMaxChannelNameLength);
    }
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"channel_token"))))
    {
        out = obj.Get(static_cast<napi_value>(Napi::String::New(env,"channel_token"))).As<Napi::String>().Utf8Value();
        memset((void *)(config.channel_token), 0, kNERtcMaxTokenLength);
        strncpy((char *)(config.channel_token), out.c_str(), kNERtcMaxTokenLength);
    }
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"uid"))))
    {
        out_64 = obj.Get(static_cast<napi_value>(Napi::String::New(env,"uid"))).As<Napi::Number>().Int64Value();
        config.uid = out_64;
    }
    return napi_ok;
}

napi_status nertc_lastmile_probe_obj_to_struct(const Napi::Env& env, const Napi::Object& obj, nertc::NERtcLastmileProbeConfig& config)
{
    int32_t out_32;
    bool enable;
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"probe_uplink"))))
    {
        enable = obj.Get(static_cast<napi_value>(Napi::String::New(env,"probe_uplink"))).As<Napi::Boolean>().Value();
        config.probe_uplink = enable;
    }
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"probe_downlink"))))
    {
        enable = obj.Get(static_cast<napi_value>(Napi::String::New(env,"probe_downlink"))).As<Napi::Boolean>().Value();
        config.probe_downlink = enable;
    }
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"expected_uplink_bitratebps"))))
    {
        out_32 = obj.Get(static_cast<napi_value>(Napi::String::New(env,"expected_uplink_bitratebps"))).As<Napi::Number>().Int32Value();
        config.expected_uplink_bitratebps = out_32;
    }
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"expected_downlink_bitratebps"))))
    {
        out_32 = obj.Get(static_cast<napi_value>(Napi::String::New(env,"expected_downlink_bitratebps"))).As<Napi::Number>().Int32Value();
        config.expected_downlink_bitratebps = out_32;
    }
    return napi_ok;
}

napi_status nertc_join_channel_option_to_struct(const Napi::Env& env, const Napi::Object& obj, nertc::NERtcJoinChannelOptions& config)
{
    std::string out;
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"custom_info"))))
    {
        out = obj.Get(static_cast<napi_value>(Napi::String::New(env,"custom_info"))).As<Napi::String>().Utf8Value();
        memset(config.custom_info, 0, kNERtcCustomInfoLength);
        strncpy(config.custom_info, out.c_str(), kNERtcCustomInfoLength);
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"permission_key"))))
    {
        out = obj.Get(static_cast<napi_value>(Napi::String::New(env,"permission_key"))).As<Napi::String>().Utf8Value();
        const char* cstr  = out.c_str();
        char* ptr = new char[out.length() + 1];
        std::strcpy(ptr, cstr);
        ptr[out.length()] = '\0';  
        config.permission_key = ptr;
    }
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"range_audio_info"))))
    {
        Napi::Object so = obj.Get(static_cast<napi_value>(Napi::String::New(env,"range_audio_info"))).As<Napi::Object>();
        
        if(so.Has(static_cast<napi_value>(Napi::String::New(env,"team_id"))))
        {
            int team_id_ = obj.Get(static_cast<napi_value>(Napi::String::New(env,"team_id"))).As<Napi::Number>().Int32Value();
            config.range_audio_info.team_id = team_id_;
        }
    
        if(so.Has(static_cast<napi_value>(Napi::String::New(env,"mode"))))
        {
            int mode_ = obj.Get(static_cast<napi_value>(Napi::String::New(env,"mode"))).As<Napi::Number>().Int32Value();
            config.range_audio_info.mode = (nertc::NERtcRangeAudioMode)mode_;
        }
    
        if(so.Has(static_cast<napi_value>(Napi::String::New(env,"audible_distance"))))
        {
            int audible_distance_ = obj.Get(static_cast<napi_value>(Napi::String::New(env,"audible_distance"))).As<Napi::Number>().Int32Value();
            config.range_audio_info.audible_distance = audible_distance_;
        }
    }
    return napi_ok;
}

napi_status nertc_channel_optionex_to_struct(const Napi::Env& env, const Napi::Object& obj, nertc::NERtcJoinChannelOptionsEx& config)
{
    std::string out;
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"custom_info"))))
    {
        out = obj.Get(static_cast<napi_value>(Napi::String::New(env,"custom_info"))).As<Napi::String>().Utf8Value();
        memset(config.custom_info, 0, kNERtcCustomInfoLength);
        strncpy(config.custom_info, out.c_str(), kNERtcCustomInfoLength);
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"permission_key"))))
    {
        out = obj.Get(static_cast<napi_value>(Napi::String::New(env,"permission_key"))).As<Napi::String>().Utf8Value();
        const char* cstr  = out.c_str();
        char* ptr = new char[out.length() + 1];
        std::strcpy(ptr, cstr);
        ptr[out.length()] = '\0';  
        config.permission_key = ptr;
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"team_id"))))
    {
        int team_id_ = obj.Get(static_cast<napi_value>(Napi::String::New(env,"team_id"))).As<Napi::Number>().Int32Value();
        config.team_id = team_id_;
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"mode"))))
    {
        int mode_ = obj.Get(static_cast<napi_value>(Napi::String::New(env,"mode"))).As<Napi::Number>().Int32Value();
        config.mode = (nertc::NERtcRangeAudioMode)mode_;
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"audible_distance"))))
    {
        int audible_distance_ = obj.Get(static_cast<napi_value>(Napi::String::New(env,"audible_distance"))).As<Napi::Number>().Int32Value();
        config.audible_distance = audible_distance_;
    }

    return napi_ok;
}

napi_status nertc_recording_option_to_struct(const Napi::Env& env, const Napi::Object& obj, nertc::NERtcAudioRecordingConfiguration& config)
{
    std::string out;
    int32_t out_32;

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"filePath"))))
    {
        out = obj.Get(static_cast<napi_value>(Napi::String::New(env,"filePath"))).As<Napi::String>().Utf8Value();
        memset(config.filePath, 0, kNERtcMaxURILength);
        strncpy(config.filePath, out.c_str(), kNERtcMaxURILength);
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"sampleRate"))))
    {
        out_32 = obj.Get(static_cast<napi_value>(Napi::String::New(env,"sampleRate"))).As<Napi::Number>().Int32Value();
        config.sampleRate = out_32;
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"quality"))))
    {
        out_32 = obj.Get(static_cast<napi_value>(Napi::String::New(env,"quality"))).As<Napi::Number>().Int32Value();
        config.quality = (nertc::NERtcAudioRecordingQuality)out_32;
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"position"))))
    {
        out_32 = obj.Get(static_cast<napi_value>(Napi::String::New(env,"position"))).As<Napi::Number>().Int32Value();
        config.position = (nertc::NERtcAudioRecordingPosition)out_32;
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"cycleTime"))))
    {
        out_32 = obj.Get(static_cast<napi_value>(Napi::String::New(env,"cycleTime"))).As<Napi::Number>().Int32Value();
        config.cycleTime = (nertc::NERtcAudioRecordingCycleTime)out_32;
    }

    return napi_ok;
}

napi_status nertc_virtual_background_option_to_struct(const Napi::Env& env, const Napi::Object& obj, nertc::VirtualBackgroundSource& config)
{
    std::string out;
    int32_t out_32;

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"background_source_type"))))
    {
        out_32 = obj.Get(static_cast<napi_value>(Napi::String::New(env,"background_source_type"))).As<Napi::Number>().Int32Value();
        config.background_source_type = (nertc::VirtualBackgroundSource::NERtcBackgroundSourceType)out_32;
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"color"))))
    {
        out_32 = obj.Get(static_cast<napi_value>(Napi::String::New(env,"color"))).As<Napi::Number>().Int32Value();
        config.color = out_32;
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"source"))))
    {
        out = obj.Get(static_cast<napi_value>(Napi::String::New(env,"source"))).As<Napi::String>().Utf8Value();
        const char* cstr  = out.c_str();
        char* ptr = new char[out.length() + 1];
        std::strcpy(ptr, cstr);
        ptr[out.length()] = '\0';  
        config.source = ptr;
    }
    return napi_ok;
}

napi_status nertc_spatializer_room_property_to_struct(const Napi::Env& env, const Napi::Object& obj, nertc::NERtcSpatializerRoomProperty& config)
{
    int32_t out_32;
    float out_f;
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"room_capacity"))))
    {
        out_32 = obj.Get(static_cast<napi_value>(Napi::String::New(env,"room_capacity"))).As<Napi::Number>().Int32Value();
        config.room_capacity = (nertc::NERtcSpatializerRoomCapacity)out_32;
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"material"))))
    {
        out_32 = obj.Get(static_cast<napi_value>(Napi::String::New(env,"material"))).As<Napi::Number>().Int32Value();
        config.material = (nertc::NERtcSpatializerMaterialName)out_32;
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"reflection_scalar"))))
    {
        out_f = obj.Get(static_cast<napi_value>(Napi::String::New(env,"reflection_scalar"))).As<Napi::Number>().FloatValue();
        config.reflection_scalar = out_f;
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"reverb_gain"))))
    {
        out_f = obj.Get(static_cast<napi_value>(Napi::String::New(env,"reverb_gain"))).As<Napi::Number>().FloatValue();
        config.reverb_gain = out_f;
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"reverb_time"))))
    {
        out_f = obj.Get(static_cast<napi_value>(Napi::String::New(env,"reverb_time"))).As<Napi::Number>().FloatValue();
        config.reverb_time = out_f;
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"reverb_brightness"))))
    {
        out_f = obj.Get(static_cast<napi_value>(Napi::String::New(env,"reverb_brightness"))).As<Napi::Number>().FloatValue();
        config.reverb_brightness = out_f;
    }
    return napi_ok;
}

napi_status nertc_position_info_to_struct(const Napi::Env& env, const Napi::Object& obj, nertc::NERtcPositionInfo& info)
{
    float out_f;
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"speaker_position"))))
    {
		Napi::Array objs = obj.Get(static_cast<napi_value>(Napi::String::New(env, "speaker_position"))).As<Napi::Array>();
        for (size_t i = 0; i < objs.Length(); i++) 
        {
            out_f = objs.Get(i).As<Napi::Number>().FloatValue();
            info.speaker_position[i] = out_f;
        }
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"speaker_quaternion"))))
    {
		Napi::Array objs = obj.Get(static_cast<napi_value>(Napi::String::New(env, "speaker_quaternion"))).As<Napi::Array>();
        for (size_t i = 0; i < objs.Length(); i++) 
        {
            out_f = objs.Get(i).As<Napi::Number>().FloatValue();
            info.speaker_quaternion[i] = out_f;
        }
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"head_position"))))
    {
		Napi::Array objs = obj.Get(static_cast<napi_value>(Napi::String::New(env, "head_position"))).As<Napi::Array>();
        for (size_t i = 0; i < objs.Length(); i++) 
        {
            out_f = objs.Get(i).As<Napi::Number>().FloatValue();
            info.head_position[i] = out_f;
        }
    }
    
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"head_quaternion"))))
    {
		Napi::Array objs = obj.Get(static_cast<napi_value>(Napi::String::New(env, "head_quaternion"))).As<Napi::Array>();
        for (size_t i = 0; i < objs.Length(); i++) 
        {
            out_f = objs.Get(i).As<Napi::Number>().FloatValue();
            info.head_quaternion[i] = out_f;
        }
    }
    return napi_ok;
}

// napi_status nertc_screen_capture_source_info_to_struct(const Napi::Env& env, const Napi::Object& obj, nertc::NERtcScreenCaptureSourceInfo& source)
// {
//     // int out_i;
//     // if(obj.has(static_cast<napi_value>(Napi::String::New(env,"type"))))
//     // {
//     //     out_i = obj.get(static_cast<napi_value>(Napi::String::New(env,"type"))).as<napi_number>().int32value();
//     //     source.type = (nertc::NERtcScreenCaptureSourceType)out_i;
//     // }
//     // if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"source_id"))))
//     // {
//     //     int64_t * source_id_ptr = new int64_t;
//     //     *source_id_ptr = obj.get(static_cast<napi_value>(Napi::String::New(env,"source_id"))).as<napi_number>().int64value();
//     // }
    
//     return napi_ok;
// }

napi_status nertc_screen_size_info_to_struct(const Napi::Env& env, const Napi::Object& obj, nertc::NERtcSize& size)
{
    int32_t out_i;
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"width"))))
    {
        out_i = obj.Get(static_cast<napi_value>(Napi::String::New(env,"width"))).As<Napi::Number>().Int32Value();
        size.width = out_i;
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"height"))))
    {
        out_i = obj.Get(static_cast<napi_value>(Napi::String::New(env,"height"))).As<Napi::Number>().Int32Value();
        size.height = out_i;
    }
    return napi_ok;    
}

napi_status nertc_video_water_mark_image_to_struct(const Napi::Env& env, const Napi::Object& obj, nertc::NERtcVideoWatermarkImageConfig& config)
{
    bool enable;
    uint32_t out_u;
    float out_f;
    int32_t out_i;
    std::string out;
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"image_paths"))))//list
    {
		Napi::Array objs = obj.Get(static_cast<napi_value>(Napi::String::New(env, "image_paths"))).As<Napi::Array>();
        for (size_t i = 0; i < objs.Length(); i++) 
        {
            out = objs.Get(i).As<Napi::String>().Utf8Value();
            memset(config.image_paths[i], 0, kNERtcMaxURILength);
            strncpy(config.image_paths[i], out.c_str(), kNERtcMaxURILength);
        }
    }
    
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"wm_alpha"))))
    {
        out_f = obj.Get(static_cast<napi_value>(Napi::String::New(env,"wm_alpha"))).As<Napi::Number>().FloatValue();
        config.wm_alpha = out_f;
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"wm_width"))))
    {
        out_i = obj.Get(static_cast<napi_value>(Napi::String::New(env,"wm_width"))).As<Napi::Number>().Int32Value();
        config.wm_width = out_i;
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"wm_height"))))
    {
        out_i = obj.Get(static_cast<napi_value>(Napi::String::New(env,"wm_height"))).As<Napi::Number>().Int32Value();
        config.wm_height = out_i;
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"offset_x"))))
    {
        out_i = obj.Get(static_cast<napi_value>(Napi::String::New(env,"offset_x"))).As<Napi::Number>().Int32Value();
        config.offset_x = out_i;
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"offset_y"))))
    {
        out_i = obj.Get(static_cast<napi_value>(Napi::String::New(env,"offset_y"))).As<Napi::Number>().Int32Value();
        config.offset_y = out_i;
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"fps"))))
    {
        out_u = obj.Get(static_cast<napi_value>(Napi::String::New(env,"fps"))).As<Napi::Number>().Uint32Value();
        config.fps = out_u;
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"loop"))))
    {
        enable = obj.Get(static_cast<napi_value>(Napi::String::New(env,"loop"))).As<Napi::Boolean>().Value();
        config.loop = enable;
    }

    return napi_ok;
}

napi_status nertc_video_water_mark_txt_to_struct(const Napi::Env& env, const Napi::Object& obj, nertc::NERtcVideoWatermarkTextConfig& config)
{
    float out_f;
    int32_t out_i;
    std::string out;
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"content"))))
    {
		out = obj.Get(static_cast<napi_value>(Napi::String::New(env, "content"))).As<Napi::String>().Utf8Value();;
        memset(config.content, 0, kNERtcMaxBuffLength);
        strncpy(config.content, out.c_str(), kNERtcMaxBuffLength);
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"font_name"))))
    {
		out = obj.Get(static_cast<napi_value>(Napi::String::New(env, "font_name"))).As<Napi::String>().Utf8Value();;
        memset(config.font_name, 0, kNERtcMaxBuffLength);
        strncpy(config.font_name, out.c_str(), kNERtcMaxBuffLength);
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"font_color"))))
    {
        out = obj.Get(static_cast<napi_value>(Napi::String::New(env,"font_color"))).As<Napi::String>().Utf8Value();
        uint32_t nValude = 0;
        sscanf(out.c_str(), "%x", &nValude);
        config.font_color = nValude;
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"font_size"))))
    {
        out_i = obj.Get(static_cast<napi_value>(Napi::String::New(env,"font_size"))).As<Napi::Number>().Int32Value();
        config.font_size = out_i;
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"wm_color"))))
    {
        out = obj.Get(static_cast<napi_value>(Napi::String::New(env,"wm_color"))).As<Napi::String>().Utf8Value();
        uint32_t nValude = 0;
        sscanf(out.c_str(), "%x", &nValude);
        config.wm_color = nValude;
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"wm_alpha"))))
    {
        out_f = obj.Get(static_cast<napi_value>(Napi::String::New(env,"wm_alpha"))).As<Napi::Number>().FloatValue();
        config.wm_alpha = out_f;
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"wm_width"))))
    {
        out_i = obj.Get(static_cast<napi_value>(Napi::String::New(env,"wm_width"))).As<Napi::Number>().Int32Value();
        config.wm_width = out_i;
    }
    
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"wm_height"))))
    {
        out_i = obj.Get(static_cast<napi_value>(Napi::String::New(env,"wm_height"))).As<Napi::Number>().Int32Value();
        config.wm_height = out_i;
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"offset_x"))))
    {
        out_i = obj.Get(static_cast<napi_value>(Napi::String::New(env,"offset_x"))).As<Napi::Number>().Int32Value();
        config.offset_x = out_i;
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"offset_y"))))
    {
        out_i = obj.Get(static_cast<napi_value>(Napi::String::New(env,"offset_y"))).As<Napi::Number>().Int32Value();
        config.offset_y = out_i;
    }

    return napi_ok;
}

napi_status nertc_video_water_mark_time_to_struct(const Napi::Env& env, const Napi::Object& obj, nertc::NERtcVideoWatermarkTimestampConfig& config)
{
    float out_f;
    int32_t out_i;
    std::string out;
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"font_name"))))
    {
		out = obj.Get(static_cast<napi_value>(Napi::String::New(env, "font_name"))).As<Napi::String>().Utf8Value();;
        memset(config.font_name, 0, kNERtcMaxURILength);
        strncpy(config.font_name, out.c_str(), kNERtcMaxURILength);
    }
    
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"font_color"))))
    {
        out = obj.Get(static_cast<napi_value>(Napi::String::New(env,"font_color"))).As<Napi::String>().Utf8Value();
        uint32_t nValude = 0;
        sscanf(out.c_str(), "%x", &nValude);
        config.font_color = nValude;
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"font_size"))))
    {
        out_i = obj.Get(static_cast<napi_value>(Napi::String::New(env,"font_size"))).As<Napi::Number>().Int32Value();
        config.font_size = out_i;
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"wm_color"))))
    {
        out = obj.Get(static_cast<napi_value>(Napi::String::New(env,"wm_color"))).As<Napi::String>().Utf8Value();
        uint32_t nValude = 0;
        sscanf(out.c_str(), "%x", &nValude);
        config.wm_color = nValude;
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"wm_alpha"))))
    {
        out_f = obj.Get(static_cast<napi_value>(Napi::String::New(env,"wm_alpha"))).As<Napi::Number>().FloatValue();
        config.wm_alpha = out_f;
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"wm_width"))))
    {
        out_i = obj.Get(static_cast<napi_value>(Napi::String::New(env,"wm_width"))).As<Napi::Number>().Int32Value();
        config.wm_width = out_i;
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"wm_height"))))
    {
        out_i = obj.Get(static_cast<napi_value>(Napi::String::New(env,"wm_height"))).As<Napi::Number>().Int32Value();
        config.wm_height = out_i;
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"offset_x"))))
    {
        out_i = obj.Get(static_cast<napi_value>(Napi::String::New(env,"offset_x"))).As<Napi::Number>().Int32Value();
        config.offset_x = out_i;
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"offset_y"))))
    {
        out_i = obj.Get(static_cast<napi_value>(Napi::String::New(env,"offset_y"))).As<Napi::Number>().Int32Value();
        config.offset_y = out_i;
    }
    return napi_ok;
}

napi_status nertc_video_water_mark_config_to_struct(const Napi::Env& env, const Napi::Object& obj, nertc::NERtcVideoWatermarkConfig& config)
{
    int32_t out_i;
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"watermark_type"))))
    {
        out_i = obj.Get(static_cast<napi_value>(Napi::String::New(env,"watermark_type"))).As<Napi::Number>().Int32Value();
        config.watermark_type = (nertc::NERtcVideoWatermarkConfig::NERtcWatermarkType)out_i;
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"image_watermarks")))) {
        Napi::Object o = obj.Get(static_cast<napi_value>(Napi::String::New(env,"image_watermarks"))).As<Napi::Object>();
        nertc_video_water_mark_image_to_struct(env, o, config.image_watermarks);
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"text_watermarks")))) {
        Napi::Object o = obj.Get(static_cast<napi_value>(Napi::String::New(env,"text_watermarks"))).As<Napi::Object>();
        nertc_video_water_mark_txt_to_struct(env, o, config.text_watermarks);
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"timestamp_watermark")))) {
        Napi::Object o = obj.Get(static_cast<napi_value>(Napi::String::New(env,"timestamp_watermark"))).As<Napi::Object>();
        nertc_video_water_mark_time_to_struct(env, o, config.timestamp_watermark);
    }

    return napi_ok;
}

napi_status nertc_spatializer_position_to_struct(const Napi::Env& env, const Napi::Object& obj, nertc::NERtcPositionInfo& config)
{
    float out_f;
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"speaker_position"))))
    {
		Napi::Array objs = obj.Get(static_cast<napi_value>(Napi::String::New(env, "speaker_position"))).As<Napi::Array>();
        for (size_t i = 0; i < objs.Length(); i++) 
        {
            out_f = objs.Get(i).As<Napi::Number>().FloatValue();
            config.speaker_position[i] = out_f;
        }
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"speaker_quaternion"))))
    {
		Napi::Array objs = obj.Get(static_cast<napi_value>(Napi::String::New(env, "speaker_quaternion"))).As<Napi::Array>();
        for (size_t i = 0; i < objs.Length(); i++) 
        {
            out_f = objs.Get(i).As<Napi::Number>().FloatValue();
            config.speaker_quaternion[i] = out_f;
        }
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"head_position"))))
    {
		Napi::Array objs = obj.Get(static_cast<napi_value>(Napi::String::New(env, "head_position"))).As<Napi::Array>();
        for (size_t i = 0; i < objs.Length(); i++) 
        {
            out_f = objs.Get(i).As<Napi::Number>().FloatValue();
            config.head_position[i] = out_f;
        }
    }
    
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"head_quaternion"))))
    {
		Napi::Array objs = obj.Get(static_cast<napi_value>(Napi::String::New(env, "head_quaternion"))).As<Napi::Array>();
        for (size_t i = 0; i < objs.Length(); i++) 
        {
            out_f = objs.Get(i).As<Napi::Number>().FloatValue();
            config.head_quaternion[i] = out_f;
        }
    }
    return napi_ok;
}

napi_status nertc_media_relay_obj_to_struct(const Napi::Env& env, const Napi::Object& obj, nertc::NERtcChannelMediaRelayConfiguration* config)
{
    // if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"src_infos"))))
    // {
    //     Napi::Object o = obj.Get(static_cast<napi_value>(Napi::String::New(env,"src_infos"))).As<Napi::Object>();
    //     nertc_channel_media_relay_obj_to_struct(env, o, config->src_infos);
    // }
    
    // if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"dest_infos"))))
    // {
    //     Napi::Array objs = obj.As<Napi::Array>();
    //     for (size_t i = 0; i < objs.Length(); i++) 
    //     {
    //         Napi::Object obj = objs.Get(i).As<Napi::Object>();
    //         nertc_channel_media_relay_obj_to_struct(env, o, config->dest_infos[i]);
    //     }
    //     // Napi::Object o = obj.Get(static_cast<napi_value>(Napi::String::New(env,"dest_infos"))).As<Napi::Object>();
    //     // nertc_channel_media_relay_obj_to_struct(env, o, config->dest_infos);
    // }
    return napi_ok;
}

napi_status nertc_encry_obj_to_struct(const Napi::Env& env, const Napi::Object& obj, nertc::NERtcEncryptionConfig& config)
{
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
        memset(config.key, 0, kNERtcMaxTokenLength);
        strncpy(config.key, out.c_str(), kNERtcMaxTokenLength);
    }
	return napi_ok;
}


napi_status nertc_video_recv_stats_to_obj(const Napi::Env env, const nertc::NERtcVideoRecvStats& config,  Napi::Object& obj)
{
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"video_layers_count")), config.video_layers_count);
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"uid")), config.uid);
    Napi::Array s = Napi::Array::New(env);
    for (int index = 0; index < config.video_layers_count; index++)
    {
        Napi::Object o = Napi::Object::New(env);
        o.Set(static_cast<napi_value>(Napi::String::New(env,"layer_type")), config.video_layers_list[index].layer_type);
        o.Set(static_cast<napi_value>(Napi::String::New(env,"width")), config.video_layers_list[index].width);
        o.Set(static_cast<napi_value>(Napi::String::New(env,"height")), config.video_layers_list[index].height);
        o.Set(static_cast<napi_value>(Napi::String::New(env,"received_bitrate")), config.video_layers_list[index].received_bitrate);
        o.Set(static_cast<napi_value>(Napi::String::New(env,"received_frame_rate")), config.video_layers_list[index].received_frame_rate);
        o.Set(static_cast<napi_value>(Napi::String::New(env,"decoder_frame_rate")), config.video_layers_list[index].decoder_frame_rate);
        o.Set(static_cast<napi_value>(Napi::String::New(env,"render_frame_rate")), config.video_layers_list[index].render_frame_rate);
        o.Set(static_cast<napi_value>(Napi::String::New(env,"packet_loss_rate")), config.video_layers_list[index].packet_loss_rate);
        o.Set(static_cast<napi_value>(Napi::String::New(env,"total_frozen_time")), config.video_layers_list[index].total_frozen_time);
        o.Set(static_cast<napi_value>(Napi::String::New(env,"frozen_rate")), config.video_layers_list[index].frozen_rate);
        o.Set(static_cast<napi_value>(Napi::String::New(env,"codec_name")), config.video_layers_list[index].codec_name);
        o.Set(static_cast<napi_value>(Napi::String::New(env,"peer_to_peer_delay")), config.video_layers_list[index].peer_to_peer_delay);
        // s[index] = o;
        s.Set(static_cast<napi_value>(Napi::Number::New(env, index)),  o);

    }
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"video_layers_list")), s);
    return napi_ok;
}

napi_status nertc_network_quality_to_obj(const Napi::Env env, const nertc::NERtcNetworkQualityInfo& config,  Napi::Object& obj)
{
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"uid")), config.uid);
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"tx_quality")), (uint32_t)config.tx_quality);
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"rx_quality")), (uint32_t)config.rx_quality);
    return napi_ok;
}

napi_status nertc_audio_volume_info_to_obj(const Napi::Env env, const nertc::NERtcAudioVolumeInfo& config,  Napi::Object& obj)
{
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"uid")), config.uid);
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"volume")), (uint32_t)config.volume);
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"subStreamVolume")), (uint32_t)config.sub_stream_volume);
    return napi_ok;
}

napi_status nertc_lastmile_probe_result_to_obj(const Napi::Env env, const nertc::NERtcLastmileProbeResult& config,  Napi::Object& obj)
{
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"rtt")), config.rtt);
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"state")), (uint32_t)config.state);
    Napi::Object obj_uplink_report = Napi::Object::New(env);
    obj_uplink_report.Set(static_cast<napi_value>(Napi::String::New(env,"jitter")), config.uplink_report.jitter);
    obj_uplink_report.Set(static_cast<napi_value>(Napi::String::New(env,"packet_loss_rate")), config.uplink_report.packet_loss_rate);
    obj_uplink_report.Set(static_cast<napi_value>(Napi::String::New(env,"available_band_width")), config.uplink_report.available_band_width);
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"uplink_report")), obj_uplink_report);
    Napi::Object obj_downlink_report = Napi::Object::New(env);
    obj_downlink_report.Set(static_cast<napi_value>(Napi::String::New(env,"jitter")), config.downlink_report.jitter);
    obj_downlink_report.Set(static_cast<napi_value>(Napi::String::New(env,"packet_loss_rate")), config.downlink_report.packet_loss_rate);
    obj_downlink_report.Set(static_cast<napi_value>(Napi::String::New(env,"available_band_width")), config.downlink_report.available_band_width);
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"downlink_report")), obj_downlink_report);
    return napi_ok;
}

napi_status nertc_user_join_extra_info_to_obj(const Napi::Env env, const nertc::NERtcUserJoinExtraInfo& join_extra_info,  Napi::Object& obj)
{
    std::string str_custom_info = join_extra_info.custom_info;
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"custom_info")), str_custom_info);
    return napi_ok;
}

napi_status nertc_screen_capture_source_data_update_to_obj(const Napi::Env env, const nertc::NERtcScreenCaptureSourceData& data,  Napi::Object& obj)
{
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"type")), (int32_t)data.type);
    int64_t source_id_ = reinterpret_cast<int64_t>(data.source_id);
    obj.Set(Napi::String::New(env, "source_id"), Napi::Number::New(env, source_id_));
    obj.Set(Napi::String::New(env, "status"), Napi::Number::New(env, data.status));
    obj.Set(Napi::String::New(env, "action"), Napi::Number::New(env, data.action));
    
    Napi::Object o1 = Napi::Object::New(env);
    nertc::NERtcRectangle capture_rect = data.capture_rect;
    nertc_rectangle_obj_to_struct(env, o1, capture_rect);
    obj.Set(Napi::String::New(env, "capture_rect"), o1);
    obj.Set(Napi::String::New(env, "level"), Napi::Number::New(env, data.level));
    return napi_ok;
}

//channnel
napi_status nertc_camera_capture_obj_to_struct(const Napi::Env env, Napi::Object& obj, nertc::NERtcCameraCaptureConfig& config)
{
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"captureWidth"))))
    {
        int captureWidthTemp = obj.Get(static_cast<napi_value>(Napi::String::New(env,"captureWidth"))).As<Napi::Number>().Int32Value();
        config.captureWidth =  captureWidthTemp;
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"captureHeight"))))
    {
        int captureHeightTemp = obj.Get(static_cast<napi_value>(Napi::String::New(env,"captureHeight"))).As<Napi::Number>().Int32Value();
        config.captureHeight =  captureHeightTemp;
    }
    return napi_ok;
}

napi_status nertc_uid_list_to_struct(const Napi::Env& env, const Napi::Object& obj, std::set<uint64_t>& list) //obj:['123', '456']}
{
    if(obj.IsArray())
    {
        Napi::Array wl = obj.As<Napi::Array>();
        for (auto i = 0; i < wl.Length(); i++)
        {
			int64_t id  = wl.Get(i).As<Napi::Number>().Int64Value();
			list.insert(id);
        }
    }

    return napi_ok;
}

napi_status nertc_virtual_backgroup_source_obj_to_struct(const Napi::Env& env, const Napi::Object& obj, nertc::VirtualBackgroundSource& config)
{
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"background_source_type"))))
    {
        int iBgSourceType = obj.Get(static_cast<napi_value>(Napi::String::New(env,"background_source_type"))).As<Napi::Number>().Int32Value();
        config.background_source_type =  (nertc::VirtualBackgroundSource::NERtcBackgroundSourceType)iBgSourceType;
    }
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"color"))))
    {
        int iColor = obj.Get(static_cast<napi_value>(Napi::String::New(env,"color"))).As<Napi::Number>().Int32Value();
        config.color =  iColor;
    }

    //todo source

    return napi_ok;
}

napi_status nertc_rever_param_obj_to_struct(const Napi::Env& env, const Napi::Object& obj, nertc::NERtcReverbParam& param)
{
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"wetGain"))))
    {
        int wetGain_ = obj.Get(static_cast<napi_value>(Napi::String::New(env,"wetGain"))).As<Napi::Number>().Int32Value();
        param.wetGain =  wetGain_/100.0;
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"dryGain"))))
    {
        int dryGain_ = obj.Get(static_cast<napi_value>(Napi::String::New(env,"dryGain"))).As<Napi::Number>().Int32Value();
        param.dryGain =  dryGain_/100.0;
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"damping"))))
    {
        int damping_ = obj.Get(static_cast<napi_value>(Napi::String::New(env,"damping"))).As<Napi::Number>().Int32Value();
        param.damping =  damping_/100.0;
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"roomSize"))))
    {
        int roomSize_ = obj.Get(static_cast<napi_value>(Napi::String::New(env,"roomSize"))).As<Napi::Number>().Int32Value();
        param.roomSize =  roomSize_/100.0;
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"decayTime"))))
    {
        int decayTime_ = obj.Get(static_cast<napi_value>(Napi::String::New(env,"decayTime"))).As<Napi::Number>().Int32Value();
        param.decayTime =  decayTime_/100.0;
    }

    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"preDelay"))))
    {
        int preDelay_ = obj.Get(static_cast<napi_value>(Napi::String::New(env,"preDelay"))).As<Napi::Number>().Int32Value();
        param.preDelay =  preDelay_/100.0;
    }

    return napi_ok;
}

 napi_status nertc_camera_capture_struct_to_obj(const Napi::Env env, nertc::NERtcCameraCaptureConfig& config,  Napi::Object& obj)
 {
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"captureWidth")), config.captureWidth);
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"captureWidth")), config.captureWidth);
    return napi_ok;
}
napi_status nertc_audio_format_to_obj(const Napi::Env env, const nertc::NERtcAudioFormat& config, Napi::Object& obj)
{
	obj.Set(static_cast<napi_value>(Napi::String::New(env,"type")), (int)config.type);
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"channels")), (int)config.channels);
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"sample_rate")), (int)config.sample_rate);
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"bytes_per_sample")), (int)config.bytes_per_sample);
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"samples_per_channel")), (int)config.samples_per_channel);
    return napi_ok;
}

napi_status nertc_audio_frame_to_obj(const Napi::Env env, const nertc::NERtcAudioFrame& config,  Napi::Object& obj)
{
    Napi::Object obj_format = Napi::Object::New(env);
    nertc_audio_format_to_obj(env, config.format, obj_format);
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"format")), obj_format);
    
    int length = config.format.samples_per_channel * config.format.channels * config.format.bytes_per_sample;
    Napi::ArrayBuffer dataBuffer = Napi::ArrayBuffer::New(env, length);
    memcpy(dataBuffer.Data(), config.data, length);
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"data")), dataBuffer);
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"sync_timestamp")), config.sync_timestamp);
	return napi_ok;
}



///////////////////////////////////////////////////////
uint8_t *ARGBToBGRA(char*src, int size)
{
    char* pixelsRGBA = src;
    void* Pixels = malloc(size);
    const int bytesPerPixel = 4;
    int count = size / bytesPerPixel;
    char* pixelsBGRA = static_cast<char*>(Pixels);
    for (size_t i = 0; i < count; i++)
    {
        pixelsBGRA[0] = pixelsRGBA[2];  // B
        pixelsBGRA[1] = pixelsRGBA[1];  // G
        pixelsBGRA[2] = pixelsRGBA[0];  // R
        pixelsBGRA[3] = pixelsRGBA[3];  // A
        pixelsBGRA += bytesPerPixel;
        pixelsRGBA += bytesPerPixel;
    }
    return static_cast<uint8_t*>(Pixels);
}


}