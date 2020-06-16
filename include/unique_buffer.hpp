//
// Unique buffer based on std::pmr::memory_resource.
//
// Copyright(c) 2020 Chronimal. All rights reserved.
//
// Licensed under the MIT license; A copy of the license that can be
// found in the LICENSE file.
//

#ifndef UB_TEMP_BUFFER_HPP_INCLUDED
#define UB_TEMP_BUFFER_HPP_INCLUDED

// Configure namespace preference for UniqueBuffer.
// By default namespace utl is used.
#define UB_NAMESPACE_NAME utl

// clang-format off
#ifndef UB_BEGIN_NAMESPACE
#define UB_BEGIN_NAMESPACE namespace UB_NAMESPACE_NAME {
#endif // UB_BEGIN_NAMESPAC
#ifndef UB_END_NAMESPACE
#define UB_END_NAMESPACE }
#endif // UB_END_NAMESPACE
// clang-format on

#include <memory_resource>

UB_BEGIN_NAMESPACE

class UniqueBuffer
{
  public:
    UniqueBuffer(UniqueBuffer const&) = delete;
    UniqueBuffer& operator=(UniqueBuffer const&) = delete;

    UniqueBuffer() noexcept = default;

    UniqueBuffer(std::size_t size)
        : UniqueBuffer(size, std::pmr::get_default_resource())
    {
    }

    UniqueBuffer(std::size_t size, std::pmr::memory_resource& memres, std::size_t alignment = alignof(std::max_align_t))
        : UniqueBuffer(size, &memres, alignment)
    {
    }

    UniqueBuffer(std::size_t size, std::pmr::memory_resource* memres, std::size_t alignment = alignof(std::max_align_t))
        : buffer_{memres->allocate(size, alignment)}
        , memres_{memres}
        , size_{size}
        , alignment_{alignment}
    {
    }

    UniqueBuffer(UniqueBuffer&& other) noexcept
        : buffer_{std::exchange(other.buffer_, nullptr)}
        , memres_{std::exchange(other.memres_, nullptr)}
        , size_{std::exchange(other.size_, 0)}
        , alignment_{std::exchange(other.alignment_, 0)}
    {
    }

    UniqueBuffer& operator=(UniqueBuffer&& other) noexcept
    {
        auto temp{std::move(other)};
        swap(temp);
        return *this;
    }

    ~UniqueBuffer()
    {
        deallocate();
    }

    void reset() noexcept
    {
        deallocate();
        size_ = alignment_ = 0;
        memres_ = nullptr;
        buffer_ = nullptr;
    }

    void swap(UniqueBuffer& other) noexcept
    {
        std::swap(buffer_, other.buffer_);
        std::swap(memres_, other.memres_);
        std::swap(size_, other.size_);
        std::swap(alignment_, other.alignment_);
    }

    [[nodiscard]] void* get() const noexcept
    {
        return buffer_;
    }

    template<typename T, typename = std::enable_if_t<std::is_trivial_v<T> && std::is_standard_layout_v<T>>>
    [[nodiscard]] T* as() const noexcept
    {
        // Some C++20 STLs for popular compilers have not yet std::bit_cast implemented.
        // To work around this issue type punning is used for now.
        union
        {
            decltype(buffer_) from;
            T* to;
        } cast{.from = buffer_};
        return cast.to;
    }

    [[nodiscard]] std::size_t size() const noexcept
    {
        return size_;
    }

    [[nodiscard]] std::size_t alignment() const noexcept
    {
        return alignment_;
    }

  private:
    void* buffer_{};
    std::pmr::memory_resource* memres_{};
    std::size_t size_{};
    std::size_t alignment_{};

    void deallocate() noexcept
    {
        if (memres_ != nullptr)
        {
            memres_->deallocate(buffer_, alignment_);
        }
    }
};

UB_END_NAMESPACE

#endif // UB_TEMP_BUFFER_HPP_INCLUDED
