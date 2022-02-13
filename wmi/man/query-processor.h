#pragma once

#include <wmi/man/management-resource.h>
#include <wmi/man/query-reader.h>

#include <chrono>


namespace wmi {

struct EnumerationOptions
{
	bool direct_read;
	bool enumerate_deep;
	bool prototype_only;
	bool return_immediately;
	bool rewindable;
	bool use_amended_qualifiers;

	std::chrono::seconds timeout;
};


class WMI_DLL QueryProcessor
{
public:
	QueryProcessor(std::shared_ptr<ManagementResource> resource, const char* query, std::optional<EnumerationOptions> options = std::nullopt);

	QueryReader Get();

private:
	std::shared_ptr<ManagementResource> resource_;

	EnumerationOptions options_;
	const char* query_;
};

}


