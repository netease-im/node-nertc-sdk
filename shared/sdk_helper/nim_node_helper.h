#ifndef NIM_NODE_SDK_HELPER_H
#define NIM_NODE_SDK_HELPER_H

#include <node.h>
#include <node_object_wrap.h>
#include <node_api.h>
#include <list>
#include <string>
#include <map>

using v8::Context;
using v8::Function;
using v8::FunctionCallback;
using v8::FunctionCallbackInfo;
using v8::FunctionTemplate;
using v8::HandleScope;
using v8::Integer;
using v8::Isolate;
using v8::Local;
using v8::Name;
using v8::NewStringType;
using v8::Object;
using v8::Persistent;
using v8::String;
using v8::Boolean;
using v8::Value;
using v8::Exception;
using v8::Array;
using v8::Number;
using v8::Map;
using v8::ArrayBuffer;

using std::string;

struct BaseCallback {
    Persistent<Function> callback_;
    Persistent<Object> data_;
};

typedef std::shared_ptr<BaseCallback> BaseCallbackPtr;

//饿汉模式
#define SINGLETON_DEFINE(TypeName)				\
static TypeName* GetInstance()					\
{												\
	static TypeName type_instance;				\
	return &type_instance;						\
}												\
												\
TypeName(const TypeName&) = delete;				\
TypeName& operator=(const TypeName&) = delete

