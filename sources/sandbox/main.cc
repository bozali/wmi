#include <iostream>

#include <wmi/common/com-manager.h>

#include <wmi/man/man-query-processor.h>
#include <wmi/man/query-iterator.h>
#include <wmi/man/man-resource.h>


int main()
{
  wmi::ComManager::Initialize();

  {
    wmi::ManagementResource resource("ROOT\\CIMV2");
    resource.Connect();


    wmi::ManagementQueryProcessor processor(resource, "SELECT * FROM Win32_Process");
    auto stream = processor.GetStream();
    
    for (auto object : stream) {
      std::wcout << object["Name"].bstrVal << "\n";
    }

  }

  wmi::ComManager::Uninitialize();

  return 0;
}
