#pragma once

#include <wmi/management/management-event-sink-adapter.h>
#include <wmi/management/management-resource.h>

#include <memory>

namespace wmi {

class WMI_DLL ManagementSubscriptionToken
{
public:
	ManagementSubscriptionToken(std::shared_ptr<ManagementResource> resource, microsoft::com_ptr<ManagementEventSinkAdapter> adapter);
	~ManagementSubscriptionToken();

	void Unsubscribe();

private:
	std::weak_ptr<ManagementResource> resource_;
	microsoft::com_ptr<ManagementEventSinkAdapter> adapter_;
};

}