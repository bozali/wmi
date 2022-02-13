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
	ManagementResource(const ManagementContext& context);

	void Connect(const char* path, std::optional<ConnectionOptions> options = std::nullopt);

	const ManagementContext& Context() const;

private:
	const ManagementContext& context_;

	ComPtr<IWbemServices> services_;

	friend class QueryProcessor;
};

}
