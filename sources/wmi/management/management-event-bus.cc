#include <wmi/management/management-event-bus.h>
#include <wmi/management/management-event-sink-adapter.h>

#include <wmi/common/com-exception.h>

using namespace wmi;


ManagementEventBus::ManagementEventBus(std::shared_ptr<ManagementResource> resource)
	: resource_(resource)
{
}


std::unique_ptr<ManagementSubscriptionToken> ManagementEventBus::Subscribe(BasicString query, std::function<void(const ManagementObject&)> handler)
{
	if (std::shared_ptr<ManagementResource> ptr = resource_.lock())
	{
		ManagementEventSinkAdapter* adapter = new ManagementEventSinkAdapter(ptr, handler);

		ComExceptionFactory::ThrowIfFailed(ptr->services_->ExecNotificationQueryAsync(BasicString("WQL"),
																																									query,
																																									WBEM_FLAG_SEND_STATUS,
																																									nullptr,
																																									adapter));

		subscriptions_.emplace_back(adapter);

		return std::make_unique<ManagementSubscriptionToken>(ptr, adapter);
	}

	throw std::exception("ManagementResource was already deleted");
}

