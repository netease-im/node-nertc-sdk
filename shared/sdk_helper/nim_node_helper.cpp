#include "../shared/sdk_helper/nim_node_helper.h"

napi_status nim_napi_get_value_utf8string_list(Isolate* isolate, const Local<Value>& value, std::list<utf8_string>& out)
{
    if (!value->IsArray())
        return napi_invalid_arg;

    napi_status status = napi_ok;
    auto values = Local<Array>::Cast(value);
    for (size_t i = 0; i < values->Length(); i++)
    {
        String::Utf8Value val(isolate, values->Get(isolate->GetCurrentContext(), i).ToLocalChecked()->ToString(isolate->GetCurrentContext()).ToLocalChecked());
        out.push_back(std::string(*val));
    }
    return status;    
}
napi_status nim_napi_get_value_utf8string(Isolate* isolate, const Local<Value>& value, UTF8String& out)
{
    if (!value->IsString())
        return napi_invalid_arg;

    out.clear();
    napi_status status = napi_ok;
    do {
        int len = value.As<String>()->Utf8Length(isolate);
        if (len == 0) break;
        utf8_char *str = (utf8_char *)calloc(len + 1, 1);
        if (!str) break;
        len = value.As<String>()->WriteUtf8(isolate, str, len, nullptr, String::REPLACE_INVALID_UTF8 | String::NO_NULL_TERMINATION);
        str[len] = '\0';
        out.set(str, len);
        free(str);
    } while (false);
    return status;
}
napi_status nim_napi_get_value_UTF8String(Isolate* isolate, const Local<Value>& value, UTF8String& out)
{
    return nim_napi_get_value_utf8string(isolate, value, out);
}
napi_status nim_napi_get_value_uint32(Isolate* isolate, const Local<Value>& value, uint32_t& out)
{
    if (!value->IsUint32())
        return napi_invalid_arg;
    out = value->Uint32Value(isolate->GetCurrentContext()).ToChecked();
    return napi_ok; 
}
napi_status nim_napi_get_value_int32(Isolate* isolate, const Local<Value>& value, int32_t& out)
{
    if (!value->IsInt32())
        return napi_invalid_arg;
    out = value->Int32Value(isolate->GetCurrentContext()).ToChecked();
    return napi_ok; 
}
napi_status nim_napi_get_value_bool(Isolate* isolate, const Local<Value>& value, bool& out)
{
    if (!value->IsBoolean())
        return napi_invalid_arg;
    out = value->BooleanValue(isolate);
    return napi_ok;
}
napi_status nim_napi_get_value_int64(Isolate* isolate, const Local<Value>& value, int64_t& out)
{
    if (!value->IsNumber())
        return napi_invalid_arg;
    out = value->IntegerValue(isolate->GetCurrentContext()).ToChecked();
    return napi_ok;
}
napi_status nim_napi_get_value_uint64(Isolate* isolate, const Local<Value>& value, uint64_t& out)
{
    if (!value->IsNumber())
        return napi_invalid_arg;
    //TODO
    int64_t o = 0;
    auto status = nim_napi_get_value_int64(isolate, value, o);
    out = (uint64_t)o;
    return status;   
}

static napi_status get_object_value(Isolate* isolate, const Local<Object>& obj, const utf8_string& keyName, Local<Value> &out)
{
    auto name = nim_napi_new_utf8string(isolate, keyName.c_str());
    auto maybeValue = obj->Get(isolate->GetCurrentContext(), name);
    if (!maybeValue.IsEmpty()) {
        out = maybeValue.ToLocalChecked();
        return napi_ok;
    }
    return napi_invalid_arg;
}

napi_status nim_napi_get_object_value(Isolate* isolate, const Local<Object>& obj, const utf8_string& keyName, Local<Value>& out)
{
    if (get_object_value(isolate, obj, keyName, out) == napi_ok && !out.IsEmpty())
        return napi_ok;
    return napi_invalid_arg;
}

napi_status nim_napi_get_object_value_utf8string(Isolate* isolate, const Local<Object>& obj, const utf8_string& keyName, UTF8String& out)
{
    Local<Value> value;
    if (get_object_value(isolate, obj, keyName, value) == napi_ok)
        return nim_napi_get_value_utf8string(isolate, value, out);
    return napi_invalid_arg;
}
napi_status nim_napi_get_object_value_uint32(Isolate* isolate, const Local<Object>& obj, const utf8_string& keyName, uint32_t& out)
{
    Local<Value> value;
    if (get_object_value(isolate, obj, keyName, value) == napi_ok)
        return nim_napi_get_value_uint32(isolate, value, out);
    return napi_invalid_arg;
}
napi_status nim_napi_get_object_value_int32(Isolate* isolate, const Local<Object>& obj, const utf8_string& keyName, int32_t& out)
{
    Local<Value> value;
    if (get_object_value(isolate, obj, keyName, value) == napi_ok)
        return nim_napi_get_value_int32(isolate, value, out);
    return napi_invalid_arg;
}
napi_status nim_napi_get_object_value_bool(Isolate* isolate, const Local<Object>& obj, const utf8_string& keyName, bool& out)
{
    Local<Value> value;
    if (get_object_value(isolate, obj, keyName, value) == napi_ok)
        return nim_napi_get_value_bool(isolate, value, out);
    return napi_invalid_arg;
}
napi_status nim_napi_get_object_value_int64(Isolate* isolate, const Local<Object>& obj, const utf8_string& keyName, int64_t& out)
{
    Local<Value> value;
    if (get_object_value(isolate, obj, keyName, value) == napi_ok)
        return nim_napi_get_value_int64(isolate, value, out);
    return napi_invalid_arg;
}
napi_status nim_napi_get_object_value_uint64(Isolate* isolate, const Local<Object>& obj, const utf8_string& keyName, uint64_t& out)
{
    Local<Value> value;
    if (get_object_value(isolate, obj, keyName, value) == napi_ok)
        return nim_napi_get_value_uint64(isolate, value, out);
    return napi_invalid_arg;
}

