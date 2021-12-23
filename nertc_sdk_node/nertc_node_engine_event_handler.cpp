#include "nertc_node_engine_event_handler.h"
#include "nertc_node_engine_helper.h"
#include "../shared/sdk_helper/nim_node_async_queue.h"
#include "../shared/sdk_helper/node_api_helper.h"
#include "../shared/util/logger.h"
namespace nertc_node
{



void EventHandler::addEvent(const std::string& eventName, Napi::FunctionReference&& function)
{
    std::shared_ptr<EventCallback> callback = std::make_shared<EventCallback>();// auto callback = new EventCallback();
    callback->function = std::move(function);
    _callbacks[eventName] = callback;
}

int EventHandler::removeEventHandler(const std::string &eventName)
{
    auto it = _callbacks.find(eventName);
    if (it != _callbacks.end())
    {
        _callbacks.erase(it);
    }
    return _callbacks.size();
}

int EventHandler::removeAll()
{
    _callbacks.clear();
    return 0;
}

//todo检查所有Number长度是否有超过32

// void NertcNodeEventHandler::addEvent(const std::string& eventName, Napi::FunctionReference&& function) {
//     auto callback = new EventCallback();
//     callback->function = std::move(function);
//     _callbacks[eventName] = callback;
// }

// int NertcNodeEventHandler::removeAll()
// {
//     _callbacks.clear();
//     return 0;
// }

void NertcNodeEventHandler::onError(int error_code, const char* msg)
{
    nim_node::node_async_call::async_call([=]() {
        auto it = _callbacks.find("onError");
        if (it != _callbacks.end())
        {
            auto function_reference = it->second;
            auto env = function_reference->function.Env();
            auto param1 = Napi::Number::New(env, error_code);
            auto param2 = Napi::String::New(env, msg);
            const std::vector<napi_value> args = {param1, param2};
            function_reference->function.Call(args);
        }
    });
}

void NertcNodeEventHandler::onWarning(int warn_code, const char* msg)
{
    nim_node::node_async_call::async_call([=]() {
        auto it = _callbacks.find("onWarning");
        if (it != _callbacks.end())
        {
            auto function_reference = it->second;
            auto env = function_reference->function.Env();
            auto param1 = Napi::Number::New(env, warn_code);
            auto param2 = Napi::String::New(env, msg);
            const std::vector<napi_value> args = {param1, param2};
            function_reference->function.Call(args);
        }
    });    
}

void NertcNodeEventHandler::onReleasedHwResources(nertc::NERtcErrorCode result)
{
    nim_node::node_async_call::async_call([=]() {
        auto it = _callbacks.find("onReleasedHwResources");
        if (it != _callbacks.end())
        {
            auto function_reference = it->second;
            auto env = function_reference->function.Env();
            auto param1 = Napi::Number::New(env, result);
            const std::vector<napi_value> args = {param1};
            function_reference->function.Call(args);
        }
    });        
}

void NertcNodeEventHandler::onJoinChannel(nertc::channel_id_t cid, nertc::uid_t uid, nertc::NERtcErrorCode result, uint64_t elapsed)
{
    nim_node::node_async_call::async_call([=]() {
        auto it = _callbacks.find("onJoinChannel");
        if (it != _callbacks.end())
        {
            auto function_reference = it->second;
            auto env = function_reference->function.Env();
            auto param1 = Napi::Number::New(env, cid);
            auto param2 = Napi::Number::New(env, uid);
            auto param3 = Napi::Number::New(env, result);
            auto param4 = Napi::Number::New(env, elapsed);
            const std::vector<napi_value> args = {param1, param2, param3, param4};
            function_reference->function.Call(args);
        }
    });      
}

void NertcNodeEventHandler::onConnectionStateChange(nertc::NERtcConnectionStateType state, nertc::NERtcReasonConnectionChangedType reason)
{
    nim_node::node_async_call::async_call([=]() {
        auto it = _callbacks.find("onConnectionStateChange");
        if (it != _callbacks.end())
        {
            auto function_reference = it->second;
            auto env = function_reference->function.Env();
            auto param1 = Napi::Number::New(env, state);
            auto param2 = Napi::Number::New(env, reason);
            const std::vector<napi_value> args = {param1, param2};
            function_reference->function.Call(args);
        }
    });     
}

void NertcNodeEventHandler::onReconnectingStart(nertc::channel_id_t cid, nertc::uid_t uid)
{
    nim_node::node_async_call::async_call([=]() {
        auto it = _callbacks.find("onReconnectingStart");
        if (it != _callbacks.end())
        {
            auto function_reference = it->second;
            auto env = function_reference->function.Env();
            auto param1 = Napi::Number::New(env, cid);
            auto param2 = Napi::Number::New(env, uid);
            const std::vector<napi_value> args = {param1, param2};
            function_reference->function.Call(args);
        }
    });       
}

void NertcNodeEventHandler::onRejoinChannel(nertc::channel_id_t cid, nertc::uid_t uid, nertc::NERtcErrorCode result, uint64_t elapsed)
{
    nim_node::node_async_call::async_call([=]() {
        auto it = _callbacks.find("onRejoinChannel");
        if (it != _callbacks.end())
        {
            auto function_reference = it->second;
            auto env = function_reference->function.Env();
            auto param1 = Napi::Number::New(env, cid);
            auto param2 = Napi::Number::New(env, uid);
            auto param3 = Napi::Number::New(env, result);
            auto param4 = Napi::Number::New(env, elapsed);
            const std::vector<napi_value> args = {param1, param2, param3, param4};
            function_reference->function.Call(args);
        }
    });        
}

void NertcNodeEventHandler::onLeaveChannel(nertc::NERtcErrorCode result)
{
    nim_node::node_async_call::async_call([=]() {
        auto it = _callbacks.find("onLeaveChannel");
        if (it != _callbacks.end())
        {
            auto function_reference = it->second;
            auto env = function_reference->function.Env();
            auto param1 = Napi::Number::New(env, result);
            const std::vector<napi_value> args = {param1};
            function_reference->function.Call(args);
        }
    });         
}

void NertcNodeEventHandler::onDisconnect(nertc::NERtcErrorCode reason)
{
    nim_node::node_async_call::async_call([=]() {
        auto it = _callbacks.find("onDisconnect");
        if (it != _callbacks.end())
        {
            auto function_reference = it->second;
            auto env = function_reference->function.Env();
            auto param1 = Napi::Number::New(env, reason);
            const std::vector<napi_value> args = {param1};
            function_reference->function.Call(args);
        }
    });      
}

void NertcNodeEventHandler::onClientRoleChanged(nertc::NERtcClientRole oldRole, nertc::NERtcClientRole newRole)
{
    nim_node::node_async_call::async_call([=]() {
        auto it = _callbacks.find("onClientRoleChanged");
        if (it != _callbacks.end())
        {
            auto function_reference = it->second;
            auto env = function_reference->function.Env();
            auto param1 = Napi::Number::New(env, oldRole);
            auto param2 = Napi::Number::New(env, newRole);
            const std::vector<napi_value> args = {param1, param2};
            function_reference->function.Call(args);
        }
    });        
}

void NertcNodeEventHandler::onUserJoined(nertc::uid_t uid, const char * user_name)
{
    std::string str_user_name = user_name;
    nim_node::node_async_call::async_call([=]() {
        auto it = _callbacks.find("onUserJoined");
        if (it != _callbacks.end())
        {
            auto function_reference = it->second;
            auto env = function_reference->function.Env();
            auto param1 = Napi::Number::New(env, uid);
            auto param2 = Napi::String::New(env, str_user_name);
            const std::vector<napi_value> args = {param1, param2};
            function_reference->function.Call(args);
        }
    });         
}

void NertcNodeEventHandler::onUserLeft(nertc::uid_t uid, nertc::NERtcSessionLeaveReason reason)
{
    nim_node::node_async_call::async_call([=]() {
        auto it = _callbacks.find("onUserLeft");
        if (it != _callbacks.end())
        {
            auto function_reference = it->second;
            auto env = function_reference->function.Env();
            auto param1 = Napi::Number::New(env, uid);
            auto param2 = Napi::Number::New(env, reason);
            const std::vector<napi_value> args = {param1, param2};
            function_reference->function.Call(args);
        }
    });       
}

void NertcNodeEventHandler::onUserAudioStart(nertc::uid_t uid)
{
    nim_node::node_async_call::async_call([=]() {
        auto it = _callbacks.find("onUserAudioStart");
        if (it != _callbacks.end())
        {
            auto function_reference = it->second;
            auto env = function_reference->function.Env();
            auto param1 = Napi::Number::New(env, uid);
            const std::vector<napi_value> args = {param1};
            function_reference->function.Call(args);
        }
    });     
}

void NertcNodeEventHandler::onUserAudioStop(nertc::uid_t uid)
{
    nim_node::node_async_call::async_call([=]() {
        auto it = _callbacks.find("onUserAudioStop");
        if (it != _callbacks.end())
        {
            auto function_reference = it->second;
            auto env = function_reference->function.Env();
            auto param1 = Napi::Number::New(env, uid);
            const std::vector<napi_value> args = {param1};
            function_reference->function.Call(args);
        }
    });     
}

void NertcNodeEventHandler::onUserVideoStart(nertc::uid_t uid, nertc::NERtcVideoProfileType max_profile)
{
    nim_node::node_async_call::async_call([=]() {
        auto it = _callbacks.find("onUserVideoStart");
        if (it != _callbacks.end())
        {
            auto function_reference = it->second;
            auto env = function_reference->function.Env();
            auto param1 = Napi::Number::New(env, uid);
            auto param2 = Napi::Number::New(env, max_profile);
            const std::vector<napi_value> args = {param1, param2};
            function_reference->function.Call(args);
        }
    });    
}

void NertcNodeEventHandler::onUserVideoStop(nertc::uid_t uid)
{
    nim_node::node_async_call::async_call([=]() {
        auto it = _callbacks.find("onUserVideoStop");
        if (it != _callbacks.end())
        {
            auto function_reference = it->second;
            auto env = function_reference->function.Env();
            auto param1 = Napi::Number::New(env, uid);
            const std::vector<napi_value> args = {param1};
            function_reference->function.Call(args);
        }
    });     
}


void NertcNodeEventHandler::onUserSubStreamVideoStart(nertc::uid_t uid, nertc::NERtcVideoProfileType max_profile)
{
    nim_node::node_async_call::async_call([=]() {
        auto it = _callbacks.find("onUserSubStreamVideoStart");
        if (it != _callbacks.end())
        {
            auto function_reference = it->second;
            auto env = function_reference->function.Env();
            auto param1 = Napi::Number::New(env, uid);
            auto param2 = Napi::Number::New(env, max_profile);
            const std::vector<napi_value> args = {param1, param2};
            function_reference->function.Call(args);
        }
    });
}

void NertcNodeEventHandler::onUserSubStreamVideoStop(nertc::uid_t uid)
{
    nim_node::node_async_call::async_call([=]() {
        auto it = _callbacks.find("onUserSubStreamVideoStop");
        if (it != _callbacks.end())
        {
            auto function_reference = it->second;
            auto env = function_reference->function.Env();
            auto param1 = Napi::Number::New(env, uid);
            const std::vector<napi_value> args = {param1};
            function_reference->function.Call(args);
        }
    });
}

void NertcNodeEventHandler::onUserVideoProfileUpdate(nertc::uid_t uid, nertc::NERtcVideoProfileType max_profile)
{
    nim_node::node_async_call::async_call([=]() {
        auto it = _callbacks.find("onUserVideoProfileUpdate");
        if (it != _callbacks.end())
        {
            auto function_reference = it->second;
            auto env = function_reference->function.Env();
            auto param1 = Napi::Number::New(env, uid);
            auto param2 = Napi::Number::New(env, max_profile);
            const std::vector<napi_value> args = {param1, param2};
            function_reference->function.Call(args);
        }
    });
}

void NertcNodeEventHandler::onUserAudioMute(nertc::uid_t uid, bool mute)
{
    nim_node::node_async_call::async_call([=]() {
        auto it = _callbacks.find("onUserAudioMute");
        if (it != _callbacks.end())
        {
            auto function_reference = it->second;
            auto env = function_reference->function.Env();
            auto param1 = Napi::Number::New(env, uid);
            auto param2 = Napi::Boolean::New(env, mute);
            const std::vector<napi_value> args = {param1, param2};
            function_reference->function.Call(args);
        }
    });
}

void NertcNodeEventHandler::onUserVideoMute(nertc::uid_t uid, bool mute)
{
    nim_node::node_async_call::async_call([=]() {
        auto it = _callbacks.find("onUserVideoMute");
        if (it != _callbacks.end())
        {
            auto function_reference = it->second;
            auto env = function_reference->function.Env();
            auto param1 = Napi::Number::New(env, uid);
            auto param2 = Napi::Boolean::New(env, mute);
            const std::vector<napi_value> args = {param1, param2};
            function_reference->function.Call(args);
        }
    });
}

void NertcNodeEventHandler::onAudioDeviceStateChanged(const char device_id[kNERtcMaxDeviceIDLength],
        nertc::NERtcAudioDeviceType device_type,
        nertc::NERtcAudioDeviceState device_state)
{
    std::string str_device_id = device_id;
    nim_node::node_async_call::async_call([=]() {
        auto it = _callbacks.find("onAudioDeviceStateChanged");
        if (it != _callbacks.end())
        {
            auto function_reference = it->second;
            auto env = function_reference->function.Env();
            auto param1 = Napi::String::New(env, str_device_id);
            auto param2 = Napi::Number::New(env, device_type);
            auto param3 = Napi::Number::New(env, device_state);
            const std::vector<napi_value> args = {param1, param2, param3};
            function_reference->function.Call(args);
        }
    });
}

void NertcNodeEventHandler::onAudioDefaultDeviceChanged(const char device_id[kNERtcMaxDeviceIDLength],
        nertc::NERtcAudioDeviceType device_type)
{
    std::string str_device_id = device_id;
    nim_node::node_async_call::async_call([=]() {
        auto it = _callbacks.find("onAudioDefaultDeviceChanged");
        if (it != _callbacks.end())
        {
            auto function_reference = it->second;
            auto env = function_reference->function.Env();
            auto param1 = Napi::String::New(env, str_device_id);
            auto param2 = Napi::Number::New(env, device_type);
            const std::vector<napi_value> args = {param1, param2};
            function_reference->function.Call(args);
        }
    });
}

void NertcNodeEventHandler::onVideoDeviceStateChanged(const char device_id[kNERtcMaxDeviceIDLength],
        nertc::NERtcVideoDeviceType device_type,
        nertc::NERtcVideoDeviceState device_state)
{
    std::string str_device_id = device_id;
    nim_node::node_async_call::async_call([=]() {
        auto it = _callbacks.find("onVideoDeviceStateChanged");
        if (it != _callbacks.end())
        {
            auto function_reference = it->second;
            auto env = function_reference->function.Env();
            auto param1 = Napi::String::New(env, str_device_id);
            auto param2 = Napi::Number::New(env, device_type);
            auto param3 = Napi::Number::New(env, device_state);
            const std::vector<napi_value> args = {param1, param2, param3};
            function_reference->function.Call(args);
        }
    });
}

void NertcNodeEventHandler::onFirstAudioDataReceived(nertc::uid_t uid)
{
    nim_node::node_async_call::async_call([=]() {
        auto it = _callbacks.find("onFirstAudioDataReceived");
        if (it != _callbacks.end())
        {
            auto function_reference = it->second;
            auto env = function_reference->function.Env();
            auto param1 = Napi::Number::New(env, uid);
            const std::vector<napi_value> args = {param1};
            function_reference->function.Call(args);
        }
    });
}

void NertcNodeEventHandler::onFirstVideoDataReceived(nertc::uid_t uid)
{
    nim_node::node_async_call::async_call([=]() {
        auto it = _callbacks.find("onFirstVideoDataReceived");
        if (it != _callbacks.end())
        {
            auto function_reference = it->second;
            auto env = function_reference->function.Env();
            auto param1 = Napi::Number::New(env, uid);
            const std::vector<napi_value> args = {param1};
            function_reference->function.Call(args);
        }
    });
}

void NertcNodeEventHandler::onFirstAudioFrameDecoded(nertc::uid_t uid)
{
    nim_node::node_async_call::async_call([=]() {
        auto it = _callbacks.find("onFirstAudioFrameDecoded");
        if (it != _callbacks.end())
        {
            auto function_reference = it->second;
            auto env = function_reference->function.Env();
            auto param1 = Napi::Number::New(env, uid);
            const std::vector<napi_value> args = {param1};
            function_reference->function.Call(args);
        }
    });
}

void NertcNodeEventHandler::onFirstVideoFrameDecoded(nertc::uid_t uid, uint32_t width, uint32_t height)
{
    nim_node::node_async_call::async_call([=]() {
        auto it = _callbacks.find("onFirstVideoFrameDecoded");
        if (it != _callbacks.end())
        {
            auto function_reference = it->second;
            auto env = function_reference->function.Env();
            auto param1 = Napi::Number::New(env, uid);
            auto param2 = Napi::Number::New(env, width);
            auto param3 = Napi::Number::New(env, height);
            const std::vector<napi_value> args = {param1, param2, param3};
            function_reference->function.Call(args);
        }
    });
}

void NertcNodeEventHandler::onCaptureVideoFrame(void *data,
                                                nertc::NERtcVideoType type,
                                                uint32_t width,
                                                uint32_t height,
                                                uint32_t count,
                                                uint32_t offset[kNERtcMaxPlaneCount],
                                                uint32_t stride[kNERtcMaxPlaneCount],
                                                nertc::NERtcVideoRotation rotation)
{
    //TODO(litianyi)
}

void NertcNodeEventHandler::onAudioMixingStateChanged(nertc::NERtcAudioMixingState state, nertc::NERtcAudioMixingErrorCode error_code)
{
    nim_node::node_async_call::async_call([=]() {
        auto it = _callbacks.find("onAudioMixingStateChanged");
        if (it != _callbacks.end())
        {
            auto function_reference = it->second;
            auto env = function_reference->function.Env();
            auto param1 = Napi::Number::New(env, state); 
            auto param2 = Napi::Number::New(env, error_code);
            const std::vector<napi_value> args = {param1, param2};
            function_reference->function.Call(args);
        }
    });
}

void NertcNodeEventHandler::onAudioMixingTimestampUpdate(uint64_t timestamp_ms)
{
    nim_node::node_async_call::async_call([=]() {
        auto it = _callbacks.find("onAudioMixingTimestampUpdate");
        if (it != _callbacks.end())
        {
            auto function_reference = it->second;
            auto env = function_reference->function.Env();
            auto param1 = Napi::Number::New(env, timestamp_ms); 
            const std::vector<napi_value> args = {param1};
            function_reference->function.Call(args);
        }
    });
}

void NertcNodeEventHandler::onAudioEffectFinished(uint32_t effect_id)
{
    nim_node::node_async_call::async_call([=]() {
        auto it = _callbacks.find("onAudioEffectFinished");
        if (it != _callbacks.end())
        {
            auto function_reference = it->second;
            auto env = function_reference->function.Env();
            auto param1 = Napi::Number::New(env, effect_id);
            const std::vector<napi_value> args = {param1};
            function_reference->function.Call(args);
        }
    });
}

void NertcNodeEventHandler::onLocalAudioVolumeIndication(int volume)
{
    nim_node::node_async_call::async_call([=]() {
        auto it = _callbacks.find("onLocalAudioVolumeIndication");
        if (it != _callbacks.end())
        {
            auto function_reference = it->second;
            auto env = function_reference->function.Env();
            auto param1 = Napi::Number::New(env, volume);
            const std::vector<napi_value> args = {param1};
            function_reference->function.Call(args);
        }
    });
}

void NertcNodeEventHandler::onRemoteAudioVolumeIndication(const nertc::NERtcAudioVolumeInfo *speakers, unsigned int speaker_number, int total_volume)
{
    nim_node::node_async_call::async_call([=]() {
        auto it = _callbacks.find("onRemoteAudioVolumeIndication");
        if (it != _callbacks.end())
        {  
            auto function_reference = it->second;
            auto env = function_reference->function.Env();
            Napi::Array param1 = Napi::Array::New(env);
            for (auto i = 0; i < (int)speaker_number; i++)
            {
                Napi::Object obj = Napi::Object::New(env);
                nertc_audio_volume_info_to_obj(env, speakers[i], obj);
                // param1[i] = obj;
                param1.Set(static_cast<napi_value>(Napi::Number::New(env, i)),  obj);
            }
            auto param2 = Napi::Number::New(env, speaker_number);
            auto param3 = Napi::Number::New(env, total_volume);
            const std::vector<napi_value> args = {param1, param2, param3};
            function_reference->function.Call(args);
        }
    });
}

void NertcNodeEventHandler::onAddLiveStreamTask(const char* task_id, const char* url, int error_code)
{
    nim_node::node_async_call::async_call([=]() {
        auto it = _callbacks.find("onAddLiveStreamTask");
        if (it != _callbacks.end())
        {
            std::string str_task_id = task_id;
            std::string str_url = url;
            auto function_reference = it->second;
            auto env = function_reference->function.Env();
            auto param1 = Napi::String::New(env, str_task_id);
            auto param2 = Napi::String::New(env, str_url);
            auto param3 = Napi::Number::New(env, error_code);
            const std::vector<napi_value> args = {param1,param2,param3};
            function_reference->function.Call(args);
        }
    });
}

void NertcNodeEventHandler::onUpdateLiveStreamTask(const char* task_id, const char* url, int error_code)
{
    nim_node::node_async_call::async_call([=]() {
        auto it = _callbacks.find("onUpdateLiveStreamTask");
        if (it != _callbacks.end())
        {
            std::string str_task_id = task_id;
            std::string str_url = url;
            auto function_reference = it->second;
            auto env = function_reference->function.Env();
            auto param1 = Napi::String::New(env, str_task_id);
            auto param2 = Napi::String::New(env, str_url);
            auto param3 = Napi::Number::New(env, error_code);
            const std::vector<napi_value> args = {param1,param2,param3};
            function_reference->function.Call(args);
        }
    });
}

void NertcNodeEventHandler::onRemoveLiveStreamTask(const char* task_id, int error_code)
{
    nim_node::node_async_call::async_call([=]() {
        auto it = _callbacks.find("onRemoveLiveStreamTask");
        if (it != _callbacks.end())
        {
            std::string str_task_id = task_id;
            auto function_reference = it->second;
            auto env = function_reference->function.Env();
            auto param1 = Napi::String::New(env, str_task_id);
            auto param2 = Napi::Number::New(env, error_code);
            const std::vector<napi_value> args = {param1,param2};
            function_reference->function.Call(args);
        }
    });
}

void NertcNodeEventHandler::onLiveStreamState(const char* task_id, const char* url, nertc::NERtcLiveStreamStateCode state)
{
    nim_node::node_async_call::async_call([=]() {
        auto it = _callbacks.find("onLiveStreamState");
        if (it != _callbacks.end())
        {
            std::string str_task_id = task_id;
            std::string str_url = url;
            auto function_reference = it->second;
            auto env = function_reference->function.Env();
             auto param1 = Napi::String::New(env, str_task_id);
            auto param2 = Napi::String::New(env, str_url);
            auto param3 = Napi::Number::New(env, state);
            const std::vector<napi_value> args = {param1,param2,param3};
            function_reference->function.Call(args);
        }
    });
}

void NertcNodeEventHandler::onAudioHowling(bool howling)
{
    nim_node::node_async_call::async_call([=]() {
        auto it = _callbacks.find("onAudioHowling");
        if (it != _callbacks.end())
        {
            auto function_reference = it->second;
            auto env = function_reference->function.Env();
            auto param1 = Napi::Boolean::New(env, howling);
            const std::vector<napi_value> args = {param1};
            function_reference->function.Call(args);
        }
    });
}

void NertcNodeEventHandler::onRecvSEIMsg(nertc::uid_t uid, const char* data, uint32_t dataSize)
{
    auto* copied = new char[dataSize];
    memset(copied, 0, dataSize);
    memcpy(copied, data, dataSize);
	nim_node::node_async_call::async_call([=]() {
		Node_onRecvSEIMsg(uid, copied, dataSize);
	});
}

void NertcNodeEventHandler::Node_onRecvSEIMsg(nertc::uid_t uid, const char* data, uint32_t length)
{
    auto it = _callbacks.find("onRecvSEIMsg");
        if (it != _callbacks.end())
        {
            auto function_reference = it->second;
            auto env = function_reference->function.Env();
            auto param1 = Napi::Number::New(env, uid);
            Napi::ArrayBuffer arrayBuffer = Napi::ArrayBuffer::New(env, length);
            memcpy(arrayBuffer.Data(), data, length);
            if (data)
            {
                delete[] data;
                data = nullptr;
            }
            const std::vector<napi_value> args = {param1, arrayBuffer};
            function_reference->function.Call(args);
        }
}

void NertcNodeEventHandler::onScreenCaptureStatus(nertc::NERtcScreenCaptureStatus status)
{
    nim_node::node_async_call::async_call([=]() {
        auto it = _callbacks.find("onScreenCaptureStatus");
        if (it != _callbacks.end())
        {
            auto function_reference = it->second;
            auto env = function_reference->function.Env();
            auto param1 = Napi::Number::New(env, status);
            const std::vector<napi_value> args = {param1};
            function_reference->function.Call(args);
        }
    });
}

void NertcNodeEventHandler::onAudioRecording(nertc::NERtcAudioRecordingCode code, const char* file_path)
{
    std::string str_file_path = file_path;
    nim_node::node_async_call::async_call([=]() {
        auto it = _callbacks.find("onAudioRecording");
        if (it != _callbacks.end())
        {
            auto function_reference = it->second;
            auto env = function_reference->function.Env();
            auto param1 = Napi::Number::New(env, code);
            auto param2 = Napi::String::New(env, str_file_path);
            const std::vector<napi_value> args = {param1, param2};
            function_reference->function.Call(args);
        }
    });
}

void NertcNodeEventHandler::onMediaRelayStateChanged(nertc::NERtcChannelMediaRelayState state, const char* channel_name)
{
    std::string str_channel_name = channel_name;
    nim_node::node_async_call::async_call([=]() {
        auto it = _callbacks.find("onMediaRelayStateChanged");
        if (it != _callbacks.end())
        {
            auto function_reference = it->second;
            auto env = function_reference->function.Env();
            auto param1 = Napi::Number::New(env, state);
            auto param2 = Napi::String::New(env, str_channel_name);
            const std::vector<napi_value> args = {param1, param2};
            function_reference->function.Call(args);
        }
    });
}

void NertcNodeEventHandler::onMediaRelayEvent(nertc::NERtcChannelMediaRelayEvent event, const char* channel_name, nertc::NERtcErrorCode error)
{
    std::string str_channel_name = channel_name;
    nim_node::node_async_call::async_call([=]() {
        auto it = _callbacks.find("onMediaRelayEvent");
        if (it != _callbacks.end())
        {
            auto function_reference = it->second;
            auto env = function_reference->function.Env();
            auto param1 = Napi::Number::New(env, event);
            auto param2 = Napi::String::New(env, str_channel_name);
            auto param3 = Napi::Number::New(env, error);
            const std::vector<napi_value> args = {param1, param2, param3};
            function_reference->function.Call(args);
        }
    });
}

void NertcNodeEventHandler::onLocalPublishFallbackToAudioOnly(bool is_fallback, nertc::NERtcVideoStreamType stream_type)
{
    nim_node::node_async_call::async_call([=]() {
        auto it = _callbacks.find("onLocalPublishFallbackToAudioOnly");
        if (it != _callbacks.end())
        {
            auto function_reference = it->second;
            auto env = function_reference->function.Env();
            auto param1 = Napi::Boolean::New(env, is_fallback);
            auto param2 = Napi::Number::New(env, stream_type);
            const std::vector<napi_value> args = {param1, param2};
            function_reference->function.Call(args);
        }
    });
}

void NertcNodeEventHandler::onRemoteSubscribeFallbackToAudioOnly(nertc::uid_t uid, bool is_fallback, nertc::NERtcVideoStreamType stream_type)
{
    nim_node::node_async_call::async_call([=]() {
        auto it = _callbacks.find("onRemoteSubscribeFallbackToAudioOnly");
        if (it != _callbacks.end())
        {
            auto function_reference = it->second;
            auto env = function_reference->function.Env();
            auto param1 = Napi::Number::New(env, uid);
            auto param2 = Napi::Boolean::New(env, is_fallback);
            auto param3 = Napi::Number::New(env, stream_type);
            const std::vector<napi_value> args = {param1, param2, param3};
            function_reference->function.Call(args);
        }
    });
}

void NertcNodeEventHandler::onPullExternalAudioFrame(Napi::FunctionReference&& function, const std::shared_ptr<unsigned char>& data, uint32_t length)
{
    auto callback = new EventCallback();
    callback->function = std::move(function);

    nim_node::node_async_call::async_call([=]() {
        auto env = callback->function.Env();
        Napi::ArrayBuffer arrayBuffer = Napi::ArrayBuffer::New(env, data.get(), length);
        const std::vector<napi_value> args = {arrayBuffer};
        callback->function.Call(args);
    });
}


void NertcNodeRtcMediaStatsHandler::onRtcStats(const nertc::NERtcStats &stats)
{
    nim_node::node_async_call::async_call([=]() {
        auto it = _callbacks.find("onRtcStats");
        if (it != _callbacks.end())
        {
            auto function_reference = it->second;
            auto env = function_reference->function.Env();
            Napi::Object param1 = Napi::Object::New(env);
            nertc_stats_to_obj(env, stats, param1);
            const std::vector<napi_value> args = {param1};
            function_reference->function.Call(args);
        }
    });
}

void NertcNodeRtcMediaStatsHandler::onLocalAudioStats(const nertc::NERtcAudioSendStats &stats)
{
    nim_node::node_async_call::async_call([=]() {
        auto it = _callbacks.find("onLocalAudioStats");
        if (it != _callbacks.end())
        {
            auto function_reference = it->second;
            auto env = function_reference->function.Env();
            Napi::Object param1 = Napi::Object::New(env);
            nertc_audio_send_stats_to_obj(env, stats, param1);
            const std::vector<napi_value> args = {param1};
            function_reference->function.Call(args);
        }
    });
}

void NertcNodeRtcMediaStatsHandler::onRemoteAudioStats(const nertc::NERtcAudioRecvStats *stats, unsigned int user_count)
{
    nertc::NERtcAudioRecvStats *ss = new nertc::NERtcAudioRecvStats[user_count];
    for (auto i = 0; i < user_count; i++) {
        ss[i] = stats[i];
    }

    nim_node::node_async_call::async_call([=]() {
        auto it = _callbacks.find("onRemoteAudioStats");
        if (it != _callbacks.end())
        {
            auto function_reference = it->second;
            auto env = function_reference->function.Env();

            Napi::Array s = Napi::Array::New(env);
            for (auto i = 0; i < (int)user_count; i++)
            {
                Napi::Object o = Napi::Object::New(env);
                nertc_audio_recv_stats_to_obj(env, ss[i], o);
                // s[i] = o;
                s.Set(static_cast<napi_value>(Napi::Number::New(env, i)),  o);
            }
            if (ss)
            {
                delete[] ss;
            }
            Napi::Number param1 = Napi::Number::New(env, (int)user_count);
            const std::vector<napi_value> args = {param1, s};
            function_reference->function.Call(args);
        }
    });
}

void NertcNodeRtcMediaStatsHandler::onLocalVideoStats(const nertc::NERtcVideoSendStats &stats)
{
    nertc::NERtcVideoSendStats ss;
    ss.video_layers_count = stats.video_layers_count;
    ss.video_layers_list = new nertc::NERtcVideoLayerSendStats[ss.video_layers_count];
    for (auto i = 0; i < ss.video_layers_count; i++) {
        ss.video_layers_list[i] = stats.video_layers_list[i];
    }

    nim_node::node_async_call::async_call([=]() {
        auto it = _callbacks.find("onLocalVideoStats");
        if (it != _callbacks.end())
        {
            auto function_reference = it->second;
            auto env = function_reference->function.Env();
            Napi::Object s = Napi::Object::New(env);
            nertc_video_send_stats_to_obj(env, ss, s);
            if (ss.video_layers_list)
            {
                delete[] ss.video_layers_list;
            }
            const std::vector<napi_value> args = {s};
            function_reference->function.Call(args);
        }
    });
}

void NertcNodeRtcMediaStatsHandler::onRemoteVideoStats(const nertc::NERtcVideoRecvStats *stats, unsigned int user_count)
{
    nertc::NERtcVideoRecvStats *ss = new nertc::NERtcVideoRecvStats[user_count];
    for (auto i = 0; i < user_count; i++) {
        ss[i].uid = stats[i].uid;
        ss[i].video_layers_count = stats[i].video_layers_count;
        ss[i].video_layers_list = new nertc::NERtcVideoLayerRecvStats[ss[i].video_layers_count];
        for (auto j = 0; j < ss[i].video_layers_count; j++){
            ss[i].video_layers_list[j] = stats[i].video_layers_list[j];
        }
    }

    nim_node::node_async_call::async_call([=]() {
        auto it = _callbacks.find("onRemoteVideoStats");
        if (it != _callbacks.end())
        {
            auto function_reference = it->second;
            auto env = function_reference->function.Env();
            Napi::Array s = Napi::Array::New(env);
            for (auto i = 0; i < (int)user_count; i++)
            {
                Napi::Object o = Napi::Object::New(env);
                nertc_video_recv_stats_to_obj(env, ss[i], o);
                // s[i] = o;
                s.Set(static_cast<napi_value>(Napi::Number::New(env, i)),  o);
            }
            if (ss)
            {
                for (int i = 0; i < user_count; i++)
                {
                    if (ss[i].video_layers_list)
                    {
                        delete[] ss[i].video_layers_list;
                    }
                }
                delete[] ss;
            }
            auto param1 = Napi::Number::New(env, (int)user_count);
            const std::vector<napi_value> args = {param1, s};
            function_reference->function.Call(args);
        }
    });
}

void NertcNodeRtcMediaStatsHandler::onNetworkQuality(const nertc::NERtcNetworkQualityInfo *infos, unsigned int user_count)
{
    nertc::NERtcNetworkQualityInfo *ss = new nertc::NERtcNetworkQualityInfo[user_count];
    for (auto i = 0; i < user_count; i++) {
        ss[i] = infos[i];
    }
    nim_node::node_async_call::async_call([=]() {
        auto it = _callbacks.find("onNetworkQuality");
        if (it != _callbacks.end())
        {
            auto function_reference = it->second;
            auto env = function_reference->function.Env();
            Napi::Array s = Napi::Array::New(env);
            for (auto i = 0; i < (int)user_count; i++)
            {
                Napi::Object o = Napi::Object::New(env);
                nertc_network_quality_to_obj(env, ss[i], o);
                // s[i] = o;
                s.Set(static_cast<napi_value>(Napi::Number::New(env, i)),  o);
            }
            if (ss) {
                delete[] ss;
            }
            auto param1 = Napi::Number::New(env, user_count);
            const std::vector<napi_value> args = {param1, s};
            function_reference->function.Call(args);
        }
    });
}

}
