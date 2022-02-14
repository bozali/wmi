#include <wmi/man/result-object.h>
#include <wmi/core/com-exception-factory.h>

using namespace wmi;


ResultObject::ResultObject(ComPtr<IWbemServices> services, ComPtr<IWbemClassObject> object) noexcept
	: object_(object)
    , services_(services)
{
}

/*
void ResultObject::ExecuteMethod(const char* name)
{
    ComPtr<IWbemClassObject> in_param;
    ComPtr<IWbemClassObject> instance;

    object_->GetMethod(bstr_t(name), 0, in_param.GetAddressOf(), nullptr);

    in_param->SpawnInstance(0, instance.GetAddressOf());
    
    VARIANT command;
    SecureZeroMemory(&command, sizeof(command));

    command.vt = VT_BSTR;
    command.bstrVal = bstr_t(TEXT("notepad.exe"));

    instance->Put(TEXT("CommandLine"), 0, &command, 0);

    ComPtr<IWbemClassObject> out_param;
    services_->ExecMethod();
}
*/

std::vector<bstr_t> ResultObject::PropertyNames()
{
    SAFEARRAY* names;
    ComExceptionFactory::ThrowIfFailed(object_->GetNames(nullptr, WBEM_FLAG_ALWAYS, nullptr, &names));

    long lower, upper;
    
    SafeArrayGetLBound(names, 1, &lower);
    SafeArrayGetUBound(names, 1, &upper);

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
