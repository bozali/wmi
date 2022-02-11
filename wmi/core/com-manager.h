#pragma once

#include <wmi/core/process-security.h>
#include <wmi/core/exports.h>

#include <optional>

#include <Windows.h>
#include <comdef.h>


namespace wmi {

class WMI_DLL ComManager
{
public:
	static void Initialize(std::optional<ProcessSecurity> security = std::nullopt);
};

}
