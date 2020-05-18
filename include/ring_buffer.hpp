//
// Fixed size ring buffer (circular buffer)
//
// Copyright(c) 2020 Chronimal. All rights reserved.
//
// Licensed under the MIT license; A copy of the license that can be
// found in the LICENSE file.
//

#ifndef RB_RING_BUFFER_HPP_INCLUDED
#define RB_RING_BUFFER_HPP_INCLUDED

#if __cplusplus < 201703L
#error "C++17 or higher required"
#endif // __cplusplus

// Configure namespace preference for RingBuffer.
// By default namespace utl is used.
#define RB_NAMESPACE_NAME utl

// clang-format off
#ifndef RB_BEGIN_NAMESPACE
#define RB_BEGIN_NAMESPACE namespace RB_NAMESPACE_NAME {
#endif // RB_BEGIN_NAMESPAC
#ifndef RB_END_NAMESPACE
#define RB_END_NAMESPACE }
#endif // RB_END_NAMESPACE
// clang-format on

#include <iterator>
#include <utility>

RB_BEGIN_NAMESPACE

template<typename T, std::size_t N>
class RingBuffer;

template<class T, std::size_t N>
class RingBufferConstIterator
{
  public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T const*;
    using reference = T const&;

    constexpr explicit RingBufferConstIterator(RingBuffer<T, N> const* rb, std::size_t current = 0)
        : rb_{rb}
        , current_{current}
    {
    }

    [[nodiscard]] constexpr reference operator*() const
    {
        return (*rb_)[current_];
    }

    [[nodiscard]] constexpr pointer operator->() const
    {
        return &((*rb_)[current_]);
    }

    constexpr RingBufferConstIterator& operator++()
    {
        ++current_;
        return *this;
    }

    constexpr RingBufferConstIterator operator++(int)
    {
        RingBufferConstIterator temp{*this};
        ++current_;
        return temp;
    }

    constexpr RingBufferConstIterator& operator--()
    {
        --current_;
        return *this;
    }

    constexpr RingBufferConstIterator operator--(int)
    {
        RingBufferConstIterator temp{*this};
        --current_;
        return temp;
    }

    constexpr RingBufferConstIterator& operator+=(std::ptrdiff_t const offset)
    {
        current_ += offset;
        return *this;
    }

    [[nodiscard]] constexpr RingBufferConstIterator operator+(std::ptrdiff_t const offset) const
    {
        RingBufferConstIterator temp{*this};
        return temp += offset;
    }

    constexpr RingBufferConstIterator& operator-=(std::ptrdiff_t const offset)
    {
        current_ -= offset;
        return *this;
    }

    [[nodiscard]] constexpr RingBufferConstIterator operator-(std::ptrdiff_t const offset) const
    {
        RingBufferConstIterator temp{*this};
        return temp -= offset;
    }

    [[nodiscard]] constexpr std::ptrdiff_t operator-(RingBufferConstIterator const& rhs) const
    {
        return current_ - rhs.current_;
    }

    [[nodiscard]] constexpr reference operator[](std::ptrdiff_t const offset) const
    {
        return (*rb_)[offset];
    }

    [[nodiscard]] constexpr bool operator==(RingBufferConstIterator const& rhs) const
    {
        return current_ == rhs.current_;
    }

    [[nodiscard]] constexpr bool operator!=(RingBufferConstIterator const& rhs) const
    {
        return !(*this == rhs);
    }

    [[nodiscard]] constexpr bool operator<(RingBufferConstIterator const& rhs) const
    {
        return current_ < rhs.current_;
    }

    [[nodiscard]] constexpr bool operator>(RingBufferConstIterator const& rhs) const
    {
        return rhs < *this;
    }

    [[nodiscard]] constexpr bool operator<=(RingBufferConstIterator const& rhs) const
    {
        return !(rhs < *this);
    }

    [[nodiscard]] constexpr bool operator>=(RingBufferConstIterator const& rhs) const
    {
        return !(*this < rhs);
    }

  private:
    RingBuffer<T, N> const* rb_;
    std::size_t current_;
};

template<class T, std::size_t N>
class RingBufferIterator : public RingBufferConstIterator<T, N>
{
  public:
    using BaseClass = RingBufferConstIterator<T, N>;

    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;

    constexpr explicit RingBufferIterator(RingBuffer<T, N>* rb, std::size_t current = 0)
        : BaseClass(rb, current)
    {
    }

    [[nodiscard]] constexpr reference operator*() const
    {
        return const_cast<reference>(BaseClass::operator*());
    }

    [[nodiscard]] constexpr pointer operator->() const
    {
        return const_cast<pointer>(BaseClass::operator->());
    }

    constexpr RingBufferIterator& operator++()
    {
        BaseClass::operator++();
        return *this;
    }

    constexpr RingBufferIterator operator++(int)
    {
        RingBufferIterator temp{*this};
        BaseClass::operator++();
        return temp;
    }

    constexpr RingBufferIterator& operator--()
    {
        BaseClass::operator--();
        return *this;
    }

    constexpr RingBufferIterator operator--(int)
    {
        RingBufferIterator temp{*this};
        BaseClass::operator--();
        return temp;
    }

    constexpr RingBufferIterator& operator+=(std::ptrdiff_t const offset)
    {
        BaseClass::operator+=(offset);
        return *this;
    }

    [[nodiscard]] constexpr RingBufferIterator operator+(std::ptrdiff_t const offset) const
    {
        RingBufferIterator temp{*this};
        return temp += offset;
    }

