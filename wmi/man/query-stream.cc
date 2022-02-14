#include <wmi/man/query-stream.h>
#include <wmi/man/query-iterator.h>
#include <wmi/core/com-exception-factory.h>

using namespace wmi;


QueryStream::QueryStream(ComPtr<IWbemServices> services, ComPtr<IEnumWbemClassObject> enumerator, EnumerationOptions options)
	: enumerator_(enumerator)
	, services_(services)
	, options_(options)
	, is_done_(false)
{
	this->Next();
}


ResultObject QueryStream::Current()
{
	return current_;
}


bool QueryStream::Next()
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

	current_ = ResultObject(services_, object);
	return true;
}


bool QueryStream::IsDone() const
{
	return is_done_;
}


QueryIterator QueryStream::begin()
{
	return QueryIterator(*this, false);
}


QueryIterator QueryStream::end()
{
	return QueryIterator(*this, true);
}
