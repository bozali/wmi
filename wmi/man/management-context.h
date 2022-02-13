#pragma once

#include <wmi/core/exports.h>
#include <wmi/core/def.h>

#include <Windows.h>
#include <comdef.h>


namespace wmi {

class WMI_DLL ManagementContext
{
public:
	ManagementContext();

private:
	ComPtr<IWbemLocator> locator_;

	friend class ManagementResource;
};

}
