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
    // if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"app_key"))))
    //     {
    //         app_key_ = obj.Get(static_cast<napi_value>(Napi::String::New(env,"app_key"))).As<Napi::String>().Utf8Value();
    //         context.app_key = new char[kNERtcMaxURILength];
    //         std::memset(context.app_key, 0, kNERtcMaxURILength);
    //         std::strncpy(context.app_key, app_key_.c_str(), kNERtcMaxURILength);
    //     }
    //     if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"log_dir_path"))))
    //     {
    //         log_path_ = obj.Get(static_cast<napi_value>(Napi::String::New(env,"log_dir_path"))).As<Napi::String>().Utf8Value();
    //         context.log_dir_path = new char[kNERtcMaxURILength];
    //         std::memset(context.log_dir_path, 0, kNERtcMaxURILength);
    //         strncpy(context.log_dir_path, log_path_.c_str(), kNERtcMaxURILength);
    //     }
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
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"bitrate"))))
    {
        out_i = obj.Get(static_cast<napi_value>(Napi::String::New(env,"bitrate"))).As<Napi::Number>().Int32Value();
        params.bitrate = out_i;
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
        o.Set(static_cast<napi_value>(Napi::String::New(env,"capture_frame_rate")), config.video_layers_list[i].capture_frame_rate);
        o.Set(static_cast<napi_value>(Napi::String::New(env,"render_frame_rate")), config.video_layers_list[i].render_frame_rate);
        o.Set(static_cast<napi_value>(Napi::String::New(env,"encoder_frame_rate")), config.video_layers_list[i].encoder_frame_rate);
        o.Set(static_cast<napi_value>(Napi::String::New(env,"sent_frame_rate")), config.video_layers_list[i].sent_frame_rate);
        o.Set(static_cast<napi_value>(Napi::String::New(env,"sent_bitrate")), config.video_layers_list[i].sent_bitrate);
        o.Set(static_cast<napi_value>(Napi::String::New(env,"target_bitrate")), config.video_layers_list[i].target_bitrate);
        o.Set(static_cast<napi_value>(Napi::String::New(env,"encoder_bitrate")), config.video_layers_list[i].encoder_bitrate);
        o.Set(static_cast<napi_value>(Napi::String::New(env,"codec_name")), config.video_layers_list[i].codec_name);
        // s[i] = o;
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
    }
    return napi_ok;
}

napi_status nertc_audio_mixing_option_obj_to_struct(const Napi::Env& env, const Napi::Object& obj, nertc::NERtcCreateAudioMixingOption& option)
{
    std::string out;
    uint32_t out_u;
    int32_t out_i;
    bool out_b;
    if(obj.Has(static_cast<napi_value>(Napi::String::New(env,"path"))))
    {
        out = obj.Get(static_cast<napi_value>(Napi::String::New(env,"path"))).As<Napi::String>().Utf8Value();
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
    Napi::ArrayBuffer dataBuffer = Napi::ArrayBuffer::New(env, config.data, length);
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"data")), dataBuffer);
    obj.Set(static_cast<napi_value>(Napi::String::New(env,"sync_timestamp")), config.sync_timestamp);
    return napi_ok;
}

}
