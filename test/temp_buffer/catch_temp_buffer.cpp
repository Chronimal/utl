//
// Test harnesses for TempBuffer<L>
//
// Copyright(c) 2020 Chronimal. All rights reserved.
//
// Licensed under the MIT license; A copy of the license that can be
// found in the LICENSE file.
//

#define CATCH_CONFIG_MAIN

#include "temp_buffer.hpp"
#include "catch.hpp"

using namespace TB_NAMESPACE_NAME;

TEST_CASE("utl.temp_buffer. Stack only allocation")
{
    SECTION("Default constructed TempBuffer")
    {
        TempBuffer<100> buffer;

        REQUIRE(buffer.empty());
        REQUIRE(buffer.size() == 0);
        REQUIRE_FALSE(buffer.dynamic());
    }

    SECTION("Allocate a 100 bytes TempBuffer")
    {
        TempBuffer<100> buffer{100};

        REQUIRE_FALSE(buffer.empty());
        REQUIRE(buffer.size() == 100);
        REQUIRE_FALSE(buffer.dynamic());
    }

    SECTION("Allocate a 100 bytes TempBuffer on the stack filled with 0xff bytes")
    {
        constexpr int pattern{0xc0};
        constexpr std::size_t size = 100;
        TempBuffer<size> buffer{size, pattern};

        REQUIRE_FALSE(buffer.empty());
        REQUIRE(buffer.size() == size);
        REQUIRE_FALSE(buffer.dynamic());

        auto p = buffer.as<std::byte const>();
        REQUIRE(std::all_of(p, p + size, [&](auto v) { return v == static_cast<std::byte>(pattern); }));
    }
}

TEST_CASE("utl.temp_buffer. Heap only allocation")
{
    SECTION("Allocate 100 bytes on the heap")
    {
        TempBuffer<1> buffer{100};

        REQUIRE_FALSE(buffer.empty());
        REQUIRE(buffer.size() == 100);
        REQUIRE(buffer.dynamic());
    }

    SECTION("Allocate a 100 bytes TempBuffer on the heap filled with 0xff bytes")
    {
        constexpr int pattern{0xc0};
        constexpr std::size_t size = 100;
        TempBuffer<size - 1> buffer{size, pattern};

        REQUIRE_FALSE(buffer.empty());
        REQUIRE(buffer.size() == size);
        REQUIRE(buffer.dynamic());

        auto p = buffer.as<std::byte const>();
        REQUIRE(std::all_of(p, p + size, [&](auto v) { return v == static_cast<std::byte>(pattern); }));
    }
}

TEST_CASE("utl.temp_buffer. Resizing")
{
    constexpr int pattern{0xc0};
    constexpr std::size_t size = 100;
    TempBuffer<size - 1> buffer;

    REQUIRE(buffer.empty());
    REQUIRE(buffer.size() == 0);
    REQUIRE_FALSE(buffer.dynamic());

    buffer.resize(size);

    REQUIRE_FALSE(buffer.empty());
    REQUIRE(buffer.size() == size);
    REQUIRE(buffer.dynamic());

    auto p = buffer.as<std::byte>();
    std::fill_n(p, size, static_cast<std::byte>(pattern));
    REQUIRE(std::all_of(p, p + size, [&](auto v) { return v == static_cast<std::byte>(pattern); }));

    constexpr std::size_t halfSize = size / 2;
    buffer.resize(halfSize);

    REQUIRE_FALSE(buffer.empty());
    REQUIRE(buffer.size() == halfSize);
    REQUIRE_FALSE(buffer.dynamic());

    p = buffer.as<std::byte>();
    std::fill_n(p, halfSize, static_cast<std::byte>(pattern - 1));
    REQUIRE(std::all_of(p, p + halfSize, [&](auto v) { return v == static_cast<std::byte>(pattern - 1); }));

    constexpr std::size_t doubleSize = size * 2;
    buffer.resize(doubleSize);

    REQUIRE_FALSE(buffer.empty());
    REQUIRE(buffer.size() == doubleSize);
    REQUIRE(buffer.dynamic());

    p = buffer.as<std::byte>();
    std::fill_n(p, doubleSize, static_cast<std::byte>(pattern + 1));
    REQUIRE(std::all_of(p, p + doubleSize, [&](auto v) { return v == static_cast<std::byte>(pattern + 1); }));
}