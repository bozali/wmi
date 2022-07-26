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


ManagementObject ManagementResource::ExecuteMethod(const std::string_view class_name, const std::string_view method_name, std::optional<std::unordered_map<std::string_view, variant_t>> parameters) noexcept(false)
{
  HRESULT result = S_OK;

  bstr_t wmi_class_name = class_name.data();
  bstr_t wmi_method_name = method_name.data();

  ComPtr<IWbemClassObject> wmi_class;
  result = services_->GetObjectW(wmi_class_name.GetBSTR(), 0, nullptr, wmi_class.GetAddressOf(), nullptr);

  ComExceptionFactory::ThrowIfFailed(result);

  ComPtr<IWbemClassObject> in_params;
  result = wmi_class->GetMethod(wmi_method_name.GetBSTR(), 0, in_params.GetAddressOf(), nullptr);

  ComExceptionFactory::ThrowIfFailed(result);

  ComPtr<IWbemClassObject> in_param_instance;

  if (parameters.has_value())
  {
    result = in_params->SpawnInstance(0, in_param_instance.GetAddressOf());
    ComExceptionFactory::ThrowIfFailed(result);

    for (const auto& param : parameters.value())
    {
      auto variant = param.second;
      result = in_param_instance->Put(bstr_t(param.first.data()), 0, &variant, 0);

      ComExceptionFactory::ThrowIfFailed(result);
    }
  }

  ComPtr<IWbemClassObject> out_param_instance;
  result = services_->ExecMethod(wmi_class_name.GetBSTR(), wmi_method_name.GetBSTR(), 0, nullptr, in_param_instance.Get(), out_param_instance.GetAddressOf(), nullptr);

  ComExceptionFactory::ThrowIfFailed(result);

  return ManagementObject(services_, out_param_instance);
}


ManagementObject ManagementResource::CreateInstance(const char* class_name)
{
  bstr_t wmi_class_name = class_name;

  ComPtr<IWbemClassObject> class_definition;
  ComPtr<IWbemClassObject> instance;

  services_->GetObjectW(wmi_class_name.GetBSTR(), 0, nullptr, class_definition.GetAddressOf(), nullptr);

  class_definition->SpawnInstance(0, instance.GetAddressOf());

  return ManagementObject(services_, instance);
}



void ManagementResource::SetOptions(ConnectionOptions options) noexcept
{
  options_ = options;
}


void ManagementResource::SetPath(const std::string_view path) noexcept
{
  resource_path_ = path;
}
