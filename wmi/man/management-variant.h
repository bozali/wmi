#pragma once

#include <wmi/core/exports.h>
#include <wmi/core/def.h>

#include <variant>

namespace wmi {

struct EmptyType { };
struct NullType { };

using ManagementVariant = std::variant <
	LONGLONG,
	LONG,
	SHORT,
	CHAR,
	ULONGLONG,
	ULONG,
	USHORT,
	BYTE,
	FLOAT,
	DOUBLE,
	BSTR>;


enum VariantType : size_t
{
	kLongLong,
	kLong,
	kShort,
	kChar,
	kULongLong,
	kULong,
	kUShort,
	kByte,
	kFloat,
	kDouble,
	kBstr
};


namespace internal { WMI_DLL VARIANT ConvertToWin32Variant(wmi::ManagementVariant input); }

}

