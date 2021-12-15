#ifndef NODE_API_HELPER_H
#define NODE_API_HELPER_H
#include <napi.h>


#define CHECK_ARGS_COUNT(info, n)                                                                dd\
do                                                                                               \
{                                                                                                \
    Napi::Env env = info.Env();                                                                  \
    size_t argvsLength = info.Length ();                                                         \
    if(argvsLength != n)                                                                         \
    {                                                                                            \
        throw Napi::Error::New(env, "param count error").ThrowAsJavaScriptException();           \
    }                                                                                            \
} while (0);                                                                                     \




namespace nertc_node
{

//get
void napi_get_value_float(const Napi::Value& value, float& result);
void napi_get_value_double(const Napi::Value& value, double& result);
void napi_get_value_int32(const Napi::Value& value, int& result);
void napi_get_value_uint32(const Napi::Value& value, unsigned int& result);
void napi_get_value_int64(const Napi::Value& value, int64_t& result);
void napi_get_value_utf8_string(const Napi::Value& value, std::string& result);
void napi_get_value_bool(const Napi::Value& value, bool& result);
void napi_get_value_obj(const Napi::Value& value, Napi::Object& result);
void napi_get_value_node_buffer(const Napi::Value& value, void*& data, int& length);
void napi_get_value_function(const Napi::Value& value, Napi::FunctionReference& result);
void napi_get_obj_bool(const Napi::Object& obj, const char* key, bool& result);
void napi_get_obj_int32(const Napi::Object& obj, const char* key, int& result);
void napi_get_obj_utf8_string(const Napi::Object& obj, const char* key, std::string& result);
void napi_get_obj_uint32(const Napi::Object& obj, const char* key, unsigned int& result);
void napi_get_obj_obj(const Napi::Object& obj, const char* key, Napi::Object& result);
void napi_get_obj_node_buffer(const Napi::Object& obj, void*& data, int& length);


//set
void napi_set_object_value_int32(Napi::Env& env, const Napi::Object& obj, const char*  key, int32_t value);
void napi_set_object_value_uint32(Napi::Env& env, const Napi::Object& obj, const char*  key, uint32_t value);
void napi_set_object_value_int64(Napi::Env& env, const Napi::Object& obj, const char*  key, int64_t value);
void napi_set_object_value_float(Napi::Env& env, const Napi::Object& obj, const char*  key, float value);
void napi_set_object_value_double(Napi::Env& env, const Napi::Object& obj, const char*  key, double value);
void napi_set_object_value_bool(Napi::Env& env, const Napi::Object& obj, const char*  key, bool value);
void napi_set_object_value_string(Napi::Env& env,  const Napi::Object& obj,const char*  key, std::string value);
void napi_set_object_value_obj(Napi::Env& env,  const Napi::Object& obj, const char* key, Napi::Object& value);

}
#endif//NODE_API_HELPER_H