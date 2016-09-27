//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include "config.hpp"
#include "pph.hpp"

namespace match3 {

class board {
 public:
  using color_t = int;

 private:
  /**
   * Row view
   *
   * @param view view
   * @param n row number
   *
   * \code{.cpp}
   *
   * | 1 2 3 |           |  1   2   3  |
   * | 3 4 5 | => n:1 -> | [3] [4] [5] |
   * | 6 7 8 |           |  6   7   8  |
   *
   * assert(row([1, 2, 3, 4, 5, 6, 7, 8], 1, 3), [3, 4, 5]);
   *
   * \endcode
   *
   * @return row view
   */
  auto row(const int n) const {
    return grids | ranges::view::drop(width * n) |
           ranges::view::take(width);  // or slice
  }

  /**
   * Column view
   *
   * @param view view
   * @param n column number
   *
   * \code{.cpp}
   *
   * | 1 2 3 |           | 1 [2] 3 |
   * | 3 4 5 | => n:1 -> | 3 [4] 5 |
   * | 6 7 8 |           | 6 [7] 8 |
   *
   * assert(col([1, 2, 3, 4, 5, 6, 7, 8], 1, 3), [2, 4, 7]);
   *
   * \endcode
   *
   * @return column view
   */
  auto col(const int n) const {
    return grids | ranges::view::drop(n) | ranges::view::stride(width);
  }

  auto col2(const int n) {
    return grids | ranges::view::drop(n) | ranges::view::stride(width);
  }

  /**
   * Look for at least n consecutive same elements
   *
   * @param view view
   * @param color element to be looked for
   *
   * \code{.cpp}
   *
   * | 1 3 3 3 2 1 | => color:3, n:3 -> {begin: 1, length: 3}
   * | 1 2 3 3 3 3 | => color:3, n:3 -> {begin: 2, length: 4}
   *
   * assert(match_n([1, 3, 3, 3, 2], 3), {1, 3});
   *
   * \endcode
   *
   * @return column view
   */
  template <class TView>
  auto match_n(TView&& view, const color_t color) const {
    const auto&& matches =
        ranges::view::ints |
        ranges::view::take(ranges::size(view) - max_match_length + 1) |
        ranges::view::transform([=](const auto i) {
          return ranges::count(view | ranges::view::drop(i) |
                                   ranges::view::take(max_match_length),
                               color) == max_match_length;
        });

    constexpr auto is_match = true;
    const auto it = ranges::find(matches, is_match);
    const auto found = it != ranges::end(matches);
    const auto mlength =
        found ? ranges::count(matches, is_match) + (max_match_length - 1) : 0;
    const auto mbegin =
        found ? ranges::distance(ranges::begin(matches), it) : 0;

    struct {
      decltype(mbegin) begin;
      decltype(mlength) length;
    } result{mbegin, mlength};
    return result;
  }

 public:
  board(const std::vector<color_t>& grids, const config c)
      : grids(grids),
        width(c.board_width),
        max_match_length(c.max_match_length) {}

  void swipe(const color_t lhs, const color_t rhs) {
    std::swap(grids[lhs], grids[rhs]);
  }

  /**
   * Checks whether there is a match for a given position
   *
   * @param value position value
   *
   * \code{.cpp}
   *
   * | 1 2 3 4 5 |                | 1 2 3  4  5 |    | 1 2 3  4  5 |
   * | 6 7 7 9 3 |                | 6 7 7  9  3 |    | 6 7 7  9  3 |
   * | 2 3 4 5 3 | => value:13 -> | 2 3 4 [5] 3 | => | 2 3 4 [5] 3 | => true
   * | 2 1 3 5 1 |                | 2 1 3  5  1 |    | 2 1 3 [5] 1 |
   * | 4 2 1 5 8 |                | 4 2 1  5  8 |    | 4 2 1 [5] 8 |
   *
   * | 1 2 3 4 5 |                | 1 2 [3] 4 5 |    | 1 2 3 4 5 |
   * | 6 7 7 9 3 |                | 6 7  7  9 3 |    | 6 7 7 9 3 |
   * | 2 3 4 5 3 | => value:2 ->  | 2 3  4  5 3 | => | 2 3 4 5 3 | => false
   * | 2 1 3 5 1 |                | 2 1  3  5 1 |    | 2 1 3 5 1 |
   * | 4 2 1 5 8 |                | 4 2  1  5 8 |    | 4 2 1 5 8 |
   *
   * assert(is_match([1, 2, 3, 4, 2, 6, 7, 2, 9], 1, 3));
   * assert(!is_match([1, 2, 3, 4, 3, 6, 7, 2, 9], 1, 3));
   *
   * \endcode
   *
   * @return true, if there is a match, false otherwise
   */
  auto is_match(const int position) const {
    const auto color = grids[position];
    const auto x = position % width;
    const auto y = position / width;
    return match_n(row(y), color).length || match_n(col(x), color).length;
  }

