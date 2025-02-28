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

#include <gtest/gtest.h>

#include <memory>
#include <tuple>

#include "codec.h"
#include "runner.h"
#include "decimal_p.h"

using namespace dingodb::expr;

class ExprDecimalTest : public testing::TestWithParam<std::tuple<std::string, Tuple *, std::string>> {};

TEST_P(ExprDecimalTest, Run) {
  const auto &para = GetParam();
  Runner runner;
  auto input = std::get<0>(para);
  auto len = input.size() / 2;
  Byte buf[len];
  HexToBytes(buf, input.data(), input.size());
  runner.Decode(buf, len);
  runner.BindTuple(std::get<1>(para));
  runner.Run();

  auto r = runner.Get();
  auto r1 = any_optional_data_adaptor::FromOperand<DecimalP>(r);
  auto r2 = (*std::any_cast<std::optional<DecimalP>>(r1))->toString();

  EXPECT_EQ(r2, std::get<2>(para));
}

static Tuple tupleDec1{std::make_shared<Decimal>(Decimal("123.123")), std::make_shared<Decimal>(Decimal("456.456"))};

// Test cases with vars
INSTANTIATE_TEST_SUITE_P(
    VarDecExpr,
    ExprDecimalTest,
    testing::Values(
        /**
         * VAR | DECIMAL | The 0 index.
         */
        std::make_tuple("3600", &tupleDec1, std::string("123.123")),

        /**
         * VAR | DECIMAL | The 1st index.
         */
        std::make_tuple("3601", &tupleDec1, std::string("456.456")),

        /**
         *  (VAR |DECIMAL) + (VAR | DECIMAL).
         */
        std::make_tuple("360036018306", &tupleDec1, std::string("579.579")),

        /**
         * CONST | DECIMAL, POS.
         */
        std::make_tuple("16073132332E313233", nullptr, "123.123"),

        /**
         * CONST | DECIMAL, NEG.
         */
        std::make_tuple("16082D3132332E313233", nullptr, "-123.123")
    ));
