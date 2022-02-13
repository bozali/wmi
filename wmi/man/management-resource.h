#pragma once

#include <wmi/man/management-context.h>
#include <wmi/man/connection-options.h>
#include <wmi/core/exports.h>

#include <optional>
#include <memory>


namespace wmi {

class WMI_DLL ManagementResource
{
public:
	ManagementResource(std::shared_ptr<ManagementContext> context);

	void Connect(const char* path, std::optional<ConnectionOptions> options = std::nullopt);

private:
	std::shared_ptr<ManagementContext> context_;

	ComPtr<IWbemServices> services_;

	friend class QueryProcessor;
};

}
