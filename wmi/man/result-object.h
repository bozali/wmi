#pragma once

#include <wmi/core/exports.h>
#include <wmi/core/def.h>


namespace wmi {

class WMI_DLL ResultObject
{
public:
	ResultObject() = default;

	ResultObject(const ResultObject&) = default;
	ResultObject& operator=(const ResultObject&) = default;

	template <typename T>
	WMI_NODISCARD inline T Get(const char* property_name) const {
		return std::get<T>(InternalGet(property_name));
	}

	void Properties();

private:
	explicit ResultObject(ComPtr<IWbemClassObject> object) noexcept;

	WMI_NODISCARD ManagementVariant InternalGet(const char* property_name) const;

	ComPtr<IWbemClassObject> object_;

	friend class QueryReader;
};

}
