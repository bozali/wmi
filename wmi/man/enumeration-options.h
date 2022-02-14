#pragma once

#include <optional>
#include <chrono>


namespace wmi {

struct EnumerationOptions
{
	bool direct_read;
	bool enumerate_deep;
	bool prototype_only;
	bool return_immediately;
	bool rewindable;
	bool use_amended_qualifiers;

	std::optional<std::chrono::seconds> timeout = std::nullopt;
};

}
