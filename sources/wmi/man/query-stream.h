#pragma once

#include <wmi/man/enumeration-options.h>
#include <wmi/man/man-object.h>


namespace wmi {

struct QueryIterator;


class WMI_DLL QueryStream
{
public:
  /**
   * Fetches the next instance.
   *
   * @return Returns false if cannot access the instance or if there is no instance.
   */
  bool Next();

  /**
   * Accesses the current instance.
   *
   * @return Returns the current instance if the stream.
   */
  _NODISCARD ManagementObject Current() const noexcept {
    return current_;
  }

  _NODISCARD inline bool IsDone() const noexcept {
    return is_done_;
  }

  _NODISCARD QueryIterator begin() noexcept;
  _NODISCARD QueryIterator end() noexcept;

private:
  explicit QueryStream(ComPtr<IWbemServices> services, ComPtr<IEnumWbemClassObject> enumerator, EnumerationOptions options) noexcept(false);

  ComPtr<IEnumWbemClassObject> enumerator_;
  ComPtr<IWbemServices> services_;

  EnumerationOptions options_;
  ManagementObject current_;
  bool is_done_;

  friend class ManagementQueryProcessor;
};

}
