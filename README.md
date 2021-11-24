# Contiguous layouts

This library provides two new layouts (C++17 compatible) to be used with `mdspan`
- `layout_contiguous_at_right`,
- `layout_contiguous_at_left`.

The layout `layout_contiguous_at_right` (resp. `layout_contiguous_at_left`) ensures contiguity in the right (resp. left) most dimension. By default the `layout_contiguous_at_right` (resp. `layout_contiguous_at_left`) behaves as a `layout_right` (`layout_left`) if dynamic strides are not explicitly specified.

## Why these layouts ?

Some algorithms can be accelerated by SIMD instructions. A typical case is an inner loop with unit-stride.

Among the different layouts provided by the `mdspan` library, only the `layout_right` and `layout_left` can provide such compile-time information about the unit-stride. If one wants to iterate over a subrange, two solutions are possible:
- applying `submdspan` that may lead to a `layout_stride` even though the existence of a contiguous dimension,
- shifting the integer range.

We aim at providing an intermediate layout between `layout_right` and `layout_stride` that ensures the compile-time unit-stride in the right most dimension. Subranges with this layout preserve this layout as long as the contiguous dimension exists.

## API

They provide the same API as `layout_stride`.

## Conversions

- `layout_right` -> `layout_contiguous_at_right`
- `layout_left` -> `layout_contiguous_at_left`
- `layout_stride` -> `layout_contiguous_at_right` (potentially throwing if right most stride is not 1)
- `layout_stride` -> `layout_contiguous_at_left` (potentially throwing if left most stride is not 1)

## Submdspan overload

The free function `submdspan` is overloaded to handle these two layouts. In the case of a layout `layout_contiguous_at_right` (resp. `layout_contiguous_at_left`), `submdspan` returns a `mdspan` with layout `layout_contiguous_at_right` (resp. `layout_contiguous_at_left`) if the right (resp. left) most spec is not an integer.

Let us write these rules with regular expressions. Let us note
- `S = Single integer`
- `R = Range of integers`
- `F = Full range`

Case `layout_contiguous_at_right`:
- if slice specifications are of the form `.*[^S]` then `layout_contiguous_at_right`
- else `layout_stride`

Case `layout_contiguous_at_left`:
- if slice specifications are of the form `[^S].*` then `layout_contiguous_at_left`
- else `layout_stride`

Case `layout_right`:
- if slice specifications are of the form `S*R?F*` then `layout_right`
- else `layout_stride`

Case `layout_left`:
- if slice specifications are of the form `F*R?S*` then `layout_left`
- else `layout_stride`

Further rules could be added considering the rank and some compile-time values in the extent.

# Implementation details

- the mapping is implemented once for any contiguous dimension (class `contiguous_mapping`)
- all strides are stored, even the compile-time known unit stride
