#pragma once

#include <SDL.h>
#include <imgui.h>

#include <string_view>



namespace browser {

class Window final
{
public:
  Window(const std::string_view title, int width, int height);
  ~Window();

  void Present();
  void Close() noexcept;

  _NODISCARD inline bool IsClosed() const noexcept {
    return is_closed_;
  }

private:
  Window(const Window&) = default;
  Window& operator=(Window&) = default;

  void Initialize(const std::string_view title, int width, int height);
  void Uninitialize();

  static SDL_HitTestResult SDLCALL HitTestCallback(SDL_Window* window, const SDL_Point* pt, void* data);


  bool is_closed_;

  SDL_Window* window_;
  SDL_GLContext context_;

  friend class Application;
};

}
