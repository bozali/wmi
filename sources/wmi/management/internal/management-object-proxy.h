#pragma once

#include <wmi/management/management-mapping.h>

#include <wmi/common/defs.h>

namespace wmi { class ManagementObject; }

namespace wmi::internal {

class ManagementObjectProxy
{
public:
	ManagementObjectProxy(const ManagementObject& object)
		: object_(object)
	{
	}

	template <typename T>
	_WMI_ATTR_NODISCARD _WMI_FORCEINLINE T As() const noexcept
	{
		return InternalManagementObjectHandler<T>(object_);
	}

private:
	const ManagementObject& object_;
};

}
