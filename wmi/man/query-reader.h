#pragma once

#include <wmi/core/exports.h>
#include <wmi/core/def.h>

#include <wmi/man/result-object.h>

#include <iterator>


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


struct QueryIterator
{
	using iterator_category = std::input_iterator_tag;
	using difference_type = ptrdiff_t;
	using value_type = const ResultObject;
	using pointer = const ResultObject*;
	using reference = const ResultObject&;

	inline explicit QueryIterator(QueryReader& reader, const bool end = false) noexcept
		: reader(reader)
		, end(end)
	{ }

	inline QueryIterator& operator++() noexcept {
		if (!IsEffectiveEnd()) {
			reader.Next();
		}

		return *this;
	}

	inline QueryIterator operator++(int) noexcept {
		auto temp = *this;
		++*this;
		return temp;
	}

	WMI_NODISCARD inline ResultObject operator*() {
		return reader.Current();
	}

	WMI_NODISCARD inline bool operator!=(const QueryIterator& other) noexcept {
		return IsEffectiveEnd() != other.IsEffectiveEnd();
	}

	WMI_NODISCARD inline bool operator==(const QueryIterator& other) noexcept {
		return !(*this != other);
	}

	WMI_NODISCARD inline bool IsEffectiveEnd() const noexcept {
		return end || reader.IsDone();
	}

	QueryReader& reader;
	const bool end = false;
};

}
