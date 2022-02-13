#pragma once

#include <wmi/core/exports.h>
#include <wmi/core/def.h>

#include <wmi/man/result-object.h>

#include <memory>


namespace wmi {

template <typename T>
struct QueryIterator
{
	QueryIterator(T& reader, const bool end = false)
		: reader(reader)
		, end(end)
	{ }

	ResultObject operator*() {
	}

	inline QueryIterator& operator++() {
		reader.Next();
	}

	[[nodiscard]] inline bool operator!=(const QueryIterator& other) {
	}

	T& reader;
	const bool end = false;
};


class WMI_DLL QueryReader
{
public:

	ResultObject Next();

	bool IsDone() const;

	QueryIterator<QueryReader> begin();
	QueryIterator<QueryReader> end();

private:
	QueryReader(ComPtr<IEnumWbemClassObject> enumerator);

	ComPtr<IEnumWbemClassObject> enumerator_;
	bool is_done_;

	friend class QueryProcessor;
};

}
