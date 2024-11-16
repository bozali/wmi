#include <wmi/management/management-resource.h>
#include <wmi/management/enumeration-options.h>
#include <wmi/management/management-query-processor.h>

#include <wmi/common/secure-string-access.h>
#include <wmi/common/secure-string.h>
#include <wmi/common/com-exception.h>

using namespace wmi;

ManagementResource::ManagementResource(const BasicString path, ConnectionOptions options) noexcept
	: resource_path_(path)
	, is_connected_(false)
	, options_(options)
{
}


ManagementResource::ManagementResource(const BasicString path) noexcept
	: resource_path_(path)
	, is_connected_(false)
{
}


void ManagementResource::Connect(const BasicString path, const ConnectionOptions options) noexcept(false)
{
	SetOptions(options);
	SetPath(path);

	try
	{
		HRESULT hr = S_OK;

		hr = CoCreateInstance(CLSID_WbemLocator,
													nullptr,
													CLSCTX_INPROC_SERVER,
													IID_IWbemLocator,
													reinterpret_cast<LPVOID*>(locator_.GetAddressOf()));

		ComExceptionFactory::ThrowIfFailed(hr);

		const auto username = options_.username.has_value() ? options.username.value() : BasicString();
		const auto locale = options_.locale.has_value() ? options.locale.value() : BasicString();
		const auto authority = options_.authority.has_value() ? options.authority.value() : BasicString();

		hr = locator_->ConnectServer(resource_path_,
																 username,
																 GetPassword(),
																 locale,
																 0,
																 authority,
																 nullptr,
																 services_.GetAddressOf());

		ComExceptionFactory::ThrowIfFailed(hr);

		is_connected_ = true;
	}
	catch (const wmi::ComException& ex)
	{
		throw ex;
	}
}


void ManagementResource::Connect() noexcept(false)
{
	Connect(resource_path_, options_);
}


ManagementObject ManagementResource::ExecuteMethod(const BasicString class_name, const BasicString method_name, std::optional<ManagementObject::ParameterSet> parameters) noexcept(false)
{
	microsoft::com_ptr<IWbemClassObject> class_object;
	microsoft::com_ptr<IWbemClassObject> input_parameter;
	microsoft::com_ptr<IWbemClassObject> input_parameter_instances;

	ComExceptionFactory::ThrowIfFailed(services_->GetObjectW(class_name, 0, nullptr, class_object.GetAddressOf(), nullptr));
	ComExceptionFactory::ThrowIfFailed(class_object->GetMethod(method_name, 0, input_parameter.GetAddressOf(), nullptr));

	if (parameters.has_value() && !parameters.value().empty())
	{
		ComExceptionFactory::ThrowIfFailed(input_parameter->SpawnInstance(0, input_parameter_instances.GetAddressOf()));

		for (const auto& param : parameters.value())
		{
			auto variant = internal::VariantCast(param.second);
			ComExceptionFactory::ThrowIfFailed(input_parameter_instances->Put(param.first, 0, &variant, 0));
		}
	}

	microsoft::com_ptr<IWbemClassObject> output_parameter_instances;

	ComExceptionFactory::ThrowIfFailed(services_->ExecMethod(class_name,
																													 method_name,
																													 0,
																													 nullptr,
																													 input_parameter_instances.Get(),
																													 output_parameter_instances.GetAddressOf(),
																													 nullptr));

	return ManagementObject(services_, output_parameter_instances);
}


ManagementObject ManagementResource::CreateInstance(const BasicString class_name) noexcept(false)
{
	microsoft::com_ptr<IWbemClassObject> class_definition;
	microsoft::com_ptr<IWbemClassObject> instance;

	ComExceptionFactory::ThrowIfFailed(services_->GetObjectW(class_name, 0, nullptr, class_definition.GetAddressOf(), nullptr));
	ComExceptionFactory::ThrowIfFailed(class_definition->SpawnInstance(0, instance.GetAddressOf()));

	return ManagementObject(services_, instance);
}


std::unique_ptr<ManagementQueryProcessor> ManagementResource::GetQueryProcessor(const BasicString query, const EnumerationOptions options) noexcept
{
	return std::make_unique<ManagementQueryProcessor>(*this, query, options);
}


std::unique_ptr<ManagementQueryProcessor> ManagementResource::GetQueryProcessor(const BasicString query) noexcept
{
	return std::make_unique<ManagementQueryProcessor>(*this, query);
}


BasicString ManagementResource::GetPassword() const
{
	if (options_.secure_password.has_value())
	{
		const auto access = options_.secure_password.value().Access();
		return access->GetData();
	}

	return BasicString();
}

