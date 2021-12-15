/**
 * @file js-convert.h
 * @brief OBJECT与JSON的转换头文件
 * @copyright Copyright (c) 2021 NetEase, Inc.  All rights reserved.\n
 *            Use of this source code is governed by a MIT license that can be found in the LICENSE file.
 * @author wjzh
 * @date 2021/07/15
 */

#ifndef JS_CONVERT_H
#define JS_CONVERT_H

#include "napi.h"

#define CONSOLE_LOG(env, str) nemeeting_util::console_log(env, str)
namespace nemeeting_util {
void console_log(const Napi::Env& env, const std::string& str);
std::string object2jstring(const Napi::Env& env, const Napi::Object& obj);
Napi::Object jstring2object(const Napi::Env& env, const std::string& json_string);
}  // namespace nemeeting_util

#endif  // !JS_CONVERT_H
