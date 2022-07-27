#include <iostream>

#include <wmi/common/com-manager.h>
#include <wmi/common/com-exception.h>

#include <wmi/man/man-query-processor.h>
#include <wmi/man/man-resource.h>


int main()
{
  wmi::ComManager::Initialize();

  try {
    wmi::ManagementResource resource("ROOT\\CIMV2");
    resource.Connect();

    wmi::ManagementQueryProcessor processor(resource, "SELECT * FROM Win32_Process");
    auto stream = processor.GetStream();
    
    for (auto object : stream) {
      std::wcout << object["Name"].bstrVal << "\n";
    }
  }
  catch (const wmi::ComException& ex) {
    std::wcout << ex.Detailed() << "\n";
    std::cout << ex.What() << "\n";
  }

  wmi::ComManager::Uninitialize();

  return 0;
}
