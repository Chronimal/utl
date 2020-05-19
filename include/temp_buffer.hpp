//
// Fixed size stack allocated temporary buffer with dynamic allocation fall back
//
// Copyright(c) 2020 Chronimal. All rights reserved.
//
// Licensed under the MIT license; A copy of the license that can be
// found in the LICENSE file.
//

#ifndef TB_TEMP_BUFFER_HPP_INCLUDED
#define TB_TEMP_BUFFER_HPP_INCLUDED

// Configure namespace preference for TempBuffer.
// By default namespace utl is used.
#define TB_NAMESPACE_NAME utl

// clang-format off
#ifndef TB_BEGIN_NAMESPACE
#define TB_BEGIN_NAMESPACE namespace TB_NAMESPACE_NAME {
#endif // TB_BEGIN_NAMESPAC
#ifndef TB_END_NAMESPACE
#define TB_END_NAMESPACE }
#endif // TB_END_NAMESPACE
// clang-format on

#include <memory_resource>
#include <algorithm>

TB_BEGIN_NAMESPACE

template<std::size_t L>
class TempBuffer
{
  public:
    TempBuffer() noexcept = default;
    TempBuffer(TempBuffer&&) = delete;
    TempBuffer(TempBuffer const&) = delete;
    TempBuffer& operator=(TempBuffer&&) = delete;
    TempBuffer& operator=(TempBuffer const&) = delete;

    TempBuffer(std::size_t size) noexcept
        : TempBuffer{}
    {
        resize(size);
    }

    TempBuffer(std::size_t size, int pattern) noexcept
        : TempBuffer{size}
    {
        std::fill_n(p_, size, static_cast<std::byte>(pattern));
    }

    ~TempBuffer()
    {
        reset();
    }

    void resize(size_t size)
    {
        if (size > sizeof(buffer_))
        {
            auto p = static_cast<decltype(p_)>(std::realloc(dynamic() ? p_ : nullptr, size));
            if (p == nullptr)
            {
                throw std::bad_alloc();
            }
            p_ = p;
        }
        else
        {
            if (dynamic())
            {
                std::copy_n(p_, size, buffer_);
                std::free(p_);
            }
            p_ = buffer_;
        }
        size_ = size;
    }

    void reset() noexcept
    {
        if (dynamic())
        {
            std::free(p_);
        }
        p_ = buffer_;
        size_ = 0;
    }

    [[nodiscard]] void* get() const noexcept
    {
        return p_;
    }

    template<typename T, typename = std::enable_if_t<std::is_trivial_v<T> && std::is_standard_layout_v<T>>>
    [[nodiscard]] T* as() const noexcept
    {
        // Some C++20 STLs for popular compilers have not yet std::bit_cast implemented.
        // To work around this issue type punning is used for now.
        union
        {
            decltype(p_) from;
            T* to;
        };
        from = p_;
        return to;
    }

    [[nodiscard]] std::size_t size() const noexcept
    {
        return size_;
    }

    [[nodiscard]] bool dynamic() const noexcept
    {
        return size_ > sizeof(buffer_);
    }

    [[nodiscard]] constexpr bool empty() const noexcept
    {
        return size_ == 0;
    }

  private:
    std::size_t size_{};
    std::byte* p_{buffer_};
    alignas(std::max_align_t) std::byte buffer_[L];
};

TB_END_NAMESPACE

#endif // TB_TEMP_BUFFER_HPP_INCLUDED
