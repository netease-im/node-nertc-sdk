// /**
//  * @file callback.h
//  * @brief 通用回调头文件
//  * @copyright Copyright (c) 2021 NetEase, Inc.  All rights reserved.\n
//  *            Use of this source code is governed by a MIT license that can be found in the LICENSE file.
//  * @author wjzh
//  * @date 2021/07/15
//  */

// #ifndef CALLBACK_H
// #define CALLBACK_H

// #include <napi.h>
// #include <map>
// #include <mutex>
// #include <variant>

// #define MAKE_ARGS(args) nemeeting_util::CallbackWorker::make_args(args)

// /////////////////////////////////////////////////////////////////////////////////////////////
// #define DEFINE_CALLBACK_CLASS(className, customType)            \
//     class className : public nemeeting_util::CallbackWorker {   \
//     public:                                                     \
//         className(Napi::Function& callback);                    \
//         virtual ~className(){};                                 \
//         virtual void OnOK() override;                           \
//         virtual void InitCallback() override;                   \
//         std::string Callback(const std::vector<ArgsType>& args, \
//                              const customType& customTypeParam, \
//                              bool hasErrorObject = true,        \
//                              bool async = true);                \
//                                                                 \
//     private:                                                    \
//         customType m_customValue;                               \
//     };

// #define DEFINE_CALLBACK_ONOK(className)                 \
//     className::className(Napi::Function& callback)      \
//         : nemeeting_util::CallbackWorker(callback) {}   \
//     void className::InitCallback() {                    \
//         nemeeting_util::CallbackWorker::InitCallback(); \
//         m_callbackWorker = false;                       \
//     }                                                   \
//     void className::OnOK() {                            \
//         if (m_callbackWorker) {                         \
//             nemeeting_util::CallbackWorker::OnOK();     \
//             return;                                     \
//         }

// #define DEFINE_CALLBACK_ERROR_CODE_MESSAGE                                                                 \
//     std::lock_guard<std::mutex> lockGuard(m_args_mutex);                                                   \
//     Napi::Env env = Env();                                                                                 \
//     size_t start = 0;                                                                                      \
//     size_t end = m_args.size() - 1;                                                                        \
//     ArgsVisitor argsVisitor(env);                                                                          \
//     argsVisitor.argsValue.reserve(m_args.size());                                                          \
//     if (m_errorObject && m_args.size() > 1) {                                                              \
//         ERROR_CODE_MESSAGE(std::get<int>(m_args.at(0)), std::get<std::string>(m_args.at(1)), errorObject); \
//         argsVisitor.argsValue.push_back(errorObject);                                                      \
//         start = 2;                                                                                         \
//     }                                                                                                      \
//     for (size_t i = start; i <= end; i++) {                                                                \
//         auto& v = m_args.at(i);                                                                            \
//         std::visit(argsVisitor, v);                                                                        \
//     }

// #define DEFINE_CALLBACK_CALLBACK(className, customType)                                                                        \
//     std::string className::Callback(const std::vector<ArgsType>& args, const customType& customTypeParam, bool hasErrorObject, \
//                                     bool async) {                                                                              \
//         WaitCompleted();                                                                                                       \
//         {                                                                                                                      \
//             std::lock_guard<std::mutex> lockGuard(m_args_mutex);                                                               \
//             className::InitCallback();                                                                                         \
//             m_errorObject = hasErrorObject;                                                                                    \
//             m_args = args;                                                                                                     \
//             m_customValue = customTypeParam;                                                                                   \
//         }                                                                                                                      \
//         return Run(async);                                                                                                     \
//     }

// #define DEFINE_CALLBACK_EMIT_CALLBACK(args) CallbackWorker::Call(args);

// #define DEFINE_CALLBACK_FUNCTION_END }

// /////////////////////////////////////////////////////////////////////////////////////////////
// #define DEFINE_EVENT_CLASS(className, customType)                     \
//     class className : public nemeeting_util::EventWorker {            \
//     public:                                                           \
//         className(Napi::Function& callback);                          \
//         virtual ~className(){};                                       \
//         virtual void OnOK() override;                                 \
//         virtual void InitCallback() override;                         \
//         std::string Callback(const std::string& name,                 \
//                              const customType& customTypeParam,       \
//                              const ArgsTypeMap& args = ArgsTypeMap(), \
//                              bool async = true);                      \
//                                                                       \
//     private:                                                          \
//         customType m_customValue;                                     \
//     };

