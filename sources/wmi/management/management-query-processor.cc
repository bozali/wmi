#include <wmi/management/management-query-processor.h>

using namespace wmi;


ManagementQueryProcessor::ManagementQueryProcessor(const ManagementResource& resource, const bstr_t query, EnumerationOptions enumeration_options) noexcept
	: resource_(&resource)
	, query_(query)
	, enumeration_options_(enumeration_options)
{
}
