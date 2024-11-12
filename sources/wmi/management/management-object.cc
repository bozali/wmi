#include <wmi/management/management-object.h>
#include <wmi/common/com-exception.h>

using namespace wmi;


void ManagementObject::Set(const bstr_t property_name, variant_t value) noexcept(false)
{
	ComExceptionFactory::ThrowIfFailed(object_->Put(property_name, 0, &value, 0));
}


void ManagementObject::Put() noexcept(false)
{
	HRESULT hr = S_OK;

	microsoft::com_ptr<IWbemCallResult> result;
	microsoft::com_ptr<IWbemClassObject> object;
	microsoft::com_ptr<IWbemContext> context;

	hr = CoCreateInstance(CLSID_WbemContext,
												nullptr,
												CLSCTX_INPROC_SERVER,
												IID_IWbemContext,
												reinterpret_cast<void**>(context.GetAddressOf()));

	ComExceptionFactory::ThrowIfFailed(hr);

	variant_t put_extensions;
	put_extensions.vt = VT_BOOL;
	put_extensions.boolVal = VARIANT_FALSE;
	ComExceptionFactory::ThrowIfFailed(context->SetValue(TEXT("__PUT_EXTENSIONS"), 0, &put_extensions));

	variant_t put_extensions_request;
	put_extensions_request.vt = VT_BOOL;
	put_extensions_request.boolVal = VARIANT_TRUE;
	ComExceptionFactory::ThrowIfFailed(context->SetValue(TEXT("__PUT_EXT_CLIENT_REQUST"), 0, &put_extensions_request));

	hr = services_->PutInstance(object_.Get(), WBEM_FLAG_UPDATE_ONLY, context.Get(), result.GetAddressOf());
	ComExceptionFactory::ThrowIfFailed(hr);

	hr = result->GetResultObject(WBEM_INFINITE, object.GetAddressOf());
	ComExceptionFactory::ThrowIfFailed(hr);
}


ManagementObject::ManagementObject(microsoft::com_ptr<IWbemServices> services, microsoft::com_ptr<IWbemClassObject> object) noexcept
{
}

