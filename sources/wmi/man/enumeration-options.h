#pragma once

#include <optional>
#include <chrono>


namespace wmi {

struct EnumerationOptions
{
	bool direct_read = false;
	bool enumerate_deep = false;
	bool prototype_only = false;
	bool return_immediately = true;
	bool use_amended_qualifiers = false;

	std::optional<std::chrono::seconds> timeout = std::nullopt;
};

}
