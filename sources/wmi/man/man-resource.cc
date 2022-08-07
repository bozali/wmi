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
  
    const auto username = options_.username.has_value() ? bstr_t(options_.username.value().data()) : bstr_t();
    const auto locale = options_.locale.has_value() ? bstr_t(options_.locale.value().data()) : bstr_t();
    const auto authority = options_.authority.has_value() ? bstr_t(options_.authority.value().data()) : bstr_t();

    result = locator_->ConnectServer(bstr_t(path.data()),
                                     username,
                                     ExtractPassword(options_),
                                     locale,
                                     0,
                                     authority,
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
  HRESULT hr = S_OK;

  bstr_t wmi_class_name = class_name.data();
  bstr_t wmi_method_name = method_name.data();

  ComPtr<IWbemClassObject> wmi_class;
  ComExceptionFactory::ThrowIfFailed(services_->GetObjectW(wmi_class_name.GetBSTR(), 0, nullptr, wmi_class.GetAddressOf(), nullptr));

  ComPtr<IWbemClassObject> in_params;
  ComExceptionFactory::ThrowIfFailed(wmi_class->GetMethod(wmi_method_name.GetBSTR(), 0, in_params.GetAddressOf(), nullptr));

  ComPtr<IWbemClassObject> in_param_instance;

  if (parameters.has_value() && !parameters.value().empty())
  {
    ComExceptionFactory::ThrowIfFailed(in_params->SpawnInstance(0, in_param_instance.GetAddressOf()));

    for (const auto& param : parameters.value())
    {
      auto variant = param.second;
      ComExceptionFactory::ThrowIfFailed(in_param_instance->Put(bstr_t(param.first.data()), 0, &variant, 0));
    }
  }

  ComPtr<IWbemClassObject> out_param_instance;

  hr = services_->ExecMethod(wmi_class_name.GetBSTR(), wmi_method_name.GetBSTR(), 0, nullptr, in_param_instance.Get(), out_param_instance.GetAddressOf(), nullptr);
  ComExceptionFactory::ThrowIfFailed(hr);

  return ManagementObject(services_, out_param_instance);
}


ManagementObject ManagementResource::CreateInstance(const std::string_view class_name) noexcept(false)
{
  HRESULT result = S_OK;

  bstr_t wmi_class_name = class_name.data();

  ComPtr<IWbemClassObject> class_definition;
  ComPtr<IWbemClassObject> instance;

  result = services_->GetObjectW(wmi_class_name.GetBSTR(), 0, nullptr, class_definition.GetAddressOf(), nullptr);
  ComExceptionFactory::ThrowIfFailed(result);

  result = class_definition->SpawnInstance(0, instance.GetAddressOf());
  ComExceptionFactory::ThrowIfFailed(result);

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


bstr_t ManagementResource::ExtractPassword(const ConnectionOptions& options) const
{
  if (options_.secure_password.has_value()) {
    const auto access = options_.secure_password.value().Access();
    return bstr_t(access->GetData());
  }

  if (options_.password.has_value()) {
    return bstr_t(options_.password.value().data());
  }

  return bstr_t();
}

