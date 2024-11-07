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

#ifndef _EXPR_TYPES_H_
#define _EXPR_TYPES_H_

#include <cstdint>

#include "expr_string.h"

namespace dingodb::expr {

//定义unsigned char的别名为Byte。
using Byte = unsigned char;

//定义了各种类型常量。
const Byte TYPE_NULL = 0x00;
const Byte TYPE_INT32 = 0x01;
const Byte TYPE_INT64 = 0x02;
const Byte TYPE_BOOL = 0x03;
const Byte TYPE_FLOAT = 0x04;
const Byte TYPE_DOUBLE = 0x05;
const Byte TYPE_DECIMAL = 0x06;
const Byte TYPE_STRING = 0x07;

//类型总数。
const size_t TYPE_NUM = 8;

//Decimal为long double。
using Decimal = long double;  // TODO: what's the real type?

//定义了一个模板类。
template <Byte T>
class CxxTraits {};

//定义模板类。
template <>
class CxxTraits<TYPE_INT32> {
 public:
  using Type = int32_t;
};

//定义模板类。
template <>
class CxxTraits<TYPE_INT64> {
 public:
  using Type = int64_t;
};

//定义模板类。
template <>
class CxxTraits<TYPE_BOOL> {
 public:
  using Type = bool;
};

//定义模板类。
template <>
class CxxTraits<TYPE_FLOAT> {
 public:
  using Type = float;
};

//定义模板类。
template <>
class CxxTraits<TYPE_DOUBLE> {
 public:
  using Type = double;
};

//定义模板类。
template <>
class CxxTraits<TYPE_DECIMAL> {
 public:
  using Type = Decimal;
};

//定义模板类。
template <>
class CxxTraits<TYPE_STRING> {
 public:
  using Type = String;
};

/*
 *给定类型值返回类型名称。
 */
const char *TypeName(Byte type);

/*
 * typename关键字用于指定其后是一个类型名而非一个变量名。
 * 定义了Typeof函数实现。
 */
template <Byte B>
using TypeOf = typename CxxTraits<B>::Type;

}  // namespace dingodb::expr

#endif /* _EXPR_TYPES_H_ */
