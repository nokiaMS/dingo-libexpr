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

#include "casting.h"

#include <cmath>
#include <iomanip>
#include <sstream>

#include "../exception.h"

namespace dingodb::expr::calc {

#define MAX_SINGLEDOUBLE_LENGTH 63
#define MAX_TMP_SPACE 70
#define MAX_DOUBLE_SIGNIFICANT_FIGURES 16

#define C_ZERO 0x30
#define C_ONE 0x31
#define C_NINE 0x39
#define C_SPACE 0x20
#define C_COMMA 0x2C
#define C_POINT 0x2E
#define C_MINUS 0x2D
#define C_POUND 0x23

const bool FLAGS_accurate_double_enable = true;

typedef struct SingleNumInfo {
  bool signedFlag;
  bool pointFlag;
  int32_t startPos;
  int32_t endPos;
} SingleNumInfo;

template <>
int32_t Cast(float v) {
  return lround(v);
}

template <>
int32_t Cast(double v) {
  return lround(v);
}

template <>
int32_t Cast(String v) {
  try {
    return std::stoi(*v);
  } catch (const std::invalid_argument& e) {
    return 0;
  }
}

template <>
int32_t Cast(DecimalP v) {
    return v.toInt();
}

template <>
int64_t Cast(float v) {
  return llround(v);
}

template <>
int64_t Cast(double v) {
  return llround(v);
}

template <>
int64_t Cast(String v) {
  try {
    return std::stoll(*v);
  } catch (const std::invalid_argument& e) {
    return 0;
  }
}

template <>
int64_t Cast(DecimalP v) {
    return v.toLong();
}

template <>
float Cast(String v) {
  try {
    return std::stof(*v);
  } catch (const std::invalid_argument& e) {
    return 0;
  }
}

template <>
float Cast(DecimalP v) {
    return (float)v.toDouble();
}

template <>
double Cast(String v) {
  try {
    return std::stod(*v);
  } catch (const std::invalid_argument& e) {
    return 0;
  }
}

template <>
double Cast(DecimalP v) {
    return Cast<double>(String(v->toString()));
}

template <>
bool Cast(DecimalP v) {
  return llround(v.toDouble());
}

template <>
String Cast(int32_t v) {
  return std::to_string(v);
}

template <>
String Cast(int64_t v) {
  return std::to_string(v);
}

template <>
String Cast(bool v) {
  return v ? "true" : "false";
}

template <typename T>
static String CastF(T v) {
  // precision is 6 by default
  // auto s = std::to_string(v);
  std::stringstream ss;
  ss << std::fixed << std::setprecision(15) << v;
  auto s = ss.str();
  s.erase(s.find_last_not_of('0') + 1);
  if (s.back() == '.') {
    s += '0';
  }
  return s;
}

static String CastFloat(float v) {
  // precision is 6 by default
  // auto s = std::to_string(v);
  std::stringstream ss;
  ss << std::fixed << std::setprecision(6) << v;
  auto s = ss.str();
  s.erase(s.find_last_not_of('0') + 1);
  if (s.back() == '.') {
    s += '0';
  }
  return s;
}

static String CastDouble(double v) {
  std::stringstream ss;
  ss << std::fixed << std::setprecision(14) << v;
  auto s = ss.str();
  s.erase(s.find_last_not_of('0') + 1);
  if (s.back() == '.') {
    s += '0';
  }
  return s;
}

template <>
String Cast(float v) {
  return CastF(v);
}

template <>
String Cast(double v) {
  return CastF(v);
}

template <>
String Cast(DecimalP v) {
  return v->toString();
}

template <>
DecimalP Cast(int32_t v) {
  return DecimalP((long)v);
}

template <>
DecimalP Cast(int64_t v) {
  return DecimalP(v);
}

template <>
DecimalP Cast(bool v) {
  return v ? DecimalP(std::string("1")) : DecimalP(std::string("0"));
}

template <>
DecimalP Cast(float v) {
  char buffer[64];
  int len = castDoubleToString(v, buffer, sizeof(buffer), true);
  return DecimalP(std::string(buffer, len));
}

template <>
DecimalP Cast(double v) {
  char buffer[64];
  int len = castDoubleToString(v, buffer, sizeof(buffer), true);
  return DecimalP(std::string(buffer, len));
}

template <>
DecimalP Cast(String v) {
  return DecimalP(*v);
}

template <>
int32_t CastCheck(int64_t v) {
  auto r = Cast<int32_t>(v);
  if (r == v) {
    return r;
  }
  throw ExceedsLimits<TYPE_INT32>();
}

static inline bool ErrorAcceptable(float a, float b) {
  return fabsf(a - b) <= 0.5f;
}

static inline bool ErrorAcceptable(double a, double b) {
  return fabs(a - b) <= 0.5;
}

template <>
int32_t CastCheck(float v) {
  auto r = Cast<int32_t>(v);
  if (ErrorAcceptable(r, v)) {
    return r;
  }
  throw ExceedsLimits<TYPE_INT32>();
}

template <>
int32_t CastCheck(double v) {
  auto r = Cast<int32_t>(v);
  if (ErrorAcceptable(r, v)) {
    return r;
  }
  throw ExceedsLimits<TYPE_INT32>();
}

template <>
int32_t CastCheck(DecimalP v) {
  return v.toInt();
}

template <>
int64_t CastCheck(float v) {
  auto r = Cast<int64_t>(v);
  if (ErrorAcceptable(r, v)) {
    return r;
  }
  throw ExceedsLimits<TYPE_INT64>();
}

template <>
int64_t CastCheck(double v) {
  auto r = Cast<int64_t>(v);
  if (ErrorAcceptable(r, v)) {
    return r;
  }
  throw ExceedsLimits<TYPE_INT64>();
}

template <>
int64_t CastCheck(DecimalP v) {
  return v.toLong();
}

void singlePrecisionDeal(char *doubleStr, SingleNumInfo &singleNumInfo) {
  int32_t count = 0, idxDS = 0;
  bool startFlag = false;

  if (doubleStr[0] == C_ZERO) {
    singleNumInfo.startPos = 1;
  }

  if (doubleStr[0] == C_MINUS && doubleStr[1] == C_ZERO) {
    singleNumInfo.startPos = 2;
  }

  while (doubleStr[idxDS] != 0) {
    if (count < MAX_DOUBLE_SIGNIFICANT_FIGURES) {
      if (!startFlag && doubleStr[idxDS] >= C_ONE && doubleStr[idxDS] <= C_NINE) {
        startFlag = true;
      }
      if (startFlag && doubleStr[idxDS] != C_POINT) {
        count++;
      }
    } else if (doubleStr[idxDS] != C_POINT) {
      doubleStr[idxDS] = C_ZERO;
    }

    if (doubleStr[idxDS] == C_POINT) {
      singleNumInfo.pointFlag = true;
    }
    idxDS++;
  }
  singleNumInfo.endPos = idxDS;
}

void tailZeroDeal(char *doubleStr, SingleNumInfo &singleNumInfo) {
  if (singleNumInfo.pointFlag) {
    int32_t idxDS = singleNumInfo.endPos - 1;

    while (idxDS > 0 && (doubleStr[idxDS] == C_ZERO || doubleStr[idxDS] == C_POINT)) {
      if (doubleStr[idxDS] == C_POINT) {
        doubleStr[idxDS] = 0;
        idxDS--;
        break;
      }
      doubleStr[idxDS] = 0;
      idxDS--;
    }

    singleNumInfo.endPos = idxDS + 1;
    singleNumInfo.pointFlag = false;
    while (idxDS > 0) {
      if (doubleStr[idxDS] == C_POINT) {
        singleNumInfo.pointFlag = true;
      }
      idxDS--;
    }
  }
}

void headZeroDeal(char *doubleStr, SingleNumInfo &singleNumInfo, bool keepLeadingZero = false) {
  if (doubleStr[0] == C_MINUS && doubleStr[1] == C_ZERO && doubleStr[2] == 0) {
    doubleStr[0] = C_ZERO;
    doubleStr[1] = 0;
    return;
  }

  if (singleNumInfo.pointFlag && singleNumInfo.startPos > 0) {
    if (keepLeadingZero) {
      if (doubleStr[0] == C_MINUS && doubleStr[1] == C_ZERO && doubleStr[2] == C_POINT) {
        int32_t idxDS1 = 0;
        int32_t idxDS2 = 2;
        doubleStr[0] = C_ZERO;
        idxDS1++;
        while (idxDS2 <= singleNumInfo.endPos) {
          doubleStr[idxDS1] = doubleStr[idxDS2];
          idxDS1++;
          idxDS2++;
        }
        doubleStr[idxDS1] = 0;
      }
      return;
    }

    int32_t idxDS1 = singleNumInfo.signedFlag ? 1 : 0;
    int32_t idxDS2 = singleNumInfo.startPos;
    while (idxDS2 <= singleNumInfo.endPos) {
      doubleStr[idxDS1] = doubleStr[idxDS2];
      idxDS1++;
      idxDS2++;
    }
    doubleStr[idxDS1] = 0;
  }
}

void singleDoubleDeal(double number, char *doubleStr, int32_t dstLen, int32_t pPerc = 6, bool keepLeadingZero = true) {
  SingleNumInfo singleNumInfo = {false, false, 0, 0};
  singleNumInfo.signedFlag = number < 0 ? true : false;
  dstLen = dstLen < MAX_SINGLEDOUBLE_LENGTH ? dstLen : MAX_SINGLEDOUBLE_LENGTH;
  if (FLAGS_accurate_double_enable) {
    char numStr[MAX_TMP_SPACE];
    memset(numStr, 0, sizeof(numStr));
    int slen = snprintf(numStr, sizeof(numStr), "%+.*lf", pPerc, number);
    if (slen < 0) {
      throw std::runtime_error("snprintf error in singleDoubleDeal");
    }

    int32_t intLen = 0;
    int idx = (numStr[0] == '+' || numStr[0] == '-') ? 1 : 0;
    for (; numStr[idx] != 0 && numStr[idx] != '.'; idx++) {
      if (numStr[idx] >= '0' && numStr[idx] <= '9') {
        intLen++;
      }
    }

    if (intLen == 1 && numStr[(numStr[0] == '+' || numStr[0] == '-') ? 1 : 0] == '0') {
      intLen = 0;
    }

    int32_t tmpLen = (15 - intLen) > 0 ? (15 - intLen) : pPerc;
    tmpLen = pPerc < tmpLen ? pPerc : tmpLen;
    slen = snprintf(doubleStr, dstLen, "%.*lf", tmpLen, number);
    if (slen < 0) {
      throw std::runtime_error("snprintf error in singleDoubleDeal");
    }
  } else {
    int slen = snprintf(doubleStr, dstLen, "%.*lf", pPerc, number);
    if (slen < 0) {
      throw std::runtime_error("snprintf error in singleDoubleDeal");
    }
  }

  singlePrecisionDeal(doubleStr, singleNumInfo);
  tailZeroDeal(doubleStr, singleNumInfo);
  headZeroDeal(doubleStr, singleNumInfo, keepLeadingZero);
}

int32_t castDoubleToString(double value, char *dst, int32_t dstLen, bool keepLeadingZero) {
  if (dst == nullptr || dstLen <= 0) {
    throw std::invalid_argument("Destination buffer is null or size is invalid");
  }

  memset(dst, 0, dstLen);
  singleDoubleDeal(value, dst, dstLen, 15, keepLeadingZero);
  return strlen(dst);
}

}  // namespace dingodb::expr::calc
