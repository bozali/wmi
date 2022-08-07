#include <iostream>
#include <memory>
#include <cstdlib>

#include <wmi/common/com-manager.h>
#include <wmi/common/com-exception.h>

#include <wmi/man/query-iterator.h>
#include <wmi/man/query-stream.h>
#include <wmi/man/man-resource.h>
#include <wmi/man/man-object.h>
#include <wmi/man/man-query-processor.h>



struct Win32Process
{
  bstr_t name;
};

void MapManagementObject(const wmi::ManagementObject& from, Win32Process& to)
{
  to.name = from["Name"].bstrVal;
}


struct Win32OperatingSystem
{
  bstr_t name;
};

void MapManagementObject(const wmi::ManagementObject& from, Win32OperatingSystem& to)
{
  to.name = from["Name"].bstrVal;
}




int main()
{
  wmi::ComManager::Initialize();

  try {

    std::wstring s(TEXT("Some nice"));

    wmi::CryptString str(s.data());
    const auto access = str.Access();

    std::wcout << access->GetData() << "\n";

    
    // secure_string str("Some nice string");
    // std::cout << str << "\n";

    /*
    LPCWSTR plain_text = TEXT("Some Secure Stuff");
    
    DWORD plain_text_byte_len = lstrlenW(plain_text) + 1 * sizeof(WCHAR);
    DWORD secure_text_byte_len = 0;

    if (const DWORD mod = plain_text_byte_len % CRYPTPROTECTMEMORY_BLOCK_SIZE) {
      secure_text_byte_len = plain_text_byte_len + (CRYPTPROTECTMEMORY_BLOCK_SIZE - mod);
    }
    else {
      secure_text_byte_len = plain_text_byte_len;
    }

    LPWSTR secure_text = (LPWSTR)LocalAlloc(LPTR, secure_text_byte_len);

    if (secure_text == nullptr) {
      return 0;
    }

    lstrcpyW(secure_text, TEXT("Some Secure Stuff"));

    std::wcout << secure_text << "\n";

    if (!CryptProtectMemory(secure_text, secure_text_byte_len, CRYPTPROTECTMEMORY_SAME_PROCESS)) {
      SecureZeroMemory(secure_text, secure_text_byte_len);
      LocalFree(secure_text);
      secure_text = nullptr;
      return 0;
    }

    std::wcout << secure_text << "\n";


    if (!CryptUnprotectMemory(secure_text, secure_text_byte_len, CRYPTPROTECTMEMORY_SAME_PROCESS)) {
      SecureZeroMemory(secure_text, secure_text_byte_len);
      LocalFree(secure_text);
      secure_text = nullptr;
      return 0;
    }

    std::wcout << secure_text << "\n";

    SecureZeroMemory(secure_text, secure_text_byte_len);
    LocalFree(secure_text);
    secure_text = nullptr;



    wmi::ManagementResource resource("ROOT\\CIMV2");
    resource.Connect();

    wmi::ManagementQueryProcessor processor(resource, "SELECT * FROM Win32_Process");
    
    auto stream = processor.GetStream<Win32Process>();
    auto object = stream.Current();

    std::wcout << object.name << "\n";
    */
  }
  catch (const wmi::ComException& ex) {
    std::wcout << ex.Detailed() << "\n";
    std::cout << ex.What() << "\n";
  }

  wmi::ComManager::Uninitialize();

  return 0;
}
