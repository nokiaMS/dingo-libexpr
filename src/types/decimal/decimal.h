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

#ifndef DINGO_LIBEXPR_DECIMAL_H
#define DINGO_LIBEXPR_DECIMAL_H

#include <string>
#include "gmpxx.h"

namespace dingodb {
namespace types {

/**
 * Max precision for c++ decimal in expr.
 */
constexpr int maxPrecision = 128;
constexpr int base = 10;

/**
 * class for Decimal.
 */
class Decimal {
 public:
  /**
   * default constructor.
   */
  Decimal(): v(maxPrecision) {
  }

  /**
   * constructor.
   * @param var The value presented by string.
   */
  Decimal(std::string var) : v(var, maxPrecision, base) {
  }

  Decimal(Decimal& dec) {
    v = dec.getMpf();
  }

  /**
   * for copy operator.
   * @param dec
   * @return
   */
  Decimal& operator =(Decimal& dec) {
    if(this == &dec) {
      return *this;
    }

    v = dec.getMpf();
  }

  /**
   * move constructor.
   * @param dec
   */
  Decimal(Decimal&& dec) {
    v = dec.moveMpf();
  }

  /**
   * Get the precision.
   * @return
   */
  int getPrecision() {
    return v.get_prec();
  }

  /**
   * trans decimal to long.
   * @return
   */
  long toLong() {
    return v.get_si();
  }

  /**
   * trans decimal to unsigned long.
   * @return
   */
  unsigned long toULong() {
    //return mpf_get_ui(v);
    return v.get_ui();
  }

  /**
   * trans decimal to double.
   * @return
   */
  double toDouble() {
    return v.get_d();
  }

 private:
  mpf_class v;

  /**
   * Get the internal mpf object.
   * @return
   */
  mpf_class getMpf() {
    return v;
  }

  /**
   * move the internal mpf object.
   * @return
   */
  mpf_class moveMpf() {
    return std::move(v);
  }
};

}  // namespace types
}  // namespace dingodb

#endif  // DINGO_LIBEXPR_DECIMAL_H
