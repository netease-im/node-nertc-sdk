#include "node_api_helper.h"

namespace nertc_node
{

void napi_get_value_float(const Napi::Value& value, float& result) {
    result = value.As<Napi::Number>().FloatValue();
}

void napi_get_value_double(const Napi::Value& value, double& result) {
    result = value.As<Napi::Number>().DoubleValue();
}

void napi_get_value_int32(const Napi::Value& value, int& result) {
    result = value.As<Napi::Number>().Int32Value();
}

void napi_get_value_uint32(const Napi::Value& value, unsigned int& result) {
    result = value.As<Napi::Number>().Uint32Value();
}

void napi_get_value_int64(const Napi::Value& value, int64_t& result) {
    result = value.As<Napi::Number>().Int64Value();
}

void napi_get_value_bool(const Napi::Value& value, bool& result) {
    result = value.As<Napi::Boolean>().Value();
}

void napi_get_value_utf8_string(const Napi::Value& value, std::string& result) {
    result = value.As<Napi::String>().Utf8Value();
}

void napi_get_value_obj(const Napi::Value& value, Napi::Object& result) {
    result = value.As<Napi::Object>();
}

void napi_get_value_node_buffer(const Napi::Value& value, void*& data, int& length) {
    data = value.As<Napi::Buffer<unsigned char>>().Data();
    length = value.As<Napi::Buffer<unsigned char>>().Length();
}

void napi_get_value_function(const Napi::Value& value,Napi::FunctionReference& result) {
    result = Napi::Persistent(value.As<Napi::Function>());
}

void napi_get_obj_bool(const Napi::Object& obj, const char* key, bool& result) {
    napi_get_value_bool(obj[key], result);
}

void napi_get_obj_int32(const Napi::Object& obj, const char* key, int& result) {
    napi_get_value_int32(obj[key], result);
}

void napi_get_obj_utf8_string(const Napi::Object& obj,const char* key, std::string& result) {
    napi_get_value_utf8_string(obj[key], result);
}

void napi_get_obj_uint32(const Napi::Object& obj, const char* key,unsigned int& result) {
    napi_get_value_uint32(obj[key], result);
}

void napi_get_obj_obj(const Napi::Object& obj, const char* key, Napi::Object& result) {
    napi_get_value_obj(obj[key], result);
}

void napi_get_obj_node_buffer(const Napi::Object& obj, void*& data, int& length) {
    napi_get_value_node_buffer(obj, data, length);
}

//set
void napi_set_object_value_int32(Napi::Env& env, const Napi::Object& obj, const char* key, int32_t value){
    // obj.Set(Napi::String::New(env, key), Napi::Number::New(env, value));
    // obj.Set(Napi::String::New(env, key), Napi::Number::New(env, value));
}

void napi_set_object_value_uint32(Napi::Env& env, const Napi::Object& obj, const char* key, uint32_t value){
    // obj.Set(Napi::String::New(env, key), Napi::Number::New(env, value));
}

void napi_set_object_value_int64(Napi::Env& env, const Napi::Object& obj, const char* key, int64_t value){
    // obj.Set(Napi::String::New(env, key), Napi::Number::New(env, value));
}

void napi_set_object_value_float(Napi::Env& env, const Napi::Object& obj, const char*  key, float value){
    // obj.Set(Napi::String::New(env, key), Napi::Number::New(env, value));
}

void napi_set_object_value_double(Napi::Env& env, const Napi::Object& obj, const char*  key, double value){
    // obj.Set(Napi::String::New(env, key), Napi::Number::New(env, value));
}

void napi_set_object_value_bool(Napi::Env& env, const Napi::Object& obj, const char*  key, bool value){
    // obj.Set(Napi::String::New(env, key), Napi::Boolean::New(env, value));
}

void napi_set_object_value_string(Napi::Env& env,  const Napi::Object& obj, const char*  key, std::string value){
    // obj.Set(Napi::String::New(env, key), Napi::String::New(env, value));
}

void napi_set_object_value_obj(Napi::Env& env,  const Napi::Object& obj, const char*  key, Napi::Object& value){
    // obj.Set(Napi::String::New(env, key), value);
}


}