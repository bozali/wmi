#include <wmi/management/management-event-sink-adapter.h>
#include <iostream>

using namespace wmi;


ManagementEventSinkAdapter::ManagementEventSinkAdapter(std::shared_ptr<ManagementResource> resource, std::function<void(const ManagementObject&)> handler) noexcept
  : ref_(0)
  , resource_(resource)
  , handler_(handler)
{
}


ManagementEventSinkAdapter::~ManagementEventSinkAdapter()
{
  is_done_ = true;
}


ULONG STDMETHODCALLTYPE ManagementEventSinkAdapter::AddRef()
{
  return InterlockedIncrement(&ref_);
}


ULONG STDMETHODCALLTYPE ManagementEventSinkAdapter::Release()
{
  LONG ref = InterlockedDecrement(&ref_);

  if (ref == 0)
  {
    delete this;
  }

  return ref;
}


HRESULT STDMETHODCALLTYPE ManagementEventSinkAdapter::QueryInterface(REFIID riid, void** ppv)
{
  if (riid == IID_IUnknown || riid == IID_IWbemObjectSink)
  {
    *ppv = (IWbemObjectSink*)this;
    AddRef();
    return WBEM_S_NO_ERROR;
  }

  else return E_NOINTERFACE;
}

HRESULT STDMETHODCALLTYPE ManagementEventSinkAdapter::Indicate(LONG object_count, IWbemClassObject __RPC_FAR* __RPC_FAR* object_array)
{
  HRESULT hres = S_OK;

  for (int i = 0; i < object_count; ++i)
  {
    std::cout << "EventSink->Indicate" << std::endl;

    if (std::shared_ptr<ManagementResource> ptr = resource_.lock())
    {
      handler_(ManagementObject(ptr->services_, object_array[i]));
    }
  }

  return WBEM_S_NO_ERROR;
}

HRESULT STDMETHODCALLTYPE ManagementEventSinkAdapter::SetStatus(LONG flags,
                                                                HRESULT result,
                                                                BSTR str_param,
                                                                IWbemClassObject __RPC_FAR* object_param)
{
  return WBEM_S_NO_ERROR;
}