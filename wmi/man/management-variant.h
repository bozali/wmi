#pragma once

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

}