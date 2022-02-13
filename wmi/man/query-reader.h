#pragma once

#include <wmi/man/query-iterator.h>
#include <wmi/man/result-object.h>


namespace wmi {

struct QueryIterator;

class WMI_DLL QueryReader
{
public:
	WMI_NODISCARD ResultObject Current();

	bool Next();
	WMI_NODISCARD bool IsDone() const;

	WMI_NODISCARD QueryIterator begin();
	WMI_NODISCARD QueryIterator end();

private:
	explicit QueryReader(ComPtr<IEnumWbemClassObject> enumerator);

	ComPtr<IEnumWbemClassObject> enumerator_;
	ResultObject current_;
	bool is_done_;

	friend class QueryProcessor;
};

}
