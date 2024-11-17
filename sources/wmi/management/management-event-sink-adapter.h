#pragma once

#include <wmi/common/exports.h>
#include <wmi/common/defs.h>

#include <wmi/management/management-resource.h>
#include <wmi/management/management-object.h>

#include <functional>
#include <vector>

namespace wmi {


class WMI_DLL ManagementEventSinkAdapter final : public IWbemObjectSink
{
public:
  ManagementEventSinkAdapter(std::shared_ptr<ManagementResource> resource, std::function<void(const ManagementObject&)> handler) noexcept;
  ~ManagementEventSinkAdapter() noexcept;

  ULONG STDMETHODCALLTYPE AddRef();

  ULONG STDMETHODCALLTYPE Release();

  HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppv);

  HRESULT STDMETHODCALLTYPE Indicate(LONG object_count, IWbemClassObject __RPC_FAR* __RPC_FAR* object_array);

  HRESULT STDMETHODCALLTYPE SetStatus(LONG flags,
                                      HRESULT result,
                                      BSTR str_param,
                                      IWbemClassObject __RPC_FAR* object_param);

private:
  LONG ref_;
  bool is_done_;

  std::weak_ptr<ManagementResource> resource_;
  std::function<void(const ManagementObject&)> handler_;
};

}