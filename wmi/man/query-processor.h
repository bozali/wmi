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

	QueryStream Get();

	void SetResource(std::shared_ptr<ManagementResource> resource);
	void SetQuery(const char* query);

	std::shared_ptr<ManagementResource> Resource() const;
	const char* Query() const;

private:
	std::shared_ptr<ManagementResource> resource_;

	EnumerationOptions options_;
	const char* query_;
};

}


