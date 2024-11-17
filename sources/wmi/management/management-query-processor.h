#pragma once

#include <wmi/common/exports.h>
#include <wmi/common/defs.h>

#include <wmi/management/enumeration-options.h>
#include <wmi/management/management-resource.h>
#include <wmi/management/management-query-stream.h>
#include <wmi/management/management-query-iterator.h>


namespace wmi {

class WMI_DLL ManagementQueryProcessor
{
public:
	ManagementQueryProcessor(std::shared_ptr<ManagementResource> resource, const BasicString query, const EnumerationOptions enumeration_options) noexcept;
	ManagementQueryProcessor(std::shared_ptr<ManagementResource> resource, const BasicString query) noexcept;

	/**
	 * This method creates and returns a `ManagementQueryStream<ManagementObject>` that streams 
	 * the results of the WMI query associated with this query processor. Each item in the stream 
	 * represents a `ManagementObject`, which contains the data retrieved from the WMI query. 
	 * The stream allows for efficient, incremental processing of query results, especially useful 
	 * when dealing with large result sets.
	 *
	 * @return A `ManagementQueryStream<ManagementObject>` object for iterating over and processing the query results, each as a `ManagementObject`.
	 */
	_WMI_ATTR_NODISCARD _WMI_FORCEINLINE ManagementQueryStream<ManagementObject> GetStream() noexcept(false)
	{
		if (std::shared_ptr<ManagementResource> ptr = resource_.lock())
		{
			return ManagementQueryStream<ManagementObject>(ptr->services_, InternalQueryExecute(), enumeration_options_);
		}
	}

	/**
	 * This method creates and returns a `ManagementQueryStream` that streams the results 
	 * of the WMI query associated with this query processor. The stream allows for efficient 
	 * and incremental processing of query results, particularly useful for handling large datasets.
	 *
	 * @tparam T The type of objects represented in the query results (e.g., a specific WMI class).
	 * 
	 * @return A `ManagementQueryStream<T>` object for iterating over and processing the query results.
	 */
	template <typename T>
	_WMI_ATTR_NODISCARD _WMI_FORCEINLINE ManagementQueryStream<T> GetStream()
	{
		if (std::shared_ptr<ManagementResource> ptr = resource_.lock())
		{
			return ManagementQueryStream<T>(ptr->services_, InternalQueryExecute(), enumeration_options_);
		}
	}

	/**
	 * This method sets or updates the WMI query string that the processor will execute. 
	 * The query string defines the data or resources being targeted, typically in the format 
	 * of a WQL (WMI Query Language) statement.
	 *
	 * @param query The new WMI query string to be executed (e.g., "SELECT * FROM Win32_Service").
	 */
	_WMI_FORCEINLINE void SetQuery(const BasicString query) noexcept {
		query_ = query;
	}

	/**
	 * This method returns the WMI query string that the processor is configured to execute. 
	 * The query defines the data or resources being targeted, typically in the format of a 
	 * WQL (WMI Query Language) statement.
	 *
	 * @return A constant `BasicString` containing the WMI query string (e.g., "SELECT * FROM Win32_Process").
	 */
	_WMI_ATTR_NODISCARD _WMI_FORCEINLINE const BasicString GetQuery() const noexcept {
		return query_;
	}

private:
	_WMI_ATTR_NODISCARD microsoft::com_ptr<IEnumWbemClassObject> InternalQueryExecute() noexcept(false);

private:
	const EnumerationOptions enumeration_options_;
	std::weak_ptr<ManagementResource> resource_;
	BasicString query_;

	friend class ManagementResource;
};

}
