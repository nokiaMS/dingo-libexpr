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

#include <sstream>
#include <string>
#include <stdexcept>
#include <iostream>
#include <sstream>

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
  Decimal(std::string var) {
    try {
       v = mpf_class(var, maxPrecision, base);
    } catch (const std::invalid_argument& e){
        std::cout << "Decimal error: " << e.what() << std::endl;
        throw e;
    }
  }

  /**
   * constructor.
   * @param var
   */
  Decimal(long var) {
    Decimal(std::to_string(var));
  }

  /**
   * constructor.
   * @param var
   */
  Decimal(double var) {
    Decimal(std::to_string(var));
  }

  /**
   * copy construcotr.
   * @param dec
   */
  Decimal(const Decimal& dec) {
    v = dec.getMpf();
  }

  /**
   * for copy construct operator.
   * @param dec
   * @return
   */
  Decimal& operator =(const Decimal& dec) {
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
   * constructor.
   * @param mp
   */
  Decimal(mpf_class& mp) : v(mp) {
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
   * trans decimal to int.
   * @return
   */
  int toInt() {
    return (int)v.get_si();
  }

  /**
   * trans decimal to unsigned int.
   * @return
   */
  int toUInt() {
    return (int)v.get_ui();
  }

  /**
   * trans decimal to double.
   * @return
   */
  double toDouble() {
    return v.get_d();
  }

  /**
   * trans decimal to string.
   * @return
   */
  std::string toString() const {
    mp_exp_t exp;
    char *str = nullptr;
    std::string result;

    str = mpf_get_str(nullptr, &exp, base, 0, v.get_mpf_t());
    std::cout << "Decimal toString - str: " << str << " expr: " << exp << std::endl;
    std::string src = std::string(str);

    if(str) {
      if(src.length() == 0) {
        result.push_back('0');
        return result;
      }

      //has flag.
      bool hasFlag = (str[0] == '-');
      int curPos = 0;

      if(hasFlag) {
        result.push_back('-');
        curPos++;
      }

      //expr
      if(exp == 0){
        result.push_back('0');
        result.push_back('.');
        result.append(src, curPos);
      }
      else {
        result.append(src, curPos, exp);
        result.push_back('.');
        result.append(src, curPos + exp);
      }
    }
    return result;
  }

  /**
   * decimal + decimal.
   * @param dec
   * @return
   */
  Decimal operator+(const Decimal &dec) const {
      mpf_class ret = v + dec.getMpf();
      return std::move(Decimal().swap(ret));
  }

  /**
   * decimal - decimal.
   * @param dec
   * @return
   */
  Decimal operator-(const Decimal &dec) const {
    mpf_class ret = v - dec.getMpf();
    return std::move(Decimal().swap(ret));
  }

  /**
   * decimal * decimal.
   * @param dec
   * @return
   */
  Decimal operator*(const Decimal &dec) const {
    mpf_class ret = v * dec.getMpf();
    return std::move(Decimal().swap(ret));
  }

  /**
   * decimal / decimal.
   * @param dec
   * @return
   */
  Decimal operator/(const Decimal &dec) const {
    mpf_class ret = v / dec.getMpf();
    return std::move(Decimal().swap(ret));
  }

    /**
   * decimal == decimal.
   * @param dec
   * @return
   */
    bool operator==(const Decimal &dec) const {
        return v == dec.getMpf();
    }

    /**
     * decimal != decimal.
     * @param dec
     * @return
     */
    bool operator!=(const Decimal &dec) const {
        return v != dec.getMpf();
    }

    /**
     * decimal < decimal.
     * @param dec
     * @return
     */
    bool operator<(const Decimal &dec) const {
        return v < dec.getMpf();
    }

    /**
     * decimal <= decimal.
     * @param dec
     * @return
     */
    bool operator<=(const Decimal &dec) const {
        return v <= dec.getMpf();
    }

    /**
     * decimal > decimal.
     * @param dec
     * @return
     */
    bool operator>(const Decimal &dec) const {
        return v > dec.getMpf();
    }

    /**
     * decimal >= decimal.
     * @param dec
     * @return
     */
    bool operator>=(const Decimal &dec) const {
        return v >= dec.getMpf();
    }

    /**
     * -decimal.
     * @return
     */
    Decimal operator-() const {
      mp_exp_t exp;
      char *str = nullptr;
      std::string result;

      str = mpf_get_str(nullptr, &exp, base, 0, v.get_mpf_t());
      std::cout << "Decimal toString - str: " << str << " expr: " << exp << std::endl;
      std::string src = std::string(str);

      if(str) {
        if(src.length() == 0) {
          result.push_back('0');
          return result;
        }

        //has flag.
        bool hasFlag = (str[0] == '-');
        int curPos = 0;

        if(!hasFlag) {
          //The origin number must be pos, so we add '-' here for '-' operator.
          result.push_back('-');
        }

        //expr
        if(exp == 0){
          result.push_back('0');
          result.push_back('.');
          result.append(src, curPos);
        }
        else {
          result.append(src, curPos, exp);
          result.push_back('.');
          result.append(src, curPos + exp);
        }
      }
      return result;
    }

    /**
     * abs.
     * @param dec
     */
    Decimal Abs() const {
      mpf_class ret = abs(v);
      return std::move(Decimal().swap(ret));
    }

 private:
  mpf_class v;

  /**
   * swapper.
   * @param mp
   */
  Decimal& swap(mpf_class & mp) {
      v.swap(mp);
      return *this;
  }

  /**
   * Get the internal mpf object.
   * @return
   */
  mpf_class getMpf() const {
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
