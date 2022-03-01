#pragma once

#include <wmi/core/exports.h>
#include <wmi/core/def.h>

#include <unordered_map>
#include <optional>
#include <vector>


namespace wmi {

class WMI_DLL ResultObject
{
public:
	ResultObject(const ResultObject&) = default;
	ResultObject& operator=(const ResultObject&) = default;

	void Set(const char* property_name, variant_t value);
	void Put();

	ResultObject ExecuteMethod(const char* method_name, std::optional<std::unordered_map<std::string_view, variant_t>> parameters = std::nullopt);

	WMI_NODISCARD std::vector<bstr_t> PropertyNames();

	WMI_NODISCARD const variant_t operator[](const char* property_name) const;

private:
	explicit ResultObject(ComPtr<IWbemServices> services, ComPtr<IWbemClassObject> object) noexcept;
	ResultObject() = default;

	ComPtr<IWbemClassObject> object_;
	ComPtr<IWbemServices> services_;

	friend class QueryStream;
	friend class ManagementResource;
};

}
