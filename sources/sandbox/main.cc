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


struct Win32_Processor
{
	wmi::BasicString name;
};


_WMI_FORCEINLINE static void HandleManagementObjectMapped(const wmi::ManagementObject& from, Win32_Processor& to) {
	to.name = std::get<wmi::BasicString>(from[TEXT("Name")]);
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
		auto resource = std::make_shared<wmi::ManagementResource>(TEXT("root\\cimv2"));
		resource->Connect();

		auto event_bus = resource->GetEventBus();

		auto token = event_bus->Subscribe(TEXT("SELECT * FROM __InstanceCreationEvent WITHIN 1 WHERE TargetInstance ISA 'Win32_Process'"), [](const wmi::ManagementObject& obj)
																			{
																				auto x = obj["TargetInstance"];
																				auto y = std::get<wmi::ManagementObject>(x);

																				std::wcout << std::get<wmi::BasicString>(y["__PATH"]) << std::endl;
																			});
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