// #define DEFINE_EVENT_ONOK(className)                 \
//     className::className(Napi::Function& callback)   \
//         : nemeeting_util::EventWorker(callback) {}   \
//     void className::InitCallback() {                 \
//         nemeeting_util::EventWorker::InitCallback(); \
//         m_eventWorker = false;                       \
//     }                                                \
//     void className::OnOK() {                         \
//         if (m_eventWorker) {                         \
//             nemeeting_util::EventWorker::OnOK();     \
//             return;                                  \
//         }

// #define DEFINE_EVENT_CALLBACK(className, customType)                                                                                   \
//     std::string className::Callback(const std::string& name, const customType& customTypeParam, const ArgsTypeMap& args, bool async) { \
//         WaitCompleted();                                                                                                               \
//         {                                                                                                                              \
//             std::lock_guard<std::mutex> lockGuard(m_args_mutex);                                                                       \
//             className::InitCallback();                                                                                                 \
//             m_errorObject = false;                                                                                                     \
//             m_eventName = name;                                                                                                        \
//             m_argsMap = args;                                                                                                          \
//             m_customValue = customTypeParam;                                                                                           \
//         }                                                                                                                              \
//         return Run(async);                                                                                                             \
//     }

// #define DEFINE_EVENT_ARGS_TO_OBJECT \
//     Napi::Env env = Env();          \
//     Napi::Object obj = OnOKInit();

// #define DEFINE_EVENT_EMIT_CALLBACK CallbackWorker::Call(m_argsVisitor.argsValue);

// #define DEFINE_EVENT_FUNCTION_END }

// /////////////////////////////////////////////////////////////////////////////////////////////
// namespace nemeeting_util {
// class CallbackWorker : public Napi::AsyncWorker {
// public:
//     using ArgsType = std::variant<bool, int, double, unsigned long long, std::string>;

//     template <typename T>
//     static ArgsType make_args(T value) {
//         if constexpr (std::is_same<typename std::remove_const<typename std::decay<T>::type>::type, const char*>()) {
//             return std::string(value);
//         }
//         return value;
//     }

//     CallbackWorker(Napi::Function& callback, bool bSuppressDestruct = false);
//     virtual ~CallbackWorker(){};

//     virtual void Execute();
//     virtual void OnOK();
//     std::string Callback(const std::vector<ArgsType>& args, bool hasErrorObject = true, bool async = true);

// protected:
//     virtual void InitCallback();
//     void Call(const std::vector<napi_value>& args);
//     void WaitCompleted();
//     std::string Run(bool async);

// protected:
//     struct ArgsVisitor {
//         ArgsVisitor(const napi_env& envIn)
//             : env(envIn) {}
//         ArgsVisitor(const napi_env& envIn, napi_value value)
//             : env(envIn) {
//             argsValue.push_back(value);
//         }

//         void operator()(const bool& b) { argsValue.push_back(Napi::Boolean::New(env, b)); }
//         void operator()(const int& i) { argsValue.push_back(Napi::Number::New(env, i)); }
//         void operator()(const double& d) { argsValue.push_back(Napi::Number::New(env, d)); }
//         void operator()(const unsigned long long& ull) { argsValue.push_back(Napi::Number::New(env, ull)); }
//         void operator()(const std::string& s) { argsValue.push_back(Napi::String::New(env, s)); }

//         napi_env env;
//         std::vector<napi_value> argsValue;
//     };

// protected:
//     std::vector<ArgsType> m_args;
//     std::mutex m_args_mutex;
//     std::atomic_bool m_errorObject = {false};
//     std::atomic_bool m_callbackWorker = {false};
//     std::atomic_bool m_runCompleted = {true};
//     std::string m_result;
// };

// /////////////////////////////////////////////////////////////////////////////////////////////
// class EventWorker : public CallbackWorker {
// public:
//     using ArgsTypeMap = std::map<std::string, ArgsType>;

//     EventWorker(Napi::Function& callback, bool bSuppressDestruct = true);
//     virtual ~EventWorker(){};
//     virtual void OnOK();
//     std::string Callback(const std::string& name, const ArgsTypeMap& args = ArgsTypeMap(), bool async = true);

// protected:
//     Napi::Object OnOKInit();
//     virtual void InitCallback();

// protected:
//     std::string m_eventName;
//     ArgsTypeMap m_argsMap;
//     ArgsVisitor m_argsVisitor;
//     std::atomic_bool m_eventWorker = {false};
// };

// }  // namespace nemeeting_util
// #endif  // CALLBACK_H
