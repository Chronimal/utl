//
// Test harnesses for RingBuffer<T, N>
//
// Copyright(c) 2020 Chronimal. All rights reserved.
//
// Licensed under the MIT license; A copy of the license that can be
// found in the LICENSE file.
//

#define CATCH_CONFIG_MAIN

#include "ring_buffer.hpp"
#include "catch.hpp"

#include <algorithm>
#include <numeric>
#include <array>

using namespace RB_NAMESPACE_NAME;

// Simple element class to aid debugging
struct Element
{
    unsigned int val;
    Element(unsigned int val)
        : val{val}
    {
    }

    ~Element()
    {
        val = 0xDEADDEAD;
    }
};

TEST_CASE("utl.ring_buffer. Capacity == 1")
{
    RingBuffer<Element, 1> rb;

    REQUIRE(rb.capacity() == 1);
    REQUIRE(rb.size() == 0);
    REQUIRE(rb.empty());
    REQUIRE(!rb.full());

    SECTION("Push repeatedly a new element into the ring buffer")
    {
        for (auto val : {1, 2, 3, 4, 5, 6, 7, 8, 9, 10})
        {
            rb.push(val);

            REQUIRE(rb.capacity() == 1);
            REQUIRE(rb.size() == 1);
            REQUIRE(!rb.empty());
            REQUIRE(rb.full());

            REQUIRE(rb.front().val == val);
            REQUIRE(rb.back().val == val);
            REQUIRE(rb[0].val == val);
        }
    }

    SECTION("Push and pop repeatedly")
    {
        for (auto val : {1, 2, 3, 4, 5, 6, 7, 8, 9, 10})
        {
            rb.push(val);

            REQUIRE(rb.capacity() == 1);
            REQUIRE(rb.size() == 1);
            REQUIRE(!rb.empty());
            REQUIRE(rb.full());

            rb.pop();

            REQUIRE(rb.capacity() == 1);
            REQUIRE(rb.size() == 0);
            REQUIRE(rb.empty());
            REQUIRE(!rb.full());
        }
    }

    rb.clear();

    REQUIRE(rb.capacity() == 1);
    REQUIRE(rb.size() == 0);
    REQUIRE(rb.empty());
    REQUIRE(!rb.full());
}

TEST_CASE("utl.ring_buffer. Capacity == 10")
{
    constexpr int values[]{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    RingBuffer<Element, 10> rb;

    REQUIRE(rb.capacity() == std::size(values));
    REQUIRE(rb.size() == 0);
    REQUIRE(rb.empty());
    REQUIRE(!rb.full());

    SECTION("Push values into the ring buffer until full")
    {
        // Fill the ring buffer with values
        for (std::size_t i = 0; i < std::size(values); ++i)
        {
            rb.push(values[i]);

            REQUIRE(rb.capacity() == 10);
            REQUIRE(rb.size() == i + 1);
            REQUIRE(!rb.empty());

            REQUIRE(rb.front().val == values[0]);
            REQUIRE(rb.back().val == values[i]);
            REQUIRE(rb[i].val == values[i]);
        }
        // Validate the values in the ring buffer
        for (std::size_t i = 0; i < std::size(values); ++i)
        {
            REQUIRE(rb[i].val == values[i]);
        }
        REQUIRE(rb.size() == std::size(values));

        // Push one more value. This should cause the ring buffer to drop the oldest value
        constexpr int val = 1000;
        rb.push(val);

        REQUIRE(rb.size() == std::size(values));

        for (std::size_t i = 1; i < std::size(values); ++i)
        {
            REQUIRE(rb[i - 1].val == values[i]);
        }

        REQUIRE(rb.back().val == val);
    }

    rb.clear();

    REQUIRE(rb.capacity() == std::size(values));
    REQUIRE(rb.size() == 0);
    REQUIRE(rb.empty());
    REQUIRE(!rb.full());
}

TEST_CASE("utl.ring_buffer. Iteration")
{
    int const values[]{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    RingBuffer<Element, 10> rb;

    REQUIRE(rb.capacity() == std::size(values));
    REQUIRE(rb.size() == 0);
    REQUIRE(rb.empty());
    REQUIRE(!rb.full());

    for (auto val : values)
    {
        rb.push(val);
    }
    REQUIRE(rb.size() == std::size(values));
    std::size_t i = 0;

    SECTION("Check values using range-based for")
    {        
        for (auto const& val : rb)
        {
            REQUIRE(val.val == values[i++]);
        }
    }

    SECTION("Check values using forward iteration")
    {
        // clang-format off
        std::for_each(std::begin(rb), std::end(rb), [&](auto const& val)
        {
            REQUIRE(val.val == values[i++]);
        });
        // clang-format on
    }

    SECTION("Check values using reverse iteration")
    {
        // clang-format off
        std::for_each(std::rbegin(rb), std::rend(rb), [&](auto const& val)
        {
            REQUIRE(val.val == values[std::size(values) - 1 - i]);
            ++i;
        });
        // clang-format on
    }

    SECTION("Random access")
    {
        std::array<int, std::size(values)> deck;
        std::iota(std::begin(deck), std::end(deck), 0);
        std::shuffle(std::begin(deck), std::end(deck), std::default_random_engine{});

        for (auto index : deck)
        {
            REQUIRE(rb[index].val == (index + 1));
        }

        auto it = rb.begin();
        for (auto index : deck)
        {
            REQUIRE(it[index].val == (index + 1));
        }
    }
}