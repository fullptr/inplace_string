#ifndef FP_INPLACE_STRING_H
#define FP_INPLACE_STRING_H

#include <cstddef>
#include <string>
#include <stdexcept>

namespace fp {
namespace detail {

template <class InplaceString>
class iterator
{
public:
    using value_type = typename InplaceString::value_type;
    using element_type = value_type;
    using difference_type = typename InplaceString::difference_type;
    using iterator_category = std::contiguous_iterator_tag;
private:
    value_type* d_value;
public:
    iterator(element_type* value = nullptr) : d_value{value} {}
    element_type* operator->() const { return d_value; }
    element_type& operator*() const { return *d_value; }
    element_type& operator[](difference_type idx) const { return *(d_value + idx); }
    iterator& operator++() { ++d_value; return *this; }
    iterator operator++(int) { auto orig = d_value; ++d_value; return {orig}; }
    iterator& operator--() { --d_value; return *this; }
    iterator operator--(int) { auto orig = d_value; --d_value; return {orig}; }
    iterator& operator+=(difference_type n) { d_value += n; return *this; }
    iterator& operator-=(difference_type n) { d_value -= n; return *this; }
    friend auto operator<=>(iterator, iterator) = default;
    friend difference_type operator-(iterator lhs, iterator rhs) { return rhs.d_value - lhs.d_value; }
    friend iterator operator+(iterator it, difference_type n) { return {it.d_value + n}; }
    friend iterator operator-(iterator it, difference_type n) { return {it.d_value - n}; }
    friend iterator operator+(difference_type n, iterator it) { return {it.d_value + n}; }
};

template <class InplaceString>
class const_iterator
{
public:
    using value_type = typename InplaceString::value_type;
    using element_type = const value_type;
    using difference_type = typename InplaceString::difference_type;
    using iterator_category = std::contiguous_iterator_tag;
private:
    const value_type* d_value;
public:
    const_iterator(element_type* value = nullptr) : d_value{value} {}
    element_type* operator->() const { return d_value; }
    element_type& operator*() const { return *d_value; }
    element_type& operator[](difference_type idx) const { return *(d_value + idx); }
    const_iterator& operator++() { ++d_value; return *this; }
    const_iterator operator++(int) { auto orig = d_value; ++d_value; return {orig}; }
    const_iterator& operator--() { --d_value; return *this; }
    const_iterator operator--(int) { auto orig = d_value; --d_value; return {orig}; }
    const_iterator& operator+=(difference_type n) { d_value += n; return *this; }
    const_iterator& operator-=(difference_type n) { d_value -= n; return *this; }
    friend auto operator<=>(const_iterator, const_iterator) = default;
    friend difference_type operator-(const_iterator lhs, const_iterator rhs) { return rhs.d_value - lhs.d_value; }
    friend const_iterator operator+(const_iterator it, difference_type n) { return {it.d_value + n}; }
    friend const_iterator operator-(const_iterator it, difference_type n) { return {it.d_value - n}; }
    friend const_iterator operator+(difference_type n, const_iterator it) { return {it.d_value + n}; }
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

    using iterator = detail::iterator<basic_inplace_string>;
    using const_iterator = detail::const_iterator<basic_inplace_string>;

    static_assert(std::contiguous_iterator<iterator>);
    static_assert(std::contiguous_iterator<const_iterator>);
    
    // These don't work currently, need to make iterator and const_iterator satisfy the correct concepts
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

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
    constexpr const_iterator cbegin() const noexcept { return begin(); }

    constexpr iterator end() noexcept { return {d_data + d_size}; }
    constexpr const_iterator end() const noexcept { return {d_data + d_size}; }
    constexpr const_iterator cend() const noexcept { return end(); }

    constexpr reverse_iterator rbegin() noexcept { return reverse_iterator{end()}; }
    constexpr const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator{end()}; }
    constexpr const_reverse_iterator crbegin() const noexcept { return rbegin(); }

    constexpr reverse_iterator rend() noexcept { return reverse_iterator{begin()}; }
    constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator{begin()}; }
    constexpr const_reverse_iterator crend() const noexcept { return rend(); }

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