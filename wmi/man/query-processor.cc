#include <wmi/man/query-processor.h>

using namespace wmi;


QueryProcessor::QueryProcessor(std::shared_ptr<ManagementResource> resource, const char* query, std::optional<EnumerationOptions> options)
	: resource_(resource)
	, query_(query)
{
	if (options.has_value()) {
		options_ = options.value();
	}
}


QueryReader QueryProcessor::Get()
{
	ComPtr<IEnumWbemClassObject> enumerator;

	resource_->services_->ExecQuery(bstr_t("WQL"),
									bstr_t(query_),
									WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
									nullptr,
									enumerator.GetAddressOf());

	return QueryReader(enumerator);
}


void QueryProcessor::SetResource(std::shared_ptr<ManagementResource> resource)
{
	resource_ = resource;
}


void QueryProcessor::SetQuery(const char* query)
{
	query_ = query;
}


std::shared_ptr<ManagementResource> QueryProcessor::Resource() const
{
	return resource_;
}


const char* QueryProcessor::Query() const
{
	return query_;
}
