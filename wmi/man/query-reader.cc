#include <wmi/man/query-reader.h>
#include <wmi/core/com-exception-factory.h>

using namespace wmi;


QueryReader::QueryReader(ComPtr<IEnumWbemClassObject> enumerator)
	: enumerator_(enumerator)
	, is_done_(false)
{
}


ResultObject QueryReader::Next()
{
	ComPtr<IWbemClassObject> object;
	ULONG returned = 0;

	ComExceptionFactory::ThrowIfFailed(enumerator_->Next(WBEM_INFINITE, 1, object.GetAddressOf(), &returned));

	if (is_done_ = returned == 0)
	{
		throw std::exception("No element");
	}

	return ResultObject(object);
}


bool QueryReader::IsDone() const
{
	return is_done_;
}


QueryIterator<QueryReader> QueryReader::begin()
{
	return QueryIterator<QueryReader>(*this);
}


QueryIterator<QueryReader> QueryReader::end()
{
	return QueryIterator<QueryReader>(*this, true);
}
