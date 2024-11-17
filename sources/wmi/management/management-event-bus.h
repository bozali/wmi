#pragma once

#include <wmi/common/non-copyable.h>
#include <wmi/common/exports.h>
#include <wmi/common/defs.h>

#include <wmi/management/management-subscription-token.h>
#include <wmi/management/management-object.h>

#include <functional>
#include <memory>
#include <vector>


namespace wmi {

class ManagementResource;
class ManagementSubscriptionToken;
class ManagementEventSinkAdapter;


class WMI_DLL ManagementEventBus
{
public:
	ManagementEventBus(std::shared_ptr<ManagementResource> resource);

	std::unique_ptr<ManagementSubscriptionToken> Subscribe(BasicString query, std::function<void(const ManagementObject&)> handler);

	/**
	 * TODO
	 */
	_WMI_ATTR_NODISCARD _WMI_FORCEINLINE const std::vector<microsoft::com_ptr<ManagementEventSinkAdapter>> Subscriptions() const noexcept {
		return subscriptions_;
	}

private:
	std::weak_ptr<ManagementResource> resource_;

	std::vector<microsoft::com_ptr<ManagementEventSinkAdapter>> subscriptions_;
};

}
