#pragma once

#include <wmi/common/exports.h>
#include <wmi/common/common.h>

#include <string>
#include <chrono>


namespace wmi {


struct ConnectionOptions
{
  std::chrono::milliseconds timeout;
};


class WMI_DLL ManagementResource
{
public:
  ManagementResource(const std::string_view path, ConnectionOptions options) noexcept;
  ManagementResource(const std::string_view path = "") noexcept;

  /**
   * Connects to a WMI resource.
   *
   * @param [path] The path to the management resource e.g. \\ROOT\CIMV2
   * @param [options] Connection options.
   *
   * @throw ComException Thrown if connection cannot be established.
   */
  void Connect(const std::string_view path, const ConnectionOptions options) noexcept(false);

  /**
   * Connects to a WMI resource using the specified options.
   *
   * @throw ComException Thrown if connection cannot be established.
   */
  void Connect() noexcept(false);

  void SetOptions(ConnectionOptions options) noexcept;

  void SetPath(const std::string_view path) noexcept;

  _NODISCARD inline const ConnectionOptions& Options() const noexcept {
    return options_;
  }

  _NODISCARD inline const std::string_view Path() const noexcept {
    return resource_path_;
  }

  _NODISCARD inline const bool IsConnected() const noexcept {
    return is_connected_;
  }

private:
  ManagementResource(const ManagementResource&) = default;
  ManagementResource& operator=(const ManagementResource&) = default;

private:
  ConnectionOptions options_;

  ComPtr<IWbemServices> services_;
  ComPtr<IWbemLocator> locator_;

  std::string_view resource_path_;
  bool is_connected_;
};

}
