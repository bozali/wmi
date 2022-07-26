#include <wmi/man/man-object.h>
#include <wmi/common/com-exception.h>

using namespace wmi;


ManagementObject::ManagementObject(ComPtr<IWbemServices> services, ComPtr<IWbemClassObject> object) noexcept
  : services_(services)
  , object_(object)
{
}


void ManagementObject::Set(const std::string_view property_name, variant_t value) noexcept(false)
{
  bstr_t name = property_name.data();

  ComExceptionFactory::ThrowIfFailed(object_->Put(name, 0, &value, 0));
}


void ManagementObject::Put() noexcept(false)
{
  HRESULT hr = E_FAIL;

  ComPtr<IWbemClassObject> object;
  ComPtr<IWbemCallResult> result;
  ComPtr<IWbemContext> context;

  hr = CoCreateInstance(CLSID_WbemContext,
                        nullptr,
                        CLSCTX_INPROC_SERVER,
                        IID_IWbemContext,
                        reinterpret_cast<void**>(context.GetAddressOf()));

  ComExceptionFactory::ThrowIfFailed(hr);

  variant_t put_extensions;
  put_extensions.vt = VT_BOOL;
  put_extensions.boolVal = VARIANT_FALSE;
  context->SetValue(TEXT("__PUT_EXTENSIONS"), 0, &put_extensions);

  variant_t put_ext_client_request;
  put_ext_client_request.vt = VT_BOOL;
  put_ext_client_request.boolVal = VARIANT_TRUE;
  context->SetValue(TEXT("__PUT_EXT_CLIENT_REQUEST"), 0, &put_ext_client_request);

  hr = services_->PutInstance(object_.Get(), WBEM_FLAG_UPDATE_ONLY, context.Get(), result.GetAddressOf());
  ComExceptionFactory::ThrowIfFailed(hr);

  hr = result->GetResultObject(WBEM_INFINITE, object.GetAddressOf());
  ComExceptionFactory::ThrowIfFailed(hr);
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
