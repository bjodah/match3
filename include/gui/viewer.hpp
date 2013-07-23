#ifndef VIEWER_ZTZFA603
#define VIEWER_ZTZFA603

#include <map>
#include <boost/shared_ptr.hpp>
#include <boost/di/ctor.hpp>
#include <boost/di/named.hpp>
#include <mpl_string.hpp>
#include "detail/position.hpp"
#include "sdl/iwindow.hpp"
#include "iviewer.hpp"

namespace game {
namespace gui {

class viewer : public iviewer
{
    const std::string images_dir          = "images/";
    const std::string background_image    = "background.png";
    const std::string select_image        = "select.png";
    const std::string match_image         = "match.png";

    enum {
        background_layer
      , board_layer
      , board_actions_layer
      , text_time_layer
      , text_points_layer
    };

public:
    BOOST_DI_CTOR(viewer
        , boost::shared_ptr<sdl::iwindow>
        , boost::di::named<int, _S("grid offset")>
        , boost::di::named<int, _S("grids offset x")>
        , boost::di::named<int, _S("grids offset y")>
        , boost::di::named<int, _S("board colors")>
        , boost::di::named<std::string, _S("font name")>
    );

    virtual void quit() override;
    virtual void render() override;
    virtual void clear_board() override;
    virtual void show_grid(const detail::position&, detail::color_t) override;
    virtual void show_match(const detail::position&) override;
    virtual void select_item(const detail::position&) override;
    virtual void show_time(const std::string& str, int x, int y, SDL_Color, int) override;
    virtual void show_points(const std::string& str, int x, int y, SDL_Color, int) override;

private:
    void show_text(const std::string& str, int x, int y, SDL_Color, int, std::size_t);

    boost::shared_ptr<sdl::iwindow> window_;
    boost::shared_ptr<SDL_Texture> background_image_;
    boost::shared_ptr<SDL_Texture> match_image_;
    boost::shared_ptr<SDL_Texture> select_image_;
    std::map<detail::color_t, boost::shared_ptr<SDL_Texture>> grid_images_;
    int grid_offset_ = 0;
    int grids_offset_x_ = 0;
    int grids_offset_y_ = 0;
    std::string font_ = "";
};

} // namespace gui
} // namespace game

#endif

