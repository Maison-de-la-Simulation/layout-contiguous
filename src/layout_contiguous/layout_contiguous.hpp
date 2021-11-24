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

#include <array>
#include <cassert>
#include <cstdint>
#include <experimental/mdspan>
#include <stdexcept>

#include "mapping_contiguous.hpp"

struct layout_contiguous_at_left
{
    template < class Extents >
    class mapping : public detail::mapping_contiguous_at< 0, Extents, std::make_index_sequence< Extents::rank() > >
    {
        static constexpr std::size_t dyn_rank = Extents::rank() - 1;

    public:
        using size_type = typename Extents::size_type;

        constexpr mapping() noexcept = default;

        constexpr mapping( mapping const& ) noexcept = default;

        constexpr mapping( Extents const& extents ) noexcept
            : mapping( std::experimental::layout_left::mapping< Extents >( extents ) )
        {
        }

        constexpr mapping( std::experimental::layout_left::mapping< Extents > const& x ) noexcept
        {
            this->m_extents = x.extents();
            for ( std::size_t i = 0; i < Extents::rank(); ++i )
            {
                this->m_strides[ i ] = x.stride( i );
            }
        }

        constexpr mapping( Extents const& extents, std::array< size_type, dyn_rank > const& strides )
        {
            this->m_extents = extents;
            this->m_strides.front() = 1;
            for ( std::size_t i = 0; i < strides.size(); ++i )
            {
                this->m_strides[ i + 1 ] = strides[ i ];
            }

            if ( !this->internal_is_unique() )
            {
                throw std::runtime_error( "The mapping should be unique" );
            }
        }

        constexpr mapping( std::experimental::layout_stride::mapping< Extents > const& x )
        {
            if ( x.stride( 0 ) != 1 )
            {
                throw std::runtime_error( "The first dimension is not contiguous" );
            }

            this->m_extents = x.extents();
            for ( std::size_t i = 0; i < Extents::rank(); ++i )
            {
                this->m_strides[ i ] = x.stride( i );
            }
        }

        template < class OtherExtents >
        constexpr mapping( mapping< OtherExtents > const& rhs ) noexcept
        {
            this->m_extents = rhs.extents();
            this->m_strides = rhs.strides();
        }

        constexpr mapping& operator=( mapping const& ) noexcept = default;

        template < class OtherExtents >
        constexpr mapping& operator=( mapping< OtherExtents > const& rhs ) noexcept
        {
            *this = mapping( rhs );
            return *this;
        }

        template < class OtherExtents >
        friend constexpr bool operator==( mapping const& lhs, mapping< OtherExtents > const& rhs ) noexcept
        {
            return lhs.m_extents == rhs.m_extents && lhs.m_strides == rhs.m_strides;
        }
    };
};

template < class ET, class EP, class AP, class... SliceSpecs >
constexpr auto
submdspan( std::experimental::mdspan< ET, EP, layout_contiguous_at_left, AP > const& contiguous_span,
           SliceSpecs&&... slices )
{
    using namespace std::experimental;
    using first_element_type = std::tuple_element_t< 0, std::tuple< SliceSpecs... > >;

    layout_stride::mapping< EP > mapping( contiguous_span.extents(), contiguous_span.mapping().strides() );
    mdspan< ET, EP, layout_stride, AP > strided_span( contiguous_span.data(), mapping );
    mdspan s = submdspan( strided_span, std::forward< SliceSpecs >( slices )... );
    if constexpr ( std::is_convertible_v< first_element_type, std::size_t > )
    {
        return s;
    }
    else
    {
        using SubEP = typename decltype( s )::extents_type;
        return mdspan< ET, SubEP, layout_contiguous_at_left, AP >( s );
    }
}

struct layout_contiguous_at_right
{
    template < class Extents >
    class mapping
        : public detail::mapping_contiguous_at< Extents::rank() - 1, Extents,
                                                std::make_index_sequence< Extents::rank() > >
    {
        static constexpr std::size_t dyn_rank = Extents::rank() - 1;

    public:
        using size_type = typename Extents::size_type;

        constexpr mapping() noexcept = default;

        constexpr mapping( mapping const& ) noexcept = default;

        constexpr mapping( Extents const& extents ) noexcept
            : mapping( std::experimental::layout_right::mapping< Extents >( extents ) )
        {
        }

        constexpr mapping( std::experimental::layout_right::mapping< Extents > const& x ) noexcept
        {
            this->m_extents = x.extents();
            for ( std::size_t i = 0; i < Extents::rank(); ++i )
            {
                this->m_strides[ i ] = x.stride( i );
            }
        }

        constexpr mapping( Extents const& extents, std::array< size_type, dyn_rank > const& strides )
        {
            this->m_extents = extents;
            for ( std::size_t i = 0; i < strides.size(); ++i )
            {
                this->m_strides[ i ] = strides[ i ];
            }
            this->m_strides.back() = 1;

            if ( !this->internal_is_unique() )
            {
                throw std::runtime_error( "The mapping should be unique" );
            }
        }

        constexpr mapping( std::experimental::layout_stride::mapping< Extents > const& x )
        {
            if ( x.stride( Extents::rank() - 1 ) != 1 )
            {
                throw std::runtime_error( "The last dimension is not contiguous" );
            }

            this->m_extents = x.extents();
            for ( std::size_t i = 0; i < Extents::rank(); ++i )
            {
                this->m_strides[ i ] = x.stride( i );
            }
        }

        template < class OtherExtents >
        constexpr mapping( mapping< OtherExtents > const& rhs ) noexcept
        {
            this->m_extents = rhs.extents();
            this->m_strides = rhs.strides();
        }

        constexpr mapping& operator=( mapping const& ) noexcept = default;

        template < class OtherExtents >
        constexpr mapping& operator=( mapping< OtherExtents > const& rhs ) noexcept
        {
            *this = mapping( rhs );
            return *this;
        }

        template < class OtherExtents >
        friend constexpr bool operator==( mapping const& lhs, mapping< OtherExtents > const& rhs ) noexcept
        {
            return lhs.m_extents == rhs.m_extents && lhs.m_strides == rhs.m_strides;
        }
    };
};

template < class ET, class EP, class AP, class... SliceSpecs >
constexpr auto
submdspan( std::experimental::mdspan< ET, EP, layout_contiguous_at_right, AP > const& contiguous_span,
           SliceSpecs&&... slices )
{
    using namespace std::experimental;
    using last_element_type = std::tuple_element_t< EP::rank() - 1, std::tuple< SliceSpecs... > >;

    layout_stride::mapping< EP > mapping( contiguous_span.extents(), contiguous_span.mapping().strides() );
    mdspan< ET, EP, layout_stride, AP > strided_span( contiguous_span.data(), mapping );
    mdspan s = submdspan( strided_span, std::forward< SliceSpecs >( slices )... );
    if constexpr ( std::is_convertible_v< last_element_type, std::size_t > )
    {
        return s;
    }
    else
    {
        using SubEP = typename decltype( s )::extents_type;
        return mdspan< ET, SubEP, layout_contiguous_at_right, AP >( s );
    }
}
