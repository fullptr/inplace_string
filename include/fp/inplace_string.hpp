#ifndef FP_INPLACE_STRING_H
#define FP_INPLACE_STRING_H

#include <cstddef>
#include <string>
#include <stdexcept>

namespace fp {
namespace detail {

template <class ElementType, class DifferenceType>
class iterator_impl
{
public:
    using element_type = ElementType;
    using difference_type = DifferenceType;
    using iterator_category = std::contiguous_iterator_tag;
private:
    element_type* d_value;
public:
    iterator_impl(element_type* value = nullptr) : d_value{value} {}
    element_type* operator->() const { return d_value; }
    element_type& operator*() const { return *d_value; }
    element_type& operator[](difference_type idx) const { return *(d_value + idx); }
    iterator_impl& operator++() { ++d_value; return *this; }
    iterator_impl operator++(int) { auto orig = d_value; ++d_value; return {orig}; }
    iterator_impl& operator--() { --d_value; return *this; }
    iterator_impl operator--(int) { auto orig = d_value; --d_value; return {orig}; }
    iterator_impl& operator+=(difference_type n) { d_value += n; return *this; }
    iterator_impl& operator-=(difference_type n) { d_value -= n; return *this; }
    friend auto operator<=>(iterator_impl, iterator_impl) = default;
    friend difference_type operator-(iterator_impl lhs, iterator_impl rhs) { return rhs.d_value - lhs.d_value; }
    friend iterator_impl operator+(iterator_impl it, difference_type n) { return {it.d_value + n}; }
    friend iterator_impl operator-(iterator_impl it, difference_type n) { return {it.d_value - n}; }
    friend iterator_impl operator+(difference_type n, iterator_impl it) { return {it.d_value + n}; }
};

template <class InplaceString>
using iterator = iterator_impl<
    typename InplaceString::value_type,
    typename InplaceString::difference_type>;

template <class InplaceString>
using const_iterator = iterator_impl<
    const typename InplaceString::value_type,
    typename InplaceString::difference_type>;

}

template <class CharT, std::size_t N, class Traits = std::char_traits<CharT>>
class basic_inplace_string
{
public:
    using traits_type = Traits;
    using value_type = CharT;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = detail::iterator<basic_inplace_string>;
    using const_iterator = detail::const_iterator<basic_inplace_string>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    static_assert(std::contiguous_iterator<iterator>);
    static_assert(std::contiguous_iterator<const_iterator>);

private:
    CharT     d_data[N + 1];
    size_type d_size;

public:
    static_assert(N > 0); // TODO: specialise the class for size 0
    constexpr basic_inplace_string() noexcept
        : d_size{0}
    {
        d_data[0] = '\0';
    }

    constexpr basic_inplace_string(size_type count, CharT ch)
        : d_size{count}
    {
        if (count > N) throw std::bad_alloc{}; // same behaviour as inplace_vector
        for (size_type i = 0; i < count; ++i) d_data[i] = ch;
        d_data[count] = '\0';
    }

    constexpr basic_inplace_string(const basic_inplace_string& other) = default;
    constexpr basic_inplace_string(basic_inplace_string&& other) = default;
    constexpr basic_inplace_string& operator=(const basic_inplace_string& other) = default;
    constexpr basic_inplace_string& operator=(basic_inplace_string&& other) = default;
    constexpr ~basic_inplace_string() = default;

    constexpr operator std::basic_string_view<CharT, Traits>() const noexcept { return {d_data, d_size}; }

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
    constexpr bool empty() const noexcept { return d_size == 0; }
    constexpr size_type size() const noexcept { return d_size; }
    constexpr size_type length() const noexcept { return d_size; }
    static constexpr size_type max_size() noexcept { return N; }
    static constexpr void reserve(size_type size) { if (size > N) throw std::bad_alloc{}; }
    static constexpr size_type capacity() noexcept { return N; }
    static constexpr void shrink_to_fit() noexcept {}

    // Modifiers
    constexpr void clear() noexcept {
        d_data[0] = '\0';
        d_size = 0;
    }

