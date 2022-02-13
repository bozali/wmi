#include <wmi/man/result-object.h>
#include <wmi/core/com-exception-factory.h>

using namespace wmi;


ResultObject::ResultObject(ComPtr<IWbemClassObject> object) noexcept
	: object_(object)
{
}


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


const ManagementVariant ResultObject::operator[](const char* property_name) const
{
    return InternalGet(property_name);
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
