#include <wmi/common/secure-string-access.h>
#include <wmi/management/management-query-iterator.h>
#include <wmi/management/management-query-stream.h>
#include <wmi/management/management-resource.h>
#include <wmi/management/enumeration-options.h>
#include <wmi/common/secure-string.h>
#include <wmi/common/com-exception.h>
#include <wmi/common/com-manager.h>

#include <wmi/management/management-query-processor.h>

#include <iostream>
#include <string>
#include <memory>


struct Win32_Processor
{
	bstr_t name;
};

_WMI_FORCEINLINE static void HandleManagementObjectMapped(const wmi::ManagementObject& from, Win32_Processor& to) {
	to.name = from[TEXT("Name")].bstrVal;
}




int main()
{
	try
	{
		wmi::ComManager::Initialize();

		auto resource = std::make_unique<wmi::ManagementResource>(TEXT("root\\cimv2"));
		resource->Connect();

		auto query_processor = resource->GetQueryProcessor(TEXT("SELECT * FROM Win32_Processor"));
		auto stream = query_processor->GetStream();

		for (auto it : stream)
		{
			std::wcout << it[TEXT("Name")].bstrVal << std::endl;
		}
		
		auto mapped_stream = query_processor->GetStream<Win32_Processor>();

		for (auto it : mapped_stream)
		{
			std::wcout << it.name << std::endl;
		}

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
