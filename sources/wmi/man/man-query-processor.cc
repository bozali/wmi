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

void ManagementQueryProcessor::SetResource(const ManagementResource& resource) noexcept
{
  resource_ = &resource;
}


void ManagementQueryProcessor::SetQuery(const std::string_view query) noexcept
{
  query_ = query;
}


ComPtr<IEnumWbemClassObject> ManagementQueryProcessor::InternalQueryExecute() noexcept(false)
{
  ComPtr<IEnumWbemClassObject> enumerator;
  HRESULT result = S_OK;

  long flags = WBEM_FLAG_FORWARD_ONLY;

  if (options_.use_amended_qualifiers) {
    flags |= WBEM_FLAG_USE_AMENDED_QUALIFIERS;
  }

  if (options_.direct_read) {
    flags |= WBEM_FLAG_DIRECT_READ;
  }

  if (options_.return_immediately) {
    flags |= WBEM_FLAG_RETURN_IMMEDIATELY;
  }

  result = resource_->services_->ExecQuery(bstr_t("WQL"),
                                           bstr_t(query_.data()),
                                           flags,
                                           nullptr,
                                           enumerator.GetAddressOf());

  ComExceptionFactory::ThrowIfFailed(result);
  return enumerator;
}