#define BEGIN_OBJECT_INIT(className, constructor, fieldCount)              \
    Isolate* isolate = module->GetIsolate();   \
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, constructor); \
    tpl->SetClassName(String::NewFromUtf8(isolate, #className, v8::NewStringType::kNormal).ToLocalChecked());               \
    tpl->InstanceTemplate()->SetInternalFieldCount(fieldCount);

#define SET_PROTOTYPE(name) NODE_SET_PROTOTYPE_METHOD(tpl, #name, name);

#define END_OBJECT_INIT(className) \
    constructor.Reset(isolate, tpl->GetFunction(isolate->GetCurrentContext()).ToLocalChecked()); \
    module->Set(isolate->GetCurrentContext(), String::NewFromUtf8(isolate, #className, v8::NewStringType::kNormal).ToLocalChecked(), tpl->GetFunction(isolate->GetCurrentContext()).ToLocalChecked());

#define BEGIN_OBJECT_INIT_EX(className, constructor, fieldCount)              \
    Isolate* isolate = context->GetIsolate();   \
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, constructor); \
    tpl->SetClassName(String::NewFromUtf8(isolate, #className, v8::NewStringType::kNormal).ToLocalChecked());               \
    tpl->InstanceTemplate()->SetInternalFieldCount(fieldCount);

#define END_OBJECT_INIT_EX(className) \
    constructor.Reset(isolate, tpl->GetFunction(context).ToLocalChecked()); \
    exports->Set(context, String::NewFromUtf8(isolate, #className, v8::NewStringType::kNormal).ToLocalChecked(), tpl->GetFunction(context).ToLocalChecked());


#define NIM_SDK_NODE_API(m) \
    static void(m)(const FunctionCallbackInfo<Value> &args)

#define NIM_SDK_NODE_API_DEF(cls, method) \
    void cls::method(const FunctionCallbackInfo<Value> &args)

#define DECLARE_CLASS \
    static v8::Persistent<v8::Function> constructor
#define DEFINE_CLASS(name) \
    v8::Persistent<Function> name::constructor

#define NIM_SDK_NODE_THROW_EXCEPTION(isolate, msg)                                           \
    isolate->ThrowException(Exception::TypeError(                                                \
        String::NewFromUtf8(isolate, msg.c_str(), NewStringType::kNormal).ToLocalChecked()));

#define CHECK_ARGS_COUNT(expectedCount)                                                                  \
    Isolate *isolate = args.GetIsolate();                                                                \
    do                                                                                                   \
    {                                                                                                    \
        if (args.Length() != expectedCount)                                                              \
        {                                                                                                \
            isolate->ThrowException(Exception::TypeError(                                                    \
                String::NewFromUtf8(isolate, "参数数量错误", NewStringType::kNormal).ToLocalChecked())); \
            return;                                                                                      \
        }                                                                                                \
    } while (0);

#define CHECK_API_FUNC(cls, argsCount)                                                                   \
    cls *instance = node::ObjectWrap::Unwrap<cls>(args.Holder());                                        \
    Isolate *isolate = args.GetIsolate();                                                                \
    do                                                                                                   \
    {                                                                                                    \
        if (!instance)                                                                                   \
        {                                                                                                \
            return;                                                                                      \
        }                                                                                                \
        if (args.Length() != argsCount)                                                                  \
        {                                                                                                \
            isolate->ThrowException(Exception::Error(                                                \
                 String::NewFromUtf8(isolate, "参数数量错误", NewStringType::kNormal).ToLocalChecked())); \
            return;                                                                                      \
        }                                                                                                \
    } while (0);

#define ASSEMBLE_BASE_CALLBACK(index)                          \
    Local<Function> cb = args[index].As<Function>();           \
    do                                                         \
    {                                                          \
        if (cb.IsEmpty())                                      \
        {                                                      \
            return;                                            \
        }                                                      \
    } while (0);                                               \
    Persistent<Function> pcb;                                  \
    pcb.Reset(isolate, cb);                                    \
    Local<Object> obj = args.This();                           \
    Persistent<Object> pdata;                                  \
    pdata.Reset(isolate, obj);                                 \
    BaseCallbackPtr bcb = BaseCallbackPtr(new BaseCallback()); \
    bcb->callback_.Reset(isolate, pcb);                        \
    bcb->data_.Reset(isolate, pdata);

#define ASSEMBLE_BASE_CALLBACK_EX(index)                          \
    Local<Function> cb_##index = args[index].As<Function>();           \
    do                                                         \
    {                                                          \
        if (cb_##index.IsEmpty())                                      \
        {                                                      \
            return;                                            \
        }                                                      \
    } while (0);                                               \
    Persistent<Function> pcb_##index;                                  \
    pcb_##index.Reset(isolate, cb_##index);                                    \
    Local<Object> obj_##index = args.This();                           \
    Persistent<Object> pdata_##index;                                  \
    pdata_##index.Reset(isolate, obj_##index);                                 \
    BaseCallbackPtr bcb_##index = BaseCallbackPtr(new BaseCallback()); \
    bcb_##index->callback_.Reset(isolate, pcb_##index);                        \
    bcb_##index->data_.Reset(isolate, pdata_##index);

typedef char utf8_char;
typedef std::string utf8_string;
class UTF8String
{
public:
    UTF8String() : _mem(nullptr), _len(0) {}
    // UTF8String(utf8_char* buf, int len) : _mem(buf), _len(len) {}
    ~UTF8String()
    {
        clear();
    }

    void clear()
    {
        if (_mem) {
            free(_mem);
            _mem = nullptr;
        }
        _len = 0;
    }

    void set(utf8_char* buf, int len)
    {
        if (_mem) {
            free(_mem);
            _mem = nullptr;
        }
        _mem = (utf8_char *)calloc(len + 1, sizeof(utf8_char));
        strcpy(_mem, buf);
        // strcpy_s(_mem, len + 1, buf);
        _len = len;
    }

    int length()
    {
        return _len;
    }

    utf8_char* get()
    {
        return _mem;
    }

    utf8_string toUtf8String() const
    {
        if (_mem)
            return (utf8_string)_mem;
        return "";
    }

private:
    utf8_char* _mem;
    int _len;
};

napi_status nim_napi_get_value_utf8string_list(Isolate* isolate, const Local<Value>& value, std::list<utf8_string>& out);
napi_status nim_napi_get_value_utf8string(Isolate* isolate, const Local<Value>& value, UTF8String& out);
napi_status nim_napi_get_value_UTF8String(Isolate* isolate, const Local<Value>& value, UTF8String& out);
napi_status nim_napi_get_value_uint32(Isolate* isolate, const Local<Value>& value, uint32_t& out);
napi_status nim_napi_get_value_int32(Isolate* isolate, const Local<Value>& value, int32_t& out);
napi_status nim_napi_get_value_bool(Isolate* isolate, const Local<Value>& value, bool& out);
napi_status nim_napi_get_value_int64(Isolate* isolate, const Local<Value>& value, int64_t& out);
napi_status nim_napi_get_value_uint64(Isolate* isolate, const Local<Value>& value, uint64_t& out);

napi_status nim_napi_get_object_value(Isolate* isolate, const Local<Object>& obj, const utf8_string& keyName, Local<Value>& out);
napi_status nim_napi_get_object_value_utf8string(Isolate* isolate, const Local<Object>& obj, const utf8_string& keyName, UTF8String& out);
napi_status nim_napi_get_object_value_uint32(Isolate* isolate, const Local<Object>& obj, const utf8_string& keyName, uint32_t& out);
napi_status nim_napi_get_object_value_int32(Isolate* isolate, const Local<Object>& obj, const utf8_string& keyName, int32_t& out);
napi_status nim_napi_get_object_value_bool(Isolate* isolate, const Local<Object>& obj, const utf8_string& keyName, bool& out);
napi_status nim_napi_get_object_value_int64(Isolate* isolate, const Local<Object>& obj, const utf8_string& keyName, int64_t& out);
napi_status nim_napi_get_object_value_uint64(Isolate* isolate, const Local<Object>& obj, const utf8_string& keyName, uint64_t& out);

Local<Value> nim_napi_new_utf8string(Isolate* isolate, const utf8_char *keyValue);
Local<Value> nim_napi_new_uint32(Isolate* isolate, const uint32_t& keyValue);
Local<Value> nim_napi_new_int32(Isolate* isolate, const int32_t& keyValue);
Local<Value> nim_napi_new_bool(Isolate* isolate, const bool& keyValue);
Local<Value> nim_napi_new_int64(Isolate* isolate, const int64_t& keyValue);
Local<Value> nim_napi_new_uint64(Isolate* isolate, const uint64_t& keyValue);

napi_status nim_napi_set_object_value_utf8string(Isolate* isolate, Local<Object>& obj, const utf8_string& keyName, const utf8_string& keyValue);
napi_status nim_napi_set_object_value_uint32(Isolate* isolate, Local<Object>& obj, const utf8_string& keyName, const uint32_t& keyValue);
napi_status nim_napi_set_object_value_int32(Isolate* isolate, Local<Object>& obj, const utf8_string& keyName, const int32_t& keyValue);
napi_status nim_napi_set_object_value_bool(Isolate* isolate, Local<Object>& obj, const utf8_string& keyName, const bool& keyValue);
napi_status nim_napi_set_object_value_int64(Isolate* isolate, Local<Object>& obj, const utf8_string& keyName, const int64_t& keyValue);
napi_status nim_napi_set_object_value_uint64(Isolate* isolate, Local<Object>& obj, const utf8_string& keyName, const uint64_t& keyValue);

napi_status nim_napi_assemble_string_array(Isolate* isolate, const std::list<utf8_string>& strs, Local<Array>& arr);
napi_status nim_napi_assemble_string_map(Isolate* isolate, const std::map<utf8_string, utf8_string>& ipt, Local<Object>& opt);

napi_status nim_napi_UTF8Strings_to_utf8strings(const std::list<UTF8String>& strs, std::list<utf8_string>& out);

#define GET_ARGS_VALUE(isolate, index, type, v)                  \
    status = nim_napi_get_value_##type(isolate, args[index], v); \
    do                                                  \
    {                                                   \
        if (status != napi_ok)                          \
        {                                               \
            return;                                     \
        }                                               \
    } while (0);

#define ASSEMBLE_REG_CALLBACK(index, cls, funcName)                 \
    Local<Function> cb = args[index].As<Function>();            \
    do                                                          \
    {                                                           \
        if (cb.IsEmpty())                                       \
        {                                                       \
            return;                                             \
        }                                                       \
        Persistent<Function> pcb;                               \
        pcb.Reset(isolate, cb);                                 \
        Local<Object> obj = args.This();                        \
        Persistent<Object> pdata;                               \
        pdata.Reset(isolate, obj);                              \
        cls::GetInstance()->AddEventHandler(funcName, pdata, pcb); \
    } while (0);

#endif //NIM_NODE_SDK_HELPER_H