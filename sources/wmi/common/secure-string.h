#pragma once

#include <wmi/common/exports.h>
#include <wmi/common/defs.h>

#include <string_view>
#include <vector>
#include <string>
#include <memory>


namespace wmi {

class SecureStringAccess;


class WMI_DLL SecureString final
{
public:
	SecureString(const bstr_t&& unprotected_data);
	~SecureString();

	_WMI_ATTR_NODISCARD std::unique_ptr<SecureStringAccess> Access() const;

private:
	_WMI_ATTR_NODISCARD _WMI_FORCEINLINE bstr_t GetData() const
	{
		return bstr_t(reinterpret_cast<tchar*>(&encrypted_data_[0]), encrypted_data_.size());
	}

	void FreeMemory() const noexcept;

	void ProtectMemory(const bstr_t&& unprotected_data) const;

	void UnprotectMemory() const;

private:
	size_t secure_byte_length_;

	mutable std::vector<tbyte> encrypted_data_;

	friend class SecureStringAccess;
};

}
