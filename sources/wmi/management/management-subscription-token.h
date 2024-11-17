#pragma once

#include <wmi/management/management-event-sink-adapter.h>
#include <wmi/management/management-resource.h>

#include <memory>

namespace wmi {

/**
 * The `ManagementSubscriptionToken` class is used to manage an active subscription to a WMI event.
 * It provides functionality to unsubscribe from the event, ensuring that resources are released
 * and no further notifications are received. Instances of this class are typically returned when
 * subscribing to events through a `ManagementEventBus`.
 */
class WMI_DLL ManagementSubscriptionToken
{
public:
	ManagementSubscriptionToken(std::shared_ptr<ManagementResource> resource, microsoft::com_ptr<ManagementEventSinkAdapter> adapter);
	~ManagementSubscriptionToken();

	/**
	 * This method cancels the subscription linked to this token, stopping further event
	 * notifications from being delivered. It ensures that system resources related to
	 * the subscription are properly released.
	 */
	void Unsubscribe();

private:
	microsoft::com_ptr<ManagementEventSinkAdapter> adapter_;
	std::weak_ptr<ManagementResource> resource_;
};

}