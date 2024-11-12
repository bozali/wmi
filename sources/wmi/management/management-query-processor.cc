#include <wmi/management/management-query-processor.h>

using namespace wmi;


ManagementQueryProcessor::ManagementQueryProcessor(const ManagementResource& resource, const bstr_t query, EnumerationOptions enumeration_options) noexcept
	: resource_(&resource)
	, query_(query)
	, enumeration_options_(enumeration_options)
{
}


ManagementQueryProcessor::ManagementQueryProcessor(const ManagementResource& resource, const bstr_t query) noexcept
	: resource_(&resource)
	, query_(query)
{
}


microsoft::com_ptr<IEnumWbemClassObject> ManagementQueryProcessor::InternalQueryExecute() noexcept(false)
{
	microsoft::com_ptr<IEnumWbemClassObject> enumerator;
	HRESULT hr = S_OK;

	long flags = WBEM_FLAG_FORWARD_ONLY;

	if (enumeration_options_.use_amended_qualifiers)
	{
		flags |= WBEM_FLAG_USE_AMENDED_QUALIFIERS;
	}

	if (enumeration_options_.direct_read)
	{
		flags |= WBEM_FLAG_DIRECT_READ;
	}

	if (enumeration_options_.return_immediately)
	{
		flags |= WBEM_FLAG_RETURN_IMMEDIATELY;
	}

	hr = resource_->services_->ExecQuery(bstr_t("WQL"),
																			 query_,
																			 flags,
																			 nullptr,
																			 enumerator.GetAddressOf());

	ComExceptionFactory::ThrowIfFailed(hr);
	return enumerator;
}

