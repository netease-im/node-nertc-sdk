/**
 * @file js-convert.cpp
 * @brief OBJECT与JSON的转换实现
 * @copyright Copyright (c) 2021 NetEase, Inc.  All rights reserved.\n
 *            Use of this source code is governed by a MIT license that can be found in the LICENSE file.
 * @author wjzh
 * @date 2021/07/15
 */

#include "js-convert.h"

namespace nemeeting_util {
void console_log(const Napi::Env& env, const std::string& str) {
    Napi::Object global = env.Global();
    Napi::Object console = global.Get("console").As<Napi::Object>();
    Napi::Function log = console.Get("log").As<Napi::Function>();
    log.Call(console, {Napi::String::New(env, str)});
}

std::string object2jstring(const Napi::Env& env, const Napi::Object& obj) {
    Napi::Object json = env.Global().Get("JSON").As<Napi::Object>();
    Napi::Function stringify = json.Get("stringify").As<Napi::Function>();
    Napi::String stringValue = stringify.Call(json, {obj}).As<Napi::String>();
    if (stringValue.IsUndefined()) {
        // const napi_extended_error_info* error_info = nullptr;
        // napi_status status = napi_get_last_error_info(env, &error_info);
        // const char* message = (error_info->error_message == nullptr) ? "empty error message" : error_info->error_message;
        // napi_throw_error(env, NULL, message);
        return "";
    }
    return stringValue.Utf8Value();
}

Napi::Object jstring2object(const Napi::Env& env, const std::string& json_string) {
    Napi::Object json = env.Global().Get("JSON").As<Napi::Object>();
    Napi::Function parse = json.Get("parse").As<Napi::Function>();
    Napi::Object obj = parse.Call(json, {Napi::String::New(env, json_string)}).As<Napi::Object>();
    if (obj.IsUndefined()) {
        // const napi_extended_error_info* error_info = nullptr;
        // napi_status status = napi_get_last_error_info(env, &error_info);
        // const char* message = (error_info->error_message == nullptr) ? "empty error message" : error_info->error_message;
        // napi_throw_error(env, NULL, message);
        return Napi::Object::New(env);
    }
    return obj;
}

}  // namespace nemeeting_util
