#include "nim_event_handler.h"

namespace nim_node
{
void EventHandler::AddEventHandler(const utf8_string &eventName, Persistent<Object> &obj, Persistent<Function> &callback)
{
    BaseCallbackPtr cb = BaseCallbackPtr(new BaseCallback());
    cb->callback_.Reset(Isolate::GetCurrent(), callback);
    cb->data_.Reset(Isolate::GetCurrent(), obj);
    callbacks_.emplace(eventName, cb);
}

int EventHandler::RemoveEventHandler(const utf8_string &eventName)
{
    auto it = callbacks_.find(eventName);
    if (it != callbacks_.end())
    {
        callbacks_.erase(it);
    }
    return callbacks_.size();
}
}