#pragma once

#include <wmi/common/secure-string.h>
#include <wmi/common/exports.h>
#include <wmi/common/defs.h>


namespace wmi {

class WMI_DLL SecureStringAccess final
{
public:
	explicit SecureStringAccess(const SecureString& secure_str);
	~SecureStringAccess();

	_WMI_ATTR_NODISCARD _WMI_FORCEINLINE bstr_t GetData() const
	{
		return secure_.GetData();
	}

private:
	const SecureString& secure_;

	friend class SecureString;
};

}