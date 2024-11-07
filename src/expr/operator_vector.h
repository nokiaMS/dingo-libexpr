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

#ifndef _EXPR_OPERATOR_VECTOR_H_
#define _EXPR_OPERATOR_VECTOR_H_

#include <vector>

#include "operator.h"

namespace dingodb::expr {

/*
 * 操作符数组。
 */
class OperatorVector {
 public:
  /*
   * 构造函数。
   */
  OperatorVector() = default;

  /*
   * 析构函数。
   */
  virtual ~OperatorVector() {
    Release();
  }

  const Byte *Decode(const Byte code[], size_t len);

  Byte GetType() const {
    return m_vector.back()->GetType();
  }

  //获得迭代器开始位置。
  auto begin() const  // NOLINT(readability-identifier-naming)
  {
    return m_vector.cbegin();
  }

  //获得迭代器结束位置。
  auto end() const  // NOLINT(readability-identifier-naming)
  {
    return m_vector.cend();
  }

 private:
  //存储操作符序列。
  std::vector<const Operator *> m_vector;
  std::vector<const Operator *> m_to_release;

  //添加一个操作符。
  void Add(const Operator *op) {
    m_vector.push_back(op);
  }

  //把解析后的操作符添加到内部队列中。
  void AddRelease(const Operator *op) {
    m_vector.push_back(op);
    m_to_release.push_back(op);
  }

  /*
   * 释放操作符数组中的各操作符。
   */
  void Release() {
    for (const auto *op : m_to_release) {
      delete op;
    }
    m_to_release.clear();
    m_vector.clear();
  }

  /**
   * @brief Add an operator of the specified type.
   *
   * @param ops The array of the operators
   * @param type The type byte
   * @return true Successful
   * @return false Failed
   */
  [[nodiscard]] bool AddOperatorByType(const Operator *const ops[], Byte type);

  /**
   * @brief  Add a cast operator of the specified type.
   *
   * @param ops The array of the operators
   * @param b The byte indicating the source and target type
   * @return true Successful
   * @return false Failed
   */
  [[nodiscard]] bool AddCastOperator(const Operator *const ops[][TYPE_NUM], Byte b);

  [[nodiscard]] bool AddFunOperator(Byte b);
};

}  // namespace dingodb::expr

#endif /* _EXPR_OPERATOR_VECTOR_H_ */
