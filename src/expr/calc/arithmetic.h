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

#ifndef _EXPR_CALC_ARITHMETIC_H_
#define _EXPR_CALC_ARITHMETIC_H_

#include "../operand.h"

/*
 *定义了一些数学计算函数的模版。
 *如果想使用这些模版，那么要求参数类型需要支持对应的操作符：+,-,*,/,%。
 */
namespace dingodb::expr::calc {

/*
 *取正数。
 */
template <typename T>
T Pos(T v) {
  return v;
}

/*
 *取负数。
 */
template <typename T>
T Neg(T v) {
  return -v;
}

/*
 *加法。
 */
template <typename T>
T Add(T v0, T v1) {
  return v0 + v1;
}

/*
 *减法。
 */
template <typename T>
T Sub(T v0, T v1) {
  return v0 - v1;
}

/*
 *乘法。
 */
template <typename T>
T Mul(T v0, T v1) {
  return v0 * v1;
}

/*
 *除法。
 *如果除数等于0则返回空指针，否则计算。
 */
template <typename T>
Operand Div(T v0, T v1) {
  if (v1 != 0) {
    return v0 / v1;
  }
  return nullptr;
}

/*
 *取模运算。
 *如果模数等于0则返回空指针，否则计算。
 */
template <typename T>
Operand Mod(T v0, T v1) {
  if (v1 != 0) {
    return v0 % v1;
  }
  return nullptr;
}

}  // namespace dingodb::expr::calc

#endif /* _EXPR_CALC_ARITHMETIC_H_ */
