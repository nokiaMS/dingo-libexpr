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

#ifndef _EXPR_CALC_CASTING_H_
#define _EXPR_CALC_CASTING_H_

#include "../types.h"

namespace dingodb::expr::calc {

/*
 *转换函数模版。
 */
template <typename D, typename S>
D Cast(S v) {
  return (D)v;
}

//float转int32.
template <>
int32_t Cast(float v);

//double转int32.
template <>
int32_t Cast(double v);

//string转int32.
template <>
int32_t Cast(String v);

//float转int64.
template <>
int64_t Cast(float v);

//double转int64.
template <>
int64_t Cast(double v);

//string转int64.
template <>
int64_t Cast(String v);

//string转float.
template <>
float Cast(String v);

//string转double.
template <>
double Cast(String v);

//int32转string。
template <>
String Cast(int32_t v);

//int64转string。
template <>
String Cast(int64_t v);

//bool转string。
template <>
String Cast(bool v);

//float转string。
template <>
String Cast(float v);

//double转string。
template <>
String Cast(double v);

//S类型转换为D类型。
template <typename D, typename S>
D CastCheck(S v) {
  return Cast<D>(v);
}

//int64转int32.
template <>
int32_t CastCheck(int64_t v);

//float转int32.
template <>
int32_t CastCheck(float v);

//double转int32.
template <>
int32_t CastCheck(double v);

//float转int64.
template <>
int64_t CastCheck(float v);

//double转int64.
template <>
int64_t CastCheck(double v);

}  // namespace dingodb::expr::calc

#endif /* _EXPR_CALC_CASTING_H_ */
