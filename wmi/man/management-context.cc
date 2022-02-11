#include <wmi/man/management-context.h>
#include <wmi/core/com-exception-factory.h>

using namespace wmi;


ManagementContext::ManagementContext()
{
	ComExceptionFactory::ThrowIfFailed(CoCreateInstance(CLSID_WbemLocator,
														nullptr,
														CLSCTX_INPROC_SERVER,
														IID_IWbemLocator,
														reinterpret_cast<void**>(locator_.GetAddressOf())));
}
