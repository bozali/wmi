#pragma once

#include <wmi/man/query-stream.h>

#include <iterator>


namespace wmi {

template <typename T>
struct MapQueryIterator
{
	using iterator_category = std::input_iterator_tag;
	using difference_type = ptrdiff_t;
	using value_type = const T;
	using pointer = const T*;
	using reference = const T&;

	using Stream = MapQueryStream<T>;

	inline MapQueryIterator(Stream& stream, const bool end = true) noexcept
		: stream(stream)
		, end(end)
	{ }

	inline MapQueryIterator(const Stream& other)
		: stream(other.stream)
		, end(other.end)
	{ }


	inline MapQueryIterator<T>& operator=(const MapQueryIterator<T>& other) {
		if (this != &other) {
			stream = other.stream;
			end = other.end;
		}

		return *this;
	}

	inline MapQueryIterator<T>& operator++() noexcept {
		if (!IsEffectiveEnd()) {
			stream.Next();
		}

		return *this;
	}

	inline MapQueryIterator<T> operator++(int) noexcept {
		auto temp = *this;
		++* this;
		return temp;
	}

	_NODISCARD inline T operator*() {
		return stream.Current();
	}

	_NODISCARD inline bool operator!=(const MapQueryIterator<T>& other) noexcept {
		return IsEffectiveEnd() != other.IsEffectiveEnd();
	}

	_NODISCARD inline bool operator==(const MapQueryIterator<T>& other) noexcept {
		return !(*this != other);
	}

	_NODISCARD inline bool IsEffectiveEnd() const noexcept {
		return end || stream.IsDone();
	}

	Stream& stream;
	bool end = true;
};


struct QueryIterator
{
	using iterator_category = std::input_iterator_tag;
	using difference_type = ptrdiff_t;
	using value_type = const ManagementObject;
	using pointer = const ManagementObject*;
	using reference = const ManagementObject&;

	using Stream = ManagementQueryStream;

	inline QueryIterator(Stream& stream, const bool end = true) noexcept
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

	_NODISCARD inline ManagementObject operator*() {
		return stream.Current();
	}

	_NODISCARD inline bool operator!=(const QueryIterator& other) noexcept {
		return IsEffectiveEnd() != other.IsEffectiveEnd();
	}

	_NODISCARD inline bool operator==(const QueryIterator& other) noexcept {
		return !(*this != other);
	}

	_NODISCARD inline bool IsEffectiveEnd() const noexcept {
		return end || stream.IsDone();
	}

	Stream& stream;
	bool end = true;
};


}
