// ============================================================================
//                 SeqAn - The Library for Sequence Analysis
// ============================================================================
//
// Copyright (c) 2006-2017, Knut Reinert & Freie Universitaet Berlin
// Copyright (c) 2016-2017, Knut Reinert & MPI Molekulare Genetik
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of Knut Reinert or the FU Berlin nor the names of
//       its contributors may be used to endorse or promote products derived
//       from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL KNUT REINERT OR THE FU BERLIN BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
// DAMAGE.
//
// ============================================================================
// Author: Hannes Hauswedell <hannes.hauswedell AT fu-berlin.de>
// ============================================================================

#pragma once

#include <iostream>
#include <string>

namespace seqan3
{

/*!\file alphabet.hpp
 * \ingroup alphabet
 * Alphabet header with concept defintions and some general purpose free functions.
 */

/*!\defgroup alphabet
 *
 * The alphabet module contains different biological alphabets and related functionality.
 *
 * TODO more details.
 */

// ------------------------------------------------------------------
// free functions to member function wrapper
// ------------------------------------------------------------------
// move to alphabet_detail.hpp?


/* The public alphabet concept requires only free function access
 * for type that have member functions we create a wrapper here
 * so you don't have to repeat it.
 */
//! \privatesection
namespace detail
{

template <typename t>
concept bool internal_alphabet_concept = requires (t t1)
{
    typename t::char_type;
    typename t::integral_type;
    t::value_size;

    { t1.to_char() } -> typename t::char_type;
    { t1.to_integral() } -> typename t::integral_type;

    { t1.from_char('a')   } -> t;
    { t1.from_integral(0) } -> t;
};
} // namespace seqan3::detail
//! \publicsection

// ------------------------------------------------------------------
// type metafunctions operator
// ------------------------------------------------------------------

//! Type metafunction that returns the `char_type` defined inside an alphabet type.
template <typename alphabet_type>
    requires detail::internal_alphabet_concept<alphabet_type>
struct underlying_char
{
    using type = typename alphabet_type::char_type;
};

//! Shortcut for @link underlying_char @endlink
template <typename alphabet_type>
using underlying_char_t = typename underlying_char<alphabet_type>::type;

//! Type metafunction that returns the `integral_type` defined inside an alphabet type.
template <typename alphabet_type>
    requires detail::internal_alphabet_concept<alphabet_type>
struct underlying_integral
{
    using type = typename alphabet_type::integral_type;
};

//! Shortcut for @link underlying_integral @endlink
template <typename alphabet_type>
using underlying_integral_t = typename underlying_integral<alphabet_type>::type;

// ------------------------------------------------------------------
// value metafunctions
// ------------------------------------------------------------------

//! Value metafunction that returns the `value_size` defined inside an alphabet type.
template <typename alphabet_type>
    requires detail::internal_alphabet_concept<alphabet_type>
struct alphabet_size
{
    static constexpr underlying_integral_t<alphabet_type> value = alphabet_type::value_size;
};

//! Shortcut for @link alphabet_size @endlink
template <typename alphabet_type>
constexpr underlying_integral_t<alphabet_type> alphabet_size_v = alphabet_size<alphabet_type>::value;

// ------------------------------------------------------------------
// free functions
// ------------------------------------------------------------------

//!\publicsection
//!@name Wrapper functions to make alphabet members "globally" visible
//!@{

//! Free function that calls `.to_char()` on the argument
template <typename alphabet_type>
    requires detail::internal_alphabet_concept<alphabet_type>
constexpr underlying_char_t<alphabet_type> to_char(alphabet_type const & c)
{
    return c.to_char();
}

//! Free function that calls `.to_integral()` on the argument
template <typename alphabet_type>
    requires detail::internal_alphabet_concept<alphabet_type>
constexpr underlying_integral_t<alphabet_type> to_integral(alphabet_type const & c)
{
    return c.to_integral();
}

//! Free function that calls `.from_char(in)` on the first argument
template <typename alphabet_type>
    requires detail::internal_alphabet_concept<alphabet_type>
constexpr alphabet_type from_char(alphabet_type & c, char const in)
{
    return c.from_char(in);
}

//! Free function that calls `.from_integral(in)` on the first argument
template <typename alphabet_type>
    requires detail::internal_alphabet_concept<alphabet_type>
constexpr alphabet_type from_integral(alphabet_type & c, underlying_integral_t<alphabet_type> const in)
{
    return c.from_integral(in);
}

//! Free ostream operator that delegates to `c.to_char()`
template <typename alphabet_type>
    requires detail::internal_alphabet_concept<alphabet_type>
std::ostream& operator<<(std::ostream & os, alphabet_type const & c)
{
    os << c.to_char();
    return os;
}

//!@}
// ------------------------------------------------------------------
// alphabet concept
// ------------------------------------------------------------------

/*!\var concept bool alphabet_concept
 * \brief A concept for container and string alphabets
 * \privatesection
 */

template <typename t>
concept bool alphabet_concept = requires (t t1, t t2)
{
    // StL concepts
    requires std::is_pod_v<t> == true;
    requires std::is_swappable_v<t> == true;

    // static data members
    alphabet_size<t>::value;

    // conversion from/to char
    { to_char(t1)     } -> underlying_char_t<t>;
    { to_integral(t1) } -> underlying_integral_t<t>;

    { from_char(t1, 0)     } -> t;
    { from_integral(t1, 0) } -> t;

    { std::cout << t1 };

    // required comparison operators
    { t1 == t2 } -> bool;
    { t1 != t2 } -> bool;
    { t1 <  t2 } -> bool;
    { t1 >  t2 } -> bool;
    { t1 <= t2 } -> bool;
    { t1 >= t2 } -> bool;
};

//TODO serialization

} // namespace seqan3
