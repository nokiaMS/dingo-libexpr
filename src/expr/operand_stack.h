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

#ifndef _OPERAND_STACK_H_
#define _OPERAND_STACK_H_

#include <deque>
#include <stdexcept>

#include "operand.h"

namespace dingodb::expr {

/*
 * 定义了操作数栈。
 */
class OperandStack {
 public:
  //默认构造函数。
  OperandStack() : m_tuple(nullptr) {
  }

  //默认析构函数。
  virtual ~OperandStack() = default;

  //弹出一个操作数。
  void Pop() {
    m_stack.pop_back();
  }

  //获得一个操作数。
  Operand Get() const {
    return m_stack.back();
  }

  //向栈中推入一个操作数。
  void Push(const Operand &v) {
    m_stack.push_back(v);
  }

  //向栈中推入T类型的操作数。
  template <typename T>
  void Push(T v) {
    m_stack.push_back(v);
  }

  //向栈中推入空指针。
  template <typename T>
  void Push() {
    m_stack.push_back(nullptr);
  }

  //进行tuple绑定。
  void BindTuple(const Tuple *tuple) {
    m_tuple = tuple;
  }

  /*
   * 向操作数栈中推入一个var，这个var应该是在一个tuple中。
   */
  void PushVar(int32_t index) {
    if (m_tuple != nullptr) {
      m_stack.push_back((*m_tuple)[index]);
    } else {
      throw std::runtime_error("No tuple provided.");
    }
  }

  //清空操作数栈。
  void Clear() {
    m_stack.clear();
  }

  //获得操作数栈的大小。
  size_t Size() const {
    return m_stack.size();
  }

  //获得操作数栈的开始位置。
  auto begin() const  // NOLINT(readability-identifier-naming)
  {
    return m_stack.cbegin();
  }

  //获得操作数栈的结束位置。
  auto end() const  // NOLINT(readability-identifier-naming)
  {
    return m_stack.cend();
  }

 private:
  //定义了一个双端队列用于存储操作数。
  std::deque<Operand> m_stack;
  const Tuple *m_tuple;
};

}  // namespace dingodb::expr

#endif /* _OPERAND_STACK_H_ */
