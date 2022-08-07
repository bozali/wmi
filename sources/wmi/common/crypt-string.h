#pragma once

#include <wmi/common/exports.h>
#include <wmi/common/common.h>

#include <memory>


namespace wmi {

class CryptStringAccess;


class WMI_DLL CryptString final
{
public:
  CryptString(LPCTSTR unprotected_data) noexcept(false);
  ~CryptString() noexcept(false);

  _NODISCARD inline LPTSTR GetData() const noexcept {
    return ptr_;
  }

  _NODISCARD std::shared_ptr<CryptStringAccess> Access() const {
    return std::make_shared<CryptStringAccess>(*this);
  }

private:
  void FreeMemory() const;
  void ProtectMemory() const;
  void UnprotectMemory() const;

  mutable LPTSTR ptr_;
  size_t secure_byte_len_;

  friend class CryptStringAccess;
};


class WMI_DLL CryptStringAccess
{
public:
  explicit CryptStringAccess(const CryptString& str);
  ~CryptStringAccess();

  void ForceProtect();

  LPCTSTR GetData();

private:
  const CryptString& str_;

  friend class CryptString;
};

}