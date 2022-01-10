#ifndef NERTC_NODE_VIDEO_FRAME_PROVIDER_H
#define NERTC_NODE_VIDEO_FRAME_PROVIDER_H

#include <napi.h>
#include <array>
#include <vector>
#include <string>
#include <unordered_map>
#include <mutex>
#include <atomic>
#include <thread>
#include <memory>
#include "nertc_engine_defines.h"

namespace nertc_node
{
    struct buffer_info
    {
        unsigned char *buffer;
        uint32_t length;
    };

    using buffer_list = std::array<buffer_info, 4>;
    using stream_buffer_type = std::vector<unsigned char>;

    struct IVideoFrame
    {
        nertc::uid_t uid;
        uint8_t *data;
        // uint32_t type;
        uint32_t width;
        uint32_t height;
        uint32_t count;
        uint32_t offset[4];
        uint32_t stride[4];
        uint32_t rotation;

        enum PLANE_TYPE
        {
            Y_PLANE = 0,
            U_PLANE = 1,
            V_PLANE = 2,
            NUM_OF_PLANES = 3
        };

        const unsigned char * buffer(PLANE_TYPE type) const
        {
            switch (type)
            {
            case Y_PLANE:
                return (const unsigned char * )data + offset[0];
            case U_PLANE:
                return (const unsigned char * )((uint8_t *)data + offset[1]);
            case V_PLANE:
                return (const unsigned char * )((uint8_t *)data + offset[2]);
            default:
                break;
            }
        };
    };

    enum NodeRenderType
    {
        NODE_RENDER_TYPE_LOCAL,
        NODE_RENDER_TYPE_REMOTE,
        NODE_RENDER_TYPE_LOCAL_SUBSTREAM,
        NODE_RENDER_TYPE_REMOTE_SUBSTREAM
    };

#define MAX_MISS_COUNT 500

    class VideoFrameInfo
    {
    public:
        NodeRenderType m_renderType;
        nertc::uid_t m_uid;
        buffer_list m_bufferList;
        stream_buffer_type m_buffer;
        uint32_t m_destWidth;
        uint32_t m_destHeight;
        bool m_needUpdate;
        uint32_t m_count;
        std::string m_channelId;
        VideoFrameInfo()
            : m_renderType(NODE_RENDER_TYPE_REMOTE), m_uid(0), m_destWidth(0), m_destHeight(0), m_needUpdate(false), m_count(0), m_channelId("")
        {
        }
        VideoFrameInfo(NodeRenderType type)
            : m_renderType(type), m_uid(0), m_destWidth(0), m_destHeight(0), m_needUpdate(false), m_count(0), m_channelId("")
        {
        }
        VideoFrameInfo(NodeRenderType type, nertc::uid_t uid, std::string channelId)
            : m_renderType(type), m_uid(uid), m_destWidth(0), m_destHeight(0), m_needUpdate(false), m_count(0), m_channelId(channelId)
        {
        }
    };

    class NodeVideoFrameTransporter
    {

    public:
        typedef struct NodeFrameCallback {
            Napi::FunctionReference function;
        } FrameDataCallback;

    public:
        NodeVideoFrameTransporter();
        ~NodeVideoFrameTransporter();

        void setLocalVideoMirrorMode(uint32_t mirrorMode) { m_localVideoMirrorMode = mirrorMode; }
        bool initialize(Napi::FunctionReference&& function);
        int deliverFrame_I420(NodeRenderType type, nertc::uid_t uid, std::string channelId, const IVideoFrame &videoFrame, int rotation, bool mirrored);
        int setVideoDimension(NodeRenderType, nertc::uid_t uid, std::string channelId, uint32_t width, uint32_t height);
        static void onFrameDataCallback(
            nertc::uid_t uid,
            void *data,
            uint32_t type,
            uint32_t width,
            uint32_t height,
            uint32_t count,
            uint32_t offset[4],
            uint32_t stride[4],
            uint32_t rotation,
            void *user_data);

        static void onSubstreamFrameDataCallback(
            nertc::uid_t uid,
            void *data,
            uint32_t type,
            uint32_t width,
            uint32_t height,
            uint32_t count,
            uint32_t offset[4],
            uint32_t stride[4],
            uint32_t rotation,
            void *user_data);

    private:
        struct image_frame_info
        {
            int stride;
            int stride0;
            int width;
            int height;
            int strideU;
            int strideV;
        };

        struct image_header_type
        {
            uint8_t format;
            uint8_t mirrored;
            uint16_t width;
            uint16_t height;
            uint16_t left;
            uint16_t top;
            uint16_t right;
            uint16_t bottom;
            uint16_t rotation;
            uint32_t timestamp;
        };
        VideoFrameInfo &getVideoFrameInfo(NodeRenderType type, nertc::uid_t uid, std::string channelId);
        bool deinitialize();
        void setupFrameHeader(image_header_type *header, int stride, int width, int height);
        void copyFrame(const IVideoFrame &videoFrame, VideoFrameInfo &info, int dest_stride, int src_stride, int width, int height);
        void copyAndCentreYuv(const unsigned char *srcYPlane, const unsigned char *srcUPlane, const unsigned char *srcVPlane, int width, int height, int srcStride,
                              unsigned char *dstYPlane, unsigned char *dstUPlane, unsigned char *dstVPlane, int dstStride);
        void FlushVideo();

    private:
        bool init;
        // Napi::Env env;
        // Isolate *env;
        // Persistent<Function> callback;
        // Napi::FunctionReference js_callback;
        FrameDataCallback* m_pFrameDataCallback;
        // Persistent<Object> js_this;
        std::unordered_map<nertc::uid_t, VideoFrameInfo> m_remoteVideoFrames;
        std::unique_ptr<VideoFrameInfo> m_localVideoFrame;
        std::unordered_map<nertc::uid_t, VideoFrameInfo> m_substreamVideoFrame;
        std::unique_ptr<VideoFrameInfo> m_localSubStreamVideoFrame;
        std::mutex m_lock;
        int m_stopFlag;
        std::unique_ptr<std::thread> m_thread;
        uint32_t m_FPS;
        uint32_t m_localVideoMirrorMode = 0; //0-auto 1-mirror 2-unmirror
        std::atomic<int> count_task = 0;

    };

    NodeVideoFrameTransporter *getNodeVideoFrameTransporter();

} // namespace nertc_node

#endif //NERTC_NODE_VIDEO_FRAME_PROVIDER_H