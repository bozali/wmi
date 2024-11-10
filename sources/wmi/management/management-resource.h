#pragma once

#include <wmi/common/secure-string.h>
#include <wmi/common/exports.h>

#include <string_view>
#include <optional>
#include <chrono>


namespace wmi {

struct ConnectionOptions
{
	std::chrono::milliseconds timeout;
	
	std::optional<bstr_t> authority = std::nullopt;
	std::optional<bstr_t> locale = std::nullopt;
	std::optional<bstr_t> username = std::nullopt;
	std::optional<bstr_t> password = std::nullopt;
	std::optional<SecureString> secure_password = std::nullopt;
};


class WMI_DLL ManagementResource
{
public:
	ManagementResource(const bstr_t path, ConnectionOptions options) noexcept;
	ManagementResource(const bstr_t path) noexcept;

	void Connect(const bstr_t path, const ConnectionOptions options) noexcept(false);
	void Connect() noexcept(false);

	void SetOptions(const ConnectionOptions options) noexcept;

	void SetPath(const bstr_t path) noexcept;

	_WMI_ATTR_NODISCARD _WMI_FORCEINLINE const ConnectionOptions& Options() const noexcept {
		return options_;
	}

	_WMI_ATTR_NODISCARD _WMI_FORCEINLINE bstr_t Path() const noexcept {
		return resource_path_;
	}

	_WMI_ATTR_NODISCARD _WMI_FORCEINLINE bool IsConnected() const noexcept {
		return is_connected_;
	}

private:
	ManagementResource(const ManagementResource&) = default;
	ManagementResource& operator=(const ManagementResource&) = default;

	bstr_t GetPassword() const;

private:
	ConnectionOptions options_;

	bool is_connected_;
	bstr_t resource_path_;

	microsoft::com_ptr<IWbemServices> services_;
	microsoft::com_ptr<IWbemLocator> locator_;
};

}
