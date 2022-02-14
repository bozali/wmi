#include <wmi/man/management-variant.h>

using namespace wmi;


WMI_DLL VARIANT wmi::internal::ConvertToWin32Variant(ManagementVariant input)
{
	VARIANT variant;
	SecureZeroMemory(&variant, sizeof(variant));

	switch (input.index())
	{
	case VariantType::kLongLong:
		variant.vt = VT_I8;
		variant.llVal = std::get<kLongLong>(input);
		break;

	case VariantType::kLong:
		variant.vt = VT_I4;
		variant.lVal = std::get<kLong>(input);
		break;

	case VariantType::kShort:
		variant.vt = VT_I2;
		variant.iVal = std::get<kShort>(input);
		break;

	case VariantType::kChar:
		variant.vt = VT_I1;
		variant.cVal = std::get<kChar>(input);
		break;

	case VariantType::kULongLong:
		variant.vt = VT_UI8;
		variant.ullVal = std::get<kULongLong>(input);
		break;

	case VariantType::kULong:
		variant.vt = VT_UI4;
		variant.ulVal = std::get<kULong>(input);
		break;

	case VariantType::kUShort:
		variant.vt = VT_UI2;
		variant.uiVal = std::get<kUShort>(input);
		break;

	case VariantType::kByte:
		variant.vt = VT_UI1;
		variant.bVal = std::get<kByte>(input);
		break;

	case VariantType::kFloat:
		variant.vt = VT_R4;
		variant.fltVal = std::get<kFloat>(input);
		break;

	case VariantType::kDouble:
		variant.vt = VT_R8;
		variant.dblVal = std::get<kDouble>(input);
		break;

	case VariantType::kBstr:
		variant.vt = VT_BSTR;
		variant.bstrVal = std::get<kBstr>(input);
		break;
	}

	return variant;
}