/**
 * @file macros.h
 * @brief 一些全局宏定义
 * @copyright Copyright (c) 2021 NetEase, Inc.  All rights reserved.\n
 *            Use of this source code is governed by a MIT license that can be found in the LICENSE file.
 * @author wjzh
 * @date 2021/07/15
 */

#ifndef MACROS_H
#define MACROS_H

#include <napi.h>
#include <stdio.h>
#include <vector>

namespace nemeeting_util {
// class CallbackWorker;
// class EventWorker;
}  // namespace nemeeting_util

#define CHECK_NEMEETINGSDK_INIT(...)                                                         \
    if (!gMeetingSDK || !gMeetingSDK->isInitialized()) {                                     \
        ERROR_CODE_MESSAGE(nem_sdk_interface::ERROR_CODE_SDK_UNINITIALIZE, "", errorObject); \
        fun.Call({errorObject});                                                             \
        return __VA_ARGS__;                                                                  \
    }

#define CHECK_NEMEETINGSDK_INIT_EX                       \
    if (!gMeetingSDK || !gMeetingSDK->isInitialized()) { \
        printf("gMeetingSDK not initialized.\n");        \
        return Env().Undefined();                        \
    }

#define REQUIRE_ARGUMENTS(n, ...)                                                            \
    if (info.Length() < (n)) {                                                               \
        Napi::TypeError::New(env, "Expected " #n " arguments").ThrowAsJavaScriptException(); \
        return __VA_ARGS__;                                                                  \
    }

#define REQUIRE_ARGUMENT_EXTERNAL(i, var, ...)                                             \
    if (info.Length() <= (i) || !info[i].IsExternal()) {                                   \
        Napi::TypeError::New(env, "Argument " #i " invalid").ThrowAsJavaScriptException(); \
        return __VA_ARGS__;                                                                \
    }                                                                                      \
    Napi::External var = info[i].As<Napi::External>();

#define REQUIRE_ARGUMENT_OBJECT(i, var, ...)                                                         \
    if (info.Length() <= (i) || !info[i].IsObject()) {                                               \
        Napi::TypeError::New(env, "Argument " #i " must be an object").ThrowAsJavaScriptException(); \
        return __VA_ARGS__;                                                                          \
    }                                                                                                \
    Napi::Object var = info[i].As<Napi::Object>();

#define REQUIRE_ARGUMENT_FUNCTION(i, var, ...)                                                        \
    if (info.Length() <= (i) || !info[i].IsFunction()) {                                              \
        Napi::TypeError::New(env, "Argument " #i " must be a function").ThrowAsJavaScriptException(); \
        return __VA_ARGS__;                                                                           \
    }                                                                                                 \
    Napi::Function var = info[i].As<Napi::Function>();

#define REQUIRE_ARGUMENT_STRING(i, var, ...)                                                        \
    if (info.Length() <= (i) || !info[i].IsString()) {                                              \
        Napi::TypeError::New(env, "Argument " #i " must be a string").ThrowAsJavaScriptException(); \
        return __VA_ARGS__;                                                                         \
    }                                                                                               \
    std::string var = info[i].As<Napi::String>();

#define REQUIRE_ARGUMENT_BOOL(i, var, ...)                                                        \
    if (info.Length() <= (i) || !info[i].IsBoolean()) {                                           \
        Napi::TypeError::New(env, "Argument " #i " must be a bool").ThrowAsJavaScriptException(); \
        return __VA_ARGS__;                                                                       \
    }                                                                                             \
    bool var = info[i].As<Napi::Boolean>();

#define REQUIRE_ARGUMENT_INT(i, var, ...)                                                             \
    if (info.Length() <= (i) || !info[i].IsNumber()) {                                                \
        Napi::TypeError::New(env, "Argument " #i " must be an integer").ThrowAsJavaScriptException(); \
        return __VA_ARGS__;                                                                           \
    }                                                                                                 \
    int var(info[i].As<Napi::Number>().Int32Value());

#define REQUIRE_ARGUMENT_INT64(i, var, ...)                                                           \
    if (info.Length() <= (i) || !info[i].IsNumber()) {                                                \
        Napi::TypeError::New(env, "Argument " #i " must be an integer").ThrowAsJavaScriptException(); \
        return __VA_ARGS__;                                                                           \
    }                                                                                                 \
    int64_t var(info[i].As<Napi::Number>().Int64Value());

#define REQUIRE_ARGUMENT_ARRAY(i, var, ...)                                                         \
    if (info.Length() <= (i) || !info[i].IsArray()) {                                               \
        Napi::TypeError::New(env, "Argument " #i " must be an array").ThrowAsJavaScriptException(); \
        return __VA_ARGS__;                                                                         \
    }                                                                                               \
    Napi::Array var = info[i].As<Napi::Array>();

#define OPTIONAL_ARGUMENT_FUNCTION(i, var, ...)                                                           \
    Napi::Function var;                                                                                   \
    if (info.Length() > i && !info[i].IsUndefined()) {                                                    \
        if (!info[i].IsFunction()) {                                                                      \
            Napi::TypeError::New(env, "Argument " #i " must be a function").ThrowAsJavaScriptException(); \
            return __VA_ARGS__;                                                                           \
        }                                                                                                 \
        var = info[i].As<Napi::Function>();                                                               \
    }

#define OPTIONAL_ARGUMENT_INTEGER(i, var, default, ...)                                               \
    int var;                                                                                          \
    if (info.Length() <= (i)) {                                                                       \
        var = (default);                                                                              \
    } else if (info[i].IsNumber()) {                                                                  \
        if (OtherIsInt(info[i].As<Number>())) {                                                       \
            var = info[i].As<Napi::Number>().Int32Value();                                            \
        }                                                                                             \
    } else {                                                                                          \
        Napi::TypeError::New(env, "Argument " #i " must be an integer").ThrowAsJavaScriptException(); \
        return __VA_ARGS__;                                                                           \
    }

#define DEFINE_CONSTANT_INTEGER(target, constant, name)                      \
    Napi::PropertyDescriptor::Value(#name, Napi::Number::New(env, constant), \
                                    static_cast<napi_property_attributes>(napi_enumerable | napi_configurable)),

#define DEFINE_CONSTANT_STRING(target, constant, name)                       \
    Napi::PropertyDescriptor::Value(#name, Napi::String::New(env, constant), \
                                    static_cast<napi_property_attributes>(napi_enumerable | napi_configurable)),

#define ERROR_CODE_MESSAGE(code, msg, name)                                 \
    Napi::Object name = Napi::Object::New(env);                             \
    name.Set(Napi::String::New(env, "code"), Napi::Number::New(env, code)); \
    name.Set(Napi::String::New(env, "msg"), Napi::String::New(env, msg));

#define DECLARE_EVENTWORKER(name) std::shared_ptr<nemeeting_util::EventWorker> name;
#define NEW_EVENTWORKER(name, fun) name = std::make_shared<nemeeting_util::EventWorker>(fun);
#define DECLARE_EVENTWORKER_EX(name, className) std::shared_ptr<className> name;
#define NEW_EVENTWORKER_EX(name, className, fun) name = std::make_shared<className>(fun);

// #define NEW_CALLBACKWORKER(name, fun) auto name = new nemeeting_util::CallbackWorker(fun);
// #define NEW_CALLBACKWORKER_EX(name, className, fun) auto name = new className(fun);

#define EMIT_EVENT(obj, argc, argv) TRY_CATCH_CALL((obj), (obj).Get("emit").As<Napi::Function>(), argc, argv);

// The Mac OS compiler complains when argv is NULL unless we
// first assign it to a locally defined variable.
#define TRY_CATCH_CALL(context, callback, argc, argv, ...)                 \
    Napi::Value* passed_argv = argv;                                       \
    std::vector<napi_value> args;                                          \
    if ((argc != 0) && (passed_argv != NULL)) {                            \
        args.assign(passed_argv, passed_argv + argc);                      \
    }                                                                      \
    Napi::Value res = (callback).MakeCallback(Napi::Value(context), args); \
    if (res.IsEmpty())                                                     \
        return __VA_ARGS__;

namespace nemeeting_util {
inline Napi::String StringConcat(Napi::Value str1, Napi::Value str2) {
    return Napi::String::New(str1.Env(), str1.As<Napi::String>().Utf8Value() + str2.As<Napi::String>().Utf8Value());
}

// A Napi substitute IsInt32()
inline bool OtherIsInt(Napi::Number source) {
    double orig_val = source.DoubleValue();
    double int_val = (double)source.Int32Value();
    if (orig_val == int_val) {
        return true;
    } else {
        return false;
    }
}
}  // namespace nemeeting_util
#endif  // MACROS_H
