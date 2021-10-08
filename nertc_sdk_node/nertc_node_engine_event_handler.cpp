#include "nertc_node_engine_event_handler.h"
#include "../shared/sdk_helper/nim_node_async_queue.h"
#include "nertc_node_engine_helper.h"

namespace nertc_node
{
void NertcNodeEventHandler::onError(int error_code, const char* msg)
{
    utf8_string str_msg = msg;
    nim_node::node_async_call::async_call([=]() {
        NertcNodeEventHandler::GetInstance()->Node_onError(error_code, str_msg);
    });
}

void NertcNodeEventHandler::onWarning(int warn_code, const char* msg)
{
    utf8_string str_msg = msg;
    nim_node::node_async_call::async_call([=]() {
        NertcNodeEventHandler::GetInstance()->Node_onWarning(warn_code, str_msg);
    });    
}

void NertcNodeEventHandler::onReleasedHwResources(nertc::NERtcErrorCode result)
{
    nim_node::node_async_call::async_call([=]() {
        NertcNodeEventHandler::GetInstance()->Node_onReleasedHwResources(result);
    });        
}

void NertcNodeEventHandler::onJoinChannel(nertc::channel_id_t cid, nertc::uid_t uid, nertc::NERtcErrorCode result, uint64_t elapsed)
{
    nim_node::node_async_call::async_call([=]() {
        NertcNodeEventHandler::GetInstance()->Node_onJoinChannel(cid, uid, result, elapsed);
    });      
}

void NertcNodeEventHandler::onConnectionStateChange(nertc::NERtcConnectionStateType state, nertc::NERtcReasonConnectionChangedType reason)
{
    nim_node::node_async_call::async_call([=]() {
        NertcNodeEventHandler::GetInstance()->Node_onConnectionStateChange(state, reason);
    });     
}

void NertcNodeEventHandler::onReconnectingStart(nertc::channel_id_t cid, nertc::uid_t uid)
{
    nim_node::node_async_call::async_call([=]() {
        NertcNodeEventHandler::GetInstance()->Node_onReconnectingStart(cid, uid);
    });       
}

void NertcNodeEventHandler::onRejoinChannel(nertc::channel_id_t cid, nertc::uid_t uid, nertc::NERtcErrorCode result, uint64_t elapsed)
{
    nim_node::node_async_call::async_call([=]() {
        NertcNodeEventHandler::GetInstance()->Node_onRejoinChannel(cid, uid, result, elapsed);
    });        
}

void NertcNodeEventHandler::onLeaveChannel(nertc::NERtcErrorCode result)
{
    nim_node::node_async_call::async_call([=]() {
        NertcNodeEventHandler::GetInstance()->Node_onLeaveChannel(result);
    });         
}

void NertcNodeEventHandler::onDisconnect(nertc::NERtcErrorCode reason)
{
    nim_node::node_async_call::async_call([=]() {
        NertcNodeEventHandler::GetInstance()->Node_onDisconnect(reason);
    });      
}

void NertcNodeEventHandler::onClientRoleChanged(nertc::NERtcClientRole oldRole, nertc::NERtcClientRole newRole)
{
    nim_node::node_async_call::async_call([=]() {
        NertcNodeEventHandler::GetInstance()->Node_onClientRoleChanged(oldRole, newRole);
    });        
}

void NertcNodeEventHandler::onUserJoined(nertc::uid_t uid, const char * user_name)
{
    utf8_string str_user_name = user_name;
    nim_node::node_async_call::async_call([=]() {
        NertcNodeEventHandler::GetInstance()->Node_onUserJoined(uid, str_user_name);
    });         
}

void NertcNodeEventHandler::onUserLeft(nertc::uid_t uid, nertc::NERtcSessionLeaveReason reason)
{
    nim_node::node_async_call::async_call([=]() {
        NertcNodeEventHandler::GetInstance()->Node_onUserLeft(uid, reason);
    });       
}

void NertcNodeEventHandler::onUserAudioStart(nertc::uid_t uid)
{
    nim_node::node_async_call::async_call([=]() {
        NertcNodeEventHandler::GetInstance()->Node_onUserAudioStart(uid);
    });     
}

void NertcNodeEventHandler::onUserAudioStop(nertc::uid_t uid)
{
    nim_node::node_async_call::async_call([=]() {
        NertcNodeEventHandler::GetInstance()->Node_onUserAudioStop(uid);
    });      
}

void NertcNodeEventHandler::onUserVideoStart(nertc::uid_t uid, nertc::NERtcVideoProfileType max_profile)
{
    nim_node::node_async_call::async_call([=]() {
        NertcNodeEventHandler::GetInstance()->Node_onUserVideoStart(uid, max_profile);
    });    
}

void NertcNodeEventHandler::onUserVideoStop(nertc::uid_t uid)
{
    nim_node::node_async_call::async_call([=]() {
        NertcNodeEventHandler::GetInstance()->Node_onUserVideoStop(uid);
    });     
}

void NertcNodeEventHandler::Node_onError(int error_code, const utf8_string& msg)
{
    Isolate *isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    const unsigned argc = 2;
    Local<Value> argv[argc] = { nim_napi_new_int32(isolate, error_code), nim_napi_new_utf8string(isolate, msg.c_str()) };
    auto it = callbacks_.find("onError");
    if (it != callbacks_.end())
    {
        it->second->callback_.Get(isolate)->Call(isolate->GetCurrentContext(), it->second->data_.Get(isolate), argc, argv);
    }   
}

void NertcNodeEventHandler::Node_onWarning(int warn_code, const utf8_string& msg)
{
    Isolate *isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    const unsigned argc = 2;
    Local<Value> argv[argc] = { nim_napi_new_int32(isolate, warn_code), nim_napi_new_utf8string(isolate, msg.c_str()) };
    auto it = callbacks_.find("onWarning");
    if (it != callbacks_.end())
    {
        it->second->callback_.Get(isolate)->Call(isolate->GetCurrentContext(), it->second->data_.Get(isolate), argc, argv);
    }   
}

void NertcNodeEventHandler::Node_onReleasedHwResources(nertc::NERtcErrorCode result)
{
    Isolate *isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    const unsigned argc = 1;
    Local<Value> argv[argc] = { nim_napi_new_int32(isolate, result) };
    auto it = callbacks_.find("onReleasedHwResources");
    if (it != callbacks_.end())
    {
        it->second->callback_.Get(isolate)->Call(isolate->GetCurrentContext(), it->second->data_.Get(isolate), argc, argv);
    }   
}

void NertcNodeEventHandler::Node_onJoinChannel(nertc::channel_id_t cid, nertc::uid_t uid, nertc::NERtcErrorCode result, uint64_t elapsed)
{
    Isolate *isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    const unsigned argc = 4;
    Local<Value> argv[argc] = {nim_napi_new_uint64(isolate, cid),
                               nim_napi_new_uint64(isolate, uid),
                               nim_napi_new_int32(isolate, result),
                               nim_napi_new_int32(isolate, elapsed)};
    auto it = callbacks_.find("onJoinChannel");
    if (it != callbacks_.end())
    {
        it->second->callback_.Get(isolate)->Call(isolate->GetCurrentContext(), it->second->data_.Get(isolate), argc, argv);
    }
}

void NertcNodeEventHandler::Node_onConnectionStateChange(nertc::NERtcConnectionStateType state, nertc::NERtcReasonConnectionChangedType reason)
{
    Isolate *isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    const unsigned argc = 2;
    Local<Value> argv[argc] = {nim_napi_new_uint32(isolate, state),
                               nim_napi_new_uint32(isolate, reason)};
    auto it = callbacks_.find("onConnectionStateChange");
    if (it != callbacks_.end())
    {
        it->second->callback_.Get(isolate)->Call(isolate->GetCurrentContext(), it->second->data_.Get(isolate), argc, argv);
    }      
}

void NertcNodeEventHandler::Node_onReconnectingStart(nertc::channel_id_t cid, nertc::uid_t uid)
{
    Isolate *isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    const unsigned argc = 2;
    Local<Value> argv[argc] = {nim_napi_new_uint64(isolate, cid),
                               nim_napi_new_uint64(isolate, uid)};
    auto it = callbacks_.find("onReconnectingStart");
    if (it != callbacks_.end())
    {
        it->second->callback_.Get(isolate)->Call(isolate->GetCurrentContext(), it->second->data_.Get(isolate), argc, argv);
    }  
}

void NertcNodeEventHandler::Node_onRejoinChannel(nertc::channel_id_t cid, nertc::uid_t uid, nertc::NERtcErrorCode result, uint64_t elapsed)
{
    Isolate *isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    const unsigned argc = 4;
    Local<Value> argv[argc] = {nim_napi_new_uint64(isolate, cid),
                               nim_napi_new_uint64(isolate, uid),
                               nim_napi_new_int32(isolate, result),
                               nim_napi_new_int32(isolate, elapsed)};
    auto it = callbacks_.find("onRejoinChannel");
    if (it != callbacks_.end())
    {
        it->second->callback_.Get(isolate)->Call(isolate->GetCurrentContext(), it->second->data_.Get(isolate), argc, argv);
    } 
}

void NertcNodeEventHandler::Node_onLeaveChannel(nertc::NERtcErrorCode result)
{
    Isolate *isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    const unsigned argc = 1;
    Local<Value> argv[argc] = { nim_napi_new_int32(isolate, result) };
    auto it = callbacks_.find("onLeaveChannel");
    if (it != callbacks_.end())
    {
        it->second->callback_.Get(isolate)->Call(isolate->GetCurrentContext(), it->second->data_.Get(isolate), argc, argv);
    }   
}

void NertcNodeEventHandler::Node_onDisconnect(nertc::NERtcErrorCode reason)
{
    Isolate *isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    const unsigned argc = 1;
    Local<Value> argv[argc] = { nim_napi_new_int32(isolate, reason) };
    auto it = callbacks_.find("onDisconnect");
    if (it != callbacks_.end())
    {
        it->second->callback_.Get(isolate)->Call(isolate->GetCurrentContext(), it->second->data_.Get(isolate), argc, argv);
    }   
}

void NertcNodeEventHandler::Node_onClientRoleChanged(nertc::NERtcClientRole oldRole, nertc::NERtcClientRole newRole)
{
    Isolate *isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    const unsigned argc = 2;
    Local<Value> argv[argc] = { nim_napi_new_uint32(isolate, oldRole), nim_napi_new_uint32(isolate, newRole) };
    auto it = callbacks_.find("onClientRoleChanged");
    if (it != callbacks_.end())
    {
        it->second->callback_.Get(isolate)->Call(isolate->GetCurrentContext(), it->second->data_.Get(isolate), argc, argv);
    }       
}

void NertcNodeEventHandler::Node_onUserJoined(nertc::uid_t uid, const utf8_string& user_name)
{
    Isolate *isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    const unsigned argc = 2;
    Local<Value> argv[argc] = {
        nim_napi_new_int64(isolate, uid),
        nim_napi_new_utf8string(isolate, user_name.c_str())
    };
    auto it = callbacks_.find("onUserJoined");
    if (it != callbacks_.end())
    {
        it->second->callback_.Get(isolate)->Call(isolate->GetCurrentContext(), it->second->data_.Get(isolate), argc, argv);
    } 
}

void NertcNodeEventHandler::Node_onUserLeft(nertc::uid_t uid, nertc::NERtcSessionLeaveReason reason)
{
    Isolate *isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    const unsigned argc = 2;
    Local<Value> argv[argc] = { nim_napi_new_int64(isolate, uid), nim_napi_new_uint32(isolate, reason) };
    auto it = callbacks_.find("onUserLeft");
    if (it != callbacks_.end())
    {
        it->second->callback_.Get(isolate)->Call(isolate->GetCurrentContext(), it->second->data_.Get(isolate), argc, argv);
    } 
}

void NertcNodeEventHandler::Node_onUserAudioStart(nertc::uid_t uid)
{
    Isolate *isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    const unsigned argc = 1;
    Local<Value> argv[argc] = { nim_napi_new_int64(isolate, uid)};
    auto it = callbacks_.find("onUserAudioStart");
    if (it != callbacks_.end())
    {
        it->second->callback_.Get(isolate)->Call(isolate->GetCurrentContext(), it->second->data_.Get(isolate), argc, argv);
    } 
}

void NertcNodeEventHandler::Node_onUserAudioStop(nertc::uid_t uid)
{
    Isolate *isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    const unsigned argc = 1;
    Local<Value> argv[argc] = { nim_napi_new_int64(isolate, uid)};
    auto it = callbacks_.find("onUserAudioStop");
    if (it != callbacks_.end())
    {
        it->second->callback_.Get(isolate)->Call(isolate->GetCurrentContext(), it->second->data_.Get(isolate), argc, argv);
    } 
}

void NertcNodeEventHandler::Node_onUserVideoStart(nertc::uid_t uid, nertc::NERtcVideoProfileType max_profile)
{
    Isolate *isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    const unsigned argc = 2;
    Local<Value> argv[argc] = { nim_napi_new_int64(isolate, uid), nim_napi_new_uint32(isolate, max_profile)};
    auto it = callbacks_.find("onUserVideoStart");
    if (it != callbacks_.end())
    {
        it->second->callback_.Get(isolate)->Call(isolate->GetCurrentContext(), it->second->data_.Get(isolate), argc, argv);
    } 
}

void NertcNodeEventHandler::Node_onUserVideoStop(nertc::uid_t uid)
{
    Isolate *isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    const unsigned argc = 1;
    Local<Value> argv[argc] = { nim_napi_new_int64(isolate, uid)};
    auto it = callbacks_.find("onUserVideoStop");
    if (it != callbacks_.end())
    {
        it->second->callback_.Get(isolate)->Call(isolate->GetCurrentContext(), it->second->data_.Get(isolate), argc, argv);
    } 
}

void NertcNodeRtcMediaStatsHandler::onRtcStats(const nertc::NERtcStats &stats)
{
    nim_node::node_async_call::async_call([=]() {
        NertcNodeRtcMediaStatsHandler::GetInstance()->Node_onRtcStats(stats);
    });   
}

void NertcNodeRtcMediaStatsHandler::onLocalAudioStats(const nertc::NERtcAudioSendStats &stats)
{
    nim_node::node_async_call::async_call([=]() {
        NertcNodeRtcMediaStatsHandler::GetInstance()->Node_onLocalAudioStats(stats);
    });   
}

void NertcNodeRtcMediaStatsHandler::onRemoteAudioStats(const nertc::NERtcAudioRecvStats *stats, unsigned int user_count)
{
    nertc::NERtcAudioRecvStats *ss = new nertc::NERtcAudioRecvStats[user_count];
    for (auto i = 0; i < user_count; i++) {
        ss[i] = stats[i];
    }
    nim_node::node_async_call::async_call([=]() {
        NertcNodeRtcMediaStatsHandler::GetInstance()->Node_onRemoteAudioStats(ss, user_count);
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
        NertcNodeRtcMediaStatsHandler::GetInstance()->Node_onLocalVideoStats(ss);
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
        NertcNodeRtcMediaStatsHandler::GetInstance()->Node_onRemoteVideoStats(ss, user_count);
    });  
}

void NertcNodeRtcMediaStatsHandler::onNetworkQuality(const nertc::NERtcNetworkQualityInfo *infos, unsigned int user_count)
{
    nertc::NERtcNetworkQualityInfo *ss = new nertc::NERtcNetworkQualityInfo[user_count];
    for (auto i = 0; i < user_count; i++) {
        ss[i] = infos[i];
    }
    nim_node::node_async_call::async_call([=]() {
        NertcNodeRtcMediaStatsHandler::GetInstance()->Node_onNetworkQuality(ss, user_count);
    });  
}

void NertcNodeRtcMediaStatsHandler::Node_onRtcStats(const nertc::NERtcStats &stats)
{
    Isolate *isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    const unsigned argc = 1;
    Local<Object> s = Object::New(isolate);
    nertc_stats_to_obj(isolate, stats, s);
    Local<Value> argv[argc] = { s };
    auto it = callbacks_.find("onRtcStats");
    if (it != callbacks_.end())
    {
        it->second->callback_.Get(isolate)->Call(isolate->GetCurrentContext(), it->second->data_.Get(isolate), argc, argv);
    }
}

void NertcNodeRtcMediaStatsHandler::Node_onLocalAudioStats(const nertc::NERtcAudioSendStats &stats)
{
    Isolate *isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    const unsigned argc = 1;
    Local<Object> s = Object::New(isolate);
    nertc_audio_send_stats_to_obj(isolate, stats, s);
    Local<Value> argv[argc] = { s };
    auto it = callbacks_.find("onLocalAudioStats");
    if (it != callbacks_.end())
    {
        it->second->callback_.Get(isolate)->Call(isolate->GetCurrentContext(), it->second->data_.Get(isolate), argc, argv);
    }
}
void NertcNodeRtcMediaStatsHandler::Node_onRemoteAudioStats(const nertc::NERtcAudioRecvStats *stats, unsigned int user_count)
{
    Isolate *isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    const unsigned argc = 2;
    Local<Array> s = Array::New(isolate);
    for (auto i = 0; i < (int)user_count; i++)
    {
        Local<Object> o = Object::New(isolate);
        nertc_audio_recv_stats_to_obj(isolate, stats[i], o);
        s->Set(isolate->GetCurrentContext(), i, o);
    }
    if (stats)
    {
        delete[] stats;
    }
    Local<Value> argv[argc] = { nim_napi_new_uint32(isolate, user_count), s };
    auto it = callbacks_.find("onRemoteAudioStats");
    if (it != callbacks_.end())
    {
        it->second->callback_.Get(isolate)->Call(isolate->GetCurrentContext(), it->second->data_.Get(isolate), argc, argv);
    }
}

void NertcNodeRtcMediaStatsHandler::Node_onLocalVideoStats(const nertc::NERtcVideoSendStats &stats)
{
    Isolate *isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    const unsigned argc = 1;
    Local<Object> s = Object::New(isolate);
    nertc_video_send_stats_to_obj(isolate, stats, s);
    if (stats.video_layers_list)
    {
        delete[] stats.video_layers_list;
    }
    Local<Value> argv[argc] = { s };
    auto it = callbacks_.find("onLocalVideoStats");
    if (it != callbacks_.end())
    {
        it->second->callback_.Get(isolate)->Call(isolate->GetCurrentContext(), it->second->data_.Get(isolate), argc, argv);
    }
}

void NertcNodeRtcMediaStatsHandler::Node_onRemoteVideoStats(const nertc::NERtcVideoRecvStats *stats, unsigned int user_count)
{
    Isolate *isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    const unsigned argc = 2;
    Local<Array> s = Array::New(isolate);
    for (auto i = 0; i < (int)user_count; i++)
    {
        Local<Object> o = Object::New(isolate);
        nertc_video_recv_stats_to_obj(isolate, stats[i], o);
        s->Set(isolate->GetCurrentContext(), i, o);
    }
    if (stats)
    {
        for (int i = 0; i < user_count; i++)
        {
            if (stats[i].video_layers_list)
            {
                delete[] stats[i].video_layers_list;
            }
        }
        delete[] stats;
    }
    Local<Value> argv[argc] = { nim_napi_new_uint32(isolate, user_count), s };
    auto it = callbacks_.find("onRemoteVideoStats");
    if (it != callbacks_.end())
    {
        it->second->callback_.Get(isolate)->Call(isolate->GetCurrentContext(), it->second->data_.Get(isolate), argc, argv);
    }
}

void NertcNodeRtcMediaStatsHandler::Node_onNetworkQuality(const nertc::NERtcNetworkQualityInfo *infos, unsigned int user_count)
{
    Isolate *isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    const unsigned argc = 2;
    Local<Array> s = Array::New(isolate);
    for (auto i = 0; i < (int)user_count; i++)
    {
        Local<Object> o = Object::New(isolate);
        nertc_network_quality_to_obj(isolate, infos[i], o);
        s->Set(isolate->GetCurrentContext(), i, o);
    }
    if (infos) {
        delete[] infos;
    }
    Local<Value> argv[argc] = { nim_napi_new_uint32(isolate, user_count), s };
    auto it = callbacks_.find("onNetworkQuality");
    if (it != callbacks_.end())
    {
        it->second->callback_.Get(isolate)->Call(isolate->GetCurrentContext(), it->second->data_.Get(isolate), argc, argv);
    }
}

void NertcNodeRtcAudioFrameHandler::onAudioFrameDidRecord(nertc::NERtcAudioFrame *frame)
{
    nim_node::node_async_call::async_call([=]() {
        NertcNodeRtcMediaStatsHandler::GetInstance()->Node_onAudioFrameDidRecord(frame);
    });  
}

void NertcNodeRtcAudioFrameHandler::onAudioFrameWillPlayback(nertc::NERtcAudioFrame *frame)
{
    nim_node::node_async_call::async_call([=]() {
        NertcNodeRtcMediaStatsHandler::GetInstance()->Node_onAudioFrameWillPlayback(frame);
    });  
}

void NertcNodeRtcAudioFrameHandler::onMixedAudioFrame(nertc::NERtcAudioFrame *frame)
{
    nim_node::node_async_call::async_call([=]() {
        NertcNodeRtcMediaStatsHandler::GetInstance()->Node_onMixedAudioFrame(frame);
    }); 
}

void NertcNodeRtcAudioFrameHandler::onPlaybackAudioFrameBeforeMixing(uint64_t userID, nertc::NERtcAudioFrame *frame)
{
    nim_node::node_async_call::async_call([=]() {
        NertcNodeRtcMediaStatsHandler::GetInstance()->Node_onPlaybackAudioFrameBeforeMixing(userID, frame);
    }); 
}

void NertcNodeRtcAudioFrameHandler::Node_onAudioFrameDidRecord(nertc::NERtcAudioFrame *frame)
{
    uint32_t length = frame->format.samples_per_channel * frame->format.channels * frame->format.bytes_per_sample;
    Local<v8::ArrayBuffer> buffer = ArrayBuffer::New(isolate, frame->data, length);
    Isolate* isolate = Isolate::GetCurrent();
    const unsigned argc = 2;
    Local<Value> argv[argc] = { 
        buffer,
        nim_napi_new_uint32(isolate, (uint32_t)length),
    };
    auto it = callbacks_.find("onAudioFrameDidRecord");
    if (it != callbacks_.end())
    {
        it->second->callback_.Get(isolate)->Call(isolate->GetCurrentContext(), it->second->data_.Get(isolate), argc, argv);
    }

}

void NertcNodeRtcAudioFrameHandler::Node_onAudioFrameWillPlayback(nertc::NERtcAudioFrame *frame)
{
    uint32_t length = frame->format.samples_per_channel * frame->format.channels * frame->format.bytes_per_sample;
    Local<v8::ArrayBuffer> buffer = ArrayBuffer::New(isolate, frame->data, length);
    Isolate* isolate = Isolate::GetCurrent();
    const unsigned argc = 2;
    Local<Value> argv[argc] = { 
        buffer,
        nim_napi_new_uint32(isolate, (uint32_t)length),
    };
    auto it = callbacks_.find("onAudioFrameWillPlayback");
    if (it != callbacks_.end())
    {
        it->second->callback_.Get(isolate)->Call(isolate->GetCurrentContext(), it->second->data_.Get(isolate), argc, argv);
    }
}

void NertcNodeRtcAudioFrameHandler::Node_onMixedAudioFrame(nertc::NERtcAudioFrame *frame)
{
    uint32_t length = frame->format.samples_per_channel * frame->format.channels * frame->format.bytes_per_sample;
    Local<v8::ArrayBuffer> buffer = ArrayBuffer::New(isolate, frame->data, length);
    Isolate* isolate = Isolate::GetCurrent();
    const unsigned argc = 2;
    Local<Value> argv[argc] = { 
        buffer,
        nim_napi_new_uint32(isolate, (uint32_t)length),
    };
    auto it = callbacks_.find("onMixedAudioFrame");
    if (it != callbacks_.end())
    {
        it->second->callback_.Get(isolate)->Call(isolate->GetCurrentContext(), it->second->data_.Get(isolate), argc, argv);
    }
}

void NertcNodeRtcAudioFrameHandler::Node_onPlaybackAudioFrameBeforeMixing(uint64_t userID, nertc::NERtcAudioFrame *frame)
{
    uint32_t length = frame->format.samples_per_channel * frame->format.channels * frame->format.bytes_per_sample;
    Local<v8::ArrayBuffer> buffer = ArrayBuffer::New(isolate, frame->data, length);
    Isolate* isolate = Isolate::GetCurrent();
    const unsigned argc = 3;
    Local<Value> argv[argc] = { 
        nim_napi_new_uint64(isolate, (uint64_t)userID),
        buffer,
        nim_napi_new_uint32(isolate, (uint32_t)length),
    };
    auto it = callbacks_.find("onPlaybackAudioFrameBeforeMixing");
    if (it != callbacks_.end())
    {
        it->second->callback_.Get(isolate)->Call(isolate->GetCurrentContext(), it->second->data_.Get(isolate), argc, argv);
    }
}

void NertcNodeEventHandler::onUserSubStreamVideoStart(nertc::uid_t uid, nertc::NERtcVideoProfileType max_profile)
{
    nim_node::node_async_call::async_call([=]() {
        NertcNodeEventHandler::GetInstance()->Node_onUserSubStreamVideoStart(uid, max_profile);
    });
}

void NertcNodeEventHandler::onUserSubStreamVideoStop(nertc::uid_t uid)
{
    nim_node::node_async_call::async_call([=]() {
        NertcNodeEventHandler::GetInstance()->Node_onUserSubStreamVideoStop(uid);
    });
}

void NertcNodeEventHandler::onUserVideoProfileUpdate(nertc::uid_t uid, nertc::NERtcVideoProfileType max_profile)
{
    nim_node::node_async_call::async_call([=]() {
        NertcNodeEventHandler::GetInstance()->Node_onUserVideoProfileUpdate(uid, max_profile);
    });
}

void NertcNodeEventHandler::onUserAudioMute(nertc::uid_t uid, bool mute)
{
    nim_node::node_async_call::async_call([=]() {
        NertcNodeEventHandler::GetInstance()->Node_onUserAudioMute(uid, mute);
    });
}

void NertcNodeEventHandler::onUserVideoMute(nertc::uid_t uid, bool mute)
{
    nim_node::node_async_call::async_call([=]() {
        NertcNodeEventHandler::GetInstance()->Node_onUserVideoMute(uid, mute);
    });
}

void NertcNodeEventHandler::onAudioDeviceStateChanged(const char device_id[kNERtcMaxDeviceIDLength],
        nertc::NERtcAudioDeviceType device_type,
        nertc::NERtcAudioDeviceState device_state)
{
    utf8_string str_device_id = device_id;
    nim_node::node_async_call::async_call([=]() {
        NertcNodeEventHandler::GetInstance()->Node_onAudioDeviceStateChanged(str_device_id, device_type, device_state);
    });
}

void NertcNodeEventHandler::onAudioDefaultDeviceChanged(const char device_id[kNERtcMaxDeviceIDLength],
        nertc::NERtcAudioDeviceType device_type)
{
    utf8_string str_device_id = device_id;
    nim_node::node_async_call::async_call([=]() {
        NertcNodeEventHandler::GetInstance()->Node_onAudioDefaultDeviceChanged(str_device_id, device_type);
    });
}

void NertcNodeEventHandler::onVideoDeviceStateChanged(const char device_id[kNERtcMaxDeviceIDLength],
        nertc::NERtcVideoDeviceType device_type,
        nertc::NERtcVideoDeviceState device_state)
{
    utf8_string str_device_id = device_id;
    nim_node::node_async_call::async_call([=]() {
        NertcNodeEventHandler::GetInstance()->Node_onVideoDeviceStateChanged(str_device_id, device_type, device_state);
    });
}

void NertcNodeEventHandler::onFirstAudioDataReceived(nertc::uid_t uid)
{
    nim_node::node_async_call::async_call([=]() {
        NertcNodeEventHandler::GetInstance()->Node_onFirstAudioDataReceived(uid);
    });
}

void NertcNodeEventHandler::onFirstVideoDataReceived(nertc::uid_t uid)
{
    nim_node::node_async_call::async_call([=]() {
        NertcNodeEventHandler::GetInstance()->Node_onFirstVideoDataReceived(uid);
    });
}

void NertcNodeEventHandler::onFirstAudioFrameDecoded(nertc::uid_t uid)
{
    nim_node::node_async_call::async_call([=]() {
        NertcNodeEventHandler::GetInstance()->Node_onFirstAudioFrameDecoded(uid);
    });
}

void NertcNodeEventHandler::onFirstVideoFrameDecoded(nertc::uid_t uid, uint32_t width, uint32_t height)
{
    nim_node::node_async_call::async_call([=]() {
        NertcNodeEventHandler::GetInstance()->Node_onFirstVideoFrameDecoded(uid, width, height);
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
        NertcNodeEventHandler::GetInstance()->Node_onAudioMixingStateChanged(state, error_code);
    });
}

void NertcNodeEventHandler::onAudioMixingTimestampUpdate(uint64_t timestamp_ms)
{
    nim_node::node_async_call::async_call([=]() {
        NertcNodeEventHandler::GetInstance()->Node_onAudioMixingTimestampUpdate(timestamp_ms);
    });
}

void NertcNodeEventHandler::onAudioEffectFinished(uint32_t effect_id)
{
    nim_node::node_async_call::async_call([=]() {
        NertcNodeEventHandler::GetInstance()->Node_onAudioEffectFinished(effect_id);
    });
}

void NertcNodeEventHandler::onLocalAudioVolumeIndication(int volume)
{
    nim_node::node_async_call::async_call([=]() {
        NertcNodeEventHandler::GetInstance()->Node_onLocalAudioVolumeIndication(volume);
    });
}

void NertcNodeEventHandler::onRemoteAudioVolumeIndication(const nertc::NERtcAudioVolumeInfo *speakers, unsigned int speaker_number, int total_volume)
{
    nim_node::node_async_call::async_call([=]() {
        NertcNodeEventHandler::GetInstance()->Node_onRemoteAudioVolumeIndication(speakers, speaker_number, total_volume);
    });
}

void NertcNodeEventHandler::onAddLiveStreamTask(const char* task_id, const char* url, int error_code)
{
    nim_node::node_async_call::async_call([=]() {
        NertcNodeEventHandler::GetInstance()->Node_onAddLiveStreamTask(task_id, url, error_code);
    });
}

void NertcNodeEventHandler::onUpdateLiveStreamTask(const char* task_id, const char* url, int error_code)
{
    utf8_string str_task_id = task_id;
    utf8_string str_url = url;
    nim_node::node_async_call::async_call([=]() {
        NertcNodeEventHandler::GetInstance()->Node_onUpdateLiveStreamTask(str_task_id, str_url, error_code);
    });
}

void NertcNodeEventHandler::onRemoveLiveStreamTask(const char* task_id, int error_code)
{
    utf8_string str_task_id = task_id;
    nim_node::node_async_call::async_call([=]() {
        NertcNodeEventHandler::GetInstance()->Node_onRemoveLiveStreamTask(str_task_id, error_code);
    });
}

void NertcNodeEventHandler::onLiveStreamState(const char* task_id, const char* url, nertc::NERtcLiveStreamStateCode state)
{
    utf8_string str_task_id = task_id;
    utf8_string str_url = url;
    nim_node::node_async_call::async_call([=]() {
        NertcNodeEventHandler::GetInstance()->Node_onLiveStreamState(str_task_id, str_url, state);
    });
}
 
void NertcNodeEventHandler::onAudioHowling(bool howling)
{
    nim_node::node_async_call::async_call([=]() {
        NertcNodeEventHandler::GetInstance()->Node_onAudioHowling(howling);
    });
}

void NertcNodeEventHandler::onRecvSEIMsg(nertc::uid_t uid, const char* data, uint32_t dataSize)
{
    auto* copied = new char[dataSize];
    memset(copied, 0, dataSize);
    memcpy(copied, data, dataSize);
	nim_node::node_async_call::async_call([=]() {
		NertcNodeEventHandler::GetInstance()->Node_onRecvSEIMsg(uid, copied, dataSize);
	});
}

void NertcNodeEventHandler::onPullExternalAudioFrame(const BaseCallbackPtr& bcb, const std::shared_ptr<unsigned char>& data, uint32_t length)
{
    nim_node::node_async_call::async_call([=]() {
        NertcNodeEventHandler::GetInstance()->Node_onPullExternalAudioFrame(bcb, data, length);
    });
}

// void NertcNodeEventHandler::onCheckNECastAudioDriverResult(nertc::NERtcInstallCastAudioDriverResult result)
// {
//     nim_node::node_async_call::async_call([=]() {
//         NertcNodeEventHandler::GetInstance()->Node_onCheckNECastAudioDriverResult(result);
//     });
// }

void NertcNodeEventHandler::onScreenCaptureStatus(nertc::NERtcScreenCaptureStatus status)
{
    nim_node::node_async_call::async_call([=]() {
        NertcNodeEventHandler::GetInstance()->Node_onScreenCaptureStatus(status);
    });
}

void NertcNodeEventHandler::onAudioRecording(nertc::NERtcAudioRecordingCode code, const char* file_path)
{
    utf8_string str_file_path = file_path;
    nim_node::node_async_call::async_call([=]() {
        NertcNodeEventHandler::GetInstance()->Node_onAudioRecording(code, str_file_path);
    });
}

void NertcNodeEventHandler::Node_onUserSubStreamVideoStart(nertc::uid_t uid, nertc::NERtcVideoProfileType max_profile)
{
    Isolate *isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    const unsigned argc = 2;
    Local<Value> argv[argc] = { nim_napi_new_uint64(isolate, uid), nim_napi_new_uint32(isolate, max_profile) };
    auto it = callbacks_.find("onUserSubStreamVideoStart");
    if (it != callbacks_.end())
    {
        it->second->callback_.Get(isolate)->Call(isolate->GetCurrentContext(), it->second->data_.Get(isolate), argc, argv);
    }
}

void NertcNodeEventHandler::Node_onUserSubStreamVideoStop(nertc::uid_t uid)
{
    Isolate *isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    const unsigned argc = 1;
    Local<Value> argv[argc] = { nim_napi_new_uint64(isolate, uid) };
    auto it = callbacks_.find("onUserSubStreamVideoStop");
    if (it != callbacks_.end())
    {
        it->second->callback_.Get(isolate)->Call(isolate->GetCurrentContext(), it->second->data_.Get(isolate), argc, argv);
    }
}

void NertcNodeEventHandler::Node_onUserVideoProfileUpdate(nertc::uid_t uid, nertc::NERtcVideoProfileType max_profile)
{
    Isolate *isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    const unsigned argc = 2;
    Local<Value> argv[argc] = { nim_napi_new_uint64(isolate, uid), nim_napi_new_uint32(isolate, max_profile) };
    auto it = callbacks_.find("onUserVideoProfileUpdate");
    if (it != callbacks_.end())
    {
        it->second->callback_.Get(isolate)->Call(isolate->GetCurrentContext(), it->second->data_.Get(isolate), argc, argv);
    }
}

void NertcNodeEventHandler::Node_onUserAudioMute(nertc::uid_t uid, bool mute)
{
    Isolate *isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    const unsigned argc = 2;
    Local<Value> argv[argc] = { nim_napi_new_uint64(isolate, uid), nim_napi_new_bool(isolate, mute) };
    auto it = callbacks_.find("onUserAudioMute");
    if (it != callbacks_.end())
    {
        it->second->callback_.Get(isolate)->Call(isolate->GetCurrentContext(), it->second->data_.Get(isolate), argc, argv);
    }
}

void NertcNodeEventHandler::Node_onUserVideoMute(nertc::uid_t uid, bool mute)
{
    Isolate *isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    const unsigned argc = 2;
    Local<Value> argv[argc] = { nim_napi_new_uint64(isolate, uid), nim_napi_new_bool(isolate, mute) };
    auto it = callbacks_.find("onUserVideoMute");
    if (it != callbacks_.end())
    {
        it->second->callback_.Get(isolate)->Call(isolate->GetCurrentContext(), it->second->data_.Get(isolate), argc, argv);
    }
}

void NertcNodeEventHandler::Node_onAudioDeviceStateChanged(const utf8_string& device_id,
        nertc::NERtcAudioDeviceType device_type,
        nertc::NERtcAudioDeviceState device_state)
{
    Isolate *isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    const unsigned argc = 3;
    Local<Value> argv[argc] = {
        nim_napi_new_utf8string(isolate, device_id.c_str()),
        nim_napi_new_uint32(isolate, device_type),
        nim_napi_new_uint32(isolate, device_state)
    };
    auto it = callbacks_.find("onAudioDeviceStateChanged");
    if (it != callbacks_.end())
    {
        it->second->callback_.Get(isolate)->Call(isolate->GetCurrentContext(), it->second->data_.Get(isolate), argc, argv);
    }
}

void NertcNodeEventHandler::Node_onAudioDefaultDeviceChanged(const utf8_string& device_id,
        nertc::NERtcAudioDeviceType device_type)
{
    Isolate *isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    const unsigned argc = 2;
    Local<Value> argv[argc] = {
        nim_napi_new_utf8string(isolate, device_id.c_str()),
        nim_napi_new_uint32(isolate, device_type)
    };
    auto it = callbacks_.find("onAudioDefaultDeviceChanged");
    if (it != callbacks_.end())
    {
        it->second->callback_.Get(isolate)->Call(isolate->GetCurrentContext(), it->second->data_.Get(isolate), argc, argv);
    }
}

void NertcNodeEventHandler::Node_onVideoDeviceStateChanged(const utf8_string& device_id,
        nertc::NERtcVideoDeviceType device_type,
        nertc::NERtcVideoDeviceState device_state)
{
    Isolate *isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    const unsigned argc = 3;
    Local<Value> argv[argc] = {
        nim_napi_new_utf8string(isolate, device_id.c_str()),
        nim_napi_new_uint32(isolate, device_type), nim_napi_new_uint32(isolate, device_state)
    };
    auto it = callbacks_.find("onVideoDeviceStateChanged");
    if (it != callbacks_.end())
    {
        it->second->callback_.Get(isolate)->Call(isolate->GetCurrentContext(), it->second->data_.Get(isolate), argc, argv);
    }
}

void NertcNodeEventHandler::Node_onFirstAudioDataReceived(nertc::uid_t uid)
{
    Isolate *isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    const unsigned argc = 1;
    Local<Value> argv[argc] = { nim_napi_new_uint64(isolate, uid) };
    auto it = callbacks_.find("onFirstAudioDataReceived");
    if (it != callbacks_.end())
    {
        it->second->callback_.Get(isolate)->Call(isolate->GetCurrentContext(), it->second->data_.Get(isolate), argc, argv);
    }
}

void NertcNodeEventHandler::Node_onFirstVideoDataReceived(nertc::uid_t uid)
{
    Isolate *isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    const unsigned argc = 1;
    Local<Value> argv[argc] = { nim_napi_new_uint64(isolate, uid) };
    auto it = callbacks_.find("onFirstVideoDataReceived");
    if (it != callbacks_.end())
    {
        it->second->callback_.Get(isolate)->Call(isolate->GetCurrentContext(), it->second->data_.Get(isolate), argc, argv);
    }
}

void NertcNodeEventHandler::Node_onFirstAudioFrameDecoded(nertc::uid_t uid)
{
    Isolate *isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    const unsigned argc = 1;
    Local<Value> argv[argc] = { nim_napi_new_uint64(isolate, uid) };
    auto it = callbacks_.find("onFirstAudioFrameDecoded");
    if (it != callbacks_.end())
    {
        it->second->callback_.Get(isolate)->Call(isolate->GetCurrentContext(), it->second->data_.Get(isolate), argc, argv);
    }
}

void NertcNodeEventHandler::Node_onFirstVideoFrameDecoded(nertc::uid_t uid, uint32_t width, uint32_t height)
{
    Isolate *isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    const unsigned argc = 3;
    Local<Value> argv[argc] = { nim_napi_new_uint64(isolate, uid), nim_napi_new_uint32(isolate, width), nim_napi_new_uint32(isolate, height) };
    auto it = callbacks_.find("onFirstVideoFrameDecoded");
    if (it != callbacks_.end())
    {
        it->second->callback_.Get(isolate)->Call(isolate->GetCurrentContext(), it->second->data_.Get(isolate), argc, argv);
    }
}

void NertcNodeEventHandler::Node_onCaptureVideoFrame(void *data,
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

void NertcNodeEventHandler::Node_onAudioMixingStateChanged(nertc::NERtcAudioMixingState state, nertc::NERtcAudioMixingErrorCode error_code)
{
    Isolate *isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    const unsigned argc = 2;
    Local<Value> argv[argc] = { nim_napi_new_uint32(isolate, state), nim_napi_new_uint32(isolate, error_code) };
    auto it = callbacks_.find("onAudioMixingStateChanged");
    if (it != callbacks_.end())
    {
        it->second->callback_.Get(isolate)->Call(isolate->GetCurrentContext(), it->second->data_.Get(isolate), argc, argv);
    }
}

void NertcNodeEventHandler::Node_onAudioMixingTimestampUpdate(uint64_t timestamp_ms)
{
    Isolate *isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    const unsigned argc = 1;
    Local<Value> argv[argc] = { nim_napi_new_uint64(isolate, timestamp_ms) };
    auto it = callbacks_.find("onAudioMixingTimestampUpdate");
    if (it != callbacks_.end())
    {
        it->second->callback_.Get(isolate)->Call(isolate->GetCurrentContext(), it->second->data_.Get(isolate), argc, argv);
    }
}

void NertcNodeEventHandler::Node_onAudioEffectFinished(uint32_t effect_id)
{
    Isolate *isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    const unsigned argc = 1;
    Local<Value> argv[argc] = { nim_napi_new_uint32(isolate, effect_id) };
    auto it = callbacks_.find("onAudioEffectFinished");
    if (it != callbacks_.end())
    {
        it->second->callback_.Get(isolate)->Call(isolate->GetCurrentContext(), it->second->data_.Get(isolate), argc, argv);
    }
}

void NertcNodeEventHandler::Node_onLocalAudioVolumeIndication(int volume)
{
    Isolate *isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    const unsigned argc = 1;
    Local<Value> argv[argc] = { nim_napi_new_int32(isolate, volume) };
    auto it = callbacks_.find("onLocalAudioVolumeIndication");
    if (it != callbacks_.end())
    {
        it->second->callback_.Get(isolate)->Call(isolate->GetCurrentContext(), it->second->data_.Get(isolate), argc, argv);
    }
}

void NertcNodeEventHandler::Node_onRemoteAudioVolumeIndication(const nertc::NERtcAudioVolumeInfo *speakers, unsigned int speaker_number, int total_volume)
{
    Isolate *isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    const unsigned argc = 3;
    Local<Array> s = Array::New(isolate);
    for (auto i = 0; i < (int)speaker_number; i++)
    {
        Local<Object> o = Object::New(isolate);
        nertc_audio_volume_info_to_obj(isolate, speakers[i], o);
        s->Set(isolate->GetCurrentContext(), i, o);
    }
    Local<Value> argv[argc] = { s, nim_napi_new_uint32(isolate, speaker_number), nim_napi_new_int32(isolate, total_volume) };
    auto it = callbacks_.find("onRemoteAudioVolumeIndication");
    if (it != callbacks_.end())
    {
        it->second->callback_.Get(isolate)->Call(isolate->GetCurrentContext(), it->second->data_.Get(isolate), argc, argv);
    }
}

void NertcNodeEventHandler::Node_onAddLiveStreamTask(const utf8_string& task_id, const utf8_string& url, int error_code)
{
    Isolate *isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    const unsigned argc = 3;
    Local<Value> argv[argc] = {
        nim_napi_new_utf8string(isolate, task_id.c_str()),
        nim_napi_new_utf8string(isolate, url.c_str()), nim_napi_new_int32(isolate, error_code)
    };
    auto it = callbacks_.find("onAddLiveStreamTask");
    if (it != callbacks_.end())
    {
        it->second->callback_.Get(isolate)->Call(isolate->GetCurrentContext(), it->second->data_.Get(isolate), argc, argv);
    }
}

void NertcNodeEventHandler::Node_onUpdateLiveStreamTask(const utf8_string& task_id, const utf8_string& url, int error_code)
{
    Isolate *isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    const unsigned argc = 3;
    Local<Value> argv[argc] = {
        nim_napi_new_utf8string(isolate, task_id.c_str()),
        nim_napi_new_utf8string(isolate, url.c_str()),
        nim_napi_new_int32(isolate, error_code)
    };
    auto it = callbacks_.find("onUpdateLiveStreamTask");
    if (it != callbacks_.end())
    {
        it->second->callback_.Get(isolate)->Call(isolate->GetCurrentContext(), it->second->data_.Get(isolate), argc, argv);
    }
}

void NertcNodeEventHandler::Node_onRemoveLiveStreamTask(const utf8_string& task_id, int error_code)
{
    Isolate *isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    const unsigned argc = 2;
    Local<Value> argv[argc] = {
        nim_napi_new_utf8string(isolate, task_id.c_str()),
        nim_napi_new_int32(isolate, error_code)
    };
    auto it = callbacks_.find("onRemoveLiveStreamTask");
    if (it != callbacks_.end())
    {
        it->second->callback_.Get(isolate)->Call(isolate->GetCurrentContext(), it->second->data_.Get(isolate), argc, argv);
    }
}

void NertcNodeEventHandler::Node_onLiveStreamState(const utf8_string& task_id, const utf8_string& url, nertc::NERtcLiveStreamStateCode state)
{
    Isolate *isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    const unsigned argc = 3;
    Local<Value> argv[argc] = {
        nim_napi_new_utf8string(isolate, task_id.c_str()),
        nim_napi_new_utf8string(isolate, url.c_str()),
        nim_napi_new_uint32(isolate, state)
    };
    auto it = callbacks_.find("onLiveStreamState");
    if (it != callbacks_.end())
    {
        it->second->callback_.Get(isolate)->Call(isolate->GetCurrentContext(), it->second->data_.Get(isolate), argc, argv);
    }
}
 
void NertcNodeEventHandler::Node_onAudioHowling(bool howling)
{
    Isolate *isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    const unsigned argc = 1;
    Local<Value> argv[argc] = { nim_napi_new_bool(isolate, howling) };
    auto it = callbacks_.find("onAudioHowling");
    if (it != callbacks_.end())
    {
        it->second->callback_.Get(isolate)->Call(isolate->GetCurrentContext(), it->second->data_.Get(isolate), argc, argv);
    }
}


void NertcNodeEventHandler::Node_onRecvSEIMsg(nertc::uid_t uid, const char* data, uint32_t length)
{
	Isolate* isolate = Isolate::GetCurrent();
	HandleScope scope(isolate);
	const unsigned argc = 2;

#if 0
    std::unique_ptr<v8::BackingStore> backing = v8::ArrayBuffer::NewBackingStore(
        (void*)data, length, [](void* data, size_t length, void* deleter_data) {
            delete[] data;
        }, nullptr);

    Local<v8::ArrayBuffer> message_buffer = ArrayBuffer::New(
        isolate, 
        std::move(backing)
    );
#else
    Local<ArrayBuffer> buffer = ArrayBuffer::New(isolate, length);
    memcpy(buffer->GetContents().Data(), data, length);
    if (data)
    {
        delete[] data;
        data = nullptr;
    }
#endif
    
	Local<Value> argv[argc] = { nim_napi_new_uint64(isolate, uid), buffer };
	auto it = callbacks_.find("onReceSEIMsg");
	if (it != callbacks_.end())
	{
		it->second->callback_.Get(isolate)->Call(isolate->GetCurrentContext(), it->second->data_.Get(isolate), argc, argv);
	}
}

void NertcNodeEventHandler::Node_onPullExternalAudioFrame(const BaseCallbackPtr& bcb, const std::shared_ptr<unsigned char>& data, uint32_t length)
{
    Isolate* isolate = Isolate::GetCurrent();
    const unsigned argc = 1;
    Local<v8::ArrayBuffer> buffer = ArrayBuffer::New(isolate, data.get(), length);
    Local<Value> argv[argc] = { buffer };
    bcb->callback_.Get(isolate)->Call(isolate->GetCurrentContext(),
        bcb->data_.Get(isolate), argc, argv);
}

// void NertcNodeEventHandler::Node_onCheckNECastAudioDriverResult(nertc::NERtcInstallCastAudioDriverResult result)
// {
//     Isolate* isolate = Isolate::GetCurrent();
//     const unsigned argc = 1;
//     Local<Value> argv[argc] = { nim_napi_new_int32(isolate, (int32_t)result) };
//     auto it = callbacks_.find("onCheckNECastAudioDriverResult");
// 	if (it != callbacks_.end())
// 	{
// 		it->second->callback_.Get(isolate)->Call(isolate->GetCurrentContext(), it->second->data_.Get(isolate), argc, argv);
// 	}
// }

void NertcNodeEventHandler::Node_onScreenCaptureStatus(nertc::NERtcScreenCaptureStatus status)
{
    Isolate* isolate = Isolate::GetCurrent();
    const unsigned argc = 1;
    Local<Value> argv[argc] = { nim_napi_new_int32(isolate, (int32_t)status) };
    auto it = callbacks_.find("onScreenCaptureStatus");
	if (it != callbacks_.end())
	{
		it->second->callback_.Get(isolate)->Call(isolate->GetCurrentContext(), it->second->data_.Get(isolate), argc, argv);
	}
}

void NertcNodeEventHandler::Node_onAudioRecording(nertc::NERtcAudioRecordingCode code, const utf8_string& file_path)
{
    Isolate* isolate = Isolate::GetCurrent();
    const unsigned argc = 2;
    Local<Value> argv[argc] = { 
        nim_napi_new_int32(isolate, (int32_t)code),
        nim_napi_new_utf8string(isolate, file_path.c_str()),
    };
    auto it = callbacks_.find("Node_onAudioRecording");
	if (it != callbacks_.end())
	{
		it->second->callback_.Get(isolate)->Call(isolate->GetCurrentContext(), it->second->data_.Get(isolate), argc, argv);
	}
}

}
