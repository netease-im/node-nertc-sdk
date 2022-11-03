#include "nertc_node_channel_event_handler.h"
#include "../shared/log/logging/logging.h"
#include "../shared/sdk_helper/nim_node_async_queue.h"
#include "../shared/sdk_helper/node_api_helper.h"

namespace nertc_node
{

void NertcChannelEventHandler::onError(int error_code, const char* msg)
{
    std::string str_msg = msg;
    LOG_F(INFO, "error_code:%d strMsg:%s", error_code, str_msg.c_str());
    nim_node::node_async_call::async_call([=]() {
        Node_onError(error_code, str_msg);
    });
}

void NertcChannelEventHandler::Node_onError(int error_code, std::string msg){
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
}

void NertcChannelEventHandler::onWarning(int warn_code, const char* msg)
{
    std::string str_msg = msg;
    LOG_F(INFO, "warn_code:%d strMsg:%s", warn_code, str_msg.c_str());
    nim_node::node_async_call::async_call([=]() {
        Node_onWarning(warn_code, str_msg);
    });
}

void NertcChannelEventHandler::Node_onWarning(int warn_code, std::string msg)
{
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
}

void NertcChannelEventHandler::onJoinChannel(nertc::channel_id_t cid, nertc::uid_t uid, nertc::NERtcErrorCode result, uint64_t elapsed)
{
    LOG_F(INFO, "cid:%llu uid:%llu result:%d elapsed:%d", cid , uid, result, elapsed);
    nim_node::node_async_call::async_call([=]() {
        Node_onJoinChannel(cid, uid, result, elapsed);
    });
}

void NertcChannelEventHandler::Node_onJoinChannel(nertc::channel_id_t cid, nertc::uid_t uid, nertc::NERtcErrorCode result, uint64_t elapsed)
{
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
}

void NertcChannelEventHandler::onReconnectingStart(nertc::channel_id_t cid, nertc::uid_t uid)
{
    LOG_F(INFO, "cid:%llu uid:%llu", cid, uid);
    nim_node::node_async_call::async_call([=]() {
        Node_onReconnectingStart(cid, uid);
    });       
}

void NertcChannelEventHandler::Node_onReconnectingStart(nertc::channel_id_t cid, nertc::uid_t uid)
{
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
}

void NertcChannelEventHandler::onConnectionStateChange(nertc::NERtcConnectionStateType state, nertc::NERtcReasonConnectionChangedType reason)
{
    LOG_F(INFO, "state:%d reason:%d", state, reason);
    nim_node::node_async_call::async_call([=]() {
        Node_onConnectionStateChange(state, reason);
    });     
}

void NertcChannelEventHandler::Node_onConnectionStateChange(nertc::NERtcConnectionStateType state, nertc::NERtcReasonConnectionChangedType reason)
{
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
}

void NertcChannelEventHandler::onRejoinChannel(nertc::channel_id_t cid, nertc::uid_t uid, nertc::NERtcErrorCode result, uint64_t elapsed)
{
    LOG_F(INFO, "cid:%llu uid:%llu result:%d elapsed:%d", cid, uid, result, elapsed);
    nim_node::node_async_call::async_call([=]() {
		Node_onRejoinChannel(cid, uid, result, elapsed);
    });        
}

void NertcChannelEventHandler::Node_onRejoinChannel(nertc::channel_id_t cid, nertc::uid_t uid, nertc::NERtcErrorCode result, uint64_t elapsed)
{
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
}

void NertcChannelEventHandler::onLeaveChannel(nertc::NERtcErrorCode result)
{
    LOG_F(INFO, "result:%d", result);
    nim_node::node_async_call::async_call([=]() {
        Node_onLeaveChannel(result);
    });         
}

void NertcChannelEventHandler::Node_onLeaveChannel(nertc::NERtcErrorCode result)
{
    auto it = _callbacks.find("onLeaveChannel");
    if (it != _callbacks.end())
    {
        auto function_reference = it->second;
        auto env = function_reference->function.Env();
        auto param1 = Napi::Number::New(env, result);
        const std::vector<napi_value> args = {param1};
        function_reference->function.Call(args);
    }
}

void NertcChannelEventHandler::onDisconnect(nertc::NERtcErrorCode reason)
{
    LOG_F(INFO, "reason:%d", reason);
    nim_node::node_async_call::async_call([=]() {
        Node_onDisconnect(reason);
    });      
}

void NertcChannelEventHandler::Node_onDisconnect(nertc::NERtcErrorCode reason)
{
    auto it = _callbacks.find("onDisconnect");
    if (it != _callbacks.end())
    {
        auto function_reference = it->second;
        auto env = function_reference->function.Env();
        auto param1 = Napi::Number::New(env, reason);
        const std::vector<napi_value> args = {param1};
        function_reference->function.Call(args);
    }
}

void NertcChannelEventHandler::onClientRoleChanged(nertc::NERtcClientRole oldRole, nertc::NERtcClientRole newRole)
{
    LOG_F(INFO, "oldRole:%d newRole:%d", oldRole, newRole);
    nim_node::node_async_call::async_call([=]() {
        Node_onClientRoleChanged(oldRole, newRole);
    });        
}

void NertcChannelEventHandler::Node_onClientRoleChanged(nertc::NERtcClientRole oldRole, nertc::NERtcClientRole newRole)
{
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
}

void NertcChannelEventHandler::onUserJoined(nertc::uid_t uid, const char * user_name)
{
    std::string str_user_name = user_name;
    LOG_F(INFO, "uid:%llu strUserName:%s", uid, str_user_name.c_str());
    nim_node::node_async_call::async_call([=]() {
        Node_onUserJoined(uid, str_user_name);
    });         
}

void NertcChannelEventHandler::Node_onUserJoined(nertc::uid_t uid, std::string user_name)
{
    auto it = _callbacks.find("onUserJoined");
    if (it != _callbacks.end())
    {
        auto function_reference = it->second;
        auto env = function_reference->function.Env();
        auto param1 = Napi::Number::New(env, uid);
        auto param2 = Napi::String::New(env, user_name);
        const std::vector<napi_value> args = {param1, param2};
        function_reference->function.Call(args);
    }

}

void NertcChannelEventHandler::onUserLeft(nertc::uid_t uid, nertc::NERtcSessionLeaveReason reason)
{
    LOG_F(INFO, "uid:%llu reason:%d", uid, reason);
    nim_node::node_async_call::async_call([=]() {
        Node_onUserLeft(uid, reason);
    });       
}

void NertcChannelEventHandler::Node_onUserLeft(nertc::uid_t uid, nertc::NERtcSessionLeaveReason reason)
{
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
}

void NertcChannelEventHandler::onUserAudioStart(nertc::uid_t uid)
{
    LOG_F(INFO, "uid:%llu", uid);
    nim_node::node_async_call::async_call([=]() {
        Node_onUserAudioStart(uid);
    });
}

void NertcChannelEventHandler::Node_onUserAudioStart(nertc::uid_t uid)
{
    auto it = _callbacks.find("onUserAudioStart");
    if (it != _callbacks.end())
    {
        auto function_reference = it->second;
        auto env = function_reference->function.Env();
        auto param1 = Napi::Number::New(env, uid);
        const std::vector<napi_value> args = {param1};
        function_reference->function.Call(args);
    }
}

void NertcChannelEventHandler::onUserAudioStop(nertc::uid_t uid)
{
    LOG_F(INFO, "uid:%llu", uid);
    nim_node::node_async_call::async_call([=]() {
        Node_onUserAudioStop(uid);
    });     
}

void NertcChannelEventHandler::Node_onUserAudioStop(nertc::uid_t uid)
{
    auto it = _callbacks.find("onUserAudioStop");
    if (it != _callbacks.end())
    {
        auto function_reference = it->second;
        auto env = function_reference->function.Env();
        auto param1 = Napi::Number::New(env, uid);
        const std::vector<napi_value> args = {param1};
        function_reference->function.Call(args);
    }
}

void NertcChannelEventHandler::onUserAudioMute(nertc::uid_t uid, bool mute)
{
    LOG_F(INFO, "uid:%llu mute:%d", uid, mute);
    nim_node::node_async_call::async_call([=]() {
        Node_onUserAudioMute(uid, mute);
    });
}

void NertcChannelEventHandler::Node_onUserAudioMute(nertc::uid_t uid, bool mute)
{
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
}

void NertcChannelEventHandler::onUserSubStreamAudioStart(nertc::uid_t uid)
{
    LOG_F(INFO, "uid:%llu", uid);
    nim_node::node_async_call::async_call([=]() {
      Node_onUserSubStreamAudioStart(uid);
    });
}

void NertcChannelEventHandler::Node_onUserSubStreamAudioStart(nertc::uid_t uid)
{
    auto it = _callbacks.find("onUserSubStreamAudioStart");
    if (it != _callbacks.end())
    {
        auto function_reference = it->second;
        auto env = function_reference->function.Env();
        auto param1 = Napi::Number::New(env, uid);
        const std::vector<napi_value> args = {param1};
        function_reference->function.Call(args);
    }
}

void NertcChannelEventHandler::onUserSubStreamAudioStop(nertc::uid_t uid)
{
    LOG_F(INFO, "uid:%llu", uid);
    nim_node::node_async_call::async_call([=]() {
      Node_onUserSubStreamAudioStop(uid);
    });
}

void NertcChannelEventHandler::Node_onUserSubStreamAudioStop(nertc::uid_t uid)
{
    auto it = _callbacks.find("onUserSubStreamAudioStop");
    if (it != _callbacks.end())
    {
        auto function_reference = it->second;
        auto env = function_reference->function.Env();
        auto param1 = Napi::Number::New(env, uid);
        const std::vector<napi_value> args = {param1};
        function_reference->function.Call(args);
    }
}

void NertcChannelEventHandler::onUserSubStreamAudioMute(nertc::uid_t uid, bool mute)
{
    LOG_F(INFO, "uid:%llu mute:%d", uid, mute);
    nim_node::node_async_call::async_call([=]() {
      Node_onUserSubStreamAudioMute(uid, mute);
    });
}

void NertcChannelEventHandler::Node_onUserSubStreamAudioMute(nertc::uid_t uid, bool mute)
{
    auto it = _callbacks.find("onUserSubStreamAudioMute");
    if (it != _callbacks.end())
    {
        auto function_reference = it->second;
        auto env = function_reference->function.Env();
        auto param1 = Napi::Number::New(env, uid);
        auto param2 = Napi::Boolean::New(env, mute);
        const std::vector<napi_value> args = {param1, param2};
        function_reference->function.Call(args);
    }
}

void NertcChannelEventHandler::onUserVideoStart(nertc::uid_t uid, nertc::NERtcVideoProfileType max_profile)
{
    LOG_F(INFO, "uid:%llu max_profile:%d", uid, max_profile);
    nim_node::node_async_call::async_call([=]() {
        Node_onUserVideoStart(uid, max_profile);
    });    
}

void NertcChannelEventHandler::Node_onUserVideoStart(nertc::uid_t uid, nertc::NERtcVideoProfileType max_profile)
{
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
}

void NertcChannelEventHandler::onUserVideoStop(nertc::uid_t uid)
{
    LOG_F(INFO, "uid:%llu", uid);
    nim_node::node_async_call::async_call([=]() {
        Node_onUserVideoStop(uid);
    });     
}

void NertcChannelEventHandler::Node_onUserVideoStop(nertc::uid_t uid)
{
    auto it = _callbacks.find("onUserVideoStop");
    if (it != _callbacks.end())
    {
        auto function_reference = it->second;
        auto env = function_reference->function.Env();
        auto param1 = Napi::Number::New(env, uid);
        const std::vector<napi_value> args = {param1};
        function_reference->function.Call(args);
    }
}

void NertcChannelEventHandler::onUserVideoMute(nertc::uid_t uid, bool mute)
{
    LOG_F(INFO, "uid:%llu mute:%d", uid, mute);
    nim_node::node_async_call::async_call([=]() {
        Node_onUserVideoMute(uid, mute);
    });
}

void NertcChannelEventHandler::Node_onUserVideoMute(nertc::uid_t uid, bool mute)
{
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
}

void NertcChannelEventHandler::onUserVideoMute(nertc::NERtcVideoStreamType videoStreamType, nertc::uid_t uid, bool mute)
{
    LOG_F(INFO, "uid:%llu mute:%d", uid, mute);
    nim_node::node_async_call::async_call([=]() {
        Node_onUserVideoMuteEx(videoStreamType, uid, mute);
    });
}

void NertcChannelEventHandler::Node_onUserVideoMuteEx(nertc::NERtcVideoStreamType type, nertc::uid_t uid, bool mute)
{
    auto it = _callbacks.find("onUserVideoMuteEx");
    if (it != _callbacks.end())
    {
        auto function_reference = it->second;
        auto env = function_reference->function.Env();
        auto param1 = Napi::Number::New(env, type);
        auto param2 = Napi::Number::New(env, uid);
        auto param3 = Napi::Boolean::New(env, mute);
        const std::vector<napi_value> args = {param1, param2, param3};
        function_reference->function.Call(args);
    }
}

void NertcChannelEventHandler::onUserSubStreamVideoStart(nertc::uid_t uid, nertc::NERtcVideoProfileType max_profile)
{
    LOG_F(INFO, "uid:%llu max_profile:%d", uid, max_profile);
    nim_node::node_async_call::async_call([=]() {
        Node_onUserSubStreamVideoStart(uid, max_profile);
    });
}

void NertcChannelEventHandler::Node_onUserSubStreamVideoStart(nertc::uid_t uid, nertc::NERtcVideoProfileType max_profile)
{
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
}

void NertcChannelEventHandler::onUserSubStreamVideoStop(nertc::uid_t uid)
{
    LOG_F(INFO, "uid:%llu", uid);
    nim_node::node_async_call::async_call([=]() {
        Node_onUserSubStreamVideoStop(uid);
    });
}

void NertcChannelEventHandler::Node_onUserSubStreamVideoStop(nertc::uid_t uid)
{
    auto it = _callbacks.find("onUserSubStreamVideoStop");
    if (it != _callbacks.end())
    {
        auto function_reference = it->second;
        auto env = function_reference->function.Env();
        auto param1 = Napi::Number::New(env, uid);
        const std::vector<napi_value> args = {param1};
        function_reference->function.Call(args);
    }
}

void NertcChannelEventHandler::onScreenCaptureStatus(nertc::NERtcScreenCaptureStatus status)
{
    LOG_F(INFO, "status:%d", status);
    nim_node::node_async_call::async_call([=]() {
        Node_onScreenCaptureStatus(status);
    });
}

void NertcChannelEventHandler::Node_onScreenCaptureStatus(nertc::NERtcScreenCaptureStatus status)
{
    auto it = _callbacks.find("onScreenCaptureStatus");
    if (it != _callbacks.end())
    {
        auto function_reference = it->second;
        auto env = function_reference->function.Env();
        auto param1 = Napi::Number::New(env, status);
        const std::vector<napi_value> args = {param1};
        function_reference->function.Call(args);
    }
}

void NertcChannelEventHandler::onFirstAudioDataReceived(nertc::uid_t uid)
{
    LOG_F(INFO, "uid:%llu", uid);
    nim_node::node_async_call::async_call([=]() {
        Node_onFirstAudioDataReceived(uid);
    });
}

void NertcChannelEventHandler::Node_onFirstAudioDataReceived(nertc::uid_t uid)
{
    auto it = _callbacks.find("onFirstAudioDataReceived");
    if (it != _callbacks.end())
    {
        auto function_reference = it->second;
        auto env = function_reference->function.Env();
        auto param1 = Napi::Number::New(env, uid);
        const std::vector<napi_value> args = {param1};
        function_reference->function.Call(args);
    }
}

void NertcChannelEventHandler::onFirstVideoDataReceived(nertc::uid_t uid)
{
    LOG_F(INFO, "uid:%llu", uid);
    nim_node::node_async_call::async_call([=]() {
        Node_onFirstVideoDataReceived(uid);
    });
}

void NertcChannelEventHandler::Node_onFirstVideoDataReceived(nertc::uid_t uid)
{
    auto it = _callbacks.find("onFirstVideoDataReceived");
    if (it != _callbacks.end())
    {
        auto function_reference = it->second;
        auto env = function_reference->function.Env();
        auto param1 = Napi::Number::New(env, uid);
        const std::vector<napi_value> args = {param1};
        function_reference->function.Call(args);
    }
}

void NertcChannelEventHandler::onFirstVideoDataReceived(nertc::NERtcVideoStreamType type, nertc::uid_t uid)
{
    LOG_F(INFO, "uid:%llu", uid);
    nim_node::node_async_call::async_call([=]() {
        Node_onFirstVideoDataReceivedEx(type, uid);
    });
}

void NertcChannelEventHandler::Node_onFirstVideoDataReceivedEx(nertc::NERtcVideoStreamType type, nertc::uid_t uid)
{
    auto it = _callbacks.find("onFirstVideoDataReceivedEx");
    if (it != _callbacks.end())
    {
        auto function_reference = it->second;
        auto env = function_reference->function.Env();
        auto param1 = Napi::Number::New(env, type);
        auto param2 = Napi::Number::New(env, uid);
        const std::vector<napi_value> args = {param1, param2};
        function_reference->function.Call(args);
    }
}

void NertcChannelEventHandler::onFirstAudioFrameDecoded(nertc::uid_t uid)
{
    LOG_F(INFO, "uid:%llu", uid);
    nim_node::node_async_call::async_call([=]() {
        Node_onFirstAudioFrameDecoded(uid);
    });
}

void NertcChannelEventHandler::Node_onFirstAudioFrameDecoded(nertc::uid_t uid)
{
    auto it = _callbacks.find("onFirstAudioFrameDecoded");
    if (it != _callbacks.end())
    {
        auto function_reference = it->second;
        auto env = function_reference->function.Env();
        auto param1 = Napi::Number::New(env, uid);
        const std::vector<napi_value> args = {param1};
        function_reference->function.Call(args);
    }
}

void NertcChannelEventHandler::onFirstVideoFrameDecoded(nertc::uid_t uid, uint32_t width, uint32_t height)
{
    LOG_F(INFO, "uid:%llu width:%d height:%d", uid, width, height);
    nim_node::node_async_call::async_call([=]() {
        Node_onFirstVideoFrameDecoded(uid, width, height);
    });
}

void NertcChannelEventHandler::Node_onFirstVideoFrameDecoded(nertc::uid_t uid, uint32_t width, uint32_t height)
{
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
}

void NertcChannelEventHandler::onFirstVideoFrameDecoded(nertc::NERtcVideoStreamType type, nertc::uid_t uid, uint32_t width, uint32_t height)
{
    LOG_F(INFO, "uid:%llu width:%d height:%d", uid, width, height);
    nim_node::node_async_call::async_call([=]() {
        Node_onFirstVideoFrameDecodedEx(type, uid, width, height);
    });
}

void NertcChannelEventHandler::Node_onFirstVideoFrameDecodedEx(nertc::NERtcVideoStreamType type, nertc::uid_t uid, uint32_t width, uint32_t height)
{
    auto it = _callbacks.find("onFirstVideoFrameDecodedEx");
    if (it != _callbacks.end())
    {
        auto function_reference = it->second;
        auto env = function_reference->function.Env();
        auto param0 = Napi::Number::New(env, type);
        auto param1 = Napi::Number::New(env, uid);
        auto param2 = Napi::Number::New(env, width);
        auto param3 = Napi::Number::New(env, height);
        const std::vector<napi_value> args = {param0, param1, param2, param3};
        function_reference->function.Call(args);
    }
}

void NertcChannelEventHandler::onLocalAudioVolumeIndication(int volume)
{
    nim_node::node_async_call::async_call([=]() {
        Node_onLocalAudioVolumeIndication(volume);
    });
}

void NertcChannelEventHandler::Node_onLocalAudioVolumeIndication(int volume)
{
    auto it = _callbacks.find("onLocalAudioVolumeIndication");
    if (it != _callbacks.end())
    {
        auto function_reference = it->second;
        auto env = function_reference->function.Env();
        auto param1 = Napi::Number::New(env, volume);
        const std::vector<napi_value> args = {param1};
        function_reference->function.Call(args);
    }
}

void NertcChannelEventHandler::onLocalAudioVolumeIndication(int volume, bool enable_vad)
{
    nim_node::node_async_call::async_call([=]() {
        Node_onLocalAudioVolumeIndicationEx(volume, enable_vad);
    });
}

void NertcChannelEventHandler::Node_onLocalAudioVolumeIndicationEx(int volume, bool enable_vad)
{
    auto it = _callbacks.find("onLocalAudioVolumeIndicationEx");
    if (it != _callbacks.end())
    {
        auto function_reference = it->second;
        auto env = function_reference->function.Env();
        auto param1 = Napi::Number::New(env, volume);
        auto param2 = Napi::Boolean::New(env, enable_vad);
        const std::vector<napi_value> args = {param1, param2};
        function_reference->function.Call(args);
    }
}

void NertcChannelEventHandler::onRemoteAudioVolumeIndication(const nertc::NERtcAudioVolumeInfo *speakers, unsigned int speaker_number, int total_volume)
{
    nertc::NERtcAudioVolumeInfo* dst_speakers = (nertc::NERtcAudioVolumeInfo*)malloc(sizeof(nertc::NERtcAudioVolumeInfo) * (1 + speaker_number));
    memset(dst_speakers, 0, sizeof(nertc::NERtcAudioVolumeInfo) * (1 + speaker_number)) ;
    if(speaker_number > 0){
        memcpy(dst_speakers, speakers, sizeof(nertc::NERtcAudioVolumeInfo) * (speaker_number));
    }

    nim_node::node_async_call::async_call([=]() {
        Node_onRemoteAudioVolumeIndication(dst_speakers, speaker_number, total_volume);
    });
}

void NertcChannelEventHandler::Node_onRemoteAudioVolumeIndication(const nertc::NERtcAudioVolumeInfo *speakers, unsigned int speaker_number, int total_volume)
{
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
    free((void*)speakers);
    speakers = nullptr;
}

void NertcChannelEventHandler::onAddLiveStreamTask(const char* task_id, const char* url, int error_code)
{
    std::string str_task_id = task_id;
    std::string str_url = url;
    LOG_F(INFO, "str_task_id:%s str_url:%s error_code:%d", str_task_id.c_str(), str_url.c_str(), error_code);
    nim_node::node_async_call::async_call([=]() {
        Node_onAddLiveStreamTask(str_task_id, str_url, error_code);
    });
}

void NertcChannelEventHandler::Node_onAddLiveStreamTask(std::string task_id, std::string url, int error_code)
{
    auto it = _callbacks.find("onAddLiveStreamTask");
    if (it != _callbacks.end())
    {
        auto function_reference = it->second;
        auto env = function_reference->function.Env();
        auto param1 = Napi::String::New(env, task_id);
        auto param2 = Napi::String::New(env, url);
        auto param3 = Napi::Number::New(env, error_code);
        const std::vector<napi_value> args = {param1, param2, param3};
        function_reference->function.Call(args);
    }
}

void NertcChannelEventHandler::onUpdateLiveStreamTask(const char* task_id, const char* url, int error_code)
{
    std::string str_task_id = task_id;
    std::string str_url = url;
    LOG_F(INFO, "str_task_id:%s str_url:%s error_code:%d", str_task_id.c_str(), str_url.c_str(), error_code);
    nim_node::node_async_call::async_call([=]() {
        Node_onUpdateLiveStreamTask(str_task_id, str_url, error_code);
    });
}

void NertcChannelEventHandler::Node_onUpdateLiveStreamTask(std::string task_id, std::string url, int error_code)
{
    auto it = _callbacks.find("onUpdateLiveStreamTask");
    if (it != _callbacks.end())
    {
        auto function_reference = it->second;
        auto env = function_reference->function.Env();
        auto param1 = Napi::String::New(env, task_id);
        auto param2 = Napi::String::New(env, url);
        auto param3 = Napi::Number::New(env, error_code);
        const std::vector<napi_value> args = {param1,param2,param3};
        function_reference->function.Call(args);
    }
}

void NertcChannelEventHandler::onRemoveLiveStreamTask(const char* task_id, int error_code)
{
    std::string str_task_id = task_id;
    LOG_F(INFO, "str_task_id:%s error_code:%d", str_task_id.c_str(), error_code);
    nim_node::node_async_call::async_call([=]() {
        Node_onRemoveLiveStreamTask(str_task_id, error_code);
    });
}

void NertcChannelEventHandler::Node_onRemoveLiveStreamTask(std::string task_id, int error_code)
{
    auto it = _callbacks.find("onRemoveLiveStreamTask");
    if (it != _callbacks.end())
    {
        auto function_reference = it->second;
        auto env = function_reference->function.Env();
        auto param1 = Napi::String::New(env, task_id);
        auto param2 = Napi::Number::New(env, error_code);
        const std::vector<napi_value> args = {param1,param2};
        function_reference->function.Call(args);
    }
}

void NertcChannelEventHandler::onLiveStreamState(const char* task_id, const char* url, nertc::NERtcLiveStreamStateCode state)
{
    std::string str_task_id = task_id;
    std::string str_url = url;
    LOG_F(INFO, "str_task_id:%s str_url:%s state:%d", str_task_id.c_str(), str_url.c_str(), state);
    nim_node::node_async_call::async_call([=]() {
        Node_onLiveStreamState(str_task_id, str_url, state);
    });
}

void NertcChannelEventHandler::Node_onLiveStreamState(std::string task_id, std::string url, nertc::NERtcLiveStreamStateCode state)
{
    auto it = _callbacks.find("onLiveStreamState");
    if (it != _callbacks.end())
    {
        auto function_reference = it->second;
        auto env = function_reference->function.Env();
         auto param1 = Napi::String::New(env, task_id);
        auto param2 = Napi::String::New(env, url);
        auto param3 = Napi::Number::New(env, state);
        const std::vector<napi_value> args = {param1,param2,param3};
        function_reference->function.Call(args);
    }
}


void NertcChannelEventHandler::onRecvSEIMsg(nertc::uid_t uid, const char* data, uint32_t dataSize)
{
    std::string strMsg(data, dataSize);
	LOG_F(INFO, "onRecvSEIMsg:%s", strMsg.c_str());
    auto* copied = new char[dataSize];
    memset(copied, 0, dataSize);
    memcpy(copied, data, dataSize);
	nim_node::node_async_call::async_call([=]() {
		Node_onRecvSEIMsg(uid, copied, dataSize);
	});
}

void NertcChannelEventHandler::Node_onRecvSEIMsg(nertc::uid_t uid, const char* data, uint32_t length)
{
    auto it = _callbacks.find("onRecvSEIMsg");
    if (it != _callbacks.end())
    {
        auto function_reference = it->second;
        auto env = function_reference->function.Env();
        auto param1 = Napi::Number::New(env, uid);
        Napi::ArrayBuffer arrayBuffer = Napi::ArrayBuffer::New(env, length);
        memcpy(arrayBuffer.Data(), data, length);
        const std::vector<napi_value> args = {param1, arrayBuffer};
        function_reference->function.Call(args);
    }
    delete [] data;
    data = nullptr;
}

void NertcChannelEventHandler::onMediaRelayStateChanged(nertc::NERtcChannelMediaRelayState state, const char* channel_name)
{
    std::string str_channel_name = channel_name;
    LOG_F(INFO, "state:%d str_channel_name:%s", state, str_channel_name.c_str());
    nim_node::node_async_call::async_call([=]() {
        Node_onMediaRelayStateChanged(state, str_channel_name);
    });
}

void NertcChannelEventHandler::Node_onMediaRelayStateChanged(nertc::NERtcChannelMediaRelayState state, std::string channel_name)
{
    auto it = _callbacks.find("onMediaRelayStateChanged");
    if (it != _callbacks.end())
    {
        auto function_reference = it->second;
        auto env = function_reference->function.Env();
        auto param1 = Napi::Number::New(env, state);
        auto param2 = Napi::String::New(env, channel_name);
        const std::vector<napi_value> args = {param1, param2};
        function_reference->function.Call(args);
    }
}

void NertcChannelEventHandler::onMediaRelayEvent(nertc::NERtcChannelMediaRelayEvent event, const char* channel_name, nertc::NERtcErrorCode error)
{
    std::string str_channel_name = channel_name;
    LOG_F(INFO, "error:%d str_channel_name:%s", error, str_channel_name.c_str());
    nim_node::node_async_call::async_call([=]() {
        Node_onMediaRelayEvent(event, str_channel_name, error);
    });
}

void NertcChannelEventHandler::Node_onMediaRelayEvent(nertc::NERtcChannelMediaRelayEvent event, std::string channel_name, nertc::NERtcErrorCode error)
{
    auto it = _callbacks.find("onMediaRelayEvent");
    if (it != _callbacks.end())
    {
        auto function_reference = it->second;
        auto env = function_reference->function.Env();
        auto param1 = Napi::Number::New(env, event);
        auto param2 = Napi::String::New(env, channel_name);
        auto param3 = Napi::Number::New(env, error);
        const std::vector<napi_value> args = {param1, param2, param3};
        function_reference->function.Call(args);
    }
}

void NertcChannelEventHandler::onLocalPublishFallbackToAudioOnly(bool is_fallback, nertc::NERtcVideoStreamType stream_type)
{
    LOG_F(INFO, "is_fallback:%d stream_type:%d", is_fallback, stream_type);
    nim_node::node_async_call::async_call([=]() {
        Node_onLocalPublishFallbackToAudioOnly(is_fallback, stream_type);
    });
}

void NertcChannelEventHandler::Node_onLocalPublishFallbackToAudioOnly(bool is_fallback, nertc::NERtcVideoStreamType stream_type)
{
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
}

void NertcChannelEventHandler::onRemoteSubscribeFallbackToAudioOnly(nertc::uid_t uid, bool is_fallback, nertc::NERtcVideoStreamType stream_type)
{
    LOG_F(INFO, "uid:%llu is_fallback:%d stream_type:%d", uid, is_fallback, stream_type);
    nim_node::node_async_call::async_call([=]() {
        Node_onRemoteSubscribeFallbackToAudioOnly(uid, is_fallback, stream_type);
    });
}

void NertcChannelEventHandler::Node_onRemoteSubscribeFallbackToAudioOnly(nertc::uid_t uid, bool is_fallback, nertc::NERtcVideoStreamType stream_type)
{
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
}

void NertcChannelEventHandler::onMediaRightChange(bool is_audio_banned, bool is_video_banned)
{
    LOG_F(INFO, "is_audio_banned:%d is_video_banned:%d", is_audio_banned, is_video_banned);
    nim_node::node_async_call::async_call([=]() {
        Node_onMediaRightChange(is_audio_banned, is_video_banned);
    });
}

void NertcChannelEventHandler::Node_onMediaRightChange(bool is_audio_banned, bool is_video_banned)
{
    auto it = _callbacks.find("onMediaRightChange");
    if (it != _callbacks.end())
    {
        auto function_reference = it->second;
        auto env = function_reference->function.Env();
        auto param1 = Napi::Boolean::New(env, is_audio_banned);
        auto param2 = Napi::Boolean::New(env, is_video_banned);
        const std::vector<napi_value> args = {param1, param2};
        function_reference->function.Call(args);
    }
}



void NertcChannelRtcMediaStatsHandler::onRtcStats(const nertc::NERtcStats &stats)
{
    nim_node::node_async_call::async_call([=]() {
        Node_onRtcStats(stats);
    });
}

void NertcChannelRtcMediaStatsHandler::Node_onRtcStats(const nertc::NERtcStats & stats)
{
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
}

void NertcChannelRtcMediaStatsHandler::onLocalAudioStats(const nertc::NERtcAudioSendStats &stats)
{
    nim_node::node_async_call::async_call([=]() {
        Node_onLocalAudioStats(stats);
    });
}

void NertcChannelRtcMediaStatsHandler::Node_onLocalAudioStats(const nertc::NERtcAudioSendStats & stats)
{
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
}

void NertcChannelRtcMediaStatsHandler::onRemoteAudioStats(const nertc::NERtcAudioRecvStats *stats, unsigned int user_count)
{
    if (user_count <= 0)
        return;
    nertc::NERtcAudioRecvStats *ss = new nertc::NERtcAudioRecvStats[user_count];
    for (auto i = 0; i < user_count; i++) {
        ss[i] = stats[i];
    }
    nim_node::node_async_call::async_call([=]() {
        Node_onRemoteAudioStats(ss, user_count);
    });
}

void NertcChannelRtcMediaStatsHandler::Node_onRemoteAudioStats(const nertc::NERtcAudioRecvStats *stats, unsigned int user_count)
{
    if (user_count <= 0)
        return;
    auto it = _callbacks.find("onRemoteAudioStats");
    if (it != _callbacks.end())
    {
        auto function_reference = it->second;
        auto env = function_reference->function.Env();
        Napi::Array s = Napi::Array::New(env);
        for (auto i = 0; i < (int)user_count; i++)
        {
            Napi::Object o = Napi::Object::New(env);
            nertc_audio_recv_stats_to_obj(env, stats[i], o);
            // s[i] = o;
            s.Set(static_cast<napi_value>(Napi::Number::New(env, i)),  o);
        }
        Napi::Number param1 = Napi::Number::New(env, (int)user_count);
        const std::vector<napi_value> args = {param1, s};
        function_reference->function.Call(args);
    }
    if (stats)
    {
        delete[] stats;
        stats = nullptr;
    }
}

void NertcChannelRtcMediaStatsHandler::onLocalVideoStats(const nertc::NERtcVideoSendStats &stats)
{
    nertc::NERtcVideoSendStats ss;
    ss.video_layers_count = stats.video_layers_count;
    ss.video_layers_list = new nertc::NERtcVideoLayerSendStats[ss.video_layers_count];
    for (auto i = 0; i < ss.video_layers_count; i++) {
        ss.video_layers_list[i] = stats.video_layers_list[i];
    }

    nim_node::node_async_call::async_call([=]() {
        Node_onLocalVideoStats(ss);
    });
}

void NertcChannelRtcMediaStatsHandler::Node_onLocalVideoStats(const nertc::NERtcVideoSendStats& ss)
{
    auto it = _callbacks.find("onLocalVideoStats");
    if (it != _callbacks.end())
    {
        auto function_reference = it->second;
        auto env = function_reference->function.Env();
        Napi::Object s = Napi::Object::New(env);
        nertc_video_send_stats_to_obj(env, ss, s);
        const std::vector<napi_value> args = {s};
        function_reference->function.Call(args);
    }
    if (ss.video_layers_list)
    {
        delete[] ss.video_layers_list;
    }
}

void NertcChannelRtcMediaStatsHandler::onRemoteVideoStats(const nertc::NERtcVideoRecvStats *stats, unsigned int user_count)
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
        Node_onRemoteVideoStats(ss, user_count);
    });
}

