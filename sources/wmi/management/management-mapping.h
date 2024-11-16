#pragma once

#include <wmi/common/defs.h>

#include <type_traits>

namespace wmi {

class ManagementObject;

template <typename T, typename = typename std::enable_if<std::is_default_constructible<T>::value>>
_WMI_FORCEINLINE static void HandleManagementObjectMapped(const ManagementObject& from, T& to) {}


template <typename T>
_WMI_ATTR_NODISCARD _WMI_FORCEINLINE T InternalManagementObjectHandler(const ManagementObject& object)
{
	T instance;
	HandleManagementObjectMapped(object, instance);
	return instance;
}

}
