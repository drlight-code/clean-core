#pragma once

#include <clean-core/detail/vector_base.hh>

namespace cc
{
// cc::vector, but backed by a given allocator
template <class T>
struct alloc_vector : public detail::vector_base<T, true>
{
    // ctors
public:
    alloc_vector() noexcept : detail::vector_base<T, true>(cc::system_allocator) {}

    explicit alloc_vector(cc::allocator* allocator) noexcept : detail::vector_base<T, true>(allocator) { CC_CONTRACT(allocator != nullptr); }

    explicit alloc_vector(size_t size, cc::allocator* allocator = cc::system_allocator) : alloc_vector(allocator)
    {
        this->_data = this->_alloc(size);
        this->_size = size;
        this->_capacity = size;
        for (size_t i = 0; i < size; ++i)
            new (placement_new, &this->_data[i]) T();
    }

    [[nodiscard]] static alloc_vector defaulted(size_t size, cc::allocator* allocator = cc::system_allocator)
    {
        return alloc_vector(size, allocator);
    }

    [[nodiscard]] static alloc_vector uninitialized(size_t size, cc::allocator* allocator = cc::system_allocator)
    {
        alloc_vector v(allocator);
        v._size = size;
        v._capacity = size;
        v._data = v._alloc(size);
        return v;
    }

    [[nodiscard]] static alloc_vector filled(size_t size, T const& value, cc::allocator* allocator = cc::system_allocator)
    {
        alloc_vector v(allocator);
        v.resize(size, value);
        return v;
    }

    alloc_vector(T const* begin, size_t num_elements, cc::allocator* allocator = cc::system_allocator) : alloc_vector(allocator)
    {
        this->reserve(num_elements);
        detail::container_copy_range<T>(begin, num_elements, this->_data);
        this->_size = num_elements;
    }
    alloc_vector(std::initializer_list<T> data, cc::allocator* allocator = cc::system_allocator) : alloc_vector(data.begin(), data.size(), allocator)
    {
    }
    alloc_vector(cc::span<T const> data, cc::allocator* allocator = cc::system_allocator) : alloc_vector(data.begin(), data.size(), allocator) {}

    template <class Range, cc::enable_if<cc::is_any_range<Range>> = true>
    explicit alloc_vector(Range const& range, cc::allocator* allocator = cc::system_allocator) : alloc_vector(allocator)
    {
        for (auto const& e : range)
            this->emplace_back(e);
    }

    ~alloc_vector()
    {
        detail::container_destroy_reverse<T>(this->_data, this->_size);
        this->_free(this->_data);
    }

    alloc_vector(alloc_vector&& rhs) noexcept : alloc_vector(rhs._allocator)
    {
        this->_data = rhs._data;
        this->_size = rhs._size;
        this->_capacity = rhs._capacity;
        rhs._data = nullptr;
        rhs._size = 0;
        rhs._capacity = 0;
        rhs._allocator = cc::system_allocator;
    }
    alloc_vector& operator=(alloc_vector&& rhs) noexcept
    {
        detail::container_destroy_reverse<T>(this->_data, this->_size);
        this->_free(this->_data);
        this->_data = rhs._data;
        this->_size = rhs._size;
        this->_capacity = rhs._capacity;
        this->_allocator = rhs._allocator;
        rhs._data = nullptr;
        rhs._size = 0;
        rhs._capacity = 0;
        rhs._allocator = cc::system_allocator;
        return *this;
    }

    alloc_vector(alloc_vector const& rhs) = delete;
    alloc_vector& operator=(alloc_vector const& rhs) = delete;
};

// hash
template <class T>
struct hash<alloc_vector<T>>
{
    [[nodiscard]] constexpr hash_t operator()(alloc_vector<T> const& a) const noexcept
    {
        size_t h = 0;
        for (auto const& v : a)
            h = cc::hash_combine(h, hash<T>{}(v));
        return h;
    }
};
}