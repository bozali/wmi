#pragma once

#include <wmi/common/com-exception.h>

#include <wmi/man/enumeration-options.h>
#include <wmi/man/man-resource.h>
#include <wmi/man/man-object.h>
#include <wmi/man/query-iterator.h>

#include <type_traits>


namespace wmi {

template <typename T, typename = typename std::enable_if<std::is_default_constructible<T>::value>>
inline static void MapManagementObject(const ManagementObject& from, T& to)
{ }

template <typename T>
_NODISCARD inline T MapManagementObjectImpl(const ManagementObject& object) {
  T instance;
  MapManagementObject(object, instance);
  return instance;
}


template <typename T> struct MapQueryIterator;
struct QueryIterator;



template <typename T, typename TIterator>
class GenericQueryStream
{
public:
  using DataType = T;
  using Iterator = TIterator;

  /**
   * Fetches the next instance.
   *
   * @return Returns false if cannot access the instance or if there is no instance.
   */
  bool Next() noexcept(false) {
    ComPtr<IWbemClassObject> object;
    ULONG returned = 0;

    using milliseconds = std::chrono::milliseconds;

    auto timeout = options_.timeout.has_value()
      ? std::chrono::duration_cast<milliseconds>(options_.timeout.value()).count()
      : WBEM_INFINITE;

    ComExceptionFactory::ThrowIfFailed(enumerator_->Next(timeout, 1, object.GetAddressOf(), &returned));

    if (is_done_ = returned == 0)
    {
      return false;
    }

    current_ = ManagementObject(services_, object);

    return true;
  }

  /**
   * Accesses the current instance.
   *
   * @return Returns the current instance if the stream.
   */
  _NODISCARD inline T Current() noexcept {
		if constexpr (std::is_same<TIterator, MapQueryIterator<T>>::value) {
			return MapManagementObjectImpl<T>(current_);
		}
		else {
			return current_;
		}
  }

  _NODISCARD inline bool IsDone() const noexcept {
    return is_done_;
  }

  _NODISCARD inline Iterator begin() {
    return Iterator(*this, false);
  }

  _NODISCARD inline Iterator end() {
    return Iterator(*this, true);
  }

private:
  GenericQueryStream(ComPtr<IWbemServices> services, ComPtr<IEnumWbemClassObject> enumerator, EnumerationOptions options) noexcept(false)
    : enumerator_(enumerator)
    , services_(services)
    , options_(options)
    , is_done_(false)
  {
    Next();
  }


  ComPtr<IEnumWbemClassObject> enumerator_;
  ComPtr<IWbemServices> services_;

  EnumerationOptions options_;
  ManagementObject current_;
  bool is_done_;

  friend class ManagementQueryProcessor;
};


template <typename T> using MapQueryStream = GenericQueryStream<T, MapQueryIterator<T>>;
using ManagementQueryStream = GenericQueryStream<ManagementObject, QueryIterator>;

}


