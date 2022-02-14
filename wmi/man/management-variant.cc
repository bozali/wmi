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
		break;

	case VariantType::kShort:
		break;

	case VariantType::kChar:
		break;
	case VariantType::kULongLong:
		break;
	case VariantType::kULong:
		break;
	case VariantType::kUShort:
		break;
	case VariantType::kByte:
		break;
	case VariantType::kFloat:
		break;
	case VariantType::kDouble:
		break;
	case VariantType::kBstr:
		variant.vt = VT_BSTR;
		variant.bstrVal = std::get<kBstr>(input);
		break;
	}

	return variant;
}