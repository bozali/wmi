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

	std::unordered_map<std::string, wmi::ManagementVariant> parameters;
	parameters["CommandLine"] = bstr_t("notepad.exe").Detach();

	auto result = resource->ExecuteMethod("Win32_Process", "Create", parameters);

	if (result.Get<LONG>("ReturnValue") == 0) {
		std::cout << "Successfully created process with the process id: " << result.Get<LONG>("ProcessId") << "\n";
	}

	return 0;
}