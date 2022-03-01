#pragma once

#include <wmi/man/management-context.h>
#include <wmi/man/connection-options.h>
#include <wmi/man/result-object.h>
#include <wmi/core/exports.h>

#include <unordered_map>
#include <optional>
#include <memory>


namespace wmi {

class WMI_DLL ManagementResource
{
public:
	explicit ManagementResource(const ManagementContext& context);

	void Connect(const char* path, std::optional<ConnectionOptions> options = std::nullopt);

	ResultObject ExecuteMethod(const char* class_name, const char* method_name, std::unordered_map<std::string_view, variant_t> parameters);

	ResultObject CreateInstance(const char* class_name);

	WMI_NODISCARD inline const ManagementContext& Context() const {
		return context_;
	}

private:
	ManagementResource(const ManagementResource&) = default;
	ManagementResource& operator=(const ManagementResource&) = default;

private:
	const ManagementContext& context_;

	ComPtr<IWbemServices> services_;

	friend class QueryProcessor;
};

}
