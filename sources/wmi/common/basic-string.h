#pragma once

#include <wmi/common/defs.h>

#include <string>


namespace wmi::internal {

struct BasicStringHash
{
	size_t operator()(const BasicString& str) const noexcept
	{
		return std::hash<std::wstring>{}(static_cast<const wchar_t*>(str));
	}
};

struct BasicStringEqual
{
	bool operator()(const BasicString& lhs, const BasicString& rhs) const noexcept
	{
		return lhs == rhs;
	}
};

}
