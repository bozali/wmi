#include <iostream>

#include <wmi/common/com-manager.h>
#include <wmi/common/com-exception.h>

#include <wmi/man/query-iterator.h>
#include <wmi/man/query-stream.h>
#include <wmi/man/man-resource.h>
#include <wmi/man/man-object.h>
#include <wmi/man/man-query-processor.h>



struct Win32Process
{
  bstr_t name;
};

void MapManagementObject(const wmi::ManagementObject& from, Win32Process& to)
{
  to.name = from["Name"].bstrVal;
}


struct Win32OperatingSystem
{
  bstr_t name;
};

void MapManagementObject(const wmi::ManagementObject& from, Win32OperatingSystem& to)
{
  to.name = from["Name"].bstrVal;
}


int main()
{
  wmi::ComManager::Initialize();

  try {
    wmi::ManagementResource resource("ROOT\\CIMV2");
    resource.Connect();

    wmi::ManagementQueryProcessor processor(resource, "SELECT * FROM Win32_Process");
    
    auto stream = processor.GetStream<Win32Process>();
    auto object = stream.Current();

    std::wcout << object.name << "\n";
  }
  catch (const wmi::ComException& ex) {
    std::wcout << ex.Detailed() << "\n";
    std::cout << ex.What() << "\n";
  }

  wmi::ComManager::Uninitialize();

  return 0;
}
