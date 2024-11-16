#pragma once

#include <wmi/common/exports.h>
#include <wmi/common/defs.h>

#include <variant>


namespace wmi {

using Variant = std::variant <
	BasicString,
	Char,
	Byte,
	UInt16,
	UInt32,
	UInt64,
	Int8,
	Int16,
	Int32,
	Int64,
	ULong,
	Long,
	bool>;


namespace internal {


WMI_EXTERN_C static variant_t VariantCast(const Variant& input)
{
	variant_t variant;
	VariantInit(&variant);

	std::visit([&](auto&& value)
						 {
							 using T = std::decay_t<decltype(value)>;

							 if constexpr (std::is_same<T, Char>::value)
							 {
								 variant.vt = VT_I1;
								 variant.cVal = value;
							 }
							 else if constexpr (std::is_same<T, Int16>::value)
							 {
								 variant.vt = VT_I2;
								 variant.iVal = value;
							 }
							 else if constexpr (std::is_same<T, Int32>::value)
							 {
								 variant.vt = VT_I4;
								 variant.intVal = value;
							 }
							 else if constexpr (std::is_same<T, Long>::value)
							 {
								 variant.vt = VT_I8;
								 variant.lVal = value;
							 }
							 else if constexpr (std::is_same<T, Byte>::value)
							 {
								 variant.vt = VT_UI1;
								 variant.bVal = value;
							 }
							 else if constexpr (std::is_same<T, UInt16>::value)
							 {
								 variant.vt = VT_UI2;
								 variant.uiVal = value;
							 }
							 else if constexpr (std::is_same<T, UInt32>::value)
							 {
								 variant.vt = VT_UI4;
								 variant.uintVal = value;
							 }
							 else if constexpr (std::is_same<T, ULong>::value)
							 {
								 variant.vt = VT_UI8;
								 variant.ulVal = value;
							 }
							 else if constexpr (std::is_same<T, bool>::value)
							 {
								 variant.vt = VT_BOOL;
								 variant.boolVal = static_cast<VARIANT_BOOL>(value);
							 }
							 else if constexpr (std::is_same<T, BasicString>::value)
							 {
								 variant.vt = VT_BSTR;
								 variant.bstrVal = value;
							 }
							 else
							 {
								 throw std::exception("Variant type not registered");
							 }

						 }, input);

	return variant;
}

}

}
