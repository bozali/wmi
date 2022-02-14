#pragma once

#include <wmi/core/exports.h>
#include <wmi/core/def.h>

#include <wmi/man/management-variant.h>

#include <vector>


namespace wmi {

class WMI_DLL ResultObject
{
public:
	ResultObject(const ResultObject&) = default;
	ResultObject& operator=(const ResultObject&) = default;

	template <typename T>
	WMI_NODISCARD inline T Get(const char* property_name) const {
		return std::get<T>(InternalGet(property_name));
	}

	// void ExecuteMethod(const char* name);

	WMI_NODISCARD std::vector<bstr_t> PropertyNames();

	WMI_NODISCARD inline const ManagementVariant operator[](const char* property_name) const {
		return InternalGet(property_name);
	}

private:
	explicit ResultObject(ComPtr<IWbemServices> services, ComPtr<IWbemClassObject> object) noexcept;
	ResultObject() = default;

	WMI_NODISCARD ManagementVariant InternalGet(const char* property_name) const;

	ComPtr<IWbemClassObject> object_;
	ComPtr<IWbemServices> services_;

	friend class QueryStream;
};

}
