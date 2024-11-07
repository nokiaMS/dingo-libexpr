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

#include "special.h"

namespace dingodb::expr::calc {

/*
 *实现了string类型的isTrue。
 */
template <>
bool IsTrue<String>([[maybe_unused]] const Operand &v) {
  return false;
}

/*
 *实现了string类型的isFalse。
 */
template <>
bool IsFalse<String>(const Operand &v) {
  return v != nullptr;
}

}  // namespace dingodb::expr::calc
