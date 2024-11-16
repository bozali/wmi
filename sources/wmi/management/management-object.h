#pragma once

#include <wmi/common/exports.h>
#include <wmi/common/defs.h>

#include <vector>


namespace wmi {

class WMI_DLL ManagementObject
{
public:
	ManagementObject(const ManagementObject&) = default;
	ManagementObject& operator=(const ManagementObject&) = default;

	void Set(const bstr_t property_name, variant_t value) noexcept(false);

	void Put() noexcept(false);

	_WMI_ATTR_NODISCARD const variant_t operator[](const bstr_t property_name) const noexcept;

private:
	explicit ManagementObject(microsoft::com_ptr<IWbemServices> services, microsoft::com_ptr<IWbemClassObject> object) noexcept;
	ManagementObject() = default;

private:
	microsoft::com_ptr<IWbemClassObject> object_;
	microsoft::com_ptr<IWbemServices> services_;

	template <typename T>
	friend class ManagementQueryStream;
	friend class ManagementResource;
};

}
