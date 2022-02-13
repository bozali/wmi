#include <wmi/man/query-stream.h>
#include <wmi/man/query-iterator.h>
#include <wmi/core/com-exception-factory.h>

using namespace wmi;


QueryStream::QueryStream(ComPtr<IEnumWbemClassObject> enumerator, EnumerationOptions options)
	: enumerator_(enumerator)
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

	auto timeout = WBEM_INFINITE; // std::chrono::duration_cast<milliseconds>(options_.timeout).count();

	ComExceptionFactory::ThrowIfFailed(enumerator_->Next(timeout, 1, object.GetAddressOf(), &returned));

	if (is_done_ = returned == 0)
	{
		return false;
	}

	current_ = ResultObject(object);
	return true;
}


bool QueryStream::IsDone() const
{
	return is_done_;
}


QueryIterator QueryStream::begin()
{
	return QueryIterator(*this);
}


QueryIterator QueryStream::end()
{
	return QueryIterator(*this, true);
}
