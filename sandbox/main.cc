#include <wmi/core/com-exception-factory.h>
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

	wmi::QueryProcessor processor(resource, "SELECT * FROM Win32_WMISetting");

	try {
		auto stream = processor.GetStream();
		auto object = stream.Current();

		std::cout << object.Get<LONG>("LoggingLevel") << "\n";
		
		object.Set("LoggingLevel", static_cast<LONG>(0));
		object.Put();

		std::cout << object.Get<LONG>("LoggingLevel") << "\n";
	}
	catch (wmi::ComException ex) {
		std::wcout << ex.Detailed() << "\n";
		std::cout << ex.What() << "\n";
	}


	return 0;
}