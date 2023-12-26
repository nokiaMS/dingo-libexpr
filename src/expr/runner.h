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

class Runner {
 public:
  Runner() = default;

  virtual ~Runner() = default;

  const Byte *Decode(const Byte *code, size_t len) {
    return m_operator_vector.Decode(code, len);
  }

  void BindTuple(const Tuple *tuple, bool own_tuple = false) const {
    m_operand_stack.BindTuple(tuple, own_tuple);
  }

  void Run() const;

  Operand GetRawResult() const {
    return m_operand_stack.GetRaw();
  }

  template <typename T>
  Wrap<T> GetResult() const {
    return m_operand_stack.Get<T>();
  }

  Byte GetType() const {
    return m_operator_vector.GetType();
  }

  Tuple *GetTuple() const;

 private:
  mutable OperandStack m_operand_stack;

  OperatorVector m_operator_vector;
};

}  // namespace dingodb::expr

#endif /* _EXPR_RUNNER_H_ */
