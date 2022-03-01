#include <wmi/man/result-object.h>
#include <wmi/core/com-exception-factory.h>

using namespace wmi;


ResultObject::ResultObject(ComPtr<IWbemServices> services, ComPtr<IWbemClassObject> object) noexcept
	: object_(object)
    , services_(services)
{
}


void ResultObject::Set(const char* property_name, ManagementVariant value)
{
    VARIANT variant = internal::ConvertToWin32Variant(value);

    bstr_t name = property_name;

    ComExceptionFactory::ThrowIfFailed(object_->Put(name, 0, &variant, 0));
}


void ResultObject::Put()
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


ResultObject ResultObject::ExecuteMethod(const char* method_name, std::optional<std::unordered_map<std::string_view, ManagementVariant>> parameters)
{
    bstr_t wmi_method_name = method_name;

    ComPtr<IWbemClassObject> input_parameters;
    object_->GetMethod(wmi_method_name.GetBSTR(), 0, input_parameters.GetAddressOf(), nullptr);

    ComPtr<IWbemClassObject> input_parameter_instances;

    if (parameters.has_value())
    {
        input_parameters->SpawnInstance(0, input_parameters.GetAddressOf());

        for (const auto& param : parameters.value())
        {
            auto variant = internal::ConvertToWin32Variant(param.second);
            input_parameter_instances->Put(bstr_t(param.first.data()), 0, &variant, 0);
        }
    }

    bstr_t system_property_path = Get<BSTR>("__PATH");

    ComPtr<IWbemClassObject> output_parameter_instances;
    HRESULT hr = E_FAIL;

    services_->ExecMethod(system_property_path.GetBSTR(),
                                       wmi_method_name.GetBSTR(),
                                       0,
                                       nullptr,
                                       parameters.has_value() ? input_parameter_instances.Get() : nullptr,
                                       output_parameter_instances.GetAddressOf(),
                                       nullptr);

    ComExceptionFactory::ThrowIfFailed(hr);

    return ResultObject(services_, output_parameter_instances);
}


std::vector<bstr_t> ResultObject::PropertyNames()
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


ManagementVariant ResultObject::InternalGet(const char* property_name) const
{
    VARIANT property;
    SecureZeroMemory(&property, sizeof(property));

    if (FAILED(object_->Get(bstr_t(property_name), 0, &property, nullptr, nullptr)))
    {
        return ManagementVariant(nullptr);
    }

    switch (property.vt)
    {
    case VT_BSTR:
        return ManagementVariant(property.bstrVal);

    case VT_I8:
        return ManagementVariant(property.llVal);

    case VT_I4:
        return ManagementVariant(property.lVal);

    case VT_I2:
        return ManagementVariant(property.iVal);

    case VT_I1:
        return ManagementVariant(property.cVal);

    case VT_UI8:
        return ManagementVariant(property.ullVal);

    case VT_UI4:
        return ManagementVariant(property.ulVal);

    case VT_UI2:
        return ManagementVariant(property.uiVal);

    case VT_UI1:
        return ManagementVariant(property.bVal);

    case VT_R4:
        return ManagementVariant(property.fltVal);

    case VT_R8:
        return ManagementVariant(property.dblVal);

    case VT_DATE:
        return ManagementVariant(property.date);

    default:
        return ManagementVariant(nullptr);
    }
}
