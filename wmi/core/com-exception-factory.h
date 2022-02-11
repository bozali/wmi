#pragma once

#include <Windows.h>
#include <stdio.h>

#include <exception>


namespace wmi {

class ComException : public std::exception
{
public:
	inline ComException(const HRESULT hr)
		: result(hr) { }

	[[nodiscard]] inline const char* what() const override {
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