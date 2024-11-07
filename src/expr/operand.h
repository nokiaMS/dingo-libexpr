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

#ifndef _EXPR_OPERAND_H_
#define _EXPR_OPERAND_H_

#include <any>
#include <optional>
#include <unordered_map>
#include <variant>
#include <vector>

#include "types.h"

namespace dingodb::expr {

/*
 *操作数对象定义。
 */
class Operand {
 public:
  /*
   *模版函数，指定了Operand对象中存储的值的真实类型。
   */
  template <typename T>
  Operand(T v) : m_data(v) {
  }

  /*
   * 通过string对象构造Operand。
   */
  Operand(String::ValueType v) : m_data(String(v)) {
  }

  /*
   * 通过空指针构造Operand对象。
   */
  Operand([[maybe_unused]] std::nullptr_t v) {
  }

  /*
   *显示声明类的默认函数。
   */
  Operand() = default;

  /*
   *判断两个操作数是否相等。
   */
  bool operator==(const Operand &v) const {
    return m_data == v.m_data;
  }

  /*
   * 判断是否为空指针。
   * C++17 引入了 [[maybe_unused]] 属性，它允许我们告诉编译器可以接受未使用的变量。编译器不会为此类变量生成未使用的变量警告。
   * std::holds_alternative -- 作用是返回是否持有T类型的值。
   * std::monostate 辅助类型std::monostate提供了处理这种情况的能力，还可以用来模拟空值的状态。
   */
  bool operator==([[maybe_unused]] std::nullptr_t v) const {
    return std::holds_alternative<std::monostate>(m_data);
  }

  /*
   *判断两个操作数不相等。
   */
  bool operator!=(const Operand &v) const {
    return m_data != v.m_data;
  }

  /*
   * 判断是否为空指针。
   */
  bool operator!=([[maybe_unused]] std::nullptr_t v) const {
    return !std::holds_alternative<std::monostate>(m_data);
  }

  /*
   * 获得Operand对象中存储的值。
   */
  template <typename T>
  inline T GetValue() const {
    return std::get<T>(m_data);
  }

 private:
  /*
   * m_data是variant类型，即几个类型的union。
   * std::variant 是C++17引入的一个新的标准库类型，它提供了一种类型安全的联合体。这个类可以在同一时间持有几种可能类型中的一个值。本文将详细介绍 std::variant 的概念、用法和实现细节。
   */
  std::variant<
      std::monostate,
      int32_t,
      int64_t,
      bool,
      float,
      double,
      String,
      std::shared_ptr<std::vector<int32_t>>,
      std::shared_ptr<std::vector<int64_t>>,
      std::shared_ptr<std::vector<bool>>,
      std::shared_ptr<std::vector<float>>,
      std::shared_ptr<std::vector<double>>,
      std::shared_ptr<std::vector<std::string>>>
      m_data;

  /*
   * hash函数是Operand类的友元。
   */
  friend class std::hash<::dingodb::expr::Operand>;

  /*
   * 定义了Operand的左移操作。
   */
  friend std::ostream &operator<<(std::ostream &os, const Operand &v);
};

/*
 * 定义了Tuple类型。
 * Tuple的定义，一个tuple就是一组操作数的集合。
 */
using Tuple = std::vector<Operand>;

namespace any_optional_data_adaptor {

/*
 * 各种数据类型的值的对象转换为Operand对象。
 */
template <typename T>
Operand ToOperand(const std::any &v) {
  const auto opt = std::any_cast<const std::optional<T>>(v);
  if (opt.has_value()) {
    return *opt;
  }
  return nullptr;
}

/*
 * 从Operand对象转换为对应的类型。
 */
template <typename T>
std::any FromOperand(const Operand &v) {
  auto opt = (v != nullptr ? std::optional<T>(v.GetValue<T>()) : std::optional<T>());
  return std::make_any<std::optional<T>>(opt);
}

// GCC does not allow template specialization in class, so we need this.
/*
 * 从string对象转换为Operand类型。
 */
template <>
std::any FromOperand<String::ValueType>(const Operand &v);

}  // namespace any_optional_data_adaptor

}  // namespace dingodb::expr

namespace std {

/*
 * 定义了Operand的hash函数，实际是使用了其内部存储值的数据类型进行hash。
 */
template <>
struct hash<::dingodb::expr::Operand> {
  size_t operator()(const ::dingodb::expr::Operand &val) const noexcept {
    return hash<decltype(val.m_data)>()(val.m_data);
  }
};

/*
 * 定义了Tuple的hash函数。
 */
template <>
struct hash<::dingodb::expr::Tuple> {
  size_t operator()(const ::dingodb::expr::Tuple &val) const noexcept {
    size_t h = 0LL;
    for (const auto &v : val) {
      h *= 31LL;
      h += hash<::dingodb::expr::Operand>()(v);
    }
    return h;
  }
};

}  // namespace std

#endif /* _EXPR_OPERAND_H_ */
