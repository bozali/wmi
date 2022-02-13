#pragma once

#include <wmi/core/process-security.h>
#include <wmi/core/exports.h>

#include <optional>
#include <memory>

namespace wmi {

struct WMI_DLL ComReleaseToken final
{
	~ComReleaseToken();
};

class WMI_DLL ComManager
{
public:
	static std::unique_ptr<ComReleaseToken> InitializeWithToken(std::optional<ProcessSecurity> security = std::nullopt);

	static void Initialize(std::optional<ProcessSecurity> security = std::nullopt);
	static void Unitialize();
};

}
