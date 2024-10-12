#ifndef FP_INPLACE_STRING_H
#define FP_INPLACE_STRING_H

#include <cstddef>
#include <string>

namespace fp {

template <
    class CharT,
    std::size_t N,
    class Traits = std::char_traits<CharT>
> class basic_inplace_string
{

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