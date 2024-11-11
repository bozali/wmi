#pragma once

#include <wmi/common/exports.h>
#include <wmi/common/defs.h>

#include <wmi/management/enumeration-options.h>


namespace wmi {

class ManagementResource;

class WMI_DLL ManagementQueryProcessor
{
public:
	_WMI_FORCEINLINE void SetQuery(const bstr_t query) noexcept {
		query_ = query;
	}

	_WMI_ATTR_NODISCARD _WMI_FORCEINLINE const bstr_t GetQuery() const noexcept {
		return query_;
	}

private:
	ManagementQueryProcessor(const ManagementResource& resource, const bstr_t query, EnumerationOptions enumeration_options) noexcept;

private:
	const EnumerationOptions enumeration_options_;
	const ManagementResource* const resource_;
	bstr_t query_;

	friend class ManagementResource;
};

}
