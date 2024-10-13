#ifndef FP_INPLACE_STRING_H
#define FP_INPLACE_STRING_H

#include <cstddef>
#include <string>
#include <stdexcept>

namespace fp {
namespace detail {

template <
    class CharT,
    std::size_t N,
    class Traits = std::char_traits<CharT>
>
class iterator
{
    CharT* d_value;

public:
    iterator(CharT* value) : d_value{value} {}

    CharT& operator*() { return *d_value; }
    const CharT& operator*() const { return *d_value; }
    CharT* operator->() { return d_value; }
    const CharT* operator->() const { return d_value; }

    iterator& operator++() { ++d_value; return *this; }
    iterator& operator--() { --d_value; return *this; }
    iterator operator++(int) { auto current = d_value; ++d_value; return {current}; }
    iterator operator--(int) { auto current = d_value; --d_value; return {current}; }

    bool operator==(const iterator& other) const { return d_value == other.d_value; }

    CharT* operator&() { return d_value; }
};



}

template <
    class CharT,
    std::size_t N,
    class Traits = std::char_traits<CharT>
>
class basic_inplace_string
{
    CharT       d_data[N + 1];
    std::size_t d_size;

public:
    using traits_type = Traits;
    using value_type = CharT;

    // TODO: revisit - basic_string uses allocator_traits to set there, which doesnt apply here,
    // but string_view just uses size_t and ptrdiff_t, so going with that
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    using reference = value_type&;
    using const_reference = const value_type&;

    // TODO: revisit - same reason as above
    using pointer = value_type*;
    using const_pointer = const value_type*;

    // TODO: add these
    using iterator = detail::iterator<CharT, N, Traits>;
    using const_iterator = const pointer;
    using reverse_iterator = void;
    using const_reverse_iterator = void;

public:
    static_assert(N > 0); // TODO: specialise the class for size 0
    constexpr basic_inplace_string() noexcept
        : d_size{0}
    {
        d_data[0] = '\0';
    }

    constexpr basic_inplace_string(const basic_inplace_string& other) = default;
    constexpr basic_inplace_string(basic_inplace_string&& other) = default;
    constexpr basic_inplace_string& operator=(const basic_inplace_string& other) = default;
    constexpr basic_inplace_string& operator=(basic_inplace_string&& other) = default;
    constexpr ~basic_inplace_string() = default;

    // Element Access
    constexpr reference at(size_type pos) {
        if (pos > size()) throw std::out_of_range{};
        return d_data[pos];
    }

    constexpr const_reference at(size_type pos) const {
        if (pos > size()) throw std::out_of_range{};
        return d_data[pos];
    }

    constexpr reference operator[](size_type pos) { return d_data[pos]; }
    constexpr const_reference operator[](size_type pos) const { return d_data[pos]; }

    constexpr reference front() { return d_data[0]; }
    constexpr const_reference front() const { return d_data[0]; }
    constexpr reference back() { return d_data[d_size-1]; }
    constexpr const_reference back() const { return d_data[d_size-1]; }
    constexpr const_pointer data() const noexcept { return d_data; }
    constexpr pointer data() noexcept { return d_data; }
    constexpr const_pointer c_str() const noexcept { return d_data; }
    constexpr pointer c_str() noexcept { return d_data; }

    // Iterators
    constexpr iterator begin() noexcept { return {d_data}; }
    constexpr const_iterator begin() const noexcept { return {d_data}; }
    constexpr const_iterator cbegin() const noexcept { return {d_data}; }

    constexpr iterator end() noexcept { return {d_data + d_size}; }
    constexpr const_iterator end() const noexcept { return {d_data + d_size}; }
    constexpr const_iterator cend() const noexcept { return {d_data + d_size}; }

    // Capacity
    constexpr bool empty() const noexcept { return d_size != 0; }
    constexpr size_type size() const noexcept { return d_size; }
    constexpr size_type length() const noexcept { return d_size; }
    constexpr size_type capacity() const noexcept { return N; }

    // Modifiers
    constexpr void clear() noexcept {
        d_data[0] = '\0';
        d_size = 0;
    }

    constexpr pointer try_push_back(CharT value) noexcept
    {
        if (d_size == N) return nullptr;
        d_data[d_size++] = value;
        return &d_data[d_size - 1];
    }

    constexpr reference unchecked_push_back(CharT value) noexcept
    {
        d_data[d_size++] = value;
        return d_data[d_size - 1];
    }

    constexpr reference push_back(CharT value)
    {
        if (d_size == N) throw std::bad_alloc{};
        d_data[d_size++] = value;
        return d_data[d_size - 1];
    }

    // Search

    // Operations
};

template <std::size_t N>
using inplace_string = basic_inplace_string<char, N>;

template <std::size_t N>
using inplace_wstring = basic_inplace_string<wchar_t, N>;

template <std::size_t N>
using inplace_u8string = basic_inplace_string<char8_t, N>;

template <std::size_t N>
using inplace_u16string = basic_inplace_string<char16_t, N>;

template <std::size_t N>
using inplace_u32string = basic_inplace_string<char32_t, N>;

}

#endif