napi_status nim_napi_set_object_value_utf8string(Isolate* isolate, Local<Object>& obj, const utf8_string& keyName, const utf8_string& keyValue)
{
    Local<Value> key = nim_napi_new_utf8string(isolate, keyName.c_str());
    Local<Value> value = nim_napi_new_utf8string(isolate, keyValue.c_str());
    obj->Set(isolate->GetCurrentContext(), key, value);
    return napi_ok;
}

napi_status nim_napi_set_object_value_uint32(Isolate* isolate, Local<Object>& obj, const utf8_string& keyName, const uint32_t& keyValue)
{
    Local<Value> key = nim_napi_new_utf8string(isolate, keyName.c_str());
    Local<Value> value = nim_napi_new_uint32(isolate, keyValue);
    obj->Set(isolate->GetCurrentContext(), key, value);
    return napi_ok;
}
napi_status nim_napi_set_object_value_int32(Isolate* isolate, Local<Object>& obj, const utf8_string& keyName, const int32_t& keyValue)
{
    Local<Value> key = nim_napi_new_utf8string(isolate, keyName.c_str());
    Local<Value> value = nim_napi_new_int32(isolate, keyValue);
    obj->Set(isolate->GetCurrentContext(), key, value);
    return napi_ok;    
}
napi_status nim_napi_set_object_value_bool(Isolate* isolate, Local<Object>& obj, const utf8_string& keyName, const bool& keyValue)
{
    Local<Value> key = nim_napi_new_utf8string(isolate, keyName.c_str());
    Local<Value> value = nim_napi_new_bool(isolate, keyValue);
    obj->Set(isolate->GetCurrentContext(), key, value);
    return napi_ok;     
}
napi_status nim_napi_set_object_value_int64(Isolate* isolate, Local<Object>& obj, const utf8_string& keyName, const int64_t& keyValue)
{
    Local<Value> key = nim_napi_new_utf8string(isolate, keyName.c_str());
    Local<Value> value = nim_napi_new_int64(isolate, keyValue);
    obj->Set(isolate->GetCurrentContext(), key, value);
    return napi_ok;      
}
napi_status nim_napi_set_object_value_uint64(Isolate* isolate, Local<Object>& obj, const utf8_string& keyName, const uint64_t& keyValue)
{
    Local<Value> key = nim_napi_new_utf8string(isolate, keyName.c_str());
    Local<Value> value = nim_napi_new_uint64(isolate, keyValue);
    obj->Set(isolate->GetCurrentContext(), key, value);
    return napi_ok;  
}
Local<Value> nim_napi_new_utf8string(Isolate* isolate, const utf8_char *keyValue)
{
    return String::NewFromUtf8(isolate, keyValue ? keyValue : "", NewStringType::kInternalized).ToLocalChecked();
}
Local<Value> nim_napi_new_uint32(Isolate* isolate, const uint32_t& keyValue)
{
    return Integer::NewFromUnsigned(isolate, keyValue);
}
Local<Value> nim_napi_new_int32(Isolate* isolate, const int32_t& keyValue)
{
    return Integer::New(isolate, keyValue);
}
Local<Value> nim_napi_new_bool(Isolate* isolate, const bool& keyValue)
{
    return Boolean::New(isolate, keyValue);
}
Local<Value> nim_napi_new_int64(Isolate* isolate, const int64_t& keyValue)
{
    // return BigInt::New(isolate, keyValue);
    //TOOD
    uint64_t k = (uint64_t)keyValue;
    return nim_napi_new_uint64(isolate, k);
}
Local<Value> nim_napi_new_uint64(Isolate* isolate, const uint64_t& keyValue)
{
    // return BigInt::NewFromUnsigned(isolate, keyValue);
    //TODO
    return Number::New(isolate, (double)keyValue);
}

napi_status nim_napi_assemble_string_array(Isolate* isolate, const std::list<utf8_string>& strs, Local<Array>& arr)
{
    int index = 0;
    for (auto const &str : strs) {
        arr->Set(isolate->GetCurrentContext(), index++, nim_napi_new_utf8string(isolate, str.c_str()));
    }
    return napi_ok;
}

napi_status nim_napi_assemble_string_map(Isolate* isolate, const std::map<utf8_string, utf8_string>& ipt, Local<Object>& opt)
{
    for (auto const &i : ipt)
    {
        opt->Set(isolate->GetCurrentContext(), nim_napi_new_utf8string(isolate, i.first.c_str()), nim_napi_new_utf8string(isolate, i.second.c_str()));
    }
    return napi_ok;    
}

napi_status nim_napi_UTF8Strings_to_utf8strings(const std::list<UTF8String>& strs, std::list<utf8_string>& out)
{
    for (auto &&i : strs)
    {
        out.push_back(i.toUtf8String());
    }
    return napi_ok;
    
}