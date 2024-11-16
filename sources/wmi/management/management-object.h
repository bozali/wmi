#pragma once

#include <wmi/common/exports.h>
#include <wmi/common/defs.h>
#include <wmi/common/variant.h>
#include <wmi/common/basic-string.h>
#include <wmi/management/management-mapping.h>

#include <unordered_map>
#include <optional>
#include <vector>


namespace wmi {

class ManagementObject;

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


class WMI_DLL ManagementObject
{
public:
	using ParameterSet = std::unordered_map<BasicString, Variant, internal::BasicStringHash, internal::BasicStringEqual>;

	ManagementObject(const ManagementObject&) = default;
	ManagementObject& operator=(const ManagementObject&) = default;

	ManagementObjectProxy Proxy() const;

	void Set(const BasicString property_name, Variant value) noexcept(false);

	void Put() noexcept(false);

	ManagementObject ExecuteMethod(const BasicString method_name, std::optional<ParameterSet> parameters = std::nullopt) noexcept(false);

	_WMI_ATTR_NODISCARD const Variant operator[](const BasicString property_name) const noexcept;

private:
	explicit ManagementObject(microsoft::com_ptr<IWbemServices> services, microsoft::com_ptr<IWbemClassObject> object) noexcept;
	ManagementObject() = default;

private:
	microsoft::com_ptr<IWbemClassObject> object_;
	microsoft::com_ptr<IWbemServices> services_;

	template <typename T>
	friend class ManagementQueryStream;
	friend class ManagementResource;
	friend class ManagementObject;
};

}
