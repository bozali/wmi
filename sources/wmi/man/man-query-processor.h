#pragma once

#include <wmi/common/com-exception.h>
#include <wmi/common/exports.h>

#include <wmi/man/enumeration-options.h>
#include <wmi/man/query-iterator.h>
#include <wmi/man/query-stream.h>


namespace wmi {


class WMI_DLL ManagementQueryProcessor
{
public:
  ManagementQueryProcessor(const ManagementResource& resource, const std::string_view query, EnumerationOptions options) noexcept;
  ManagementQueryProcessor(const ManagementResource& resource, const std::string_view query = "") noexcept;

  /**
   * Executes the specified query and returns a stream of instances.
   *
   * @return Stream of instances.
   */
  _NODISCARD ManagementQueryStream GetStream() noexcept(false) {
    return ManagementQueryStream(resource_->services_, InternalQueryExecute(), options_);
  }

  template <typename T>
  _NODISCARD inline GenericQueryStream<T, MapQueryIterator<T>> GetStream() {
    return GenericQueryStream<T, MapQueryIterator<T>>(resource_->services_, InternalQueryExecute(), options_);
  }

  void SetResource(const ManagementResource& resource) noexcept;

  void SetQuery(const std::string_view query) noexcept;

  _NODISCARD inline const ManagementResource& GetResource() const {
    return *resource_;
  }

  _NODISCARD inline const std::string_view GetQuery() const noexcept {
    return query_;
  }

private:
  _NODISCARD ComPtr<IEnumWbemClassObject> InternalQueryExecute() noexcept(false);

  const ManagementResource* resource_;
  EnumerationOptions options_;

  std::string_view query_;
};

}