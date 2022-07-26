#pragma once

#include <winerror.h>
#include <windows.h>
#include <windef.h>
#include <stdio.h>

#include <exception>



namespace wmi {

class ComException : public std::exception
{
public:
  inline ComException(const HRESULT hr)
    : result_(hr) { }

  _NODISCARD inline const LPWSTR Detailed() const {
    static TCHAR buffer[1024];
    SecureZeroMemory(buffer, sizeof(buffer));

    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                  nullptr,
                  GetLastError(),
                  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                  buffer, sizeof(buffer) / sizeof(TCHAR),
                  nullptr);

    return buffer;
  }

  _NODISCARD inline const char* What() const {
    return what();
  }

  _NODISCARD inline const char* what() const override {
    static char buffer[1024];
    SecureZeroMemory(buffer, sizeof(buffer));

    sprintf_s(buffer, "Fail with HRESULT: %08X", static_cast<DWORD>(result_));

    return buffer;
  }

private:
  const HRESULT result_;
};


class ComExceptionFactory
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