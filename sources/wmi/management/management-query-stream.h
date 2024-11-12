#pragma once

#include <wmi/common/defs.h>
#include <wmi/common/com-exception.h>

#include <wmi/management/enumeration-options.h>
#include <wmi/management/management-object.h>

#include <type_traits>
#include <chrono>


namespace wmi {


template <typename T, typename = typename std::enable_if<std::is_default_constructible<T>::value>>
_WMI_FORCEINLINE static void HandleManagementObjectMapped(const ManagementObject& from, T& to) {}


template <typename T>
_WMI_ATTR_NODISCARD _WMI_FORCEINLINE T InternalManagementObjectHandler(const ManagementObject& object)
{
	T instance;
	HandleManagementObjectMapped(object, instance);
	return instance;
}


template <typename T>
class MappedManagementQueryIterator;
class ManagementObjectQueryIterator;


template <typename T, typename TIterator>
class ManagementQueryStream
{
public:
	using DataType = T;
	using Iterator = TIterator;

	_WMI_FORCEINLINE bool Next() noexcept(false)
	{
		microsoft::com_ptr<IWbemClassObject> object;
		ULONG returned = 0;

		using milliseconds = std::chrono::milliseconds;

		auto timeout = enumeration_options_.timeout.has_value()
			? std::chrono::duration_cast<milliseconds>(enumeration_options_.timeout.value()).count()
			: WBEM_INFINITE;

		ComExceptionFactory::ThrowIfFailed(enumerator_->Next(timeout, 1, object.GetAddressOf(), &returned));

		if (is_done_ = returned == 0)
		{
			return false;
		}

		current_ = ManagementObject(services_, object);

		return true;
	}

	_WMI_ATTR_NODISCARD _WMI_FORCEINLINE T Current() noexcept
	{
		if constexpr (std::is_same<TIterator, MappedManagementQueryIterator<T>>::value)
		{
			return InternalManagementObjectHandler<T>(current_);
		}
		
		return current_;
	}

	_WMI_ATTR_NODISCARD _WMI_FORCEINLINE bool IsDone() const noexcept
	{
		return is_done_;
	}

	_WMI_ATTR_NODISCARD _WMI_FORCEINLINE Iterator begin()
	{
		return Iterator(*this, false);
	}

	_WMI_ATTR_NODISCARD _WMI_FORCEINLINE Iterator end()
	{
		return Iterator(*this, true);
	}

private:
	ManagementQueryStream(microsoft::com_ptr<IWbemServices> services, microsoft::com_ptr<IEnumWbemClassObject> enumerator, EnumerationOptions options) noexcept(false)
		: enumerator_(enumerator)
		, services_(services)
		, enumeration_options_(options)
		, is_done_(false)
	{
		Next(); 
	}

private:
	microsoft::com_ptr<IEnumWbemClassObject> enumerator_;
	microsoft::com_ptr<IWbemServices> services_;

	EnumerationOptions enumeration_options_;
	ManagementObject current_;
	bool is_done_;

	friend class ManagementQueryProcessor;
};


template <typename T>
using MappedManagementQueryStream = ManagementQueryStream<T, MappedManagementQueryIterator<T>>;
using ManagementObjectQueryStream = ManagementQueryStream<ManagementObject, ManagementObjectQueryIterator>;

}