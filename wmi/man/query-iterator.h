#pragma once

#include <wmi/man/query-stream.h>

#include <iterator>


namespace wmi {

struct QueryIterator
{
	using iterator_category = std::input_iterator_tag;
	using difference_type = ptrdiff_t;
	using value_type = const ResultObject;
	using pointer = const ResultObject*;
	using reference = const ResultObject&;

	inline QueryIterator(QueryStream& stream, const bool end = true) noexcept
		: stream(stream)
		, end(end)
	{ }

	inline QueryIterator(const QueryIterator& other)
		: stream(other.stream)
		, end(other.end)
	{ }

	inline QueryIterator& operator=(const QueryIterator& other) {
		if (this != &other) {
			stream = other.stream;
			end = other.end;
		}

		return *this;
	}

	inline QueryIterator& operator++() noexcept {
		if (!IsEffectiveEnd()) {
			stream.Next();
		}

		return *this;
	}

	inline QueryIterator operator++(int) noexcept {
		auto temp = *this;
		++* this;
		return temp;
	}

	WMI_NODISCARD inline ResultObject operator*() {
		return stream.Current();
	}

	WMI_NODISCARD inline bool operator!=(const QueryIterator& other) noexcept {
		return IsEffectiveEnd() != other.IsEffectiveEnd();
	}

	WMI_NODISCARD inline bool operator==(const QueryIterator& other) noexcept {
		return !(*this != other);
	}

	WMI_NODISCARD inline bool IsEffectiveEnd() const noexcept {
		return end || stream.IsDone();
	}

	QueryStream& stream;
	bool end = true;
};

}
