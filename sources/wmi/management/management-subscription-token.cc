#include <wmi/management/management-subscription-token.h>
#include <wmi/management/management-resource.h>

using namespace wmi;


void ManagementSubscriptionToken::Unsubscribe()
{
	if (std::shared_ptr<ManagementResource> ptr = resource_.lock())
	{
		ptr->services_->CancelAsyncCall(adapter_.Get());
	}
}


ManagementSubscriptionToken::ManagementSubscriptionToken(std::shared_ptr<ManagementResource> resource, microsoft::com_ptr<ManagementEventSinkAdapter> adapter)
	: resource_(resource)
	, adapter_(adapter)
{
}


ManagementSubscriptionToken::~ManagementSubscriptionToken()
{
	Unsubscribe();
}
