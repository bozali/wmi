#include <wmi/man/man-query-processor.h>
#include <wmi/man/query-stream.h>

#include <wmi/common/com-exception.h>

using namespace wmi;


ManagementQueryProcessor::ManagementQueryProcessor(const ManagementResource& resource, const std::string_view query, EnumerationOptions options) noexcept
  : resource_(&resource)
  , query_(query)
  , options_(options)
{
}


ManagementQueryProcessor::ManagementQueryProcessor(const ManagementResource& resource, const std::string_view query) noexcept
  : resource_(&resource)
  , query_(query)
{
}


QueryStream ManagementQueryProcessor::GetStream() noexcept(false)
{
  ComPtr<IEnumWbemClassObject> enumerator;
  HRESULT result = S_OK;

  result = resource_->services_->ExecQuery(bstr_t("WQL"),
                                           bstr_t(query_.data()),
                                           WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
                                           nullptr,
                                           enumerator.GetAddressOf());

  ComExceptionFactory::ThrowIfFailed(result);

  return QueryStream(resource_->services_, enumerator, options_);
}


void ManagementQueryProcessor::SetResource(const ManagementResource& resource) noexcept
{
  resource_ = &resource;
}


void ManagementQueryProcessor::SetQuery(const std::string_view query) noexcept
{
  query_ = query;
}
