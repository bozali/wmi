#pragma once

#include <wmi/man/result-object.h>
#include <wmi/man/enumeration-options.h>

#include <optional>


namespace wmi {

struct QueryIterator;

class WMI_DLL QueryStream
{
public:
	WMI_NODISCARD ResultObject Current();

	bool Next();
	WMI_NODISCARD bool IsDone() const;

	WMI_NODISCARD QueryIterator begin();
	WMI_NODISCARD QueryIterator end();

private:
	explicit QueryStream(ComPtr<IEnumWbemClassObject> enumerator, EnumerationOptions option);

	ComPtr<IEnumWbemClassObject> enumerator_;
	EnumerationOptions options_;
	ResultObject current_;
	bool is_done_;

	friend class QueryProcessor;
};

}
