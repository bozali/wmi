#include <wmi/management/management-object.h>
#include <wmi/common/com-exception.h>

using namespace wmi;
using namespace wmi::internal;


ManagementObjectProxy ManagementObject::Proxy() const
{
	return ManagementObjectProxy(*this);
}


void ManagementObject::Set(const BasicString property_name, Variant value) noexcept(false)
{
	auto variant = internal::VariantCast(value);
	ComExceptionFactory::ThrowIfFailed(object_->Put(property_name, 0, &variant, 0));
}


void ManagementObject::Put() noexcept(false)
{
	microsoft::com_ptr<IWbemCallResult> result;
	microsoft::com_ptr<IWbemClassObject> object;
	microsoft::com_ptr<IWbemContext> context;

	ComExceptionFactory::ThrowIfFailed(CoCreateInstance(CLSID_WbemContext,
																											nullptr,
																											CLSCTX_INPROC_SERVER,
																											IID_IWbemContext,
																											reinterpret_cast<void**>(context.GetAddressOf())));

	variant_t put_extensions;
	put_extensions.vt = VT_BOOL;
	put_extensions.boolVal = VARIANT_FALSE;
	ComExceptionFactory::ThrowIfFailed(context->SetValue(TEXT("__PUT_EXTENSIONS"), 0, &put_extensions));

	variant_t put_extensions_request;
	put_extensions_request.vt = VT_BOOL;
	put_extensions_request.boolVal = VARIANT_TRUE;
	ComExceptionFactory::ThrowIfFailed(context->SetValue(TEXT("__PUT_EXT_CLIENT_REQUST"), 0, &put_extensions_request));

	ComExceptionFactory::ThrowIfFailed(services_->PutInstance(object_.Get(), WBEM_FLAG_UPDATE_ONLY, context.Get(), result.GetAddressOf()));
	ComExceptionFactory::ThrowIfFailed(result->GetResultObject(WBEM_INFINITE, object.GetAddressOf()));
}


ManagementObject ManagementObject::ExecuteMethod(const BasicString method_name, std::optional<ParameterSet> parameters) noexcept(false)
{
	microsoft::com_ptr<IWbemClassObject> input_parameters;
	microsoft::com_ptr<IWbemClassObject> input_parameter_instances;

	object_->GetMethod(method_name, 0, input_parameters.GetAddressOf(), nullptr);

	if (parameters.has_value() && !parameters.value().empty())
	{
		input_parameters->SpawnInstance(0, input_parameters.GetAddressOf());

		for (const auto& param : parameters.value())
		{
			auto variant = internal::VariantCast(param.second);
			ComExceptionFactory::ThrowIfFailed(input_parameter_instances->Put(param.first, 0, &variant, 0));
		}
	}

	BasicString system_property_path = std::get<BasicString>((*this)[TEXT("__PATH")]);

	microsoft::com_ptr<IWbemClassObject> output_parameter_instances;

	ComExceptionFactory::ThrowIfFailed(services_->ExecMethod(system_property_path,
																													 method_name,
																													 0,
																													 nullptr,
																													 parameters.has_value() ? input_parameter_instances.Get() : nullptr,
																													 output_parameter_instances.GetAddressOf(),
																													 nullptr));

	return ManagementObject(services_, output_parameter_instances);
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

	// TODO Think about how we can get LONG values
	// case VT_I8:
	//  	return wmi::Variant(fetched_value.lVal);

	case VT_UI1:
		return Variant(fetched_value.bVal);

	case VT_UI2:
		return Variant(fetched_value.uiVal);

	case VT_UI4:
		return Variant(fetched_value.uintVal);

	case VT_UI8:
		return Variant(fetched_value.ulVal);

	case VT_UNKNOWN:
	{
		microsoft::com_ptr<IWbemClassObject> o1;
		fetched_value.punkVal->QueryInterface(IID_IWbemClassObject, reinterpret_cast<void**>(o1.GetAddressOf()));

		return Variant(ManagementObject(services_, o1));
	}

	case VT_BOOL:
		return Variant(static_cast<bool>(fetched_value.boolVal));

	case VT_BSTR:
		return Variant(BasicString(fetched_value.bstrVal));

	default:
		throw std::exception("Variant type not registered");
	}
}