    constexpr RingBufferIterator& operator-=(std::ptrdiff_t const offset)
    {
        BaseClass::operator-=(offset);
        return *this;
    }

    using BaseClass::operator-;

    [[nodiscard]] constexpr RingBufferIterator operator-(std::ptrdiff_t const offset) const
    {
        RingBufferIterator temp{*this};
        return temp -= offset;
    }

    [[nodiscard]] constexpr reference operator[](std::ptrdiff_t const offset) const
    {
        return const_cast<reference>(BaseClass::operator[](offset));
    }
};

template<typename T, std::size_t N>
class RingBuffer
{
  public:
    static_assert(N > 0, "Template argument N must not be zero");

    using value_type = T;
    using pointer = T*;
    using const_pointer = T const*;
    using reference = T&;
    using const_reference = T const&;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using iterator = RingBufferIterator<T, N>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_iterator = RingBufferConstIterator<T, N>;    
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    RingBuffer() = default;
    RingBuffer(RingBuffer&&) = delete;
    RingBuffer(RingBuffer const&) = delete;
    RingBuffer& operator=(RingBuffer&&) = delete;
    RingBuffer& operator=(RingBuffer const&) = delete;

    ~RingBuffer() noexcept
    {
        clear();
    }

    template<typename... Args>
    decltype(auto) emplace(Args&&... args)
    {
        // NOTE: Replace with std::construct_at() when moving to C++20
        auto element = new (storageAt(write_)) T(std::forward<Args>(args)...);
        if (full())
        {
            destruct(increment(read_));
        }
        increment(write_);
        return *element;
    }

    void push(T const& element)
    {
        emplace(element);
    }

    void push(T&& element)
    {
        emplace(std::move(element));
    }

    void pop() noexcept
    {
        destruct(increment(read_));
    }

    void clear() noexcept
    {
        while (!empty())
        {
            pop();
        }
        write_ = read_ = 0;
    }

    [[nodiscard]] reference front() noexcept
    {
        return *objectAt(read_);
    }

    [[nodiscard]] const_reference front() const noexcept
    {
        return *objectAt(read_);
    }

    [[nodiscard]] reference back() noexcept
    {
        return *objectAt(previousIndex(write_));
    }

    [[nodiscard]] const_reference back() const noexcept
    {
        return *objectAt(previousIndex(write_));
    }

    [[nodiscard]] reference operator[](size_type const index) noexcept
    {
        return *objectAt(calculateIndex(read_ + index));
    }

    [[nodiscard]] const_reference operator[](size_type const index) const noexcept
    {
        return *objectAt(calculateIndex(read_ + index));
    }

    [[nodiscard]] iterator begin() noexcept
    {
        return iterator{this};
    }

    [[nodiscard]] const_iterator begin() const noexcept
    {
        return const_iterator{this};
    }

    [[nodiscard]] const_iterator cbegin() const noexcept
    {
        return begin();
    }

    [[nodiscard]] iterator end() noexcept
    {
        return iterator{this, size()};
    }

    [[nodiscard]] const_iterator end() const noexcept
    {
        return const_iterator{this, size()};
    }

    [[nodiscard]] const_iterator cend() const noexcept
    {
        return end();
    }

    [[nodiscard]] reverse_iterator rbegin() noexcept
    {
        return reverse_iterator(end());
    }

    [[nodiscard]] const_reverse_iterator rbegin() const noexcept
    {
        return const_reverse_iterator(end());
    }

    [[nodiscard]] const_reverse_iterator crbegin() const noexcept
    {
        return rbegin();
    }

    [[nodiscard]] reverse_iterator rend() noexcept
    {
        return reverse_iterator(begin());
    }

    [[nodiscard]] const_reverse_iterator rend() const noexcept
    {
        return const_reverse_iterator(begin());
    }

    [[nodiscard]] const_reverse_iterator crend() const noexcept
    {
        return rend();
    }

    [[nodiscard]] size_type size() const noexcept
    {
        return (write_ - read_ + N + 1) % (N + 1);
    }

    [[nodiscard]] size_type capacity() const noexcept
    {
        return N;
    }

    [[nodiscard]] bool empty() const noexcept
    {
        return read_ == write_;
    }

    [[nodiscard]] bool full() const noexcept
    {
        return calculateIndex(write_ + 1) == read_;
    }

  private:
    std::ptrdiff_t read_{};  // Storage index of object referenced by front()
    std::ptrdiff_t write_{}; // Storage index to created the next object in
    std::aligned_storage_t<sizeof(T), alignof(T)> storage_[N + 1];

    // index must be zero or higher
    auto calculateIndex(std::ptrdiff_t index) const noexcept
    {
        return index % (N + 1);
    }

    auto previousIndex(std::ptrdiff_t index) const noexcept
    {
        return (index + N)  % (N + 1);
    }

    auto increment(std::ptrdiff_t& index) noexcept
    {
        return std::exchange(index, calculateIndex(index + 1));
    }

    T* objectAt(std::ptrdiff_t index) noexcept
    {
        return std::launder(static_cast<T*>(static_cast<void*>(&storage_[index])));
    }

    T const* objectAt(std::ptrdiff_t index) const noexcept
    {
        return std::launder(static_cast<T const*>(static_cast<void const*>(&storage_[index])));
    }

    void* storageAt(std::ptrdiff_t index) noexcept
    {
        return static_cast<void*>(&storage_[index]);
    }

    void destruct(std::ptrdiff_t index) noexcept
    {
        std::destroy_at(objectAt(index));
    }
};

RB_END_NAMESPACE

#endif // RB_RING_BUFFER_HPP_INCLUDED
