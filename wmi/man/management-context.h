#pragma once

#include <wmi/core/exports.h>
#include <wmi/core/def.h>


namespace wmi {

class WMI_DLL ManagementContext
{
public:
	ManagementContext();

private:
	ComPtr<IWbemLocator> locator_;
};

}
