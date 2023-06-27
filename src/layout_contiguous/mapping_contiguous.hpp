// MIT License

// Copyright (c) 2021 CEA
// Contributors: T. Padioleau (thomas.padioleau@cea.fr), J. Bigot

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <experimental/mdspan>
#include <utility>

namespace detail
{

template < std::size_t ContIdx, class Extents, class IndexSequence >
class mapping_contiguous_at;

template < std::size_t ContIdx, class Extents, std::size_t... Is >
class mapping_contiguous_at< ContIdx, Extents, std::index_sequence< Is... > >
{
    static_assert( ContIdx < sizeof...( Is ) );
    static_assert( Extents::rank() == sizeof...( Is ) );

    static constexpr typename Extents::rank_type dyn_strides = Extents::rank() - 1;

public:
    using index_type = typename Extents::index_type;
    using size_type = typename Extents::size_type;
    using rank_type = typename Extents::rank_type;

protected:
    template < std::size_t I >
    MDSPAN_FORCE_INLINE_FUNCTION constexpr size_type stride() const noexcept
    {
        static_assert( I < Extents::rank() );
        if constexpr ( I == ContIdx )
        {
            return 1;
        }
        else
        {
            return m_strides[ I ];
        }
    }

    bool internal_is_unique() const noexcept
    {
        std::array< index_type, Extents::rank() > rem { Is... };
        std::sort( rem.begin(), rem.end(),
                   [ this ]( index_type i1, index_type i2 ) { return m_strides[ i1 ] < m_strides[ i2 ]; } );
        index_type a = 1;
        for ( index_type i : rem )
        {
            if ( m_strides[ i ] >= a )
            {
                a += m_strides[ i ] * ( m_extents.extent( i ) - 1 );
            }
            else
            {
                return false;
            }
        }
        return true;
    }

    constexpr mapping_contiguous_at() noexcept = default;

    constexpr mapping_contiguous_at( const mapping_contiguous_at& ) noexcept = default;

    constexpr mapping_contiguous_at( mapping_contiguous_at&& ) noexcept = default;

    constexpr mapping_contiguous_at& operator=( const mapping_contiguous_at& ) noexcept = default;

    constexpr mapping_contiguous_at& operator=( mapping_contiguous_at&& ) noexcept = default;

public:
    constexpr Extents const& extents() const noexcept
    {
        return m_extents;
    }

    constexpr std::array< index_type, Extents::rank() > strides() const noexcept
    {
        return m_strides;
    }

    constexpr index_type required_span_size() const noexcept
    {
        return ( 1 + ... + ( ( m_extents.extent( Is ) - 1 ) * stride< Is >() ) );
    }

    template < class... Indices >
    MDSPAN_FORCE_INLINE_FUNCTION constexpr index_type operator()( Indices&&... indices ) const noexcept
    {
        static_assert( sizeof...( Indices ) == Extents::rank() );
        assert( ( ... && ( indices < m_extents.extent( Is ) ) ) );

        return ( 0 + ... + ( std::forward< Indices >( indices ) * stride< Is >() ) );
    }

    static constexpr bool is_always_unique() noexcept
    {
        return true;
    }

    static constexpr bool is_always_exhaustive() noexcept
    {
        return false;
    }

    static constexpr bool is_always_strided() noexcept
    {
        return true;
    }

    constexpr bool is_unique() const noexcept
    {
        return true;
    }

    bool is_exhaustive() const noexcept
    {
        std::array< index_type, Extents::rank() > rem { Is... };
        std::sort( rem.begin(), rem.end(),
                   [ this ]( index_type i1, index_type i2 ) { return m_strides[ i1 ] < m_strides[ i2 ]; } );
        index_type a = 1;
        for ( index_type i : rem )
        {
            if ( m_strides[ i ] <= a )
            {
                a += m_strides[ i ] * ( m_extents.extent( i ) - 1 );
            }
            else
            {
                return false;
            }
        }
        return true;
    }

    constexpr bool is_strided() const noexcept
    {
        return true;
    }

    constexpr index_type stride( rank_type i ) const noexcept
    {
        assert( i < m_strides.size() );
        return m_strides[ i ];
    }

protected:
    Extents m_extents = Extents( std::array< index_type, Extents::rank_dynamic() > {} );

    std::array< index_type, Extents::rank() > m_strides {};
};

} // namespace detail
