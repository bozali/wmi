#include <wmi/man/man-resource.h>
#include <wmi/common/com-exception.h>

using namespace wmi;


ManagementResource::ManagementResource(const std::string_view path, ConnectionOptions options) noexcept
  : resource_path_(path)
  , is_connected_(false)
  , options_(options)
{
}


ManagementResource::ManagementResource(const std::string_view path) noexcept
  : resource_path_(path)
  , is_connected_(false)
{
}


void ManagementResource::Connect(const std::string_view path, ConnectionOptions options) noexcept(false)
{
  SetOptions(options);
  SetPath(path);


  try {
    HRESULT result = S_OK;

    result = CoCreateInstance(CLSID_WbemLocator,
                              nullptr,
                              CLSCTX_INPROC_SERVER,
                              IID_IWbemLocator,
                              reinterpret_cast<LPVOID*>(locator_.GetAddressOf()));


    ComExceptionFactory::ThrowIfFailed(result);
  
    result = locator_->ConnectServer(bstr_t(path.data()),
                                     nullptr,
                                     nullptr,
                                     nullptr,
                                     0,
                                     nullptr,
                                     nullptr,
                                     services_.GetAddressOf());

    ComExceptionFactory::ThrowIfFailed(result);

    is_connected_ = true;
  }
  catch (const wmi::ComException& e) {
    throw e;
  }
}


void ManagementResource::Connect() noexcept(false)
{
  Connect(resource_path_, options_);
}


void ManagementResource::SetOptions(ConnectionOptions options) noexcept
{
  options_ = options;
}


void ManagementResource::SetPath(const std::string_view path) noexcept
{
  resource_path_ = path;
}
