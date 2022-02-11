#pragma once

#include <chrono>


namespace wmi {

struct ConnectionOptions
{
	std::chrono::milliseconds timeout;
};

}
