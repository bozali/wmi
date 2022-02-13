#pragma once

#include <wmi/core/exports.h>
#include <wmi/core/def.h>

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

	WMI_NODISCARD std::vector<bstr_t> PropertyNames();

	const ManagementVariant operator[](const char* property_name) const;

private:
	explicit ResultObject(ComPtr<IWbemClassObject> object) noexcept;
	ResultObject() = default;

	WMI_NODISCARD ManagementVariant InternalGet(const char* property_name) const;

	ComPtr<IWbemClassObject> object_;

	friend class QueryStream;
};

}
