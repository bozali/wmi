#pragma once

#include <wmi/common/exports.h>
#include <wmi/common/common.h>

#include <string_view>
#include <vector>


namespace wmi {

class WMI_DLL ManagementObject
{
public:
  ManagementObject(const ManagementObject&) = default;
  ManagementObject& operator=(const ManagementObject&) = default;

  /**
   * Sets the value of a specified property.
   *
   * @param property_name Name of the property.
   * @param value Value to set.
   */
  void Set(const std::string_view property_name, variant_t value) noexcept(false);

  /**
   * Saves the set properties of the instance.
   */
  void Put() noexcept(false);

  /**
   * Property names.
   *
   * @return Returns a list of property names of the current instance.
   */
  _NODISCARD std::vector<bstr_t> PropertyNames();

  _NODISCARD const variant_t operator[](const char* property_name) const;

private:
  explicit ManagementObject(ComPtr<IWbemServices> services, ComPtr<IWbemClassObject> object) noexcept;
  ManagementObject() = default;

  ComPtr<IWbemClassObject> object_;
  ComPtr<IWbemServices> services_;

  friend class QueryStream;
  friend class ManagementResource;
};

}