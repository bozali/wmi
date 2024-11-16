#pragma once

#include <wmi/management/management-query-stream.h>

#include <iterator>

namespace wmi {

template <typename T, typename TStream>
class ManagementQueryIterator
{
public:
	using iterator_category = std::input_iterator_tag;
	using difference_type = ptrdiff_t;
	using value_type = const ManagementObject;
	using pointer = const ManagementObject*;
	using reference = const ManagementObject&;
	using stream_type = TStream;


	_WMI_FORCEINLINE ManagementQueryIterator(stream_type& stream, const bool end = true) noexcept
		: stream_(stream)
		, end_(end)
	{
	}

	_WMI_FORCEINLINE ManagementQueryIterator& operator=(const ManagementQueryIterator& other)
	{
		if (this != &other)
		{
			stream_ = other.stream_;
			end_ = other.end_;
		}

		return *this;
	}

	_WMI_FORCEINLINE ManagementQueryIterator& operator++() noexcept
	{
		if (!IsEffectiveEnd())
		{
			stream_.Next();
		}

		return *this;
	}

	_WMI_FORCEINLINE ManagementQueryIterator operator++(int) noexcept
	{
		auto temp = *this;
		++*this;
		return temp;
	}

	_WMI_ATTR_NODISCARD _WMI_FORCEINLINE T operator*() noexcept
	{
		return stream_.Current();
	}

	_WMI_ATTR_NODISCARD _WMI_FORCEINLINE bool operator!=(const ManagementQueryIterator& other) noexcept
	{
		return IsEffectiveEnd() != other.IsEffectiveEnd();
	}

	_WMI_ATTR_NODISCARD _WMI_FORCEINLINE bool operator==(const ManagementQueryIterator& other) noexcept
	{
		return !(*this != other);
	}

	_WMI_ATTR_NODISCARD _WMI_FORCEINLINE bool IsEffectiveEnd() const noexcept
	{
		return end_ || stream_.IsDone();
	}

private:
	stream_type& stream_;
	bool end_ = true;
};


// template <typename T>
// class MappedManagementQueryIterator : public ManagementQueryIterator<T, MappedManagementQueryStream<T>> { };
// class ManagementObjectQueryIterator : public ManagementQueryIterator<ManagementObject> { };

}
