#pragma once

#include <wmi/common/exports.h>

#include <optional>
#include <chrono>


namespace wmi {

class WMI_DLL EnumerationOptions
{
public:
	bool direct_read = false;
	bool enuemrate_deep = false;
	bool prototype_only = false;
	bool return_immediately = true;
	bool use_amended_qualifiers = false;

	std::optional<std::chrono::seconds> timeout = std::nullopt;
};

}