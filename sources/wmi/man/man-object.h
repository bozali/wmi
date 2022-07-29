#pragma once

#include <wmi/common/exports.h>
#include <wmi/common/common.h>

#include <unordered_map>
#include <string_view>
#include <optional>
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
   * @param [property_name] Name of the property.
   * @param [value] Value to set.
   */
  void Set(const std::string_view property_name, variant_t value) noexcept(false);

  /**
   * Saves the set properties of the instance.
   */
  void Put() noexcept(false);

  /**
   * Executes the method of the current instance.
   *
   * @param [method_name] Name of the method to execute.
   * @param [parameters] Input for the method, is optional.
   *
   * @return Returns the result of the method execution.
   */
  ManagementObject ExecuteMethod(const std::string_view method_name, std::optional<std::unordered_map<std::string_view, variant_t>> parameters = std::nullopt) noexcept(false);

  /**
   * Property names.
   *
   * @return Returns a list of property names of the current instance.
   */
  _NODISCARD std::vector<bstr_t> PropertyNames() noexcept(false);

  /**
   * Read the value of a property.
   * 
   * @param [property_name] Name of the property to read.
   * 
   * @return Returns the variant of the property.
   */
  _NODISCARD const variant_t operator[](const std::string_view property_name) const noexcept(false);

private:
  explicit ManagementObject(ComPtr<IWbemServices> services, ComPtr<IWbemClassObject> object) noexcept;
  ManagementObject() = default;

  ComPtr<IWbemClassObject> object_;
  ComPtr<IWbemServices> services_;

  template <typename T, typename TIterator> friend class GenericQueryStream;
  friend class ManagementResource;
};

}