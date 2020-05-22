//
// Test harnesses for ScopeGuard
//
// Copyright(c) 2020 Chronimal. All rights reserved.
//
// Licensed under the MIT license; A copy of the license that can be
// found in the LICENSE file.
//

#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "scope_guard.hpp"

using namespace SG_NAMESPACE_NAME;

TEST_CASE("utl.scope_guard. makeGuard/dismiss")
{
    int counter = 0;
    {
        auto guard = makeGuard([&] { ++counter; });
    }
    {
        auto guard = makeGuard([&] { ++counter; });
        guard.dismiss();
    }
    REQUIRE(counter == 1);
}

TEST_CASE("utl.scope_guard. SCOPE_EXIT")
{
    int counter = 0;
    {
        SCOPE_EXIT
        {
            ++counter;
        };
    }
    REQUIRE(counter == 1);
}

TEST_CASE("utl.scope_guard. SCOPE_FAIL")
{
    // clang-format off
    int counter = 0;
    auto doThrow = [&]()
    {
        SCOPE_FAIL{++counter;};
        throw std::exception{};
    };
    // clang-format on

    REQUIRE_THROWS_AS(doThrow(), std::exception);
    REQUIRE(counter == 1);
}

TEST_CASE("utl.scope_guard. SCOPE_SUCCESS")
{
    // clang-format off
    int counter = 0;
    auto doNotThrow = [&]()
    {
        SCOPE_SUCCESS{++counter;};
    };
    // clang-format on

    doNotThrow();
    REQUIRE(counter == 1);
}
