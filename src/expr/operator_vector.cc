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

#include "operator_vector.h"

#include "codec.h"
#include "exception.h"
#include "operators.h"
#include "types.h"
#include "utils.h"

namespace dingodb::expr {

static const Byte NULL_PREFIX  = 0x00;
static const Byte NULL_INT32   = NULL_PREFIX | TYPE_INT32;
static const Byte NULL_INT64   = NULL_PREFIX | TYPE_INT64;
static const Byte NULL_BOOL    = NULL_PREFIX | TYPE_BOOL;
static const Byte NULL_FLOAT   = NULL_PREFIX | TYPE_FLOAT;
static const Byte NULL_DOUBLE  = NULL_PREFIX | TYPE_DOUBLE;
static const Byte NULL_DECIMAL = NULL_PREFIX | TYPE_DECIMAL;
static const Byte NULL_STRING  = NULL_PREFIX | TYPE_STRING;

static const Byte CONST_PREFIX  = 0x10;
static const Byte CONST_INT32   = CONST_PREFIX | TYPE_INT32;
static const Byte CONST_INT64   = CONST_PREFIX | TYPE_INT64;
static const Byte CONST_BOOL    = CONST_PREFIX | TYPE_BOOL;
static const Byte CONST_FLOAT   = CONST_PREFIX | TYPE_FLOAT;
static const Byte CONST_DOUBLE  = CONST_PREFIX | TYPE_DOUBLE;
static const Byte CONST_DECIMAL = CONST_PREFIX | TYPE_DECIMAL;
static const Byte CONST_STRING  = CONST_PREFIX | TYPE_STRING;

static const Byte CONST_N_PREFIX = 0x20;
static const Byte CONST_N_INT32  = CONST_N_PREFIX | TYPE_INT32;
static const Byte CONST_N_INT64  = CONST_N_PREFIX | TYPE_INT64;
static const Byte CONST_N_BOOL   = CONST_N_PREFIX | TYPE_BOOL;

static const Byte VAR_I_PREFIX  = 0x30;
static const Byte VAR_I_INT32   = VAR_I_PREFIX | TYPE_INT32;
static const Byte VAR_I_INT64   = VAR_I_PREFIX | TYPE_INT64;
static const Byte VAR_I_BOOL    = VAR_I_PREFIX | TYPE_BOOL;
static const Byte VAR_I_FLOAT   = VAR_I_PREFIX | TYPE_FLOAT;
static const Byte VAR_I_DOUBLE  = VAR_I_PREFIX | TYPE_DOUBLE;
static const Byte VAR_I_DECIMAL = VAR_I_PREFIX | TYPE_DECIMAL;
static const Byte VAR_I_STRING  = VAR_I_PREFIX | TYPE_STRING;

/*
 * 定义了pos, neg, add, sub, mul, div, mod操作的类型值。
 */
static const Byte POS = 0x81;
static const Byte NEG = 0x82;
static const Byte ADD = 0x83;
static const Byte SUB = 0x84;
static const Byte MUL = 0x85;
static const Byte DIV = 0x86;
static const Byte MOD = 0x87;

/*
 * 定义了eq, ge, gt, le, lt, ne操作的类型值。
 */
static const Byte EQ = 0x91;
static const Byte GE = 0x92;
static const Byte GT = 0x93;
static const Byte LE = 0x94;
static const Byte LT = 0x95;
static const Byte NE = 0x96;

/*
 * 定义了is_null, is_true, is_false操作的类型值。
 */
static const Byte IS_NULL  = 0xA1;
static const Byte IS_TRUE  = 0xA2;
static const Byte IS_FALSE = 0xA3;

/*
 * 定义了min, max, abs, abs_c操作的类型值。
 */
static const Byte MIN   = 0xB1;
static const Byte MAX   = 0xB2;
static const Byte ABS   = 0xB3;
static const Byte ABS_C = 0xB4;

/*
 * 定义了note, and, or的类型值。
 */
static const Byte NOT = 0x51;
static const Byte AND = 0x52;
static const Byte OR  = 0x53;

/*
 * 定义了cast, cast_c, fun操作的类型值。
 */
static const Byte CAST   = 0xF0;
static const Byte CAST_C = 0xFC;
static const Byte FUN    = 0xF1;

/*
 * 定义了eoe的操作类型值。
 */
static const Byte EOE = 0x00;

/*
 * 给定一个字节序列，然后进行解码。
 */
const Byte *OperatorVector::Decode(const Byte code[], size_t len) {
  //解析前首先清理内存存储结构，以处理新的表达式编码数据。
  Release();

  bool successful = true;
  const Byte *p   = code;
  const Byte *b;

  //执行操作符解析操作。
  while (successful && p < code + len) {
    b = p;
    switch (*p) {
    case NULL_INT32:
      ++p;
      Add(OP_NULL[TYPE_INT32]);
      break;
    case NULL_INT64:
      ++p;
      Add(OP_NULL[TYPE_INT64]);
      break;
    case NULL_BOOL:
      ++p;
      Add(OP_NULL[TYPE_BOOL]);
      break;
    case NULL_FLOAT:
      ++p;
      Add(OP_NULL[TYPE_FLOAT]);
      break;
    case NULL_DOUBLE:
      ++p;
      Add(OP_NULL[TYPE_DOUBLE]);
      break;
    case NULL_STRING:
      ++p;
      Add(OP_NULL[TYPE_STRING]);
      break;
    case CONST_INT32: {   //解析int32整型常量，生成ConstOperator操作符。
      ++p;
      int32_t v;
      //解析int32类型值。
      p = DecodeValue(v, p);
      AddRelease(new ConstOperator<TYPE_INT32>(v));
      break;
    }
    case CONST_INT64: {   //解析int64整型常量，生成ConstOperator操作符。
      ++p;
      int64_t v;
      p = DecodeValue(v, p);
      AddRelease(new ConstOperator<TYPE_INT64>(v));
      break;
    }
    case CONST_BOOL:    //解析bool常量，生成ConstBoolOperator操作符。
      ++p;
      Add(OP_CONST_TRUE);
      break;
    case CONST_FLOAT: {   //解析float常量，生成ConstOperator操作符。
      ++p;
      float v;
      p = DecodeValue(v, p);
      AddRelease(new ConstOperator<TYPE_FLOAT>(v));
      break;
    }
    case CONST_DOUBLE: {   //解析double常量，生成ConstOperator操作符。
      ++p;
      double v;
      p = DecodeValue(v, p);
      AddRelease(new ConstOperator<TYPE_DOUBLE>(v));
      break;
    }
    case CONST_DECIMAL: {   //解析double常量，生成ConstOperator操作符。
      ++p;
      // TODO
      break;
    }
    case CONST_STRING: {    //解析string常量，生成ConstOperator操作符。
      ++p;
      String v;
      p = DecodeValue(v, p);
      AddRelease(new ConstOperator<TYPE_STRING>(v));
      break;
    }
    case CONST_N_INT32: {    //解析int32常量，生成ConstOperator操作符。
      ++p;
      int32_t v;
      p = DecodeValue(v, p);
      AddRelease(new ConstOperator<TYPE_INT32>(-v));
      break;
    }
    case CONST_N_INT64: {    //解析int64常量，生成ConstOperator操作符。
      ++p;
      int64_t v;
      p = DecodeValue(v, p);
      AddRelease(new ConstOperator<TYPE_INT64>(-v));
      break;
    }
    case CONST_N_BOOL:    //解析bool常量，生成ConstOperator操作符。
      ++p;
      Add(OP_CONST_FALSE);
      break;
    case VAR_I_INT32: {
      ++p;
      int32_t v;
      p = DecodeValue(v, p);
      AddRelease(new IndexedVarOperator<TYPE_INT32>(v));
      break;
    }
    case VAR_I_INT64: {
      ++p;
      int32_t v;
      p = DecodeValue(v, p);
      AddRelease(new IndexedVarOperator<TYPE_INT64>(v));
      break;
    }
    case VAR_I_BOOL: {
      ++p;
      int32_t v;
      p = DecodeValue(v, p);
      AddRelease(new IndexedVarOperator<TYPE_BOOL>(v));
      break;
    }
    case VAR_I_FLOAT: {
      ++p;
      int32_t v;
      p = DecodeValue(v, p);
      AddRelease(new IndexedVarOperator<TYPE_FLOAT>(v));
      break;
    }
    case VAR_I_DOUBLE: {
      ++p;
      int32_t v;
      p = DecodeValue(v, p);
      AddRelease(new IndexedVarOperator<TYPE_DOUBLE>(v));
      break;
    }
    case VAR_I_DECIMAL: {
      ++p;
      int32_t v;
      p = DecodeValue(v, p);
      AddRelease(new IndexedVarOperator<TYPE_DECIMAL>(v));
      break;
    }
    case VAR_I_STRING: {
      ++p;
      int32_t v;
      p = DecodeValue(v, p);
      AddRelease(new IndexedVarOperator<TYPE_STRING>(v));
      break;
    }
    case POS:
      ++p;
      successful = AddOperatorByType(OP_POS, *p);
      ++p;
      break;
    case NEG:
      ++p;
      successful = AddOperatorByType(OP_NEG, *p);
      ++p;
      break;
    case ADD:
      ++p;
      successful = AddOperatorByType(OP_ADD, *p);
      ++p;
      break;
    case SUB:
      ++p;
      successful = AddOperatorByType(OP_SUB, *p);
      ++p;
      break;
    case MUL:
      ++p;
      successful = AddOperatorByType(OP_MUL, *p);
      ++p;
      break;
    case DIV:
      ++p;
      successful = AddOperatorByType(OP_DIV, *p);
      ++p;
      break;
    case MOD:
      ++p;
      successful = AddOperatorByType(OP_MOD, *p);
      ++p;
      break;
    case EQ:
      ++p;
      successful = AddOperatorByType(OP_EQ, *p);
      ++p;
      break;
    case GE:
      ++p;
      successful = AddOperatorByType(OP_GE, *p);
      ++p;
      break;
    case GT:
      ++p;
      successful = AddOperatorByType(OP_GT, *p);
      ++p;
      break;
    case LE:
      ++p;
      successful = AddOperatorByType(OP_LE, *p);
      ++p;
      break;
    case LT:
      ++p;
      successful = AddOperatorByType(OP_LT, *p);
      ++p;
      break;
    case NE:
      ++p;
      successful = AddOperatorByType(OP_NE, *p);
      ++p;
      break;
    case IS_NULL:
      ++p;
      successful = AddOperatorByType(OP_IS_NULL, *p);
      ++p;
      break;
    case IS_TRUE:
      ++p;
      successful = AddOperatorByType(OP_IS_TRUE, *p);
      ++p;
      break;
    case IS_FALSE:
      ++p;
      successful = AddOperatorByType(OP_IS_FALSE, *p);
      ++p;
      break;
    case MIN:   //解析min操作符。   （ type1: min操作编码 + type2: 参数类型）
      ++p;
      successful = AddOperatorByType(OP_MIN, *p);
      ++p;
      break;
    case MAX:   //解析max操作符。   （type1: max操作编码 + type2: 参数类型）
      ++p;
      successful = AddOperatorByType(OP_MAX, *p);
      ++p;
      break;
    case ABS:
      ++p;
      successful = AddOperatorByType(OP_ABS, *p);
      ++p;
      break;
    case ABS_C:
      ++p;
      successful = AddOperatorByType(OP_ABS_CHECK, *p);
      ++p;
      break;
    case NOT:
      Add(OP_NOT);
      ++p;
      break;
    case AND:
      Add(OP_AND);
      ++p;
      break;
    case OR:
      Add(OP_OR);
      ++p;
      break;
    case CAST:
      ++p;
      successful = AddCastOperator(OP_CAST, *p);
      ++p;
      break;
    case CAST_C:
      ++p;
      successful = AddCastOperator(OP_CAST_CHECK, *p);
      ++p;
      break;
    case FUN:
      ++p;
      successful = AddFunOperator(*p);
      ++p;
      break;
    case EOE:   //发现是eoe，表示指令结束。
      ++p;
      goto eoe;
    default:    //此处说明指令解码失败。
      successful = false;
      break;
    }
  }
eoe:    //处理eoe情况，如果成功返回当前指针位置，否则抛出异常。
  if (successful) {
    return p;
  }
  throw UnknownCode(b, len - (b - code));
}

/*
 * 通过类型添加操作符。
 */
bool OperatorVector::AddOperatorByType(const Operator *const ops[], Byte type) {
  const auto *op = ops[type];
  if (op != nullptr) {
    Add(op);
    return true;
  }
  return false;
}

bool OperatorVector::AddCastOperator(const Operator *const ops[][TYPE_NUM], Byte b) {
  Byte dst = (Byte)(b >> 4);
  Byte src = (Byte)(b & 0x0F);
  if (dst == src) {
    return true;
  }
  const auto *op = ops[dst][src];
  if (op != nullptr) {
    Add(op);
    return true;
  }
  return false;
}

bool OperatorVector::AddFunOperator(Byte b) {
  if (0 <= b && b < FUN_NUM) {
    const auto *op = OP_FUN[b];
    if (op != nullptr) {
      Add(op);
      return true;
    }
  }
  return false;
}

}  // namespace dingodb::expr
