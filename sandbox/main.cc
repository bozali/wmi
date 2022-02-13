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


int main()
{
	wmi::ComManager::Initialize();

	auto context = std::make_shared<wmi::ManagementContext>();
	auto resource = std::make_shared<wmi::ManagementResource>(context);
	resource->Connect("ROOT\\CIMV2");

	auto processor = std::make_shared<wmi::QueryProcessor>(resource, "SELECT * FROM Win32_Process");
	auto reader = processor->Get();

	auto obj = reader.Next();

	auto name = obj.Get<BSTR>("Name");
	
	std::wcout << name << "\n";


	return 0;
}