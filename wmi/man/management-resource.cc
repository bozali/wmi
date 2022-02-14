#include <wmi/man/management-resource.h>
#include <wmi/core/com-exception-factory.h>

using namespace wmi;


ManagementResource::ManagementResource(const ManagementContext& context)
	: context_(context)
{
}


void ManagementResource::Connect(const char* path, std::optional<ConnectionOptions> options)
{
	ComExceptionFactory::ThrowIfFailed(context_.locator_->ConnectServer(bstr_t(path),
																		 nullptr,
																		 nullptr,
																		 nullptr,
																		 0,
																		 nullptr,
																		 nullptr,
																		 services_.GetAddressOf()));
}


ResultObject ManagementResource::ExecuteMethod(const char* class_name, const char* method_name, std::unordered_map<std::string, ManagementVariant> parameters)
{
	ComPtr<IWbemClassObject> wmi_class;
	services_->GetObject(bstr_t(class_name), 0, nullptr, wmi_class.GetAddressOf(), nullptr);

	ComPtr<IWbemClassObject> in_params;
	wmi_class->GetMethod(bstr_t(method_name), 0, in_params.GetAddressOf(), nullptr);

	ComPtr<IWbemClassObject> in_param_instance;
	in_params->SpawnInstance(0, in_param_instance.GetAddressOf());

	for (const auto& param : parameters)
	{
		auto variant = internal::ConvertToWin32Variant(param.second);
		in_param_instance->Put(bstr_t(param.first.c_str()), 0, &variant, 0);
	}

	ComPtr<IWbemClassObject> out_param_instance;
	services_->ExecMethod(bstr_t(class_name), bstr_t(method_name), 0, nullptr, in_param_instance.Get(), out_param_instance.GetAddressOf(), nullptr);

	return ResultObject(services_, out_param_instance);
}
