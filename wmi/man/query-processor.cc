#include <wmi/man/query-processor.h>
#include <wmi/core/com-exception-factory.h>

using namespace wmi;

QueryProcessor::QueryProcessor(const ManagementResource& resource, const char* query, std::optional<EnumerationOptions> options)
	: resource_(&resource)
	, query_(query)
{
	if (options.has_value()) {
		options_ = options.value();
	}
}


QueryStream QueryProcessor::GetStream()
{
	ComPtr<IEnumWbemClassObject> enumerator;
	HRESULT hr = E_FAIL;

	hr = resource_->services_->ExecQuery(bstr_t("WQL"),
										 bstr_t(query_),
										 WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
										 nullptr,
										 enumerator.GetAddressOf());

	ComExceptionFactory::ThrowIfFailed(hr);

	return QueryStream(resource_->services_, enumerator, options_);
}
