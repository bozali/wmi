#include <iostream>

#include <browser/application/window.h>

#include <memory>

#undef main


int main()
{
  auto window = std::make_unique<browser::Window>("WMI Browser", 1024, 720);

  while (!window->IsClosed())
  {
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
      case SDL_QUIT:
        window->Close();
        break;
      }
    }

    window->Present();
  }


  return 0;
}
