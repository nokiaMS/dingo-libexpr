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

#ifndef _EXPR_RUNNER_H_
#define _EXPR_RUNNER_H_

#include "operand_stack.h"
#include "operator_vector.h"
#include "types.h"

namespace dingodb::expr {

/*
 * 定义了Runner类，这个类是整个libexpr库的入口。
 */
class Runner {
 public:
  //默认构造函数。
  Runner() = default;

  //默认析构函数。
  virtual ~Runner() = default;

  //给定一个字节序列及其长度，然后对这个字节序列解码获得对应的表达式树。
  const Byte *Decode(const Byte *code, size_t len) {
    //直接调用m_operator_vector进行解析。
    return m_operator_vector.Decode(code, len);
  }

  void BindTuple(const Tuple *tuple) const {
    m_operand_stack.BindTuple(tuple);
  }

  void Run() const;

  Operand Get() const {
    return m_operand_stack.Get();
  }

  template <typename T>
  std::optional<T> GetOptional() const {
    auto operand = Get();
    if (operand != nullptr) {
      return std::optional<T>(operand.GetValue<T>());
    }
    return std::optional<T>();
  }

  Byte GetType() const {
    return m_operator_vector.GetType();
  }

  Tuple *GetAll() const;

 private:
  //存储表达式的操作数。
  mutable OperandStack m_operand_stack;

  //存储表带的操作符序列。
  OperatorVector m_operator_vector;
};

}  // namespace dingodb::expr

#endif /* _EXPR_RUNNER_H_ */
