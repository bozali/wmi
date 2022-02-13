#include <wmi/core/exports.h>
#include <wmi/core/com-manager.h>
#include <wmi/man/management-resource.h>
#include <wmi/man/query-iterator.h>
#include <wmi/man/query-processor.h>
#include <wmi/man/query-stream.h>

#include <iostream>
#include <memory>
#include <vector>


int main()
{
	auto token = wmi::ComManager::InitializeWithToken();

	wmi::ManagementContext context;
	auto resource = std::make_shared<wmi::ManagementResource>(context);
	resource->Connect("ROOT\\CIMV2");

	auto processor = std::make_shared<wmi::QueryProcessor>(resource, "SELECT * FROM Win32_Process");
	auto reader = processor->Get();
	auto property = reader.Current();

	std::wcout << std::get<BSTR>(property["Name"]) << std::endl;


	return 0;
}