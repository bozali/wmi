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



int main()
{
	try
	{
		wmi::ComManager::Initialize();


		auto resource = std::make_unique<wmi::ManagementResource>(TEXT("root\\cimv2"));
		resource->Connect();

		wmi::ManagementObject::ParameterSet parameters
		{
			{ TEXT("CommandLine"), wmi::Variant(wmi::BasicString(TEXT("notepad.exe"))) }
		};

		resource->ExecuteMethod(TEXT("Win32_Process"), TEXT("Create"), parameters);

		/*
		std::variant<bstr_t, int8_t, int16_t, int32_t, uint64_t, uint8_t, uint16_t, uint32_t, uint64_t> v1;

		variant_t v2(TEXT("MyV2Text"));

		std::wcout << v2.bstrVal << std::endl;

		v1 = TEXT("MyText");

		std::wcout << std::get<bstr_t>(v1) << std::endl;
		*/

		/*
		auto resource = std::make_unique<wmi::ManagementResource>(TEXT("root\\cimv2"));
		resource->Connect();

		auto query_processor = resource->GetQueryProcessor(TEXT("SELECT * FROM Win32_Processor"));
		auto stream = query_processor->GetStream();
		*/
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
