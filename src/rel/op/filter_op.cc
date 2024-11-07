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

#include "filter_op.h"

#include "../../expr/calc/special.h"
#include "../../expr/runner.h"

namespace dingodb::rel::op {

/*
 * 构造函数。
 */
FilterOp::FilterOp(const expr::Runner *filter) : m_filter(filter) {
}

/*
 * 析构函数。
 */
FilterOp::~FilterOp() {
  delete m_filter;
}

/*
 * 把一个tuple输入给算子，然后进行计算。
 */
const expr::Tuple *FilterOp::Put(const expr::Tuple *tuple) const {
  //tuple绑定。
  m_filter->BindTuple(tuple);
  //进行算子运算。
  m_filter->Run();
  //获得计算结果。
  auto v = m_filter->Get();

  //如果满足条件返回tuple，否则返回空指针。
  if (expr::calc::IsTrue<bool>(v)) {
    return tuple;
  }
  delete tuple;
  return nullptr;
}

}  // namespace dingodb::rel::op
