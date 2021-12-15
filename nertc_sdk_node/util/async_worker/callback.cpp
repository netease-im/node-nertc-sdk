// /**
//  * @file callback.cpp
//  * @brief 通用回调实现
//  * @copyright Copyright (c) 2021 NetEase, Inc.  All rights reserved.\n
//  *            Use of this source code is governed by a MIT license that can be found in the LICENSE file.
//  * @author wjzh
//  * @date 2021/07/15
//  */

// #include "callback.h"
// #include "util/js-convert.h"
// #include "util/macros.h"
// #include <thread>

// namespace nemeeting_util {
// CallbackWorker::CallbackWorker(Napi::Function& callback, bool bSuppressDestruct)
//     : Napi::AsyncWorker(callback) {
//     if (bSuppressDestruct)
//         SuppressDestruct();
// }

// void CallbackWorker::Execute() {}

// void CallbackWorker::OnOK() {
//     std::lock_guard<std::mutex> lockGuard(m_args_mutex);
//     Napi::Env env = Env();
//     size_t start = 0;
//     size_t end = m_args.size() - 1;
//     ArgsVisitor argsVisitor(env);
//     argsVisitor.argsValue.reserve(m_args.size());
//     if (m_errorObject && m_args.size() > 1) {
//         ERROR_CODE_MESSAGE(std::get<int>(m_args.at(0)), std::get<std::string>(m_args.at(1)), errorObject);
//         argsVisitor.argsValue.push_back(errorObject);
//         start = 2;
//     }
//     for (size_t i = start; i <= end; i++) {
//         auto& v = m_args.at(i);
//         std::visit(argsVisitor, v);
//     }

//     Call(argsVisitor.argsValue);
// }

// void CallbackWorker::Call(const std::vector<napi_value>& args) {
//     Napi::Value ret = AsyncWorker::Callback().Call(args);
//     if (napi_function != ret.Type()) {
//         Napi::Object obj = Napi::Object::New(Env());
//         obj["result"] = ret;
//         m_result = object2jstring(Env(), obj);
//         if (m_result == "{}") {
//             m_result.clear();
//         }
//     }
//     m_runCompleted = true;
// }

// std::string CallbackWorker::Run(bool async) {
//     Queue();
//     if (async) {
//         return "";
//     }

//     while (!m_runCompleted) {
//         std::this_thread::yield();
//     }
//     return m_result;
// }

// void CallbackWorker::WaitCompleted() {
//     while (!m_runCompleted) {
//         std::this_thread::yield();
//     }
// }

// void CallbackWorker::InitCallback() {
//     m_runCompleted = false;
//     m_callbackWorker = true;
//     m_errorObject = true;
//     m_args.clear();
//     m_result.clear();
// }

// std::string CallbackWorker::Callback(const std::vector<ArgsType>& args, bool hasErrorObject, bool async) {
//     WaitCompleted();
//     {
//         std::lock_guard<std::mutex> lockGuard(m_args_mutex);
//         CallbackWorker::InitCallback();
//         m_errorObject = hasErrorObject;
//         m_args.reserve(args.size());
//         m_args = args;
//     }
//     return Run(async);
// }

// //////////////////////////////////////////////////////////////////////////////////////////
// EventWorker::EventWorker(Napi::Function& callback, bool bSuppressDestruct)
//     : CallbackWorker(callback, bSuppressDestruct)
//     , m_argsVisitor(Env()) {
// }

// void EventWorker::OnOK() {
//     if (m_callbackWorker) {
//         CallbackWorker::OnOK();
//         return;
//     }

//     OnOKInit();
//     std::lock_guard<std::mutex> lockGuard(m_args_mutex);
//     CallbackWorker::Call(m_argsVisitor.argsValue);
// }

// Napi::Object EventWorker::OnOKInit() {
//     std::lock_guard<std::mutex> lockGuard(m_args_mutex);
//     Napi::Env env = Env();
//     ArgsVisitor argsVisitorTmp(env);
//     argsVisitorTmp.argsValue.reserve(m_argsMap.size());

//     Napi::Object obj = Napi::Object::New(env);
//     for (auto& it : m_argsMap) {
//         std::visit(argsVisitorTmp, it.second);
//         obj.Set(it.first, argsVisitorTmp.argsValue.back());
//     }

//     m_argsVisitor.argsValue.reserve(2);
//     m_argsVisitor.argsValue.push_back(Napi::String::New(env, m_eventName));
//     m_argsVisitor.argsValue.push_back(obj);
//     return obj;
// }

// void EventWorker::InitCallback() {
//     CallbackWorker::InitCallback();
//     m_callbackWorker = false;
//     m_eventWorker = true;
//     m_eventName.clear();
//     m_argsMap.clear();
//     m_argsVisitor.argsValue.clear();
// }

// std::string EventWorker::Callback(const std::string& name, const ArgsTypeMap& args, bool async) {
//     WaitCompleted();
//     {
//         std::lock_guard<std::mutex> lockGuard(m_args_mutex);
//         EventWorker::InitCallback();
//         m_errorObject = false;
//         m_eventName = name;
//         m_argsMap = args;
//     }
//     return Run(async);
// }

// }  // namespace nemeeting_util
