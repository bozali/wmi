#include <wmi/man/query-reader.h>
#include <wmi/core/com-exception-factory.h>

using namespace wmi;


QueryReader::QueryReader(ComPtr<IEnumWbemClassObject> enumerator)
	: enumerator_(enumerator)
	, is_done_(false)
{
	this->Next();
}


ResultObject QueryReader::Current()
{
	return current_;
}


bool QueryReader::Next()
{
	ComPtr<IWbemClassObject> object;
	ULONG returned = 0;

	ComExceptionFactory::ThrowIfFailed(enumerator_->Next(WBEM_INFINITE, 1, object.GetAddressOf(), &returned));

	if (is_done_ = returned == 0)
	{
		return false;
	}

	current_ = ResultObject(object);
	return true;
}


bool QueryReader::IsDone() const
{
	return is_done_;
}


QueryIterator QueryReader::begin()
{
	return QueryIterator(*this);
}


QueryIterator QueryReader::end()
{
	return QueryIterator(*this, true);
}
