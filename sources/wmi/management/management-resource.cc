#include <wmi/management/management-resource.h>
#include <wmi/management/enumeration-options.h>
#include <wmi/management/management-query-processor.h>

#include <wmi/common/secure-string-access.h>
#include <wmi/common/secure-string.h>
#include <wmi/common/com-exception.h>

using namespace wmi;

ManagementResource::ManagementResource(const bstr_t path, ConnectionOptions options) noexcept
	: resource_path_(path)
	, is_connected_(false)
	, options_(options)
{
}


ManagementResource::ManagementResource(const bstr_t path) noexcept
	: resource_path_(path)
	, is_connected_(false)
{
}


void ManagementResource::Connect(const bstr_t path, const ConnectionOptions options) noexcept(false)
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

		const auto username = options_.username.has_value() ? options.username.value() : bstr_t();
		const auto locale = options_.locale.has_value() ? options.locale.value() : bstr_t();
		const auto authority = options_.authority.has_value() ? options.authority.value() : bstr_t();

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


std::unique_ptr<ManagementQueryProcessor> ManagementResource::GetQueryProcessor(const bstr_t query, const EnumerationOptions options) noexcept
{
	return std::make_unique<ManagementQueryProcessor>(*this, query, options);
}


std::unique_ptr<ManagementQueryProcessor> ManagementResource::GetQueryProcessor(const bstr_t query) noexcept
{
	return std::make_unique<ManagementQueryProcessor>(*this, query);
}


bstr_t ManagementResource::GetPassword() const
{
	if (options_.secure_password.has_value())
	{
		const auto access = options_.secure_password.value().Access();
		return access->GetData();
	}

	return bstr_t();
}

