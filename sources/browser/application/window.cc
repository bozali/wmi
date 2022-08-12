#include <browser/application/window.h>

#include <browser/glad/glad.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>

using namespace browser;

#define RESIZE_BORDER 10

const SDL_Rect kDragAreas[] = {
  { 20, 20, 100, 100 },
  { 200, 70, 100, 100 },
  { 400, 90, 100, 100 }
};

static const SDL_Rect* kAreas = kDragAreas;
static int kNumAreas = SDL_arraysize(kDragAreas);


Window::Window(const std::string_view title, int width, int height)
{
  Initialize(title, width, height);
}


Window::~Window()
{
  Uninitialize();
}


void Window::Present()
{
  SDL_GL_SwapWindow(window_);
}


void Window::Close() noexcept
{
  is_closed_ = true;
}


void Window::Initialize(const std::string_view title, int width, int height)
{
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER);

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

  window_ = SDL_CreateWindow("WMI Browser", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_BORDERLESS | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_OPENGL);
  SDL_SetWindowHitTest(window_, HitTestCallback, nullptr);
  SDL_SetWindowResizable(window_, SDL_TRUE);

  context_ = SDL_GL_CreateContext(window_);
  SDL_GL_MakeCurrent(window_, context_);
  SDL_GL_SetSwapInterval(1);

  gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
}


void Window::Uninitialize()
{
  SDL_GL_DeleteContext(context_);

  SDL_DestroyWindow(window_);
  SDL_Quit();
}


SDL_HitTestResult SDLCALL Window::HitTestCallback(SDL_Window* window, const SDL_Point* pt, void* data)
{
  int i, w, h;

  for (i = 0; i < kNumAreas; i++) {
    if (SDL_PointInRect(pt, &kAreas[i])) {
      return SDL_HITTEST_DRAGGABLE;
    }
  }

  SDL_GetWindowSize(window, &w, &h);

  if (pt->x < RESIZE_BORDER && pt->y < RESIZE_BORDER) {
    return SDL_HITTEST_RESIZE_TOPLEFT;
  }
  else if (pt->x > RESIZE_BORDER && pt->x < w - RESIZE_BORDER && pt->y < RESIZE_BORDER) {
    return SDL_HITTEST_RESIZE_TOP;
  }
  else if (pt->x > w - RESIZE_BORDER && pt->y < RESIZE_BORDER) {
    return SDL_HITTEST_RESIZE_TOPRIGHT;
  }
  else if (pt->x > w - RESIZE_BORDER && pt->y > RESIZE_BORDER && pt->y < h - RESIZE_BORDER) {
    return SDL_HITTEST_RESIZE_RIGHT;
  }
  else if (pt->x > w - RESIZE_BORDER && pt->y > h - RESIZE_BORDER) {
    return SDL_HITTEST_RESIZE_BOTTOMRIGHT;
  }
  else if (pt->x < w - RESIZE_BORDER && pt->x > RESIZE_BORDER && pt->y > h - RESIZE_BORDER) {
    return SDL_HITTEST_RESIZE_BOTTOM;
  }
  else if (pt->x < RESIZE_BORDER && pt->y > h - RESIZE_BORDER) {
    return SDL_HITTEST_RESIZE_BOTTOMLEFT;
  }
  else if (pt->x < RESIZE_BORDER && pt->y < h - RESIZE_BORDER && pt->y > RESIZE_BORDER) {
    return SDL_HITTEST_RESIZE_LEFT;
  }

  return SDL_HITTEST_NORMAL;

}
