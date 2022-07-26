#pragma once

#include <wmi/common/exports.h>
#include <wmi/common/common.h>

#include <vector>


namespace wmi {

class WMI_DLL ManagementObject
{
public:
  ManagementObject(const ManagementObject&) = default;
  ManagementObject& operator=(const ManagementObject&) = default;


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