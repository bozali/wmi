#pragma once

#include <wmi/common/exports.h>

#include <string>


namespace wmi {

class WMI_DLL SecureString
{
public:
	SecureString(std::string_view unprotected_data);


private:

};

}
