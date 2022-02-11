#include <wmi/core/com-manager.h>
#include <wmi/core/com-exception-factory.h>

using namespace wmi;


void ComManager::Initialize(std::optional<ProcessSecurity> security)
{
	ProcessSecurity security_settings = security.has_value() ? security.value() : ProcessSecurity();

	ComExceptionFactory::ThrowIfFailed(CoInitializeEx(nullptr, COINIT_MULTITHREADED));
	ComExceptionFactory::ThrowIfFailed(CoInitializeSecurity(nullptr,
															-1,
															nullptr,
															nullptr,
															static_cast<DWORD>(security_settings.authentication),
															static_cast<DWORD>(security_settings.impersonation),
															nullptr,
															EOAC_NONE,
															nullptr));
}