  // clang-format off
  /**
   * Creates a sorted and unique list of positions which matches
   *
   * @param view view`
   * @param value position value
   *
   * \code{.cpp}
   *
   * | 1 2 3 4 5 |                 | 1  2 3  4  5 |    | 1  2   3   4  5 |
   * | 6 7 7 9 3 |                 | 6  7 7  9  3 |    | 6  7   7   9  3 |
   * | 2 5 5 5 3 |  => value:13 -> | 2  5 5 [5] 3 | => | 2 [5] [5] [5] 3 | => [11, 12, 13, 18, 23]
   * | 2 1 3 5 1 |                 | 2  1 3  5  3 |    | 2  1   3  [5] 3 |
   * | 4 2 1 5 8 |                 | 4  2 1  5  8 |    | 4  2   1  [5] 8 |
   *
   * assert(match([1, 2, 3, 4, 4, 3, 7, 2, 3], 2, 3) == [2, 5, 8]);
   * assert(!match([1, 2, 3, 4, 4, 3, 7, 2, 1], 2, 3) == []);
   *
   * \endcode
   *
   * @return sorted, unique list of positions which matches
   */
  // clang-format on
  auto match(/*const*/ int position) const {
    const auto color = grids[position];
    const auto x = position % width;
    const auto y = position / width;
    const auto match_r = match_n(row(y), color);
    const auto match_c = match_n(col(x), color);
    const auto transform = [](auto length, auto expr) {
      return ranges::view::ints | ranges::view::take(length) |
             ranges::view::transform(expr);
    };
    std::vector<decltype(position)> result = ranges::view::concat(
        transform(
            match_r.length,
            [=](const auto i) { return y * width + match_r.begin + i; }),
        transform(match_c.length, [=](const auto i) {
          return (match_c.begin + i) * width + x;
        }));
    result |= ranges::action::sort | ranges::action::unique;
    return result;
  }

  /**
   * Scrolls given view
   *
   * \code{.cpp}
   *
   * | 3 |                |  3  |    | 0 |
   * | 0 |                | [0] |    | 0 |
   * | 0 |  => value:1 -> |  0  | => | 0 |
   * | 0 |                |  0  |    | 3 |
   * | 4 |                |  4  |    | 4 |
   *
   * assert(scroll([3, 0, 0, 0, 4], 1, 3) == [0, 0, 0, 3, 4]);
   *
   * \endcode
   *
   * @param position position
   */
  auto scroll(const int position) {
    auto&& c =
        col2(position % width) | ranges::view::take(position / width + 1);
    auto begin = ranges::begin(c);
    ranges::advance(begin, position / width);
    ranges::rotate(c, begin);
  }

  // clang-format off
  /**
   * Creates a list of elements which could generate new matches
   *
   * @param matches list of matches
   *
   * \code{.cpp}
   *
   *                         | 1  2   3   4  5 |    | 1  [2] [3] [4] 5 |
   *                         | 6  7   7   9  3 |    | 6  [7] [7] [9] 3 |
   * [11, 12, 13, 18, 23] => | 2 [5] [5] [5] 3 | => | 2  [5] [5] [5] 3 | => [1, 2, 3, 6, 7, 8, 11, 12, 13, 18, 23]
   *                         | 2  1   3  [5] 3 |    | 2   1   3  [5] 3 |
   *                         | 4  2   1  [5] 8 |    | 4   2   1  [5] 8 |
   *
   * assert(affected([0, 1, 2], 3) == [0, 1, 2]);
   *
   * \endcode
   *
   * @return sorted, unique list of positions
   */
  // clang-format on
  auto update(const int position, const color_t/* color*/) {
    //grids[position] = color;
    return ranges::view::ints | ranges::view::take(position / width + 1) |
           ranges::view::transform(
               [=](const auto i) { return position % width + (i * width); });
  }

  const auto& operator[](const int i) const { return grids[i]; }

 //private:
  std::vector<color_t> grids;
  int width = 0;
  int max_match_length = 0;
};

}  // match3
