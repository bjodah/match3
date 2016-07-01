/// Events

struct touch_down {
  static constexpr auto id = SDL_FINGERDOWN;
  explicit touch_down(const SDL_Event& event)
      : x(int(event.tfinger.x)), y(int(event.tfinger.y)) {}
  const int x = 0;
  const int y = 0;
};

struct button_down {
  static constexpr auto id = SDL_MOUSEBUTTONDOWN;
  explicit button_down(const SDL_Event& event)
      : x(event.button.x), y(event.button.y) {}
  const int x = 0;
  const int y = 0;
};

struct touch_up {
  static constexpr auto id = SDL_FINGERUP;
  explicit touch_up(const SDL_Event& event)
      : x(int(event.tfinger.x)), y(int(event.tfinger.y)) {}
  const int x = 0;
  const int y = 0;
};

struct button_up {
  static constexpr auto id = SDL_MOUSEBUTTONUP;
  explicit button_up(const SDL_Event& event)
      : x(event.button.x), y(event.button.y) {}
  const int x = 0;
  const int y = 0;
};

struct matches {
  static constexpr auto id = __LINE__;
  int arity = 0;
  mutable std::vector<int> matches;
};

struct time_tick {
  static constexpr auto id = __LINE__;
};

struct quit {
  static constexpr auto id = SDL_QUIT;
};

struct key_pressed {
  static constexpr auto id = SDL_KEYDOWN;
  explicit key_pressed(const SDL_Event& event) : key(event.key.keysym.sym) {}
  int key = 0;
};

