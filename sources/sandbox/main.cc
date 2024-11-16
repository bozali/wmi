#include <wmi/common/secure-string-access.h>
#include <wmi/management/management-query-iterator.h>
#include <wmi/management/management-query-stream.h>
#include <wmi/management/management-resource.h>
#include <wmi/management/enumeration-options.h>
#include <wmi/common/secure-string.h>
#include <wmi/common/com-exception.h>
#include <wmi/common/com-manager.h>

#include <wmi/management/management-query-processor.h>

#include <unordered_map>
#include <iostream>
#include <string>
#include <memory>
#include <variant>


struct Win32_Processor
{
	wmi::BasicString name;
};


_WMI_FORCEINLINE static void HandleManagementObjectMapped(const wmi::ManagementObject& from, Win32_Processor& to) {
	to.name = std::get<wmi::BasicString>(from[TEXT("Name")]);
}


int main()
{
	try
	{
		wmi::ComManager::Initialize();

	}
	catch (wmi::ComException ex)
	{
		std::cout << ex.What() << std::endl;
		std::wcout << ex.Detailed() << std::endl;
	}
	catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
	}

	wmi::ComManager::Uninitialize();

	return 0;
}
