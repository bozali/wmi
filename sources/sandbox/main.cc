#include <iostream>

#include <wmi/common/com-manager.h>
#include <wmi/man/man-resource.h>


int main()
{
  wmi::ComManager::Initialize();

  {
    wmi::ManagementResource resource("ROOT\\CIMV2");
    resource.Connect();
  }

  wmi::ComManager::Uninitialize();

  return 0;
}
