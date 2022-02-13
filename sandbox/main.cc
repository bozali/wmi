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

	auto processor = std::make_shared<wmi::QueryProcessor>(resource, "SELECT * FROM Win32_Process");
	auto stream = processor->GetStream();

	//std::vector<wmi::ResultObject> objects;
	//std::copy(std::begin(stream), std::end(stream), std::back_inserter(objects));

	auto found = std::find_if(std::begin(stream), std::end(stream), [](const wmi::ResultObject& object)
							  {
								  auto str = object.Get<BSTR>("Name");
								  return wcscmp(str, L"Code.exe") == 0;
							  });

	if (found != std::end(stream)) {
		std::cout << "Found\n";
	}

	return 0;
}