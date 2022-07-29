#pragma once

#include <wmi/man/query-stream.h>

#include <iterator>


namespace wmi {

template <typename T, typename TStream>
struct QueryIteratorBase
{
	using iterator_category = std::input_iterator_tag;
	using difference_type = ptrdiff_t;
	using value_type = const T;
	using pointer = const T*;
	using reference = const T&;

	using Stream = TStream;

	inline QueryIteratorBase(Stream& stream, const bool end = true) noexcept
		: stream(stream)
		, end(end)
	{ }

	inline QueryIteratorBase(const Stream& other)
		: stream(other.stream)
		, end(other.end)
	{ }


	inline QueryIteratorBase& operator=(const QueryIteratorBase& other) {
		if (this != &other) {
			stream = other.stream;
			end = other.end;
		}

		return *this;
	}

	inline QueryIteratorBase& operator++() noexcept {
		if (!IsEffectiveEnd()) {
			stream.Next();
		}

		return *this;
	}

	inline QueryIteratorBase operator++(int) noexcept {
		auto temp = *this;
		++* this;
		return temp;
	}

	_NODISCARD inline T operator*() {
		return stream.Current();
	}

	_NODISCARD inline bool operator!=(const QueryIteratorBase& other) noexcept {
		return IsEffectiveEnd() != other.IsEffectiveEnd();
	}

	_NODISCARD inline bool operator==(const QueryIteratorBase& other) noexcept {
		return !(*this != other);
	}

	_NODISCARD inline bool IsEffectiveEnd() const noexcept {
		return end || stream.IsDone();
	}

	Stream& stream;
	bool end = true;
};



template <typename T>
struct MapQueryIterator : public QueryIteratorBase<T, MapQueryStream<T>> { };
struct QueryIterator : public QueryIteratorBase<ManagementObject, ManagementQueryStream> { };


}
