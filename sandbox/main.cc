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

	wmi::QueryProcessor processor(resource, "SELECT * FROM Win32_Process");

	auto stream = processor.GetStream();
	auto object = stream.Current();

	std::unordered_map<std::string_view, wmi::ManagementVariant> parameters;
	// parameters["CommandLine"] = bstr_t("notepad.exe").Detach();

	auto result = object.ExecuteMethod("GetOwner");
	std::cout << result.Get<BSTR>("User") << "\n";

	/*
	std::unordered_map<std::string_view, wmi::ManagementVariant> parameters;
	parameters["CommandLine"] = bstr_t("notepad.exe").Detach();

	auto result = resource->ExecuteMethod("Win32_Process", "Create", parameters);

	if (result.Get<LONG>("ReturnValue") == 0) {
		std::cout << "Successfully created process with the process id: " << result.Get<LONG>("ProcessId") << "\n";
	}
	*/

	return 0;
}