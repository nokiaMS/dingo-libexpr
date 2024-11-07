// Copyright (c) 2023 dingodb.com, Inc. All Rights Reserved
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef _EXPR_CALC_MATHEMATIC_H_
#define _EXPR_CALC_MATHEMATIC_H_

#include "../types.h"

namespace dingodb::expr::calc {

//min函数模板声明。
template <typename T>
T Min(T v0, T v1) {
  return std::min(v0, v1);
}

//min函数声明，编译器优先检查是否有函数定义了，如果已经存在则不使用模板。
template <>
String Min(String v0, String v1);

//max模板声明。
template <typename T>
T Max(T v0, T v1) {
  return std::max(v0, v1);
}

//max函数声明。
template <>
String Max(String v0, String v1);

//abs模板声明。
template <typename T>
T Abs(T v);

//abs int32函数声明。
template <>
int32_t Abs(int32_t v);

//abs int64函数声明。
template <>
int64_t Abs(int64_t v);

//abs float函数声明。
template <>
float Abs(float v);

//abs double函数声明。
template <>
double Abs(double v);

//带有溢出检测的abs模板声明。
template <typename T>
T AbsCheck(T v) {
  return Abs(v);
}

//带有溢出检测的abs int32函数声明。
template <>
int32_t AbsCheck(int32_t v);

//带有溢出检测的int64函数声明。
template <>
int64_t AbsCheck(int64_t v);

}  // namespace dingodb::expr::calc

#endif /* _EXPR_CALC_MATHEMATIC_H_ */
