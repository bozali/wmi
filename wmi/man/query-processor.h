#pragma once

#include <wmi/man/management-resource.h>
#include <wmi/man/enumeration-options.h>
#include <wmi/man/query-stream.h>

#include <optional>


namespace wmi {

class WMI_DLL QueryProcessor
{
public:
	QueryProcessor(const ManagementResource& resource, const char* query, std::optional<EnumerationOptions> options = std::nullopt);

	WMI_NODISCARD QueryStream GetStream();

	inline void SetResource(const ManagementResource& resource) {
		resource_ = &resource;
	}

	inline void SetQuery(const char* query) {
		query_ = query;
	}

	WMI_NODISCARD inline const ManagementResource* const Resource() const {
		return resource_;
	}

	WMI_NODISCARD inline const char* Query() const {
		return query_;
	}

private:
	const ManagementResource* resource_;
	EnumerationOptions options_;
	const char* query_;
};

}


