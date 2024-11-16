#include <wmi/common/secure-string.h>
#include <wmi/common/secure-string-access.h>

#include <iostream>
using namespace wmi;


SecureString::SecureString(const BasicString&& unprotected_data)
{
	ProtectMemory(std::move(unprotected_data));
}


SecureString::~SecureString()
{
}


std::unique_ptr<SecureStringAccess> SecureString::Access() const
{
	return std::make_unique<SecureStringAccess>(*this);
}


void SecureString::FreeMemory() const noexcept
{
	std::fill(std::begin(encrypted_data_), std::end(encrypted_data_), 0);

	encrypted_data_.clear();
	encrypted_data_.shrink_to_fit();
}


void SecureString::ProtectMemory(const BasicString&& unprotected_data) const
{
	encrypted_data_.resize(unprotected_data.length());

	std::memcpy(&encrypted_data_[0], &unprotected_data, unprotected_data.length() * sizeof(tchar));

	if (!CryptProtectMemory(&encrypted_data_[0], static_cast<Dword>(encrypted_data_.size() * sizeof(tchar)), CRYPTPROTECTMEMORY_SAME_PROCESS))
	{
		// Failed
		return;
	}
}


void SecureString::UnprotectMemory() const
{
	if (!CryptUnprotectMemory(&encrypted_data_[0], static_cast<Dword>(encrypted_data_.size() * sizeof(tchar)), CRYPTPROTECTMEMORY_SAME_PROCESS))
	{
		return;
	}
}

