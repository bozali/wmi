#pragma once

#include <vcruntime.h>

#define _WIN32_DCOM

#include <wrl/client.h>
#include <Windows.h>
#include <comdef.h>
#include <WbemIdl.h>

namespace microsoft {
template <typename T>
using com_ptr = Microsoft::WRL::ComPtr<T>;
}

#include <string>
#include <string_view>

namespace std {

#ifdef UNICODE
using tstring = wstring;
using tstring_view = wstring_view;
#else
using tstring = std::string;
using tstring_view = std::string_view;
#endif // UNICODE

}

#ifdef UNICODE
using tchar = wchar_t;
using tbyte = wchar_t;
#else
using tchar = char;
using tbyte = char;
#endif // UNICODE


#define _WMI_SEALED final
#define _WMI_ATTR_NODISCARD _NODISCARD

#if defined(_MSC_VER)
// Microsoft Visual Studio Compiler
#	define _WMI_FORCEINLINE __forceinline
#else
// Other compilers, fallback to regular inline
#	define _WMI_FORCEINLINE inline
#endif


namespace wmi {

using BasicString = bstr_t;

using Byte = BYTE;
using Char = CHAR;

using UInt8 = UINT8;
using UInt16 = UINT16;
using UInt32 = UINT32;
using UInt64 = UINT64;
using Int8 = INT8;
using Int16 = INT16;
using Int32 = INT32;
using Int64 = INT64;
using Dword = DWORD;

using ULong = ULONG;
using Long = LONG;

}

namespace wmi::internal {

class MallocDeleter
{
public:
	void operator()(void* ptr) {
		free(ptr);
	}
};

}
