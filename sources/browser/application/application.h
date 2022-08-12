#pragma once

#include <string_view>
#include <memory>


namespace browser {

class Window;


class Application
{
public:
  Application();
  ~Application();

  int Run();

  void HandleEvents();
  void Render();

private:
  Application(const Application&) = default;
  Application& operator=(const Application&) = default;

  void Initialize();


  std::unique_ptr<browser::Window> window_;
};

}
