/*
 * file generated by gmock: iviewer_mock.hpp
 */
#ifndef IVIEWER_MOCK_HPP
#define IVIEWER_MOCK_HPP

#include <gmock/gmock.h>
#include "iviewer.hpp"

namespace game {
namespace mocks {

class iviewer_mock : public iviewer
{
public:
    virtual ~iviewer_mock() { }
    MOCK_METHOD0(quit, void());
    MOCK_METHOD1(render, void(sdl::milliseconds_t));
    MOCK_METHOD0(clear_board, void());
    MOCK_METHOD2(show_grid, void(const game::position &, color_t));
    MOCK_METHOD2(scroll_grid, void(const game::position &, color_t));
    MOCK_METHOD1(show_match, void(const game::position &));
    MOCK_METHOD1(select_item, void(const game::position &));
    MOCK_METHOD1(show_time, void(int));
    MOCK_METHOD1(show_points, void(int));
    MOCK_METHOD1(show_results, void(int));
    MOCK_METHOD1(fade_screen, void(sdl::milliseconds_t));
};

} // namespace mocks
} // namespace game

#endif // IVIEWER_MOCK_HPP

