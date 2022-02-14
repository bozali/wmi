#pragma once

#include <wmi/core/def.h>

#include <stdio.h>

#include <exception>


namespace wmi {

class ComException : public std::exception
{
public:
	inline ComException(const HRESULT hr)
		: result(hr) { }

	WMI_NODISCARD inline const TCHAR* Detailed() const {
		TCHAR buffer[256];
		SecureZeroMemory(buffer, sizeof(buffer));

		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
					  nullptr,
					  GetLastError(),
					  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
					  buffer, sizeof(buffer) / sizeof(TCHAR),
					  nullptr);

		return buffer;
	}

	WMI_NODISCARD inline const char* What() const {
		return what();
	}

	WMI_NODISCARD inline const char* what() const override {
		static char str[64] = {};

		sprintf_s(str, "Fail with HRESULT: %08X", static_cast<DWORD>(result));

		return str;
	}

	const HRESULT result;
};


class ComExceptionFactory
{
public:
	inline static void ThrowIfFailed(const HRESULT hr) noexcept(false) {
		if (FAILED(hr)) {
			throw ComException(hr);
		}
	}
};

}