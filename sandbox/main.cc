#include <wmi/core/com-exception-factory.h>
#include <wmi/core/com-manager.h>
#include <wmi/man/query-iterator.h>
#include <wmi/man/query-processor.h>

#include <iostream>


int main()
{
	auto token = wmi::ComManager::InitializeWithToken();

	wmi::ManagementContext context;
	wmi::ManagementResource resource(context);
	resource.Connect("ROOT\\CIMV2");

	wmi::QueryProcessor processor(resource, "SELECT * FROM Win32_Process");

	try {
		auto stream = processor.GetStream();
		auto object = stream.Current();

		for (auto result : stream) {
			std::wcout << result["Name"].bstrVal << "\n";
		}
	}
	catch (wmi::ComException ex) {
		std::wcout << ex.Detailed() << "\n";
		std::cout << ex.What() << "\n";
	}


	return 0;
}