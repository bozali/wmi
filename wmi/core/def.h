#pragma once

#define _WIN32_DCOM

#include <wrl/client.h>

#include <Windows.h>
#include <comdef.h>

#include <WbemIdl.h>

#pragma once(lib, "wbemuuid.lib")


template <typename T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

/*
#include <variant>

using ManagementVariant = std::variant<
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
*/

// TODO Check if it is supported
#if !defined(WMI_NODISCARD)
#	define WMI_NODISCARD [[nodiscard]]
#endif // !WMI_NODISCARD
