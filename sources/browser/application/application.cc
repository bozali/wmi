#include <browser/application/application.h>
#include <browser/application/window.h>

#include <browser/glad/glad.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>

using namespace browser;


Application::Application()
{
}


Application::~Application()
{
  ImGui_ImplSDL2_Shutdown();
  ImGui_ImplOpenGL3_Shutdown();

  ImGui::DestroyContext();
}


int Application::Run()
{
  Initialize();

  while (!window_->IsClosed())
  {
    HandleEvents();
    Render();
  }

  return 0;
}


void Application::HandleEvents()
{
  SDL_Event event;

  while (SDL_PollEvent(&event))
  {
    switch (event.type)
    {
    case SDL_QUIT:
      window_->Close();
      break;
    }
  }

}


void Application::Render()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL2_NewFrame(window_->window_);
  ImGui::NewFrame();

  // TODO Rendering...

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  window_->Present();
}


void Application::Initialize()
{
  window_ = std::make_unique<Window>("WMI Browser", 1024, 720);

  ImGui::CreateContext();

  ImGui_ImplSDL2_InitForOpenGL(window_->window_, window_->context_);
  ImGui_ImplOpenGL3_Init("#version 130");
}
