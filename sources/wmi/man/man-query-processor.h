#pragma once

#include <wmi/common/exports.h>

#include <wmi/man/enumeration-options.h>
#include <wmi/man/query-iterator.h>
#include <wmi/man/query-stream.h>


namespace wmi {


class WMI_DLL ManagementQueryProcessor final
{
public:
  ManagementQueryProcessor(const ManagementResource& resource, const std::string_view query, EnumerationOptions options) noexcept;
  ManagementQueryProcessor(const ManagementResource& resource, const std::string_view query = "") noexcept;

  /**
   * Executes the specified query and returns a stream of instances.
   *
   * @return Stream of instances.
   */
  _NODISCARD QueryStream GetStream() noexcept(false);

  void SetResource(const ManagementResource& resource) noexcept;

  void SetQuery(const std::string_view query) noexcept;

  _NODISCARD inline const ManagementResource& Resource() const {
    return *resource_;
  }

  _NODISCARD inline const std::string_view Query() const noexcept {
    return query_;
  }

private:
  const const ManagementResource* resource_;
  EnumerationOptions options_;

  std::string_view query_;
};

}