void NertcChannelRtcMediaStatsHandler::Node_onRemoteVideoStats(const nertc::NERtcVideoRecvStats *ss, unsigned int user_count)
{
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
        auto param1 = Napi::Number::New(env, (int)user_count);
        const std::vector<napi_value> args = {param1, s};
        function_reference->function.Call(args);
    }
    if (ss)
    {
        for (int i = 0; i < user_count; i++)
        {
            if (ss[i].video_layers_list)
            {
                delete[] ss[i].video_layers_list;
                // ss[i].video_layers_list = nullptr;
            }
        }
        delete[] ss;
        // ss = nullptr;
    }
}

void NertcChannelRtcMediaStatsHandler::onNetworkQuality(const nertc::NERtcNetworkQualityInfo *infos, unsigned int user_count)
{
    nertc::NERtcNetworkQualityInfo *ss = new nertc::NERtcNetworkQualityInfo[user_count];
    for (auto i = 0; i < user_count; i++) {
        ss[i] = infos[i];
    }
    nim_node::node_async_call::async_call([=]() {
        Node_onNetworkQuality(ss, user_count);
    });
    
}

void NertcChannelRtcMediaStatsHandler::Node_onNetworkQuality(const nertc::NERtcNetworkQualityInfo *ss, unsigned int user_count)
{
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

        auto param1 = Napi::Number::New(env, user_count);
        const std::vector<napi_value> args = {param1, s};
        function_reference->function.Call(args);
    }
    if (ss) {
        delete[] ss;
        ss = nullptr;
    }
}


}