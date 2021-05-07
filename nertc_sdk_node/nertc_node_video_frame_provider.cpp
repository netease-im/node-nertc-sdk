#include "nertc_node_video_frame_provider.h"
#include "../shared/sdk_helper/nim_node_helper.h"
#include "../shared/sdk_helper/nim_node_async_queue.h"
#include <chrono>
#include <string>
#include "libyuv.h"

using namespace libyuv;
namespace nertc_node
{
NodeVideoFrameTransporter g_transport;
NodeVideoFrameTransporter* getNodeVideoFrameTransporter()
{
    return &g_transport;
}

NodeVideoFrameTransporter::NodeVideoFrameTransporter()
: init(false)
, env(nullptr)
, m_FPS(10)
{
    
}

NodeVideoFrameTransporter::~NodeVideoFrameTransporter()
{
    deinitialize();
}

bool NodeVideoFrameTransporter::initialize(v8::Isolate *isolate, const v8::FunctionCallbackInfo<v8::Value> &callbackinfo)
{
    if (init) {
        deinitialize();
    }
    m_stopFlag = false;
    env = isolate;
    callback.Reset(isolate, callbackinfo[0].As<Function>());
    js_this.Reset(isolate, callbackinfo.This());
    m_thread.reset(new std::thread(&NodeVideoFrameTransporter::FlushVideo, this));
    init = true;
    return true;
}

bool NodeVideoFrameTransporter::deinitialize()
{
    if (!init)
        return true;
    m_stopFlag = 1;
    if (m_thread->joinable())
        m_thread->join();
    init = false;
    m_thread.reset();
    env = nullptr;
    callback.Reset();
    js_this.Reset();
    return true;
}

int NodeVideoFrameTransporter::setVideoDimension(NodeRenderType type, nertc::uid_t uid, std::string channelId, uint32_t width, uint32_t height)
{
    if (!init)
        return -1;
    std::lock_guard<std::mutex> lck(m_lock);
    VideoFrameInfo& info = getVideoFrameInfo(type, uid, channelId);
    info.m_destWidth = width;
    info.m_destHeight = height;
    return 0;
}

VideoFrameInfo& NodeVideoFrameTransporter::getVideoFrameInfo(NodeRenderType type, nertc::uid_t uid, std::string channelId)
{
    if (type == NodeRenderType::NODE_RENDER_TYPE_LOCAL) {
        if (!m_localVideoFrame.get())
            m_localVideoFrame.reset(new VideoFrameInfo(NODE_RENDER_TYPE_LOCAL));
        return *m_localVideoFrame.get();
    }
    else if (type == NODE_RENDER_TYPE_REMOTE) {
        auto it = m_remoteVideoFrames.find(uid);
        if (it == m_remoteVideoFrames.end()) {
            m_remoteVideoFrames[uid] = VideoFrameInfo(NODE_RENDER_TYPE_REMOTE, uid, channelId);
        }
        return m_remoteVideoFrames[uid];
    }
    else if (type == NODE_RENDER_TYPE_LOCAL_SUBSTREAM) {
        if (!m_localSubStreamVideoFrame.get()) {
            m_localSubStreamVideoFrame.reset(new VideoFrameInfo(NODE_RENDER_TYPE_LOCAL_SUBSTREAM));
        }
        return *m_localSubStreamVideoFrame.get();
    }
    else if (type == NODE_RENDER_TYPE_REMOTE_SUBSTREAM) {
        auto it = m_substreamVideoFrame.find(uid);
        if (it == m_substreamVideoFrame.end()) {
            m_substreamVideoFrame[uid] = VideoFrameInfo(NODE_RENDER_TYPE_REMOTE_SUBSTREAM, uid, channelId);
        }
        return m_substreamVideoFrame[uid];
    }
}

int NodeVideoFrameTransporter::deliverFrame_I420(NodeRenderType type, nertc::uid_t uid, std::string channelId, const IVideoFrame& videoFrame, int rotation, bool mirrored)
{
    if (!init)
        return -1;
        
    std::lock_guard<std::mutex> lck(m_lock);
    VideoFrameInfo& info = getVideoFrameInfo(type, uid, channelId);
    int destStride = info.m_destWidth ? info.m_destWidth : videoFrame.stride[0];
    int destWidth = info.m_destWidth ? info.m_destWidth : videoFrame.width;
    int destHeight = info.m_destHeight ? info.m_destHeight : videoFrame.height;
    size_t imageSize = sizeof(image_header_type) + destStride * destHeight * 3 / 2;
    auto s = info.m_buffer.size();
    if (s < imageSize || s >= imageSize * 2)
        info.m_buffer.resize(imageSize);
    image_header_type* hdr = reinterpret_cast<image_header_type*>(&info.m_buffer[0]);
    if (type == NODE_RENDER_TYPE_LOCAL || type == NODE_RENDER_TYPE_REMOTE)
    {
        if (uid == 0)
        {
            if (m_localVideoMirrorMode == 0)
            {
                hdr->mirrored = mirrored ? 1 : 0;
            }
            else
            {
                hdr->mirrored = m_localVideoMirrorMode == 1 ? 1 : 0;
            }
        }
        else
        {
            hdr->mirrored = 0;
        }
    } else {
        hdr->mirrored = mirrored ? 1 : 0;
    }
    hdr->rotation = htons(rotation);
    setupFrameHeader(hdr, destStride, destWidth, destHeight);
    copyFrame(videoFrame, info, destStride, videoFrame.stride[0], destWidth, destHeight);
    info.m_count = 0;
    info.m_needUpdate = true;
    // if (videoFrame.data)
    // {
    //     delete videoFrame.data;
    //     // videoFrame.data = nullptr;
    // }
    return 0;
}

void NodeVideoFrameTransporter::setupFrameHeader(image_header_type*header, int stride, int width, int height)
{
    int left = (stride - width) / 2;
    int top = 0;
    header->format = 0;
    header->width = htons(width);
    header->height = htons(height);
    header->left = htons((uint16_t)left);
    header->top = htons((uint16_t)top);
    header->right = htons((uint16_t)(stride - width - left));
    header->bottom = htons((uint16_t)0);
    header->timestamp = 0;
}

void NodeVideoFrameTransporter::copyFrame(const IVideoFrame& videoFrame, VideoFrameInfo& info, int dest_stride, int src_stride, int width, int height)
{
    int width2 = dest_stride / 2, heigh2 = height / 2;
    int strideY = videoFrame.stride[IVideoFrame::Y_PLANE];
    int strideU = videoFrame.stride[IVideoFrame::U_PLANE];
    int strideV = videoFrame.stride[IVideoFrame::V_PLANE];

    unsigned char* y = &info.m_buffer[0] + sizeof(image_header_type);
    unsigned char* u = y + dest_stride * height;
    unsigned char* v = u + width2 * heigh2;
    const unsigned char* planeY = videoFrame.buffer(IVideoFrame::Y_PLANE);
    const unsigned char* planeU = videoFrame.buffer(IVideoFrame::U_PLANE);
    const unsigned char* planeV = videoFrame.buffer(IVideoFrame::V_PLANE);

    if (videoFrame.width == width && videoFrame.height == height)
    {
        copyAndCentreYuv(planeY, planeU, planeV, videoFrame.width, videoFrame.height, src_stride, y, u, v, dest_stride);
    }
    else
    {
        I420Scale(planeY, strideY, planeU, strideU, planeV, strideV, videoFrame.width, videoFrame.height, (uint8_t*)y, dest_stride, (uint8_t*)u, width2, (uint8_t*)v, width2, width, height, kFilterNone);
    }

    info.m_bufferList[0].buffer = &info.m_buffer[0];
    info.m_bufferList[0].length = sizeof(image_header_type);

    info.m_bufferList[1].buffer = y;
    info.m_bufferList[1].length = dest_stride * height;

    info.m_bufferList[2].buffer = u;
    info.m_bufferList[2].length = width2 * heigh2;

    info.m_bufferList[3].buffer = v;
    info.m_bufferList[3].length = width2 * heigh2;
}

void NodeVideoFrameTransporter::copyAndCentreYuv(const unsigned char* srcYPlane, const unsigned char* srcUPlane, const unsigned char* srcVPlane, int width, int height, int srcStride,
unsigned char* dstYPlane, unsigned char* dstUPlane, unsigned char* dstVPlane, int dstStride)
{
    if (srcStride == width && dstStride == width)
    {
        memcpy(dstYPlane, srcYPlane, width * height);
        memcpy(dstUPlane, srcUPlane, width * height / 4);
        memcpy(dstVPlane, srcVPlane, width * height / 4);
        return;
    }

    int dstDiff = dstStride - width;
    //RGB(0,0,0) to YUV(0,128,128)
    memset(dstYPlane, 0, dstStride * height);
    memset(dstUPlane, 128, dstStride * height / 4);
    memset(dstVPlane, 128, dstStride * height / 4);

    for (int i = 0; i < height; ++i)
    {
        memcpy(dstYPlane + (dstDiff >> 1), srcYPlane, width);
        srcYPlane += srcStride;
        dstYPlane += dstStride;

        if (i % 2 == 0)
        {
            memcpy(dstUPlane + (dstDiff >> 2), srcUPlane, width >> 1);
            srcUPlane += srcStride >> 1;
            dstUPlane += dstStride >> 1;

            memcpy(dstVPlane + (dstDiff >> 2), srcVPlane, width >> 1);
            srcVPlane += srcStride >> 1;
            dstVPlane += dstStride >> 1;
        }
    }
}

#define NODE_SET_OBJ_PROP_HEADER(obj, it) \
    { \
        Local<Value> propName = String::NewFromUtf8(isolate, "header", NewStringType::kInternalized).ToLocalChecked(); \
        Local<v8::ArrayBuffer> buff = v8::ArrayBuffer::New(isolate, (it)->buffer, (it)->length); \
        v8::Maybe<bool> ret = obj->Set(isolate->GetCurrentContext(), propName, buff); \
        if(!ret.IsNothing()) { \
            if(!ret.ToChecked()) { \
                break; \
            } \
        } \
    }

#define NODE_SET_OBJ_PROP_DATA(obj, name, it) \
    { \
        Local<Value> propName = String::NewFromUtf8(isolate, name, NewStringType::kInternalized).ToLocalChecked(); \
        Local<v8::ArrayBuffer> buff = v8::ArrayBuffer::New(isolate, (it)->buffer, (it)->length); \
        Local<v8::Uint8Array> dataarray = v8::Uint8Array::New(buff, 0, it->length);\
        v8::Maybe<bool> ret = obj->Set(isolate->GetCurrentContext(), propName, dataarray); \
        if(!ret.IsNothing()) { \
            if(!ret.ToChecked()) { \
                break; \
            } \
        } \
    }

bool AddObj(Isolate* isolate, Local<v8::Array>& infos, int index, VideoFrameInfo& info)
{
    if (!info.m_needUpdate)
        return false;
    info.m_needUpdate = false;
    bool result = false;
    do {
        Local<v8::Object> obj = Object::New(isolate);
        nim_napi_set_object_value_uint32(isolate, obj, "type", info.m_renderType);
        nim_napi_set_object_value_uint64(isolate, obj, "uid", info.m_uid);
        nim_napi_set_object_value_utf8string(isolate, obj, "channelId", info.m_channelId);
        auto it = info.m_bufferList.begin();
        NODE_SET_OBJ_PROP_HEADER(obj, it);
        ++it;
        NODE_SET_OBJ_PROP_DATA(obj, "ydata", it);
        ++it;
        NODE_SET_OBJ_PROP_DATA(obj, "udata", it);
        ++it;
        NODE_SET_OBJ_PROP_DATA(obj, "vdata", it);
        result = infos->Set(isolate->GetCurrentContext(), index, obj).FromJust();
    } while (false);
    return result;
}

void NodeVideoFrameTransporter::FlushVideo()
{
    while (!m_stopFlag) {
        {
            nim_node::node_async_call::async_call([this]() {
                Isolate *isolate = env;
                HandleScope scope(isolate);
                std::lock_guard<std::mutex> lock(m_lock);
                Local<v8::Array> infos = v8::Array::New(isolate);

                uint32_t i = 0;
                for (auto& it : m_remoteVideoFrames) {
                    if (AddObj(isolate, infos, i, it.second))
                        ++i;
                    else {
                        ++it.second.m_count;
                    }
                }

                if (m_localVideoFrame.get()) {
                    if (AddObj(isolate, infos, i, *m_localVideoFrame.get()))
                        ++i;
                    else {
                        ++m_localVideoFrame->m_count;
                    }
                }

                for (auto& it : m_substreamVideoFrame) {
                    if (AddObj(isolate, infos, i, it.second))
                        ++i;
                    else {
                        ++it.second.m_count;
                    }
                } 

                if (m_localSubStreamVideoFrame.get()) {
                    if (AddObj(isolate, infos, i, *m_localSubStreamVideoFrame.get()))
                        ++i;
                    else {
                        ++m_localSubStreamVideoFrame->m_count;
                    }
                }

                if (i > 0) {
                    Local<v8::Value> args[1] = { infos };
                    callback.Get(isolate)->Call(isolate->GetCurrentContext(), js_this.Get(isolate), 1, args);
                }
            });
            std::this_thread::sleep_for(std::chrono::milliseconds(1000 / m_FPS));
        }
    }
}

void NodeVideoFrameTransporter::onFrameDataCallback(
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

    frame.uid = *((nertc::uid_t *)user_data);
    frame.width = width;
    frame.height = height;

    NodeRenderType nrt = frame.uid == 0 ? NODE_RENDER_TYPE_LOCAL : NODE_RENDER_TYPE_REMOTE;
    auto *pTransporter = getNodeVideoFrameTransporter();
    if (pTransporter)
    {
        pTransporter->deliverFrame_I420(nrt, frame.uid, "", frame, rotate, frame.uid == 0);
    }
}

void NodeVideoFrameTransporter::onSubstreamFrameDataCallback(
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

    frame.uid = *((nertc::uid_t *)user_data);
    frame.width = width;
    frame.height = height;

    NodeRenderType nrt = frame.uid == 0 ? NODE_RENDER_TYPE_LOCAL_SUBSTREAM : NODE_RENDER_TYPE_REMOTE_SUBSTREAM;
    auto *pTransporter = getNodeVideoFrameTransporter();
    if (pTransporter)
    {
        pTransporter->deliverFrame_I420(nrt, frame.uid, "", frame, rotate, false);
    }
}
}