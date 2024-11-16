#include <wmi/management/management-object.h>
#include <wmi/common/com-exception.h>

using namespace wmi;


void ManagementObject::Set(const BasicString property_name, Variant value) noexcept(false)
{
	// TODO Conversion to variant_t
	// ComExceptionFactory::ThrowIfFailed(object_->Put(property_name, 0, &value, 0));
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
	: services_(services)
	, object_(object)
{
}


const Variant ManagementObject::operator[](const BasicString property_name) const noexcept
{
	variant_t fetched_value;

	ComExceptionFactory::ThrowIfFailed(object_->Get(property_name, 0, &fetched_value, nullptr, nullptr));

	Variant variant;

	switch (fetched_value.vt)
	{
	case VT_I1:
		return Variant(fetched_value.cVal);

	case VT_I2:
		return Variant(fetched_value.iVal);

	case VT_I4:
		return Variant(fetched_value.intVal);

	case VT_I8:
		return wmi::Variant(fetched_value.lVal);

	case VT_UI1:
		return Variant(fetched_value.bVal);

	case VT_UI2:
		return Variant(fetched_value.uiVal);

	case VT_UI4:
		return Variant(fetched_value.uintVal);

	case VT_UI8:
		return Variant(fetched_value.ulVal);

	case VT_BOOL:
		return Variant(static_cast<bool>(fetched_value.boolVal));

	case VT_BSTR:
		return Variant(fetched_value.bstrVal);

	default:
		throw std::exception("Variant type not registered");
	}
}
