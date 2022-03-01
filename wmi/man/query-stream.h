#pragma once

#include <wmi/man/result-object.h>
#include <wmi/man/enumeration-options.h>

#include <optional>


namespace wmi {

struct QueryIterator;

class WMI_DLL QueryStream
{
public:
	/**
	 * Accesses the current instance.
	 * 
	 * @return Returns the current instance if the stream.
	 */
	WMI_NODISCARD ResultObject Current();

	/**
	 * Fetches the next instance.
	 * 
	 * @return Returns false if cannot access the instance or if there is no instance.
	 */
	bool Next();

	/** Checks if the stream is done. */
	WMI_NODISCARD bool IsDone() const;

	WMI_NODISCARD QueryIterator begin();
	WMI_NODISCARD QueryIterator end();

private:
	explicit QueryStream(ComPtr<IWbemServices> services, ComPtr<IEnumWbemClassObject> enumerator, EnumerationOptions option);

	ComPtr<IEnumWbemClassObject> enumerator_;
	ComPtr<IWbemServices> services_;

	EnumerationOptions options_;
	ResultObject current_;
	bool is_done_;

	friend class QueryProcessor;
};

}
