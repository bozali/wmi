#include <browser/application/application.h>

#undef main


int main()
{
  browser::Application application;
  return application.Run();
}
