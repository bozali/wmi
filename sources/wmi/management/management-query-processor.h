#pragma once

#include <wmi/common/exports.h>
#include <wmi/common/defs.h>

#include <wmi/management/enumeration-options.h>
#include <wmi/management/management-resource.h>
#include <wmi/management/management-query-stream.h>
#include <wmi/management/management-query-iterator.h>


namespace wmi {

class WMI_DLL ManagementQueryProcessor
{
public:
	ManagementQueryProcessor(const ManagementResource& resource, const bstr_t query, const EnumerationOptions enumeration_options) noexcept;
	ManagementQueryProcessor(const ManagementResource& resource, const bstr_t query) noexcept;


	_WMI_ATTR_NODISCARD _WMI_FORCEINLINE ManagementQueryStream<ManagementObject> GetStream() noexcept(false)
	{
		return ManagementQueryStream<ManagementObject>(resource_->services_, InternalQueryExecute(), enumeration_options_);
	}

	template <typename T>
	_WMI_ATTR_NODISCARD _WMI_FORCEINLINE ManagementQueryStream<T> GetStream()
	{
		return ManagementQueryStream<T>(resource_->services_, InternalQueryExecute(), enumeration_options_);
	}

	// template <typename T>
	// _WMI_ATTR_NODISCARD _WMI_FORCEINLINE ManagementQueryStream<T, MappedManagementQueryIterator<T>> GetStream() noexcept(false)
	// {
	// 	return ManagementQueryStream<T, MappedManagementQueryIterator<T>>(resource_->services_, InternalQueryExecute(), enumeration_options_);
	// }

	_WMI_FORCEINLINE void SetQuery(const bstr_t query) noexcept {
		query_ = query;
	}

	_WMI_ATTR_NODISCARD _WMI_FORCEINLINE const bstr_t GetQuery() const noexcept {
		return query_;
	}

private:
	_WMI_ATTR_NODISCARD microsoft::com_ptr<IEnumWbemClassObject> InternalQueryExecute() noexcept(false);

private:
	const EnumerationOptions enumeration_options_;
	const ManagementResource* const resource_;
	bstr_t query_;

	friend class ManagementResource;
};

}