    constexpr pointer try_push_back(CharT value) noexcept
    {
        if (d_size == N) return nullptr;
        d_data[d_size++] = value;
        d_data[d_size] = '\0';
        return &d_data[d_size - 1];
    }

    constexpr reference unchecked_push_back(CharT value) noexcept
    {
        d_data[d_size++] = value;
        d_data[d_size] = '\0';
        return d_data[d_size - 1];
    }

    constexpr reference push_back(CharT value)
    {
        if (d_size == N) throw std::bad_alloc{};
        d_data[d_size++] = value;
        d_data[d_size] = '\0';
        return d_data[d_size - 1];
    }

    constexpr pop_back()
    {
        d_size--;
        d_data[d_size] = '\0';
    }

    // constexpr void resize(size_type count)
    // std::string has this overload, but I don't think this should be provided here
    // because the default value is surely the null char and including a bunch of those
    // at the end seems like a bad idea

    constexpr void resize(size_type count, CharT ch)
    {
        if (count > N) throw std::bad_alloc{};
        if (count < d_size) {
            d_size = count;
            d_data[d_size] = '\0';
        } else {
            // TODO: This can just be a memset + setting the size and null char
            for (std::size_t i = 0; i < count - d_size; ++i) {
                unchecked_push_back(ch);
            }
        }
    }

    constexpr void swap(basic_inplace_string& other) noexcept
    {
        std::swap(d_data, other.d_data);
        std::swap(d_size, other.d_size);
    }

    // Search
    size_type find(const basic_inplace_string& str, size_type pos = 0) const
    {
        return std::string_view{*this}.find(str, pos);
    }
    size_type find(const CharT* s, size_type pos, size_type count) const
    {
        return std::string_view{*this}.find(s, pos, count);
    }
    size_type find(const CharT* s, size_type pos = 0) const
    {
        return std::string_view{*this}.find(s, pos);
    }
    size_type find(CharT ch, size_type pos = 0) const
    {
        return std::string_view{*this}.find(ch, pos);
    }
    template <class StringViewLike>
    size_type find(const StringViewLike& t, size_type pos = 0) const
        noexcept(noexcept(std::is_nothrow_convertible_v<const StringViewLike&,
                                                        basic_inplace_string>))
    {
        return std::string_view{*this}.find(t, pos);
    }

    size_type rfind(const basic_inplace_string& str, size_type pos = 0) const
    {
        return std::string_view{*this}.rfind(str, pos);
    }
    size_type rfind(const CharT* s, size_type pos, size_type count) const
    {
        return std::string_view{*this}.rfind(s, pos, count);
    }
    size_type rfind(const CharT* s, size_type pos = 0) const
    {
        return std::string_view{*this}.rfind(s, pos);
    }
    size_type rfind(CharT ch, size_type pos = 0) const
    {
        return std::string_view{*this}.rfind(ch, pos);
    }
    template <class StringViewLike>
    size_type rfind(const StringViewLike& t, size_type pos = 0) const
        noexcept(noexcept(std::is_nothrow_convertible_v<const StringViewLike&,
                                                        basic_inplace_string>))
    {
        return std::string_view{*this}.rfind(t, pos);
    }

    size_type find_first_of(const basic_inplace_string& str, size_type pos = 0) const
    {
        return std::string_view{*this}.find_first_of(str, pos);
    }
    size_type find_first_of(const CharT* s, size_type pos, size_type count) const
    {
        return std::string_view{*this}.find_first_of(s, pos, count);
    }
    size_type find_first_of(const CharT* s, size_type pos = 0) const
    {
        return std::string_view{*this}.find_first_of(s, pos);
    }
    size_type find_first_of(CharT ch, size_type pos = 0) const
    {
        return std::string_view{*this}.find_first_of(ch, pos);
    }
    template <class StringViewLike>
    size_type find_first_of(const StringViewLike& t, size_type pos = 0) const
        noexcept(noexcept(std::is_nothrow_convertible_v<const StringViewLike&,
                                                        basic_inplace_string>))
    {
        return std::string_view{*this}.find_first_of(t, pos);
    }

