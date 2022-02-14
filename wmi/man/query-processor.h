#pragma once

#include <wmi/man/management-resource.h>
#include <wmi/man/enumeration-options.h>
#include <wmi/man/query-stream.h>

#include <optional>
#include <chrono>


namespace wmi {

class WMI_DLL QueryProcessor
{
public:
	QueryProcessor(std::shared_ptr<ManagementResource> resource, const char* query, std::optional<EnumerationOptions> options = std::nullopt);

	QueryStream GetStream();

	inline void SetResource(std::shared_ptr<ManagementResource> resource) {
		resource_ = resource;
	}

	inline void SetQuery(const char* query) {
		query_ = query;
	}

	WMI_NODISCARD inline std::shared_ptr<ManagementResource> Resource() const {
		return resource_;
	}

	WMI_NODISCARD inline const char* Query() const {
		return query_;
	}

private:
	std::shared_ptr<ManagementResource> resource_;

	EnumerationOptions options_;
	const char* query_;
};

}


