#include <wmi/man/man-object.h>
#include <wmi/common/com-exception.h>

using namespace wmi;


ManagementObject::ManagementObject(ComPtr<IWbemServices> services, ComPtr<IWbemClassObject> object) noexcept
  : services_(services)
  , object_(object)
{
}


std::vector<bstr_t> ManagementObject::PropertyNames()
{
  SAFEARRAY* names;
  ComExceptionFactory::ThrowIfFailed(object_->GetNames(nullptr, WBEM_FLAG_ALWAYS, nullptr, &names));

  long lower, upper;

  SafeArrayGetLBound(names, 1, &lower);
  SafeArrayGetUBound(names, 1, &upper);

  // TODO Cache these values
  std::vector<bstr_t> result;
  result.reserve(static_cast<size_t>(upper - lower));

  for (long i = lower; i <= upper; ++i)
  {
    BSTR name;
    SafeArrayGetElement(names, &i, &name);

    result.push_back(bstr_t(name));
  }

  SafeArrayDestroy(names);

  return result;
}


const variant_t ManagementObject::operator[](const char* property_name) const
{
  variant_t value;

  HRESULT hr = object_->Get(bstr_t(property_name), 0, &value, nullptr, nullptr);
  ComExceptionFactory::ThrowIfFailed(hr);

  return value;
}
