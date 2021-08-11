#ifndef NIM_NODE_SDK_EVENTHANDLER_H
#define NIM_NODE_SDK_EVENTHANDLER_H

#include <node.h>
#include <unordered_map>
#include "../shared/sdk_helper/nim_node_helper.h"

using v8::Persistent;
using v8::Object;
using v8::Function;
namespace nim_node
{
class EventHandler
{
private:
    /* data */
public:
    EventHandler(/* args */){};
    ~EventHandler(){};

    void AddEventHandler(const utf8_string &eventName, Persistent<Object> &obj, Persistent<Function> &callback);
    int RemoveEventHandler(const utf8_string &eventName);
    int RemoveAll();

protected:
    std::unordered_map<utf8_string, BaseCallbackPtr> callbacks_;
};
}
#endif //NIM_NODE_SDK_EVENTHANDLER_H