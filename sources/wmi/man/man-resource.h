#pragma once

#include <wmi/common/exports.h>
#include <wmi/common/common.h>

#include <wmi/man/man-object.h>

#include <string_view>
#include <optional>
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

  /**
   * Executes a WMI class method.
   *
   * @param [class_name] Path to the class to execute method from e.g. \\ROOT\CIMV2\Win32_Process.
   * @param [method_name] Name of the method to execute.
   * @param [parameters] Input for the method, is optional.
   *
   * @return Returns the result of the method execution.
   */
  ManagementObject ExecuteMethod(const std::string_view class_name, const std::string_view method_name, std::optional<std::unordered_map<std::string_view, variant_t>> parameters = std::nullopt) noexcept(false);

  /**
   * Creating WMI instance.
   *
   * @param [class_name] Name of the class.
   *
   * @return Returns the newly created instance.
   */
  ManagementObject CreateInstance(const char* class_name) noexcept(false);


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

  friend class ManagementQueryProcessor;
};

}
