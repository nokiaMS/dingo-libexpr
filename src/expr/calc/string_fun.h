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

#ifndef _EXPR_CALC_STRING_FUN_H_
#define _EXPR_CALC_STRING_FUN_H_

#include "../types.h"

namespace dingodb::expr::calc {

/*
 *实现了一些字符串函数。
 */
//实现了concat.
String Concat(String v0, String v1);

//实现了Lower.
String Lower(String v);

//实现了Upper
String Upper(String v);

//实现了Left
String Left(String v0, int32_t v1);

//实现了Right
String Right(String v0, int32_t v1);

//实现了Trim
String Trim(String v);

//实现了LTrim
String LTrim(String v);

//实现了RTrim
String RTrim(String v);

//实现了Substr
String Substr(String v0, int32_t v1, int32_t v2);

//实现了Substr
String Substr(String v0, int32_t v1);

//实现了Mid
String Mid(String v0, int32_t v1, int32_t v2);

//实现了Mid
String Mid(String v0, int32_t v1);

}  // namespace dingodb::expr::calc

#endif /* _EXPR_CALC_STRING_FUN_H_ */
