#include <wmi/core/com-manager.h>
#include <wmi/man/query-iterator.h>
#include <wmi/man/query-processor.h>

#include <iostream>


int main()
{
	auto token = wmi::ComManager::InitializeWithToken();

	wmi::ManagementContext context;
	auto resource = std::make_shared<wmi::ManagementResource>(context);
	resource->Connect("ROOT\\CIMV2");

	auto commandline = bstr_t("notepad.exe");

	std::unordered_map<std::string, wmi::ManagementVariant> parameters;
	parameters["CommandLine"] = commandline;

	resource->ExecuteMethod("Win32_Process", "Create", parameters);

	return 0;
}