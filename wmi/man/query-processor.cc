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


QueryStream QueryProcessor::GetStream()
{
	ComPtr<IEnumWbemClassObject> enumerator;

	resource_->services_->ExecQuery(bstr_t("WQL"),
									bstr_t(query_),
									WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
									nullptr,
									enumerator.GetAddressOf());

	return QueryStream(resource_->services_, enumerator, options_);
}
