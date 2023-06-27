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

TEST( Submdspan, PreserveLayoutContiguousAtRight )
{
    std::array< double, 2 * 3 * 4 > a_data;
    std::iota( a_data.begin(), a_data.end(), 0 );

    mdspan< double, dextents< int, 3 >, layout_contiguous_at_right > a_mdspan( a_data.data(), 2, 3, 4 );
    mdspan< double, dextents< int, 1 >, layout_contiguous_at_right > a_submdspan = submdspan( a_mdspan, 1, 1, full_extent );
    for ( int i = 0; i < a_submdspan.extent( 0 ); ++i )
    {
        EXPECT_EQ( a_submdspan( i ), 1 * ( 3 * 4 ) + 1 * 4 + i * 1 );
    }
}

TEST( Submdspan, PreserveLayoutContiguousAtLeft )
{
    std::array< double, 2 * 3 * 4 > a_data;
    std::iota( a_data.begin(), a_data.end(), 0 );

    mdspan< double, dextents< int, 3 >, layout_contiguous_at_left > a_mdspan( a_data.data(), 2, 3, 4 );
    mdspan< double, dextents< int, 1 >, layout_contiguous_at_left > a_submdspan = submdspan( a_mdspan, full_extent, 1, 1 );
    for ( int i = 0; i < a_submdspan.extent( 0 ); ++i )
    {
        EXPECT_EQ( a_submdspan( i ), i * 1 + 1 * 2 + 1 * ( 2 * 3 ) );
    }
}
