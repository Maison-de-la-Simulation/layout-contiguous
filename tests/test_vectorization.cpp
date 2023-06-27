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
#include <cmath>
#include <experimental/mdspan>
#include <layout_contiguous.hpp>
#include <vector>

using namespace std::experimental;

void
vectorization_layout_contiguous_right_ivdep( mdspan< double, dextents< int, 2 >, layout_contiguous_at_right > a,
                                             mdspan< const double, dextents< int, 2 >, layout_contiguous_at_right > b )
{
    const std::size_t i_end = a.extent( 0 );
    const std::size_t j_end = a.extent( 1 );
    for ( std::size_t i = 0; i < i_end; ++i )
    {
#if defined( __INTEL_COMPILER )
#pragma ivdep
#elif defined( __GNUG__ ) && !defined( __clang__ )
#pragma GCC ivdep
#endif
        for ( std::size_t j = 0; j < j_end; ++j )
        {
            a( i, j ) += std::sqrt( b( i, j ) ) + b( i, j ) * b( i, j );
        }
    }
}

void
vectorization_layout_contiguous_right_omp_simd( mdspan< double, dextents< int, 2 >, layout_contiguous_at_right > a,
                                                mdspan< const double, dextents< int, 2 >, layout_contiguous_at_right > b )
{
    const std::size_t i_end = a.extent( 0 );
    const std::size_t j_end = a.extent( 1 );
    for ( std::size_t i = 0; i < i_end; ++i )
    {
#pragma omp simd
        for ( std::size_t j = 0; j < j_end; ++j )
        {
            a( i, j ) += std::sqrt( b( i, j ) ) + b( i, j ) * b( i, j );
        }
    }
}

void
vectorization_layout_stride_ivdep( mdspan< double, dextents< int, 2 >, layout_stride > a,
                                   mdspan< const double, dextents< int, 2 >, layout_stride > b )
{
    const std::size_t i_end = a.extent( 0 );
    const std::size_t j_end = a.extent( 1 );
    for ( std::size_t i = 0; i < i_end; ++i )
    {
#if defined( __INTEL_COMPILER )
#pragma ivdep
#elif defined( __GNUG__ ) && !defined( __clang__ )
#pragma GCC ivdep
#endif
        for ( std::size_t j = 0; j < j_end; ++j )
        {
            a( i, j ) += std::sqrt( b( i, j ) ) + b( i, j ) * b( i, j );
        }
    }
}

void
vectorization_layout_stride_omp_simd( mdspan< double, dextents< int, 2 >, layout_stride > a,
                                      mdspan< const double, dextents< int, 2 >, layout_stride > b )
{
    const std::size_t i_end = a.extent( 0 );
    const std::size_t j_end = a.extent( 1 );
    for ( std::size_t i = 0; i < i_end; ++i )
    {
#pragma omp simd
        for ( std::size_t j = 0; j < j_end; ++j )
        {
            a( i, j ) += std::sqrt( b( i, j ) ) + b( i, j ) * b( i, j );
        }
    }
}
