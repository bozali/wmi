#include <wmi/common/crypt-string.h>

#include <cstdlib>

using namespace wmi;


CryptString::CryptString(LPCTSTR unprotected_data) noexcept(false)
{
  size_t unprotected_data_byte_len = lstrlen(unprotected_data) * sizeof(TCHAR);
  size_t secure_data_byte_len = 0;

  if (const size_t mod = unprotected_data_byte_len % CRYPTPROTECTMEMORY_BLOCK_SIZE) {
    secure_data_byte_len = unprotected_data_byte_len + (CRYPTPROTECTMEMORY_BLOCK_SIZE - mod);
  }
  else {
    secure_data_byte_len = unprotected_data_byte_len;
  }

  secure_byte_len_ = secure_data_byte_len;

  ptr_ = reinterpret_cast<LPTSTR>(std::malloc(secure_data_byte_len));

  if (ptr_ == nullptr) {
    throw std::exception("Failed to allocate memory");
  }

  lstrcpy(ptr_, unprotected_data);
  ProtectMemory();

  SecureZeroMemory(static_cast<void*>(const_cast<TCHAR*&>(unprotected_data)), unprotected_data_byte_len);
}


CryptString::~CryptString() noexcept(false)
{
  FreeMemory();
}


void CryptString::FreeMemory() const
{
  if (ptr_ != nullptr) {
    SecureZeroMemory(reinterpret_cast<void*>(ptr_), secure_byte_len_);
    std::free(ptr_);
    ptr_ = nullptr;
  }
}


void CryptString::ProtectMemory() const
{
  if (CryptProtectMemory(ptr_, secure_byte_len_, CRYPTPROTECTMEMORY_SAME_PROCESS)) {
    return;
  }

  FreeMemory();
}


void CryptString::UnprotectMemory() const
{
  if (CryptUnprotectMemory(ptr_, secure_byte_len_, CRYPTPROTECTMEMORY_SAME_PROCESS)) {
    return;
  }

  FreeMemory();
}


CryptStringAccess::CryptStringAccess(const CryptString& str) : str_(str) {
  str_.UnprotectMemory();
}


CryptStringAccess::~CryptStringAccess()
{
  str_.ProtectMemory();
}


void CryptStringAccess::ForceProtect()
{
  str_.ProtectMemory();
}


LPCTSTR CryptStringAccess::GetData()
{
  return str_.GetData();
}
