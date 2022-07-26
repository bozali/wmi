#include <wmi/man/query-stream.h>
#include <wmi/man/query-iterator.h>

#include <wmi/common/com-exception.h>

using namespace wmi;

QueryStream::QueryStream(ComPtr<IWbemServices> services, ComPtr<IEnumWbemClassObject> enumerator, EnumerationOptions options) noexcept(false)
	: enumerator_(enumerator)
	, services_(services)
	, options_(options)
	, is_done_(false)
{
	this->Next();
}


bool QueryStream::Next() noexcept(false)
{
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


QueryIterator QueryStream::begin() noexcept
{
	return QueryIterator(*this, false);
}


QueryIterator QueryStream::end() noexcept
{
	return QueryIterator(*this, true);
}
