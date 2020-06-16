//
// Test harnesses for UniqueBuffer
//
// Copyright(c) 2020 Chronimal. All rights reserved.
//
// Licensed under the MIT license; A copy of the license that can be
// found in the LICENSE file.
//

#define CATCH_CONFIG_MAIN

#include "unique_buffer.hpp"
#include "catch.hpp"

using namespace UB_NAMESPACE_NAME;

TEST_CASE("utl.unique_buffer. Standard move")
{
    static const int pattern = 666;
    static const std::size_t count = 10;
    UniqueBuffer ub{sizeof(int) * count};
    REQUIRE(ub.size() == (sizeof(int) * count));

    std::fill_n(ub.as<int>(), count, pattern);
    std::for_each(ub.as<int>(), ub.as<int>() + count, [&](auto value) { REQUIRE(value == pattern); });

    SECTION("Move construction")
    {
        UniqueBuffer ub2{std::move(ub)};
        REQUIRE(ub.size() == 0);
        REQUIRE(ub2.size() == (sizeof(int) * count));
        std::for_each(ub2.as<int>(), ub2.as<int>() + count, [&](auto value) { REQUIRE((value == pattern)); });
    }

    SECTION("Move assignment")
    {
        UniqueBuffer ub2;
        ub2 = {std::move(ub)};
        REQUIRE(ub.size() == 0);
        REQUIRE(ub2.size() == (sizeof(int) * count));
        std::for_each(ub2.as<int>(), ub2.as<int>() + count, [&](auto value) { REQUIRE((value == pattern)); });
    }
}