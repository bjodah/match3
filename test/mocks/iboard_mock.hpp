/*
 * file generated by gmock: iboard_mock.hpp
 */
#ifndef IBOARD_MOCK_HPP
#define IBOARD_MOCK_HPP

#include <gmock/gmock.h>
#include "iboard.hpp"

namespace game {
namespace mocks {

class iboard_mock : public iboard
{
public:
    virtual ~iboard_mock() { }
    MOCK_METHOD0(init_with_randoms, void());
    MOCK_CONST_METHOD1(is_within_board, bool(const game::position &));
    MOCK_CONST_METHOD1(is_neighbor, bool(const game::position &));
    MOCK_CONST_METHOD1(is_same_selected, bool(const game::position &));
    MOCK_CONST_METHOD0(are_selected, bool());
    MOCK_METHOD1(is_same_color, bool(const game::position &));
    MOCK_METHOD0(is_swap_winning, bool());
    MOCK_METHOD0(new_randoms, std::set<position>());
    MOCK_METHOD0(matches, std::set<position>());
    MOCK_METHOD0(scroll_down, std::set<position>());
    MOCK_METHOD1(select, void(const game::position &));
    MOCK_METHOD1(unselect_item, void(std::size_t));
    MOCK_METHOD0(unselect_all, void());
    MOCK_METHOD0(swap, void());
    MOCK_METHOD2(set, void(const game::position &, const color_t &));
    MOCK_METHOD1(get_grid_color, color_t(const game::position &));
    MOCK_CONST_METHOD0(begin, const_iterator());
    MOCK_CONST_METHOD0(end, const_iterator());
};

} // namespace mocks
} // namespace game

#endif // IBOARD_MOCK_HPP

