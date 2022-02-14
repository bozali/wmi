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


void ManagementResource::ExecuteMethod(const char* class_name, const char* method_name, std::unordered_map<std::string, ManagementVariant> parameters)
{
	ComPtr<IWbemClassObject> wmi_class;
	services_->GetObject(bstr_t(class_name), 0, nullptr, wmi_class.GetAddressOf(), nullptr);

	ComPtr<IWbemClassObject> in_params;
	wmi_class->GetMethod(bstr_t(method_name), 0, in_params.GetAddressOf(), nullptr);

	ComPtr<IWbemClassObject> instance;
	in_params->SpawnInstance(0, instance.GetAddressOf());

	for (const auto& param : parameters)
	{
		auto management_variant = param.second;

		VARIANT variant;
		SecureZeroMemory(&variant, sizeof(variant));

		switch (management_variant.index())
		{
		case VariantType::kLongLong:
			variant.vt = VT_I8;
			variant.llVal = std::get<kLongLong>(management_variant);
			break;

		case VariantType::kLong:
			break;

		case VariantType::kShort:
			break;

		case VariantType::kChar:
			break;
		case VariantType::kULongLong:
			break;
		case VariantType::kULong:
			break;
		case VariantType::kUShort:
			break;
		case VariantType::kByte:
			break;
		case VariantType::kFloat:
			break;
		case VariantType::kDouble:
			break;
		case VariantType::kBstr:
			variant.vt = VT_BSTR;
			variant.bstrVal = std::get<kBstr>(management_variant);
			break;
		}
	
		instance->Put(bstr_t(param.first.c_str()), 0, &variant, 0);
	}

	ComPtr<IWbemClassObject> out_params;
	services_->ExecMethod(bstr_t(class_name), bstr_t(method_name), 0, nullptr, instance.Get(), out_params.GetAddressOf(), nullptr);
}

