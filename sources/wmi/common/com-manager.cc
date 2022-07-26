#include <wmi/common/com-manager.h>
#include <wmi/common/com-exception.h>

#include <Windows.h>


using namespace wmi;


void ComManager::Initialize(std::optional<ProcessSecurity> security)
{
  auto security_settings = security.has_value() ? security.value() : ProcessSecurity();

  ComExceptionFactory::ThrowIfFailed(CoInitializeEx(nullptr, COINIT_MULTITHREADED));

  auto result = CoInitializeSecurity(nullptr,
                                     -1,
                                     nullptr,
                                     nullptr,
                                     static_cast<DWORD>(security_settings.authentication),
                                     static_cast<DWORD>(security_settings.impersonation),
                                     nullptr,
                                     EOAC_NONE,
                                     nullptr);

  ComExceptionFactory::ThrowIfFailed(result);
}


void ComManager::Uninitialize()
{
  CoUninitialize();
}
