#include <wmi/common/secure-string-access.h>
#include <wmi/management/management-query-processor.h>
#include <wmi/management/management-resource.h>
#include <wmi/management/enumeration-options.h>
#include <wmi/common/secure-string.h>
#include <wmi/common/com-exception.h>
#include <wmi/common/com-manager.h>

#include <iostream>
#include <string>
#include <memory>


int main()
{
	try
	{
		wmi::ComManager::Initialize();

		auto resource = std::make_unique<wmi::ManagementResource>(TEXT("root\\cimv2"));
		resource->Connect();

		auto query_processor = resource->GetQueryProcessor(TEXT("SELECT * FROM Win32_Processor"));
		auto stream = query_processor->GetStream();

		for (auto object : stream)
		{
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
