#pragma once

#include <wmi/common/exports.h>
#include <wmi/common/defs.h>

#include <variant>


namespace wmi {

class ManagementObject;

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
	ManagementObject,
	bool>;


namespace internal {

WMI_EXTERN_C variant_t VariantCast(const Variant& input);

}

}
