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

#include <algorithm>
#include <array>
#include <experimental/mdspan>
#include <gtest/gtest.h>
#include <layout_contiguous.hpp>

using namespace std::experimental;

TEST( LayoutContiguousAtRight, ConstexprDefaultConstructor )
{
    using E = dextents< 3 >;
    using M = layout_contiguous_at_right::mapping< E >;

    constexpr E e( 0, 0, 0 );
    constexpr M mapping;
    EXPECT_EQ( mapping.extents(), e );
    EXPECT_EQ( mapping.required_span_size(), 0 );
    EXPECT_EQ( mapping.stride( 0 ), 0 );
    EXPECT_EQ( mapping.stride( 1 ), 0 );
    EXPECT_EQ( mapping.stride( 2 ), 0 );
    EXPECT_TRUE( mapping.is_contiguous() );
    EXPECT_TRUE( mapping.is_unique() );
    EXPECT_TRUE( mapping.is_strided() );
}

TEST( LayoutContiguousAtRight, ConstexprExtentsConstructor )
{
    using E = dextents< 3 >;
    using M = layout_contiguous_at_right::mapping< E >;

    constexpr E e( 2, 3, 4 );
    constexpr M mapping( e );
    EXPECT_EQ( mapping.extents(), e );
    EXPECT_EQ( mapping.required_span_size(), ( mapping( 1, 2, 3 ) - mapping( 0, 0, 0 ) + 1 ) );
    EXPECT_EQ( mapping.stride( 0 ), 12 );
    EXPECT_EQ( mapping.stride( 1 ), 4 );
    EXPECT_EQ( mapping.stride( 2 ), 1 );
    EXPECT_EQ( ( mapping( 1, 0, 0 ) - mapping( 0, 0, 0 ) ), 12 );
    EXPECT_EQ( ( mapping( 0, 1, 0 ) - mapping( 0, 0, 0 ) ), 4 );
    EXPECT_EQ( ( mapping( 0, 0, 1 ) - mapping( 0, 0, 0 ) ), 1 );
    EXPECT_TRUE( mapping.is_contiguous() );
    EXPECT_TRUE( mapping.is_unique() );
    EXPECT_TRUE( mapping.is_strided() );
}

TEST( LayoutContiguousAtRight, ConstexprLayoutRightConstructor )
{
    using E = dextents< 3 >;
    using M = layout_contiguous_at_right::mapping< E >;

    constexpr E e( 2, 3, 4 );
    constexpr layout_right::mapping mapping_right( e );
    constexpr M mapping( mapping_right );
    EXPECT_EQ( mapping.extents(), e );
    EXPECT_EQ( mapping.required_span_size(), ( mapping( 1, 2, 3 ) - mapping( 0, 0, 0 ) + 1 ) );
    EXPECT_EQ( mapping.stride( 0 ), 12 );
    EXPECT_EQ( mapping.stride( 1 ), 4 );
    EXPECT_EQ( mapping.stride( 2 ), 1 );
    EXPECT_EQ( ( mapping( 1, 0, 0 ) - mapping( 0, 0, 0 ) ), 12 );
    EXPECT_EQ( ( mapping( 0, 1, 0 ) - mapping( 0, 0, 0 ) ), 4 );
    EXPECT_EQ( ( mapping( 0, 0, 1 ) - mapping( 0, 0, 0 ) ), 1 );
    EXPECT_TRUE( mapping.is_contiguous() );
    EXPECT_TRUE( mapping.is_unique() );
    EXPECT_TRUE( mapping.is_strided() );
}

TEST( LayoutContiguousAtRight, ExtentsStridesConstructor )
{
    using E = dextents< 3 >;
    using M = layout_contiguous_at_right::mapping< E >;

    constexpr E e( 2, 3, 4 );
    constexpr std::array< std::size_t, 2 > strides { 20, 5 };
    M mapping( e, strides );
    EXPECT_EQ( mapping.extents(), e );
    EXPECT_EQ( mapping.required_span_size(), ( mapping( 1, 2, 3 ) - mapping( 0, 0, 0 ) + 1 ) );
    EXPECT_EQ( mapping.stride( 0 ), 20 );
    EXPECT_EQ( mapping.stride( 1 ), 5 );
    EXPECT_EQ( mapping.stride( 2 ), 1 );
    EXPECT_EQ( ( mapping( 1, 0, 0 ) - mapping( 0, 0, 0 ) ), 20 );
    EXPECT_EQ( ( mapping( 0, 1, 0 ) - mapping( 0, 0, 0 ) ), 5 );
    EXPECT_EQ( ( mapping( 0, 0, 1 ) - mapping( 0, 0, 0 ) ), 1 );
    EXPECT_FALSE( mapping.is_contiguous() );
    EXPECT_TRUE( mapping.is_unique() );
    EXPECT_TRUE( mapping.is_strided() );
}

TEST( LayoutContiguousAtRight, ConstexprLayoutStrideConstructor )
{
    using E = dextents< 3 >;
    using M = layout_contiguous_at_right::mapping< E >;

    constexpr E e( 2, 3, 4 );
    constexpr std::array< std::size_t, 3 > strides { 20, 5, 1 };
    constexpr layout_stride::mapping mapping_stride( e, strides );
    constexpr M mapping( mapping_stride );
    EXPECT_EQ( mapping.extents(), e );
    EXPECT_EQ( mapping.required_span_size(), ( mapping( 1, 2, 3 ) - mapping( 0, 0, 0 ) + 1 ) );
    EXPECT_EQ( mapping.stride( 0 ), 20 );
    EXPECT_EQ( mapping.stride( 1 ), 5 );
    EXPECT_EQ( mapping.stride( 2 ), 1 );
    EXPECT_EQ( ( mapping( 1, 0, 0 ) - mapping( 0, 0, 0 ) ), 20 );
    EXPECT_EQ( ( mapping( 0, 1, 0 ) - mapping( 0, 0, 0 ) ), 5 );
    EXPECT_EQ( ( mapping( 0, 0, 1 ) - mapping( 0, 0, 0 ) ), 1 );
    EXPECT_FALSE( mapping.is_contiguous() );
    EXPECT_TRUE( mapping.is_unique() );
    EXPECT_TRUE( mapping.is_strided() );
}

TEST( LayoutContiguousAtRight, constexpr_other_extents_constructor )
{
    using OE = extents< 2, 3, 4 >;
    using E = dextents< 3 >;
    using M = layout_contiguous_at_right::mapping< E >;

    constexpr OE oe;
    constexpr layout_contiguous_at_right::mapping omapping( oe );
    constexpr M mapping( omapping );
    EXPECT_EQ( mapping.required_span_size(), ( mapping( 1, 2, 3 ) - mapping( 0, 0, 0 ) + 1 ) );
    EXPECT_EQ( mapping.stride( 0 ), 12 );
    EXPECT_EQ( mapping.stride( 1 ), 4 );
    EXPECT_EQ( mapping.stride( 2 ), 1 );
    EXPECT_EQ( ( mapping( 1, 0, 0 ) - mapping( 0, 0, 0 ) ), 12 );
    EXPECT_EQ( ( mapping( 0, 1, 0 ) - mapping( 0, 0, 0 ) ), 4 );
    EXPECT_EQ( ( mapping( 0, 0, 1 ) - mapping( 0, 0, 0 ) ), 1 );
    EXPECT_TRUE( mapping.is_contiguous() );
    EXPECT_TRUE( mapping.is_unique() );
    EXPECT_TRUE( mapping.is_strided() );
}
