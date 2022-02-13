#include <wmi/man/result-object.h>

using namespace wmi;


ResultObject::ResultObject(ComPtr<IWbemClassObject> object) noexcept
	: object_(object)
{
}


void ResultObject::Properties()
{
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
