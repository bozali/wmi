#include <wmi/common/secure-string-access.h>
#include <wmi/common/secure-string.h>
#include <wmi/common/com-exception.h>
#include <wmi/common/com-manager.h>

#include <wmi/management/management-query-iterator.h>
#include <wmi/management/management-query-stream.h>
#include <wmi/management/management-resource.h>
#include <wmi/management/enumeration-options.h>
#include <wmi/management/management-query-processor.h>
#include <wmi/management/management-event-bus.h>
#include <wmi/management/management-subscription-token.h>

#include <unordered_map>
#include <iostream>
#include <string>
#include <memory>
#include <variant>
#include <thread>


struct Win32_Process
{
	wmi::UInt32 process_id;
	wmi::BasicString name;
};


_WMI_FORCEINLINE static void HandleManagementObjectMapped(const wmi::ManagementObject& from, Win32_Process& to) {
	to.process_id = std::get<wmi::Int32>(from["ProcessId"]);
	to.name = std::get<wmi::BasicString>(from["Name"]);
}


int main()
{
	try
	{
		wmi::ProcessSecurity security;
		security.authentication = wmi::AuthenticationLevel::kDefault;
		security.impersonation = wmi::ImpersonationLevel::kImpersonate;

		wmi::ComManager::Initialize(security);

		// TODO Must be shared_ptr make factory method...
		auto resource = std::make_shared<wmi::ManagementResource>("root\\cimv2");
		resource->Connect();

		auto query_processor = resource->GetQueryProcessor("SELECT * FROM Win32_Process");
		auto stream = query_processor->GetStream<Win32_Process>();

		auto found = std::find_if(stream.begin(), stream.end(), [&](Win32_Process process)
															{
																bool r = wcscmp(process.name, TEXT("code.exe")) == 0;

																return r;
															});

		if (found != stream.end())
		{
			// std::wcout << (*found).name << std::endl;
		}

		/*

		auto event_bus = resource->GetEventBus();

		auto token = event_bus->Subscribe(TEXT("SELECT * FROM __InstanceCreationEvent WITHIN 1 WHERE TargetInstance ISA 'Win32_Process'"), [](const wmi::ManagementObject& obj)
																			{
																				auto x = obj["TargetInstance"];
																				auto y = std::get<wmi::ManagementObject>(x);

																				auto process = y.Proxy().As< Win32_Process>();

																				std::wcout << process.process_id << std::endl;
																				std::wcout << process.name << std::endl;
																			});


		std::this_thread::sleep_for(std::chrono::seconds(15));
		*/
	}
	catch (const wmi::ComException& ex)
	{
		std::cout << ex.What() << std::endl;
		std::wcout << ex.Detailed() << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	wmi::ComManager::Uninitialize();

	return 0;
}
