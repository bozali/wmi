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


const ManagementContext& ManagementResource::Context() const
{
	return context_;
}
