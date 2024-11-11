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

#define MILLISECONDS_IN_SECOND 1000

namespace dingodb::expr::calc {

/*
 * casting template.
 */
template <typename D, typename S>
D Cast(S v) {
  return (D)v;
}

template <>
int32_t Cast(float v);
template <>
int32_t Cast(double v);
template <>
int32_t Cast(String v);

template <>
int64_t Cast(float v);
template <>
int64_t Cast(double v);
template <>
int64_t Cast(String v);

template <>
float Cast(String v);

template <>
double Cast(String v);

template <>
String Cast(int32_t v);
template <>
String Cast(int64_t v);
template <>
String Cast(bool v);
template <>
String Cast(float v);
template <>
String Cast(double v);

/*
 * Date casting template.
 */
template <typename D, typename S>
D DateCast(S v) {
  return (D)v;
}

template <>
Date DateCast(String v);

template <>
Date DateCast(int64_t v);

template <>
Date DateCast(int32_t v);

/*
 * Date casting template with check.
 */
template <typename D, typename S>
D DateCastCheck(S v) {
  return DateCast<D>(v);
}

template <>
Date DateCastCheck(String v);
template <>
Date DateCastCheck(int64_t v);
template <>
Date DateCastCheck(int32_t v);

/*
 * casting template with check.
 */
template <typename D, typename S>
D CastCheck(S v) {
  return Cast<D>(v);
}

template <>
int32_t CastCheck(int64_t v);
template <>
int32_t CastCheck(float v);
template <>
int32_t CastCheck(double v);
template <>
int64_t CastCheck(float v);
template <>
int64_t CastCheck(double v);

}  // namespace dingodb::expr::calc

#endif /* _EXPR_CALC_CASTING_H_ */
