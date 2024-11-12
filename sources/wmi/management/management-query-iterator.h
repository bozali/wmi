#pragma once

#include <wmi/management/management-query-stream.h>

#include <iterator>

namespace wmi {

template <typename T, typename TStream>
class WMI_DLL ManagementQueryIteratorBase
{
public:
	using iterator_category = std::input_iterator_tag;
	using difference_type = ptrdiff_t;
	using value_type = const T;
	using pointer = const T*;
	using reference = const T&;
	using stream_type = TStream;


	_WMI_FORCEINLINE ManagementQueryIteratorBase(const stream_type& stream, const bool end = true) noexcept
		: stream_(stream)
		, end_(end)
	{
	}

	_WMI_FORCEINLINE ManagementQueryIteratorBase(const stream_type& other_stream) noexcept
		: stream_(other_stream.stream_)
		, end_(other_stream.end_)
	{
	}

	_WMI_FORCEINLINE ManagementQueryIteratorBase& operator=(const ManagementQueryIteratorBase& other)
	{
		if (this != other)
		{
			stream_ = other.stream_;
			end_ = other.end_;
		}

		return *this;
	}

	_WMI_FORCEINLINE ManagementQueryIteratorBase& operator++() noexcept
	{
		if (!IsEffectiveEnd())
		{
			stream_.Next();
		}

		return *this;
	}

	_WMI_FORCEINLINE ManagementQueryIteratorBase operator++(int) noexcept
	{
		auto temp = *this;
		++*this;
		return temp;
	}

	_WMI_ATTR_NODISCARD _WMI_FORCEINLINE T operator*() noexcept(false)
	{
		return stream_.Current();
	}

	_WMI_ATTR_NODISCARD _WMI_FORCEINLINE bool operator!=(const ManagementQueryIteratorBase& other) noexcept
	{
		return IsEffectiveEnd() != other.IsEffectiveEnd();
	}

	_WMI_ATTR_NODISCARD _WMI_FORCEINLINE bool operator==(const ManagementQueryIteratorBase& other) noexcept
	{
		return !(*this != other);
	}

	_WMI_ATTR_NODISCARD _WMI_FORCEINLINE bool IsEffectiveEnd() const noexcept
	{
		return end_ || stream_.IsDone();
	}

private:
	const stream_type& stream_;
	bool end_ = true;
};


template <typename T>
class MappedManagementQueryIterator : public ManagementQueryIteratorBase<T, MappedManagementQueryIterator<T>> {};
class ManagementObjectQueryIterator : public ManagementQueryIteratorBase<ManagementObject, ManagementObjectQueryStream> {};

}
