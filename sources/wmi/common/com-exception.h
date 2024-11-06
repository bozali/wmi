#pragma once

#include <wmi/common/exports.h>
#include <wmi/common/defs.h>

#include <winerror.h>
#include <stdio.h>


namespace wmi {

class WMI_DLL ComException : public std::exception
{
public:
	inline ComException(const HRESULT hr)
		: result_(hr)
	{
	}

	_WMI_ATTR_NODISCARD _WMI_FORCEINLINE const std::tstring_view Detailed() const {
		static tchar buffer[1024];
		SecureZeroMemory(buffer, sizeof(buffer));

		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
									nullptr,
									GetLastError(),
									MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
									buffer, sizeof(buffer) / sizeof(TCHAR),
									nullptr);

		return buffer;
	}

	_WMI_ATTR_NODISCARD _WMI_FORCEINLINE const char* What() const {
		return what();
	}

	_WMI_ATTR_NODISCARD _WMI_FORCEINLINE const char* what() const override {
		static char buffer[1024];
		SecureZeroMemory(buffer, sizeof(buffer));

		sprintf_s(buffer, "Fail with HRESULT: %08X", static_cast<DWORD>(result_));

		return buffer;
	}

private:
	const HRESULT result_;

};


class WMI_DLL ComExceptionFactory
{
public:
	inline static void ThrowIfFailed(const HRESULT hr) noexcept(false)
	{
		if (FAILED(hr))
		{
			throw ComException(hr);
		}
	}
};

}

