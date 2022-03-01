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

	/**
	 * Connects to a WMI resource.
	 * 
	 * @param path The path to the management resource e.g. \\ROOT\CIMV2
	 * @param options Connection options.
	 * 
	 * @throw ComException Thrown if connection cannot be established.
	 */
	void Connect(const char* path, std::optional<ConnectionOptions> options = std::nullopt);

	/**
	 * Executes a WMI class method.
	 * 
	 * @param class_name Path to the class to execute method from e.g. \\ROOT\CIMV2\Win32_Process.
	 * @param method_name Name of the method to execute.
	 * @param parameters Input for the method, is optional.
	 * 
	 * @return Returns the result of the method execution.
	 */
	ResultObject ExecuteMethod(const char* class_name, const char* method_name, std::optional<std::unordered_map<std::string_view, variant_t>> parameters = std::nullopt);

	/**
	 * Creating WMI instance.
	 * 
	 * @param class_name Name of the class.
	 * 
	 * @return Returns the newly created instance.
	 */
	ResultObject CreateInstance(const char* class_name);

	/**
	 * The context.
	 * 
	 * @return Returns the context.
	 */
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