    size_type find_first_not_of(const basic_inplace_string& str, size_type pos = 0) const
    {
        return std::string_view{*this}.find_first_not_of(str, pos);
    }
    size_type find_first_not_of(const CharT* s, size_type pos, size_type count) const
    {
        return std::string_view{*this}.find_first_not_of(s, pos, count);
    }
    size_type find_first_not_of(const CharT* s, size_type pos = 0) const
    {
        return std::string_view{*this}.find_first_not_of(s, pos);
    }
    size_type find_first_not_of(CharT ch, size_type pos = 0) const
    {
        return std::string_view{*this}.find_first_not_of(ch, pos);
    }
    template <class StringViewLike>
    size_type find_first_not_of(const StringViewLike& t, size_type pos = 0) const
        noexcept(noexcept(std::is_nothrow_convertible_v<const StringViewLike&,
                                                        basic_inplace_string>))
    {
        return std::string_view{*this}.find_first_not_of(t, pos);
    }

    size_type find_last_of(const basic_inplace_string& str, size_type pos = 0) const
    {
        return std::string_view{*this}.find_last_of(str, pos);
    }
    size_type find_last_of(const CharT* s, size_type pos, size_type count) const
    {
        return std::string_view{*this}.find_last_of(s, pos, count);
    }
    size_type find_last_of(const CharT* s, size_type pos = 0) const
    {
        return std::string_view{*this}.find_last_of(s, pos);
    }
    size_type find_last_of(CharT ch, size_type pos = 0) const
    {
        return std::string_view{*this}.find_last_of(ch, pos);
    }
    template <class StringViewLike>
    size_type find_last_of(const StringViewLike& t, size_type pos = 0) const
        noexcept(noexcept(std::is_nothrow_convertible_v<const StringViewLike&,
                                                        basic_inplace_string>))
    {
        return std::string_view{*this}.find_last_of(t, pos);
    }

    size_type find_last_not_of(const basic_inplace_string& str, size_type pos = 0) const
    {
        return std::string_view{*this}.find_last_not_of(str, pos);
    }
    size_type find_last_not_of(const CharT* s, size_type pos, size_type count) const
    {
        return std::string_view{*this}.find_last_not_of(s, pos, count);
    }
    size_type find_last_not_of(const CharT* s, size_type pos = 0) const
    {
        return std::string_view{*this}.find_last_not_of(s, pos);
    }
    size_type find_last_not_of(CharT ch, size_type pos = 0) const
    {
        return std::string_view{*this}.find_last_not_of(ch, pos);
    }
    template <class StringViewLike>
    size_type find_last_not_of(const StringViewLike& t, size_type pos = 0) const
        noexcept(noexcept(std::is_nothrow_convertible_v<const StringViewLike&,
                                                        basic_inplace_string>))
    {
        return std::string_view{*this}.find_last_not_of(t, pos);
    }

    // Operations
    constexpr bool starts_with(std::basic_string_view<CharT, Traits> sv) const noexcept
    {
        return std::string_view{*this}.starts_with(sv);
    }
    constexpr bool starts_with(CharT ch) const noexcept
    {
        return std::string_view{*this}.starts_with(ch);
    }
    constexpr bool starts_with(const CharT* s) const
    {
        return std::string_view{*this}.starts_with(s);
    }

    constexpr bool ends_with(std::basic_string_view<CharT, Traits> sv) const noexcept
    {
        return std::string_view{*this}.ends_with(sv);
    }
    constexpr bool ends_with(CharT ch) const noexcept
    {
        return std::string_view{*this}.ends_with(ch);
    }
    constexpr bool ends_with(const CharT* s) const
    {
        return std::string_view{*this}.ends_with(s);
    }

    constexpr bool contains(std::basic_string_view<CharT, Traits> sv) const noexcept
    {
        return std::string_view{*this}.contains(sv);
    }
    constexpr bool contains(CharT ch) const noexcept
    {
        return std::string_view{*this}.contains(ch);
    }
    constexpr bool contains(const CharT* s) const
    {
        return std::string_view{*this}.contains(s);
    }
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