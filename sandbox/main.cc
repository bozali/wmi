#define _WIN32_DCOM

#include <comdef.h>
#include <WbemIdl.h>

#pragma once(lib, "wbemuuid.lib")

#include <wmi/core/exports.h>
#include <wmi/core/com-manager.h>
#include <wmi/man/management-resource.h>
#include <wmi/man/query-processor.h>
#include <wmi/man/query-reader.h>

#include <iostream>
#include <memory>
#include <vector>


int main()
{
	wmi::ComManager::Initialize();

	auto context = std::make_shared<wmi::ManagementContext>();
	auto resource = std::make_shared<wmi::ManagementResource>(context);
	resource->Connect("ROOT\\CIMV2");

	auto processor = std::make_shared<wmi::QueryProcessor>(resource, "SELECT * FROM Win32_Process");
	auto reader = processor->Get();

	std::vector<wmi::ResultObject> results;
	std::copy(std::begin(reader), std::end(reader), std::back_inserter(results));
	std::cout << results.size() << "\n";

	return 0;
}